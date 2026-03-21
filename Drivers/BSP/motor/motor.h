#ifndef __MOTOR_H__
#define __MOTOR_H__
#include "pid.h"

void motorA_B_init(void);

void motor_A_forward(void);

void motor_A_backward(void);

void motor_B_forward(void);

void motor_B_backward(void);

void motor_A_changespeed(int16_t speed);

void motor_B_changespeed(int16_t speed);

void motor_A_stop(void);

void motor_B_stop(void);

void motor_set_TargetSpeed(struct PID_PRAM* pid_pram,int32_t sp);


#endif
