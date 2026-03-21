#include  "sys.h"


TIM_HandleTypeDef counter_initstructer={0};

uint16_t num=0;

void counter_init(uint16_t arr,uint16_t psc)
{
    counter_initstructer.Instance=TIM2;
    counter_initstructer.Init.CounterMode=TIM_COUNTERMODE_UP;
    counter_initstructer.Init.Period=arr;
    counter_initstructer.Init.Prescaler=psc;
    HAL_TIM_IC_Init(&counter_initstructer);
    
    TIM_SlaveConfigTypeDef slave_initstructer={0};
    slave_initstructer.SlaveMode=TIM_SLAVEMODE_EXTERNAL1;
    slave_initstructer.TriggerPolarity=TIM_TRIGGERPOLARITY_FALLING;
    slave_initstructer.TriggerFilter=0xF;
    slave_initstructer.InputTrigger=TIM_TS_TI2FP2;
    
    HAL_TIM_SlaveConfigSynchro(&counter_initstructer,&slave_initstructer);
    HAL_TIM_IC_Start(&counter_initstructer,TIM_CHANNEL_2);
}

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM2)
    {
        __HAL_RCC_TIM2_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        
        GPIO_InitTypeDef gpioa_initstructer;
        gpioa_initstructer.Mode=GPIO_MODE_AF_INPUT;
        gpioa_initstructer.Pin=GPIO_PIN_1;
        gpioa_initstructer.Pull=GPIO_PULLUP;
        gpioa_initstructer.Speed=GPIO_SPEED_FREQ_HIGH;

        HAL_GPIO_Init(GPIOA,&gpioa_initstructer);
    }
}

void get_counter(void)
{
    num=__HAL_TIM_GET_COUNTER(&counter_initstructer);
}
