#include "sys.h"

TIM_HandleTypeDef sg_initstruct={0};


void sg90_pwm_init(uint16_t arr,uint16_t psc)
{
   sg_initstruct.Instance = TIM2;
   sg_initstruct.Init.Prescaler = psc;
   sg_initstruct.Init.Period = arr;
   sg_initstruct.Init.CounterMode = TIM_COUNTERMODE_UP;
    HAL_TIM_PWM_Init(&sg_initstruct);
    
    TIM_OC_InitTypeDef pwm_set_structer={0};
    
    pwm_set_structer.OCMode=TIM_OCMODE_PWM1;
    pwm_set_structer.Pulse=arr/2;
    pwm_set_structer.OCPolarity=TIM_OCPOLARITY_HIGH;
    HAL_TIM_PWM_ConfigChannel(&sg_initstruct ,&pwm_set_structer,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&sg_initstruct,TIM_CHANNEL_3);
}

void sg90_set_angle(uint16_t angle)
{
    uint16_t ccr=(1.0/9.0)*angle+5.0;
    __HAL_TIM_SET_COMPARE(&sg_initstruct,TIM_CHANNEL_3,ccr);
    
}

