#include "bsp_raindrop.h"
#include "delay.h"
#include "stdio.h"
#include "oled.h"
#include "string.h"
#include "uart1.h"

static ADC_HandleTypeDef hadc1;

/* 上一次显示的字符串长度 */
static uint8_t last_rain_str_len = 0;
static uint8_t _bsp_raindrop_percent_cache = 0;

/* AO (模拟) 初始化：配置 PA5 为模拟输入并初始化 ADC1 */
void bsp_raindrop_analog_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* 使能 GPIO 与 ADC 时钟 */
    RAINDROP_AO_GPIO_CLK_ENABLE();
    __HAL_RCC_ADC1_CLK_ENABLE();

    /* 配置 AO 引脚为 模拟 输入 */
    GPIO_InitStruct.Pin = RAINDROP_AO_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(RAINDROP_AO_GPIO_PORT, &GPIO_InitStruct);

    /* ADC 基本初始化（单次转换，软件触发） */
    hadc1.Instance = ADC1;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.NbrOfConversion = 1;
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        /* 初始化失败可在此处理 */
        printf("ADC init failed\r\n");
    }

    /* 校准 ADC（STM32F1 系列 HAL 提供校准接口） */
#ifdef HAL_ADCEx_Calibration_Start
    HAL_ADCEx_Calibration_Start(&hadc1);
#endif
}

/* 读取 AO 原始 ADC 值（12-bit，0..4095） */
uint16_t bsp_raindrop_read_analog_raw(void)
{
    ADC_ChannelConfTypeDef sConfig;
    uint16_t val = 0;

    sConfig.Channel = RAINDROP_AO_ADC_CHANNEL;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;

    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        printf("ADC channel config failed\r\n");
        return 0;
    }

    if (HAL_ADC_Start(&hadc1) != HAL_OK)
    {
        printf("ADC start failed\r\n");
        return 0;
    }

    /* 等待转换完成，超时 10ms */
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
    {
        val = (uint16_t)HAL_ADC_GetValue(&hadc1);
    }
    else
    {
        printf("ADC poll timeout\r\n");
    }

    HAL_ADC_Stop(&hadc1);
    return val;
}

/* 返回 0..100 的百分比（基于 12-bit ADC）*/
uint8_t bsp_raindrop_read_analog_percent(void)
{
    uint16_t raw = bsp_raindrop_read_analog_raw();
    /* 4095 -> 100%，注意映射方向：AO 板上湿度增大时电压增减取决于模块，
       如果值方向相反（电压越低表示越湿），可改为 100 - percent */
    uint8_t percent = (uint8_t)((raw * 100UL) / 4095UL);
    _bsp_raindrop_percent_cache = 100-percent;
    return percent;
}

/* 更新 AO 显示：计算百分比并在 OLED 上显示和返回 */
void bsp_raindrop_update_display(void)
{
    uint16_t raw = bsp_raindrop_read_analog_raw();
    uint8_t percent = (uint8_t)((raw * 100UL) / 4095UL);
    _bsp_raindrop_percent_cache = 100 - percent;
    
}

uint8_t bsp_raindrop_get_percent(void)
{
    uint8_t pct = _bsp_raindrop_percent_cache;
    const char *level_ascii;
    if (pct < 10) level_ascii = "No rain";
    else if (pct < 40) level_ascii = "Light";
    else if (pct < 70) level_ascii = "Moderate";
    else if (pct < 90) level_ascii = "Heavy";
    else level_ascii = "VeryHeavy";
    char buf[32];
    sprintf(buf, "rain:%s", level_ascii);
    uint8_t current_len = strlen(buf);
    
    // 发送符合协议的数据帧
    //uart_send_frame(0x04, (float)pct); // 雨滴传感器数据，使用类型 0x04
    
    // 只有当新字符串长度比上一次短时，才清除该行
    if (current_len < last_rain_str_len) {
        oled_clear_line(6, 12); // 清除第6行
    }
    
    oled_show_string(0, 6, buf, 12);
    last_rain_str_len = current_len;
    return _bsp_raindrop_percent_cache;
}
