/*
 * bh1750.h
 * BH1750 光照传感器驱动（基于工程内的 my_iic 软件 I2C）
 */

#ifndef __BH1750_H__
#define __BH1750_H__

#include "stdint.h"

// 返回值/错误码
#define BH1750_OK            0
#define BH1750_ERR_PARAM     2

// BH1750 操作指令
#define BH1750_POWER_ON   0x01
#define BH1750_RESET      0x07
#define BH1750_CONTINUOUS_HIGH_RES_MODE  0x10

// 写入单个字节
void bh1750_write_single(uint8_t data);

// 读取一次光照值（lx，浮点），返回 BH1750_OK 并通过 out_lx 输出；出错返回错误码
uint8_t bh1750_read_lux(float *out_lx);

// 高层封装：读取光照值并更新显示/串口
void bh1750_update_display(void);

void bh1750_init(void);// 初始化 BH1750

#endif

