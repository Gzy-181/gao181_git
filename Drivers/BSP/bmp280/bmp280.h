/*
 * bmp280.h
 * BMP280 气压传感器驱动（基于工程内的 my_iic 软件 I2C）
 */

#ifndef __BMP280_H__
#define __BMP280_H__

#include "stdint.h"

// BMP280 I2C 地址
#define BMP280_ADDR 0x76 // 0x76 或 0x77，取决于SDO引脚

// BMP280 寄存器地址
#define BMP280_REG_T1       0x88
#define BMP280_REG_T2       0x8A
#define BMP280_REG_T3       0x8C
#define BMP280_REG_P1       0x8E
#define BMP280_REG_P2       0x90
#define BMP280_REG_P3       0x92
#define BMP280_REG_P4       0x94
#define BMP280_REG_P5       0x96
#define BMP280_REG_P6       0x98
#define BMP280_REG_P7       0x9A
#define BMP280_REG_P8       0x9C
#define BMP280_REG_P9       0x9E
#define BMP280_REG_CONFIG   0xF5
#define BMP280_REG_CTRL_MEAS 0xF4
#define BMP280_REG_STATUS   0xF3
#define BMP280_REG_RESET    0xE0
#define BMP280_REG_ID       0xD0
#define BMP280_REG_PRESSURE 0xF7
#define BMP280_REG_TEMPERATURE 0xFA

// 校准参数
typedef struct {
    uint16_t t1;
    int16_t t2;
    int16_t t3;
    uint16_t p1;
    int16_t p2;
    int16_t p3;
    int16_t p4;
    int16_t p5;
    int16_t p6;
    int16_t p7;
    int16_t p8;
    int16_t p9;
} bmp280_calib_t;

// 测量结果
typedef struct {
    float temperature; // 温度，单位：℃
    float pressure;    // 气压，单位：hPa
} bmp280_data_t;

// 初始化 BMP280
uint8_t bmp280_init(void);

// 读取校准参数
void bmp280_read_calib(bmp280_calib_t *calib);

// 读取原始数据
void bmp280_read_raw(int32_t *pressure, int32_t *temperature);

// 计算温度和气压
void bmp280_calculate(bmp280_calib_t *calib, int32_t pressure_raw, int32_t temperature_raw, bmp280_data_t *data);

// 读取温度和气压
uint8_t bmp280_read_data(bmp280_data_t *data);

// 高层封装：读取数据并更新显示/串口
void bmp280_update_display(void);

#endif
