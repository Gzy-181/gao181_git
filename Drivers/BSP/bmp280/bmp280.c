#include "bmp280.h"
#include "my_iic.h"
#include "delay.h"
#include "stdio.h"
#include "oled.h"
#include "string.h"
#include "uart1.h"

/* 上一次显示的字符串长度 */
static uint8_t last_bmp280_str_len = 0;

// 校准参数
static bmp280_calib_t bmp280_calib;

// 写入单个字节到 BMP280
static void bmp280_write_byte(uint8_t reg, uint8_t data)
{
    iic_start();
    iic_write_bit(BMP280_ADDR << 1); // 写地址
    iic_cpu_receive_ack();
    iic_write_bit(reg);
    iic_cpu_receive_ack();
    iic_write_bit(data);
    iic_cpu_receive_ack();
    iic_stop();
}

// 读取单个字节
static uint8_t bmp280_read_byte(uint8_t reg)
{
    uint8_t data;
    
    iic_start();
    iic_write_bit(BMP280_ADDR << 1); // 写地址
    iic_cpu_receive_ack();
    iic_write_bit(reg);
    iic_cpu_receive_ack();
    
    iic_start();
    iic_write_bit((BMP280_ADDR << 1) | 1); // 读地址
    iic_cpu_receive_ack();
    data = iic_receive_bit();
    iic_cpu_send_ack(1); // 发送 NACK
    iic_stop();
    
    return data;
}

// 读取多个字节
static void bmp280_read_bytes(uint8_t reg, uint8_t *buf, uint8_t len)
{
    iic_start();
    iic_write_bit(BMP280_ADDR << 1); // 写地址
    iic_cpu_receive_ack();
    iic_write_bit(reg);
    iic_cpu_receive_ack();
    
    iic_start();
    iic_write_bit((BMP280_ADDR << 1) | 1); // 读地址
    iic_cpu_receive_ack();
    
    for (uint8_t i = 0; i < len; i++) {
        buf[i] = iic_receive_bit();
        if (i < len - 1) {
            iic_cpu_send_ack(0); // 发送 ACK
        } else {
            iic_cpu_send_ack(1); // 发送 NACK
        }
    }
    
    iic_stop();
}

// 初始化 BMP280
uint8_t bmp280_init(void)
{
    // 读取设备 ID
    uint8_t id = bmp280_read_byte(BMP280_REG_ID);
    if (id != 0x58) {
        return 1; // 设备 ID 不正确
    }
    
    // 复位设备
    bmp280_write_byte(BMP280_REG_RESET, 0xB6);
    delay_ms(10);
    
    // 读取校准参数
    bmp280_read_calib(&bmp280_calib);
    
    // 配置设备
    // 配置寄存器：t_sb=101 (1000ms), filter=100 (16), spi3w_en=0
    bmp280_write_byte(BMP280_REG_CONFIG, 0xA4);
    
    // 控制测量寄存器：osrs_t=101 (16x), osrs_p=101 (16x), mode=11 (Normal)
    bmp280_write_byte(BMP280_REG_CTRL_MEAS, 0xFF);
    
    return 0;
}

// 读取校准参数
void bmp280_read_calib(bmp280_calib_t *calib)
{
    uint8_t buf[24];
    bmp280_read_bytes(BMP280_REG_T1, buf, 24);
    
    calib->t1 = (buf[1] << 8) | buf[0];
    calib->t2 = (buf[3] << 8) | buf[2];
    calib->t3 = (buf[5] << 8) | buf[4];
    calib->p1 = (buf[7] << 8) | buf[6];
    calib->p2 = (buf[9] << 8) | buf[8];
    calib->p3 = (buf[11] << 8) | buf[10];
    calib->p4 = (buf[13] << 8) | buf[12];
    calib->p5 = (buf[15] << 8) | buf[14];
    calib->p6 = (buf[17] << 8) | buf[16];
    calib->p7 = (buf[19] << 8) | buf[18];
    calib->p8 = (buf[21] << 8) | buf[20];
    calib->p9 = (buf[23] << 8) | buf[22];
}

