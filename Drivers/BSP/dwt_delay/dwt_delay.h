#ifndef DWT_DELAY_H
#define DWT_DELAY_H

#include "stm32f1xx_hal.h"

// 初始化DWT计数器
void dwt_delay_init(void);

// 微秒级延时
void dwt_delay_us(uint32_t nus);

// 毫秒级延时
void dwt_delay_ms(uint32_t nms);

#endif /* DWT_DELAY_H */
