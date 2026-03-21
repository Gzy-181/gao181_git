#include "dht22.h"
#include "dwt_delay.h"
#include "stm32f1xx_hal.h"
#include "stdio.h"
#include "oled.h"
#include "timer.h"
#include "string.h"
#include "uart1.h"
#include "FreeRTOS.h"
#include "task.h"

/* 采样标志 */
uint8_t dht22_sample_flag = 0;

/* 定时器句柄 */
TIM_HandleTypeDef htim3;

/* 上一次显示的字符串长度 */
static uint8_t last_dht22_str_len = 0;

/* 如果需要打印每位高电平时长用于调试，解除下面宏注释 */
#define DHT22_DEBUG_TIMINGS 0

/* 判定 0/1 的高电平时间阈值（单位：delay_us 的单位，通常为微秒）
    通过观察你的板子测得的 timings：0 -> ~6-7, 1 -> ~17-18，
    因此把阈值设为 12（在 6 与 17 之间）。 */
#define DHT22_BIT_THRESHOLD 40

#define DHT_PORT GPIOA
#define DHT_PIN  GPIO_PIN_6

static void dht_set_output(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD; /* 开漏 */
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DHT_PORT, &GPIO_InitStruct);
}

static void dht_set_input(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(DHT_PORT, &GPIO_InitStruct);
}

/* 临界区内使用的等待函数，使用DWT计数器精确测量时间，返回等待的微秒数 */
static int wait_level_critical(uint8_t level, uint32_t timeout_us)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t cycles = timeout_us * (SystemCoreClock / 1000000);
    
    while (HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN) != (GPIO_PinState)level)
    {
        if ((DWT->CYCCNT - start) >= cycles)
        {
            return -1;
        }
    }
    
    return (int)((DWT->CYCCNT - start) / (SystemCoreClock / 1000000));
}

static int dht_read_raw(uint8_t data[5])
{
    int i;
    
    // 进入临界区，禁止任务切换和中断，确保整个通信过程不被打断
    taskENTER_CRITICAL();
    
    // 执行启动信号和检查
    dht_set_output();
    HAL_GPIO_WritePin(DHT_PORT, DHT_PIN, GPIO_PIN_RESET);
    dwt_delay_ms(2); /* 拉低至少1ms，使用DWT延时 */
    
    // 主机释放总线，拉高20~40us
    dht_set_input();
    dwt_delay_us(30); /* 等待30us，让DHT22检测到上升沿 */

    if (wait_level_critical(0, 200) < 0) {
        taskEXIT_CRITICAL();
        return DHT22_ERR_RESPONSE;
    }
    if (wait_level_critical(1, 200) < 0) {
        taskEXIT_CRITICAL();
        return DHT22_ERR_RESPONSE;
    }
    
    // 读取40位数据
    for (i = 0; i < 5; i++) data[i] = 0;

#if DHT22_DEBUG_TIMINGS
    uint16_t lens[40];
#endif

    for (i = 0; i < 40; i++) {
        if (wait_level_critical(0, 200) < 0) {
            taskEXIT_CRITICAL();
            return DHT22_ERR_DATA_TIMEOUT;
        }
        if (wait_level_critical(1, 200) < 0) {
            taskEXIT_CRITICAL();
            return DHT22_ERR_DATA_TIMEOUT;
        }

        uint32_t high_start = DWT->CYCCNT;
        while (HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN) == GPIO_PIN_SET) {
            if ((DWT->CYCCNT - high_start) >= (200 * (SystemCoreClock / 1000000))) break;
        }
        uint32_t high_duration = (DWT->CYCCNT - high_start) / (SystemCoreClock / 1000000);

#if DHT22_DEBUG_TIMINGS
        lens[i] = (uint16_t)high_duration;
#endif

    data[i / 8] <<= 1;
    if (high_duration > DHT22_BIT_THRESHOLD) data[i / 8] |= 1; /* 高电平>40us为1，<40us为0 */
    }

    // 退出临界区，恢复任务调度
    taskEXIT_CRITICAL();

#if DHT22_DEBUG_TIMINGS
    /* 打印每位高电平持续时间和解析出的比特序列，便于调参 */
    printf("DHT22 timings (us): ");
    for (i = 0; i < 40; i++) {
        printf("%u", lens[i]);
        if (i < 39) printf(",");
    }
    printf("\r\n");

    printf("DHT22 bits: ");
    for (i = 0; i < 40; i++) {
        int b = (data[i/8] >> (7 - (i%8))) & 0x01;
        /* 注意：此 data 已经是经过左移/或的结果，按当前位置输出时需重新计算，
           这里使用简便方法：输出从高位到低位的每一位 */
        printf("%d", b);
        if (i < 39) printf("");
    }
    printf("\r\n");
