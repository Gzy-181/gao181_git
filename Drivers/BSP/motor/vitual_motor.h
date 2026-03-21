#ifndef __VITUAL_MOTOR_H__
#define __VITUAL_MOTOR_H__

#include <stdint.h>

typedef struct {
    float position;        // 当前位置
    float velocity;        // 当前速度 (rad/s)
    float target_velocity; // 目标速度 (rad/s)
    float current_duty;    // 当前PWM占空比
    float target_duty;     // 目标PWM占空比
    float max_acceleration; // 最大加速度
    float max_deceleration; // 最大减速度
    float duty_rate_limit;  // PWM变化速率限制
    uint32_t encoder_count; // 编码器计数值
    float motor_constant;   // 电机常数
    float friction;         // 摩擦系数
    float encoder_resolution; // 编码器分辨率
} VirtualMotor_t;

// 函数声明
void virtual_motor_init(VirtualMotor_t* motor, float max_accel, float max_decel, float duty_rate_limit);
void virtual_motor_update(VirtualMotor_t* motor, float dt);
void virtual_motor_set_duty(VirtualMotor_t* motor, float duty);
void virtual_motor_set_target_velocity(VirtualMotor_t* motor, float target_velocity_rad_s);
int32_t virtual_encoder_get_count(VirtualMotor_t* motor);
void virtual_encoder_clear_count(VirtualMotor_t* motor);
float virtual_motor_get_current_velocity(VirtualMotor_t* motor);
float virtual_motor_get_position(VirtualMotor_t* motor);
float virtual_motor_get_current_duty(VirtualMotor_t* motor);

#endif
