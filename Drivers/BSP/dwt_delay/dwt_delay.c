#include "dwt_delay.h"

// 初始化DWT计数器
void dwt_delay_init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // 使能DWT
    DWT->CYCCNT = 0; // 清零计数器
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; // 使能周期计数器
}

// 微秒级延时
void dwt_delay_us(uint32_t nus)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t cycles = nus * (SystemCoreClock / 1000000); // 计算需要的时钟周期数
    
    while ((DWT->CYCCNT - start) < cycles)
    {
        __NOP(); // 空操作，防止编译器优化
    }
}

// 毫秒级延时
void dwt_delay_ms(uint32_t nms)
{
    while(nms--)
    {
        dwt_delay_us(1000);
    }
}
