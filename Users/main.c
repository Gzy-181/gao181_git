#include "sys.h"
#include "delay.h"
#include "oled.h"
#include "led.h"
#include "string.h"
#include "uart1.h"
#include "stdio.h"
#include "bsp_raindrop.h"
#include "dht22.h"
#include "bh1750.h"
#include "mpu6050.h"
#include "my_iic.h"
#include "bmp280.h"

int32_t num1=0;
int fputc(int ch, FILE *f)
{
    while((USART2->SR & 0X40) == 0);
    USART2->DR = (uint8_t)ch;
    return ch;
}

int main(void)
{
    HAL_Init();
    stm32_clock_init(RCC_PLL_MUL9);
    SystemCoreClockUpdate();

    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);
    oled_init();
    oled_fill(0x00);

    uart1_init(115200);

    /* DHT22 init */
    dht22_init();
    my_iic_init();
    bmp280_init();
    bsp_raindrop_analog_init();
    bh1750_init();
    delay_ms(100);

    while (1)
    {

        dht22_update_display();//温湿度 

        bh1750_update_display();//光照

        bmp280_update_display();//气压

        bsp_raindrop_update_display();//雨滴

        bsp_raindrop_get_percent();

         /* 根据最新的 AO 百分比计算等级并显示 */

        delay_ms(500); // 延时500ms
        
    }
}





