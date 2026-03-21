#include "sys.h"
#include "delay.h"
#include "led.h"
#include "oled.h"
TIM_HandleTypeDef tim3_init_handle={0};


void get_distance_init(void)
{
    
    tim3_init_handle.Instance=TIM3;
    tim3_init_handle.Init.CounterMode=TIM_COUNTERMODE_UP;
    tim3_init_handle.Init.Period=65535;
    tim3_init_handle.Init.Prescaler=71;
    HAL_TIM_Base_Init(&tim3_init_handle);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM3)
    {
        __HAL_RCC_TIM3_CLK_ENABLE();
    }
}

void dis_gpio_init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef gpio_initstructer1;
    gpio_initstructer1.Mode=GPIO_MODE_OUTPUT_PP;
    gpio_initstructer1.Pin=GPIO_PIN_6;//trig
    gpio_initstructer1.Pull=GPIO_PULLDOWN;
    gpio_initstructer1.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA,&gpio_initstructer1);
    
    GPIO_InitTypeDef gpio_initstructer2;
    gpio_initstructer2.Mode=GPIO_MODE_INPUT;
    gpio_initstructer2.Pin=GPIO_PIN_5;//echo
    gpio_initstructer2.Pull=GPIO_PULLDOWN;
    gpio_initstructer2.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA,&gpio_initstructer2);
}

float get_distance(void)
{
    led_init();
    uint16_t num=0;
    uint16_t distance=0;
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET);
    delay_us(15);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);
    //led_off();
    while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)==GPIO_PIN_RESET);
    HAL_TIM_Base_Start(&tim3_init_handle);
    __HAL_TIM_SetCounter(&tim3_init_handle,0);
    
    //led_off();
    while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)==GPIO_PIN_SET);
    HAL_TIM_Base_Stop(&tim3_init_handle);
    num=__HAL_TIM_GetCounter(&tim3_init_handle);
    
    distance=0.01715*num;
    led_off();
    return distance;
}

void test_timer(void)
  {
      get_distance_init(); // ≥ı ºªØTIM4
      HAL_TIM_Base_Start(&tim3_init_handle);
      while(1)
      {
          HAL_TIM_Base_Start(&tim3_init_handle);
          uint16_t count = __HAL_TIM_GetCounter(&tim3_init_handle);
          OLED_ShowNum(1,1,count,9);
          //HAL_Delay(500);
      }
  }
