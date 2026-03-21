/**
 * @file main.h
 * @brief 主程序头文件
 * @details 该文件包含了平衡小车系统的主要全局变量定义，包括PID控制参数、电机反馈数据等
 * @author [作者名]
 * @date [日期]
 */
#ifndef __MAIN_H__
#define __MAIN_H__



#define Position_Div 2
//**********定时器相关参数*******************
uint16_t arr;                    // 定时器自动重装载值
uint16_t psc;                    // 定时器预分频值

//**********PID控制相关结构体*******************
struct PID_PRAM Speed_PID_left={0};  // 左电机速度PID结构体参数
struct PID_PRAM Speed_PID_right={0}; // 右电机速度PID结构体参数

struct PID_PRAM Position_PID_left={0};  // 左电机位置PID结构体参数
struct PID_PRAM Position_PID_right={0}; // 右电机位置PID结构体参数

//**********反馈数据*******************
int32_t fb_left;                 // 左电机速度反馈值
int32_t fb_right;                // 右电机速度反馈值
float battery_V;                 // 电池电压
float pid_run_priod;             // PID运行周期
float duty_l;                    // 左电机占空比

float duty_r;                    // 右电机占空比

//**********PID输出数据*******************
double left_pid_out_V;           // 左电机PID输出电压
double right_pid_out_V;          // 右电机PID输出电压

//**********PID调试变量*******************
double errors;                   // 误差值
double dev;                      // 误差微分
double error_int;                // 误差积分
double cout_p;                   // 比例项输出
double cout_i;                   // 积分项输出
double cout_d;                   // 微分项输出
double cout_all;                 // PID总输出

#endif