#endif

    dht_set_input();
    return DHT22_OK;
}

int dht22_init(void)
{
    dht_set_input();
    dht22_timer_init();
    return DHT22_OK;
}

/* 初始化定时器（2秒中断一次） */
void dht22_timer_init(void)
{
    // 初始化定时器句柄
    memset(&htim3, 0, sizeof(TIM_HandleTypeDef));
    
    __HAL_RCC_TIM3_CLK_ENABLE();
    
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 7199;  // 72MHz / (7199+1) = 10kHz
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 19999;    // 10kHz * 2s = 20000
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    
    HAL_TIM_Base_Init(&htim3);
    HAL_TIM_Base_Start_IT(&htim3);
    
    HAL_NVIC_SetPriority(TIM3_IRQn, 5, 0); // 设置优先级为5，避免干扰FreeRTOS任务调度
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
}

int dht22_read(float *temperature, float *humidity)
{
    uint8_t raw[5];
    
    int r = dht_read_raw(raw);
    if (r != DHT22_OK) return r;

    uint8_t checksum = raw[0] + raw[1] + raw[2] + raw[3];
    if (checksum != raw[4]) return DHT22_ERR_CHECKSUM;

    uint16_t raw_h = (raw[0] << 8) | raw[1];
    uint16_t raw_t = (raw[2] << 8) | raw[3];

    *humidity = raw_h / 10.0f;
    if (raw_t & 0x8000) {
        raw_t &= 0x7FFF;
        *temperature = - (raw_t / 10.0f);
    } else {
        *temperature = raw_t / 10.0f;
    }
    return DHT22_OK;
}

/* 定时器中断回调函数 - 由main.c中的HAL_TIM_PeriodElapsedCallback调用 */
void dht22_timer_callback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM3)
    {
        dht22_sample_flag = 1;
    }
}

/* 导出：直接读取 5 字节原始数据到 buf（必须至少5字节），返回 DHT22_OK 或错误代码 */
int dht22_read_raw_bytes(uint8_t buf[5])
{
    if (!buf) return DHT22_ERR_READ_FAIL;
    int r = dht_read_raw(buf);
    return r;
}

/* 高层封装：读取温湿度并更新 OLED/串口（仅输出成功或错误） */
void dht22_update_display(void)
{
    if (dht22_sample_flag) {
        float temp = 0.0f, hum = 0.0f;
        int dr = dht22_read(&temp, &hum);
        char tbuf[24];
        uint8_t current_len;
        
        if (dr == DHT22_OK) {
            sprintf(tbuf, "T:%04.1fC H:%04.1f%%", temp, hum);
            current_len = strlen(tbuf);
            
            // 发送符合协议的数据帧
            uart_send_frame(0x01, temp); // 温度
            uart_send_frame(0x02, hum);  // 湿度
        } else {
            sprintf(tbuf, "T: --.-C  H: ---%");
            current_len = strlen(tbuf);
        }
        
        // 只有当新字符串长度比上一次短时，才清除该行
        if (current_len < last_dht22_str_len) {
            oled_clear_line(0, 12); // 清除第0行
        }
        
        oled_show_string(0,0,tbuf,12);
        last_dht22_str_len = current_len;
        dht22_sample_flag = 0;
    }
}

const char *dht22_strerror(int err)
{
    switch (err) {
        case DHT22_OK: return "OK";
        case DHT22_ERR_RESPONSE: return "No response / timeout";
        case DHT22_ERR_DATA_TIMEOUT: return "Data bit timeout";
        case DHT22_ERR_CHECKSUM: return "Checksum mismatch";
        case DHT22_ERR_READ_FAIL: return "Read failed";
        default: return "Unknown error";
    }
}

int dht22_dump_pin(char *out, int maxlen, int samples, int interval_us)
{
    if (maxlen <= 0 || out == NULL) return -1;
    int written = 0;
    int s;
    for (s = 0; s < samples && written < maxlen - 1; s++) {
        GPIO_PinState st = HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN);
        out[written++] = (st == GPIO_PIN_SET) ? '1' : '0';
        if (written < maxlen - 1) out[written++] = ',';
        else break;
        dwt_delay_us(interval_us);
    }
    if (written > 0 && out[written-1] == ',') written--; /* 去掉末尾逗号 */
    out[written] = '\0';
    return written;
}
