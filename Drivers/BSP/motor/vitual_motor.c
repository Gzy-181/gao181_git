#include "vitual_motor.h"

void virtual_motor_init(VirtualMotor_t* motor, float max_accel, float max_decel, float duty_rate_limit)
{
    motor->position = 0.0f;
    motor->velocity = 0.0f;
    motor->target_velocity = 0.0f;
    motor->current_duty = 0.0f;
    motor->target_duty = 0.0f;
    motor->max_acceleration = max_accel;
    motor->max_deceleration = max_decel;
    motor->duty_rate_limit = duty_rate_limit;
    motor->encoder_count = 0;
    motor->motor_constant = 0.01f;
    motor->friction = 0.1f;
    motor->encoder_resolution = 1000.0f;
}

void virtual_motor_set_duty(VirtualMotor_t* motor, float duty)
{
    // ����PWM��Χ��0~7200
    if (duty > 7200.0f) {
        duty = 7200.0f;
    } else if (duty < -7200.0f) {
        duty = -7200.0f;
    }
    
    motor->target_duty = duty;
}

void virtual_motor_set_target_velocity(VirtualMotor_t* motor, float target_velocity_rad_s)
{
    motor->target_velocity = target_velocity_rad_s;
}

void virtual_motor_update(VirtualMotor_t* motor, float dt)
{
    // 1. PWMռ�ձȽ���
    float duty_diff = motor->target_duty - motor->current_duty;
    float max_duty_change = motor->duty_rate_limit * dt;
    
    if (duty_diff > max_duty_change) {
        motor->current_duty += max_duty_change;
    } else if (duty_diff < -max_duty_change) {
        motor->current_duty -= max_duty_change;
    } else {
        motor->current_duty = motor->target_duty;
    }
    
    // 2. ����Ŀ���ٶȣ�����PWMռ�ձȣ�
    float target_velocity = motor->current_duty * motor->motor_constant;
    
    // 3. �ٶȽ��䣨���������ٶȺͼ��ٶȣ�
    float velocity_diff = target_velocity - motor->velocity;
    float max_velocity_change;
    
    if (velocity_diff > 0) {
        max_velocity_change = motor->max_acceleration * dt;
    } else {
        max_velocity_change = motor->max_deceleration * dt;
    }
    
    if (velocity_diff > max_velocity_change) {
        motor->velocity += max_velocity_change;
    } else if (velocity_diff < -max_velocity_change) {
        motor->velocity -= max_velocity_change;
    } else {
        motor->velocity = target_velocity;
    }
    
    // 4. ����Ħ������
    motor->velocity -= motor->friction * motor->velocity * dt;
    
    // 5. ����λ��
    motor->position += motor->velocity * dt;
    
    // 6. ���±���������ֵ
    float angular_velocity_rad_s = motor->velocity;
    float pulses_per_second = angular_velocity_rad_s * motor->encoder_resolution / (2.0f * 3.14159f);
    int32_t encoder_delta = (int32_t)(pulses_per_second * dt);
    motor->encoder_count += encoder_delta;
}

int32_t virtual_encoder_get_count(VirtualMotor_t* motor)
{
    return motor->encoder_count;
}

void virtual_encoder_clear_count(VirtualMotor_t* motor)
{
    motor->encoder_count = 0;
}

float virtual_motor_get_current_velocity(VirtualMotor_t* motor)
{
    return motor->velocity;
}

float virtual_motor_get_position(VirtualMotor_t* motor)
{
    return motor->position;
}

float virtual_motor_get_current_duty(VirtualMotor_t* motor)
{
    return motor->current_duty;
}