// 读取原始数据
void bmp280_read_raw(int32_t *pressure, int32_t *temperature)
{
    uint8_t buf[6];
    bmp280_read_bytes(BMP280_REG_PRESSURE, buf, 6);
    
    *pressure = ((int32_t)buf[0] << 16) | ((int32_t)buf[1] << 8) | buf[2];
    *temperature = ((int32_t)buf[3] << 16) | ((int32_t)buf[4] << 8) | buf[5];
    
    // 按数据手册要求，右移4位
    *pressure >>= 4;
    *temperature >>= 4;
}

// 计算温度和气压
void bmp280_calculate(bmp280_calib_t *calib, int32_t pressure_raw, int32_t temperature_raw, bmp280_data_t *data)
{
    // 计算温度
    int32_t t_fine;
    int32_t var1, var2;
    
    var1 = ((((temperature_raw >> 3) - ((int32_t)calib->t1 << 1)) * ((int32_t)calib->t2)) >> 11);
    var2 = (((((temperature_raw >> 4) - ((int32_t)calib->t1)) * ((temperature_raw >> 4) - ((int32_t)calib->t1))) >> 12) * ((int32_t)calib->t3)) >> 14;
    t_fine = var1 + var2;
    data->temperature = (t_fine * 5 + 128) >> 8;
    data->temperature /= 100.0f;
    
    // 计算气压
    int64_t p;
    int64_t var3, var4;
    
    var3 = ((int64_t)t_fine) - 128000;
    var4 = var3 * var3 * (int64_t)calib->p6;
    var4 = var4 + ((var3 * (int64_t)calib->p5) << 17);
    var4 = var4 + (((int64_t)calib->p4) << 35);
    var3 = ((var3 * var3 * (int64_t)calib->p3) >> 8) + ((var3 * (int64_t)calib->p2) << 12);
    var3 = (((((int64_t)1) << 47) + var3)) * ((int64_t)calib->p1) >> 33;
    
    if (var3 == 0) {
        data->pressure = 0;
        return;
    }
    
    p = 1048576 - pressure_raw;
    p = (((p << 31) - var4) * 3125) / var3;
    var3 = (((int64_t)calib->p9) * (p >> 13) * (p >> 13)) >> 25;
    var4 = (((int64_t)calib->p8) * p) >> 19;
    
    p = ((p + var3 + var4) >> 8) + (((int64_t)calib->p7) << 4);
    data->pressure = (float)p / 256.0f;
    data->pressure /= 100.0f; // 转换为 hPa
}

// 读取温度和气压
uint8_t bmp280_read_data(bmp280_data_t *data)
{
    int32_t pressure_raw, temperature_raw;
    
    bmp280_read_raw(&pressure_raw, &temperature_raw);
    bmp280_calculate(&bmp280_calib, pressure_raw, temperature_raw, data);
    
    return 0;
}

// 高层封装：读取数据并更新显示/串口
void bmp280_update_display(void)
{
    bmp280_data_t data;
    uint8_t status = bmp280_read_data(&data);
    char buf[64];
    uint8_t current_len;
    
    if (status == 0) {
        sprintf(buf, "press:%.2fhPa", data.pressure);
        current_len = strlen(buf);
        
        // 发送符合协议的数据帧
        uart_send_frame(0x03, data.pressure); // 气压
    } else {
        sprintf(buf, "气压: --.- hPa");
        current_len = strlen(buf);
    }
    
    // 只有当新字符串长度比上一次短时，才清除该行
    if (current_len < last_bmp280_str_len) {
        oled_clear_line(4, 12); // 清除第4行
    }
    
    oled_show_string(0,4,buf,12);
    last_bmp280_str_len = current_len;
}
