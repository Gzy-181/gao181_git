
#include "bh1750.h"
#include "delay.h"
#include "stdint.h"
#include "stdio.h"
#include "my_iic.h"
#include "oled.h"
#include "string.h"

/* 上一次显示的字符串长度 */
static uint8_t last_bh1750_str_len = 0;

// 写单个字节到 BH1750
void bh1750_write_single(uint8_t data)
{
    iic_start();
    iic_write_bit(0x46); // BH1750 写地址（0x23 << 1）
    iic_cpu_receive_ack();
    iic_write_bit(data);
    iic_cpu_receive_ack();
    iic_stop();
}

void bh1750_init(void)
{
    bh1750_write_single(BH1750_POWER_ON);
    bh1750_write_single(BH1750_CONTINUOUS_HIGH_RES_MODE);
}

// 从 BH1750 读取两个字节原始结果
static uint16_t bh1750_read_raw(void)
{
    uint16_t raw = 0;
    uint8_t high, low;
    
    iic_start();
    iic_write_bit(0x47); // BH1750 读地址（0x23 << 1 | 1）
    iic_cpu_receive_ack();
    
    high = iic_receive_bit();
    iic_cpu_send_ack(0); // 发送 ACK
    low = iic_receive_bit();
    iic_cpu_send_ack(1); // 发送 NACK
    
    iic_stop();
    
    raw = ((uint16_t)high << 8) | low;
    return raw;
}

// 读取光照值
uint8_t bh1750_read_lux(float *out_lx)
{
    if (out_lx == NULL) return BH1750_ERR_PARAM;
    
    delay_ms(200); // 等待测量完成（高分辨率模式需要120ms）
    
    uint16_t raw = bh1750_read_raw();
    *out_lx = (float)raw / 1.2f;
    return BH1750_OK;
}

// 高层封装：读取光照值并更新显示/串口
void bh1750_update_display(void)
{
    float lux = 0.0f;
    uint8_t status = bh1750_read_lux(&lux);
    char buf[32];
    uint8_t current_len;
    
    if (status == BH1750_OK) {
        sprintf(buf, "Light: %.1f lx", lux);
        current_len = strlen(buf);
        printf("%s\r\n", buf);
    } else {
        sprintf(buf, "Light: --.- lx");
        current_len = strlen(buf);
        printf("BH1750 read error: %d\r\n", status);
    }
    
    // 只有当新字符串长度比上一次短时，才清除该行
    if (current_len < last_bh1750_str_len) {
        oled_clear_line(2, 12); // 清除第2行
    }
    
    oled_show_string(0,2,buf,12);
    last_bh1750_str_len = current_len;
}

