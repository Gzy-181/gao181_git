/**
 * @file pid.h
 * @brief PID控制器头文件
 * @details 该文件定义了PID控制器的结构体和相关函数，用于平衡小车的速度和姿态控制
 * @author [作者名]
 * @date [日期]
 */
#ifndef __PID_H__
#define __PID_H__

#include "stdint.h"

//**********PID调试全局变量*******************
extern double errors;         // 误差值
extern double dev;            // 误差微分
extern double error_int;      // 误差积分
extern double cout_p;         // 比例项输出
extern double cout_i;         // 积分项输出
extern double cout_d;         // 微分项输出
extern double cout_all;       // PID总输出

//**********PID参数结构体定义*******************
/**
 * @brief PID控制器参数结构体
 * @details 存储PID控制器的各项参数和状态
 */
struct PID_PRAM
{
    double kp;               // 比例系数
    double ki;               // 积分系数
    double kd;               // 微分系数
    int32_t sp;              // 设定值
    double err_k_1;          // 上一次PID计算的误差
    double err_int_k_1;      // 上一次PID计算的误差积分值
    int32_t fb_k_1;          // 上一次的反馈值(速度或角度)，用于PID实际计算，防止微分环节的冲击影响
};

/**
 * @brief 设置PID比例系数
 * @param pid_pram PID参数结构体指针
 * @param kp 比例系数值
 */
void PID_SET_KP(struct PID_PRAM* pid_pram, float kp);

/**
 * @brief 设置PID积分系数
 * @param pid_pram PID参数结构体指针
 * @param ki 积分系数值
 */
void PID_SET_KI(struct PID_PRAM* pid_pram, float ki);

/**
 * @brief 设置PID微分系数
 * @param pid_pram PID参数结构体指针
 * @param kd 微分系数值
 */
void PID_SET_KD(struct PID_PRAM* pid_pram, float kd);

/**
 * @brief 设置PID设定值
 * @param pid_pram PID参数结构体指针
 * @param sp 设定值
 */
void PID_SET_SP(struct PID_PRAM* pid_pram, int32_t sp);

/**
 * @brief PID计算函数
 * @param pid_pram PID参数结构体指针
 * @param fb 反馈值
 * @param time 采样时间
 * @return PID计算结果
 */
double PID_Compute(struct PID_PRAM* pid_pram, int32_t fb, float time);

/**
 * @brief 设置PID所有参数
 * @param pid_pram PID参数结构体指针
 * @param kp 比例系数
 * @param ki 积分系数
 * @param kd 微分系数
 */
void PID_SET_ALL(struct PID_PRAM* pid_pram, float kp, float ki, float kd);

/**
 * @brief PID初始化函数
 * @param pid_pram PID参数结构体指针
 */
void PID_Init(struct PID_PRAM* pid_pram);

#endif
