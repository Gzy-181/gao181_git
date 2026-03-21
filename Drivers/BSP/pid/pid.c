#include "pid.h"
#include "stm32f10x.h"
#include "stdio.h"
#include "math.h"



//*********************pid参数优化选择********************

#define Jifen_Fenli 0    //是否使用积分分离(建议位置式使用，由于位置式pid的最终误差比较小 ，所以在一开始不进行累计误差计算，后面再加入)
           //**********积分分离***********
    /*
    积分项作用一般位于调控后期，用来消除持续的误差，
    调控前期一般误差较大且不需要积分项作用，
    如果此时仍然进行积分，则调控进行到后期时，
    积分项可能已经累积了过大的调控力，这会导致超调
    积分分离实现思路:对误差大小进行判断，如果误差绝对值小于指定阈值
    则加入积分项作用，反之，则直接将误差积分清零或不加入积分项作用  
    */

#define wei_fen_xian_xing 1 //是否使用微分先行优化  优点：变化更加平滑  缺点：系统跟踪速度变慢 
#define bu_wan_quan_wei_fen 0 //是否使用不完全微分 （降低噪声造成的干扰）
#define WEIGHT 0.5           //权重
#define output_dead_line 1.5   //输入死区
#define output_Pian_Yi    0    //是否使用输出偏移 
#define output_add 50;         //输出偏移偏移量

float dev_plus;
float dev_plus_1;
/*
    微分先行存在的意义就是，由于微分的作用是给系统增加阻尼，但是由于目标值切换的瞬间，误差曲线会瞬间增大,微分项会产生一个
    很大的正向的作用力，就像被推了一把一样，不符合阻尼的特性，所以我们要进行优化
    @解决思路：把对误差的微分改成对实际值的微分。记得要加负号，因为是要起阻尼的作用
    对误差的微分：dev=(errors-pid_pram->err_k_1)/time
    对实际值的微分：dev=-(fb-fb_k_1)/time  fb是编码器返回的值，代表速度值
*/      

#define Yu_Zhi 20  //实际值与目标值的差值
//********************************************************    
    




void PID_SET_KP(struct PID_PRAM* pid_pram,float kp)
{
    pid_pram->kp=kp;
    
}


void PID_SET_KI(struct PID_PRAM* pid_pram,float ki)
{
    pid_pram->ki=ki;
    
}

void PID_SET_KD(struct PID_PRAM* pid_pram,float kd)
{
    pid_pram->kd=kd;
    
}

void PID_SET_SP(struct PID_PRAM* pid_pram,int32_t sp)
{
    pid_pram->sp=sp;
}


void PID_SET_ALL(struct PID_PRAM* pid_pram,float kp,float ki,float kd)
{
    PID_SET_KP(pid_pram,kp);
    PID_SET_KI(pid_pram,ki);
    PID_SET_KD(pid_pram,kd);
}


double PID_Compute(struct PID_PRAM* pid_pram,int32_t fb,float time)//fb是编码器反馈的值，time是数据更新的时间间隔
{
    
   errors=1.0*(pid_pram->sp-fb);
    
   //微分先行
   if(!wei_fen_xian_xing)
   {
       dev_plus=(errors-pid_pram->err_k_1)/time;
   }
   else{
       // 使用实例内的上次反馈，避免左右轮互相污染
       dev_plus=-(fb-pid_pram->fb_k_1)/time;
   }
   if(bu_wan_quan_wei_fen) dev=(1-WEIGHT)*dev_plus+WEIGHT*dev_plus_1;
   else{
       dev=dev_plus;
   }
   
   error_int=pid_pram->err_int_k_1+(pid_pram->err_k_1+errors)*time/2.0;
  
   cout_p=pid_pram->kp*errors;
   cout_i=pid_pram->ki*error_int;
    
    //积分限幅
   if(pid_pram->ki*error_int>4) error_int=4.0/pid_pram->ki;  // 对称夹紧
   if(pid_pram->ki*error_int<-4) error_int=-4.0/pid_pram->ki;
   // 重新计算积分输出，避免限幅后继续放大
   cout_i=pid_pram->ki*error_int;
    
    
    //积分分离
    if (Jifen_Fenli)
    {
        
        if(fabs(errors)<Yu_Zhi)
        {
            ;
        }
        else
        {
            cout_i=0;
            
        }
        
    }
   cout_d=pid_pram->kd*dev;
   cout_all=cout_p+cout_i+cout_d;
    if(fabs(errors)<output_dead_line) //输入死区
    {
        cout_all=0;
        error_int=0;
    }
    
    //更新
    pid_pram->err_k_1=errors;
    pid_pram->err_int_k_1=error_int;
    pid_pram->fb_k_1=fb;
    dev_plus_1=dev_plus;
    //printf("error:%.5f,dev:%.5f,error_int:%.5f,cout_p:%.5f,cout_i:%.5f,cout_d:%.5f,cout_all:%.5f\n",error,dev,error_int,cout_p,cout_i,cout_d,cout_all);
    
    if(output_Pian_Yi)
    {
        cout_all+=output_add;
    }
    return cout_all;
   
}

void PID_Init(struct PID_PRAM* pid_pram)
{
    pid_pram->kd=0;
    pid_pram->ki=0;
    pid_pram->kp=0;
    pid_pram->sp=0;
    // 初始化误差历史值
    pid_pram->err_k_1=0;
    pid_pram->err_int_k_1=0;
    pid_pram->fb_k_1=0;
}



