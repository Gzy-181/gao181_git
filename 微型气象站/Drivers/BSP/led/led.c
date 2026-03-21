#include "led.h"
#include "sys.h"

void led_init(void)
{
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    GPIO_InitTypeDef gpioc_initstruct;
    gpioc_initstruct.Mode=GPIO_MODE_OUTPUT_PP;
    gpioc_initstruct.Pin=GPIO_PIN_13;
    gpioc_initstruct.Pull=GPIO_PULLUP;
    gpioc_initstruct.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC,&gpioc_initstruct);
    
}

void led_on(void)
{
    HAL_GPIO_WritePin(GPIOC,  GPIO_PIN_13,  GPIO_PIN_RESET);
}

void led_off(void)
{
    HAL_GPIO_WritePin(GPIOC,  GPIO_PIN_13,  GPIO_PIN_SET);
}

void led_turn(void)
{
    HAL_GPIO_TogglePin(GPIOC,  GPIO_PIN_13);
}

