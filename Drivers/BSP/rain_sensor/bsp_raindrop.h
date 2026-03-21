#ifndef __BSP_RAINDROP_H__
#define __BSP_RAINDROP_H__

#include "stm32f1xx_hal.h"
#include "stdint.h"

/* DO（数字输出）设置 - 默认 PA0 */
#ifndef RAINDROP_DO_GPIO_PORT
#define RAINDROP_DO_GPIO_PORT      GPIOA
#endif
#ifndef RAINDROP_DO_GPIO_PIN
#define RAINDROP_DO_GPIO_PIN       GPIO_PIN_0
#endif
#ifndef RAINDROP_DO_GPIO_CLK_ENABLE
#define RAINDROP_DO_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#endif
#ifndef RAINDROP_ACTIVE_LEVEL
#define RAINDROP_ACTIVE_LEVEL   0 /* 0 = LOW 表示有雨 */
#endif

/* AO（模拟输出）设置 - 默认 PA5 (ADC1_IN5)
   若你使用别的引脚，请修改下面三个宏和 ADC 通道宏 */
#ifndef RAINDROP_AO_GPIO_PORT
#define RAINDROP_AO_GPIO_PORT      GPIOA
#endif
#ifndef RAINDROP_AO_GPIO_PIN
#define RAINDROP_AO_GPIO_PIN       GPIO_PIN_5
#endif
#ifndef RAINDROP_AO_GPIO_CLK_ENABLE
#define RAINDROP_AO_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#endif
#ifndef RAINDROP_AO_ADC_CHANNEL
#define RAINDROP_AO_ADC_CHANNEL    ADC_CHANNEL_5
#endif


/* 模拟（AO）接口 */
void bsp_raindrop_analog_init(void);           /* 初始化 ADC 与 AO 引脚 */
uint16_t bsp_raindrop_read_analog_raw(void);   /* 返回原始 ADC 值 0..4095 */
uint8_t  bsp_raindrop_read_analog_percent(void); /* 返回 0..100 百分比 */

/* 返回当前计算的百分比（0..100），不做额外打印（方便 main 使用） */
uint8_t bsp_raindrop_get_percent(void);

/* 高层封装：读取 AO 值、更新 OLED 显示（AO 百分比）
   由 main 调用以封装主循环中与雨滴传感器相关的显示逻辑 */
void bsp_raindrop_update_display(void);

#endif /* __BSP_RAINDROP_H__ */
