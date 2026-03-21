#include "sys.h"
#include "led.h"
#include "input_catch.h"

TIM_HandleTypeDef input_catch_initstruct={0};

void input_catch_init(void)
{
    led_init();
    input_catch_initstruct.Instance=TIM2;
    input_catch_initstruct.Init.Prescaler=71;
    input_catch_initstruct.Init.Period=65535;
    input_catch_initstruct.Init.CounterMode=TIM_COUNTERMODE_UP;
    HAL_TIM_IC_Init(&input_catch_initstruct);
    
    TIM_IC_InitTypeDef input_catch_structure={0};
    input_catch_structure.ICFilter=0x0;
    input_catch_structure.ICPolarity=TIM_ICPOLARITY_RISING;
    input_catch_structure.ICPrescaler=TIM_ICPSC_DIV1;
    input_catch_structure.ICSelection=TIM_ICSELECTION_DIRECTTI;
    HAL_TIM_IC_ConfigChannel(&input_catch_initstruct,&input_catch_structure,TIM_CHANNEL_2);
   
    __HAL_TIM_ENABLE_IT(&input_catch_initstruct,TIM_IT_UPDATE);
     HAL_TIM_IC_Start_IT(&input_catch_initstruct,TIM_CHANNEL_2);
    //led_off();
}    

//void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
//{
//    
//    if(htim->Instance==TIM2)
//    {
//        __HAL_RCC_TIM2_CLK_ENABLE();
//        __HAL_RCC_GPIOA_CLK_ENABLE();
//        
//        GPIO_InitTypeDef gpioa_initstructer;
//        gpioa_initstructer.Mode=GPIO_MODE_INPUT;
//        gpioa_initstructer.Pin=GPIO_PIN_1;
//        gpioa_initstructer.Pull=GPIO_PULLUP;
//        gpioa_initstructer.Speed=GPIO_SPEED_FREQ_HIGH;
//        HAL_GPIO_Init(GPIOA,&gpioa_initstructer);
//        

//        
//        HAL_NVIC_SetPriority(TIM2_IRQn,0,0);
//        HAL_NVIC_EnableIRQ(TIM2_IRQn);
//      
//        led_off();

//    }
//}

//volatile uint32_t start_time=0;
//volatile uint32_t end_time=0;
volatile uint32_t zhan_bi=0;
//volatile uint8_t capture_state=0;
volatile uint32_t overflow_count=0;
//volatile uint32_t startoverflow_count=0;
volatile uint32_t imput_catch_value=0;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM2)
    {
        
        imput_catch_value=HAL_TIM_ReadCapturedValue(&input_catch_initstruct,TIM_CHANNEL_2);
        __HAL_TIM_SET_COUNTER(&input_catch_initstruct,0);
        zhan_bi=imput_catch_value/10;
//        if(capture_state==0)
//        {
//            press_duration=0;
//            led_on();
//            capture_state=1;
//            start_time=imput_catch_value;
//            startoverflow_count=overflow_count;
//        }
//        else
//        {
//            end_time=imput_catch_value;
//            press_duration=end_time-start_time+(overflow_count-startoverflow_count)*65536;
//            capture_state=0;
//            led_off();
//        }
    }
   //startoverflow_count++;
    //led_on();
}

void TIM2_IRQHandler(void)
{
    // 使用HAL库的中断处理函数
    HAL_TIM_IRQHandler(&input_catch_initstruct);

    // 或者手动检查中断标志位并清除
//    if (__HAL_TIM_GET_FLAG(&timerbase_struct, TIM_FLAG_UPDATE) == SET)
//    {
//          __HAL_TIM_CLEAR_IT(&timerbase_struct, TIM_IT_UPDATE);
//            led_turn();
//       
//    }
}
  
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//    if(htim->Instance==TIM2)
//    {
//        overflow_count++;
//    }
//}



