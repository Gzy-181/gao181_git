#include "sys.h"


TIM_HandleTypeDef pwm_initstruct={0};

//***********结合平衡车原理图，平衡车用的是定时器1**********

//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
//TIM1_PWM_Init(7199,0);//PWM频率=72000/(7199+1)=10Khz

void pwm_init(uint16_t arr,uint16_t psc)
{
    pwm_initstruct.Instance = TIM1;
    pwm_initstruct.Init.Prescaler = psc;
    pwm_initstruct.Init.Period = arr;
    pwm_initstruct.Init.CounterMode = TIM_COUNTERMODE_UP;
    HAL_TIM_PWM_Init(&pwm_initstruct);
    
    TIM_OC_InitTypeDef pwm_set_structer={0};
    
    pwm_set_structer.OCMode=TIM_OCMODE_PWM1;
    pwm_set_structer.OCPolarity=TIM_OCPOLARITY_HIGH;
    HAL_TIM_PWM_ConfigChannel(&pwm_initstruct ,&pwm_set_structer,TIM_CHANNEL_4); //PWMA----PA11
    HAL_TIM_PWM_Start(&pwm_initstruct,TIM_CHANNEL_4);
    
    pwm_set_structer.Pulse=0;
    HAL_TIM_PWM_ConfigChannel(&pwm_initstruct ,&pwm_set_structer,TIM_CHANNEL_1);//PWMA----PA8
    HAL_TIM_PWM_Start(&pwm_initstruct,TIM_CHANNEL_1);
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM1)
    {
        //打开时钟，初始化gpio
        __HAL_RCC_TIM1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitTypeDef gpioA_initstruct;
        gpioA_initstruct.Mode=GPIO_MODE_AF_PP;
        gpioA_initstruct.Pin=GPIO_PIN_11|GPIO_PIN_8;   
        gpioA_initstruct.Pull=GPIO_PULLUP;
        gpioA_initstruct.Speed=GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA,&gpioA_initstruct);
        
        
    }
    
}


