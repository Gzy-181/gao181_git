#include "sys.h"
#include "pwm.h"
#include "motor.h"


//PWMA----PA11--CH4,PWMB----PA8--CH1
//motorA-------PB12,PB13
//motorB-------PB14,PB15
#define motorA_In1 GPIO_PIN_12
#define motorA_In2 GPIO_PIN_13
#define PWMA GPIO_PIN_11


#define motorB_In1 GPIO_PIN_14
#define motorB_In2 GPIO_PIN_15
#define PWMB GPIO_PIN_8

extern TIM_HandleTypeDef pwm_initstruct;

void motorA_B_init(void)  //初始化A，B两个电机
{
    pwm_init(7199,0);  // PWM分辨率为1/7200，频率为10Khz,(检查没问题)
    
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef gpiob_initstructer;
    gpiob_initstructer.Mode=GPIO_MODE_OUTPUT_PP;
    gpiob_initstructer.Pin=motorA_In1|motorA_In2|motorB_In1|motorB_In2;
    gpiob_initstructer.Pull=GPIO_PULLUP;
    gpiob_initstructer.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB,&gpiob_initstructer);
    HAL_GPIO_WritePin(GPIOB,motorA_In1,GPIO_PIN_SET);//初始化电机先不转
}

void motor_A_forward(void)
{
    HAL_GPIO_WritePin(GPIOB,motorA_In1,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB,motorA_In2,GPIO_PIN_SET);
}

void motor_A_backward(void)
{
    HAL_GPIO_WritePin(GPIOB,motorA_In1,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB,motorA_In2,GPIO_PIN_RESET);
}

void motor_B_forward(void)
{
    HAL_GPIO_WritePin(GPIOB,motorB_In1,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB,motorB_In2,GPIO_PIN_RESET);
}

void motor_B_backward(void)
{
    HAL_GPIO_WritePin(GPIOB,motorB_In1,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB,motorB_In2,GPIO_PIN_SET);
}

void motor_A_changespeed(int16_t speed)  //speed：0---7199
{
    if(speed>0)
    {
        motor_A_forward();
        __HAL_TIM_SET_COMPARE(&pwm_initstruct,TIM_CHANNEL_4,speed);
    }
    else
    {
        motor_A_backward();
        __HAL_TIM_SET_COMPARE(&pwm_initstruct,TIM_CHANNEL_4,-speed);
    }
}

void motor_B_changespeed(int16_t speed)
{
    if(speed>0)
    {
        motor_B_forward();
        __HAL_TIM_SET_COMPARE(&pwm_initstruct,TIM_CHANNEL_1,speed);
    }
   else
    {
        motor_B_backward();
        __HAL_TIM_SET_COMPARE(&pwm_initstruct,TIM_CHANNEL_1,-speed);
    }
}


void motor_A_stop()
{
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET);
}

void motor_B_stop()
{
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET);
}

void motor_set_TargetSpeed(struct PID_PRAM* pid_pram,int32_t speed )
{
    PID_SET_SP(pid_pram,speed);
}



