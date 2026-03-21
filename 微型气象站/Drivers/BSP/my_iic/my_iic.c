#include "sys.h"
#include "delay.h"
#include "stdio.h"

// 软件 I2C 实现（bit-bang），使用 GPIOB 的 PB10 作为 SCL，PB11 作为 SDA
void iic_scl_init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef gpio_initstructer;
    gpio_initstructer.Mode=GPIO_MODE_OUTPUT_OD;
    gpio_initstructer.Pin=GPIO_PIN_10; 
    gpio_initstructer.Pull=GPIO_PULLUP;
    gpio_initstructer.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB,&gpio_initstructer);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);
}

void iic_sda_init(void) {
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef gpio = {0};
    gpio.Mode = GPIO_MODE_OUTPUT_OD;
    gpio.Pin = GPIO_PIN_11;
    gpio.Pull = GPIO_PULLUP;  // 启用内部上拉
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &gpio);
    
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
    
    // 额外措施：反复切换强化上拉
    for(int i = 0; i < 10; i++) {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
        delay_us(1);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
        delay_us(1);
    }
}

void iic_scl(uint8_t state)
{
     HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,(GPIO_PinState)state);
}

void iic_sda(uint8_t state)
{
     HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,(GPIO_PinState)state);
}

void my_iic_init(void)
{
    iic_scl_init();
    iic_sda_init();
    
    // 确保总线处于空闲状态（SDA和SCL都为高）
    iic_sda(1);
    iic_scl(1);
    delay_us(10);
}

void iic_start(void)
{
    // 确保SDA和SCL都为高
    iic_sda(1);
    iic_scl(1);
    delay_us(5);
    // SDA从高变低，产生起始信号
    iic_sda(0);
    delay_us(5);
    // SCL拉低，准备传输数据
    iic_scl(0);
    delay_us(2);
}

void iic_stop(void)
{
    iic_sda(0);      // 确保SDA为低
    delay_us(5);
    iic_scl(1);      // SCL拉高
    delay_us(5);
    iic_sda(1);      // SDA从低变高，产生停止信号
    delay_us(10);    // 确保停止信号完成，总线释放
}

// 将SDA设置为输入模式以读取总线状态
void iic_sda_in(void)
{
    GPIO_InitTypeDef gpio = {0};
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pin = GPIO_PIN_11;
    gpio.Pull = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &gpio);
}

// 将SDA设置为输出模式以控制总线
void iic_sda_out(void)
{
    GPIO_InitTypeDef gpio = {0};
    gpio.Mode = GPIO_MODE_OUTPUT_OD;
    gpio.Pin = GPIO_PIN_11;
    gpio.Pull = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &gpio);
}

uint8_t iic_read_sda(void)
{
    uint8_t num;
    iic_sda_in();   // 切换到输入模式
    delay_us(2);
    num = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11);
    iic_sda_out();  // 切换回输出模式
    delay_us(2);
    return num;
}


void iic_cpu_send_ack(uint8_t state)//  ������Ϊ���շ�����ack,
{
    
    // 在SCL为低时设置SDA
    iic_sda(state);
    delay_us(2);
    // SCL拉高，ACK/NACK被发送
    iic_scl(1);
    delay_us(5);
    // SCL拉低
    iic_scl(0);
    delay_us(2);
    
}

uint8_t iic_cpu_receive_ack(void) {
    uint8_t ack;
    
    // 先释放SDA（设置为输入模式，让上拉电阻拉高）
    iic_sda_in();   // 切换到输入模式，释放总线
    delay_us(5);    // 等待总线稳定
    
    // SCL拉高，在SCL高电平期间读取ACK
    iic_scl(1);
    delay_us(5);
    
    // 读取SDA状态（0表示ACK，1表示NACK）
    ack = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11);
    
    // SCL拉低
    iic_scl(0);
    delay_us(5);
    
    // 切换回输出模式
    iic_sda_out();
    delay_us(2);
    
    return ack;  // 返回1表示NACK（错误），返回0表示ACK（正常）
}

void iic_write_bit(uint8_t data)
{
    for(int i=0;i<8;i++)
    {
        // 在SCL为低时设置SDA
        iic_sda((data & (0x80 >> i)) ? 1 : 0);//iic发送数据时，从高位到低位发送,从高到低获取每一位
        delay_us(2);
        // SCL拉高，数据被采样
        iic_scl(1);
        delay_us(5);
        // SCL拉低
        iic_scl(0);
        delay_us(2);
    }
}

uint8_t iic_receive_bit(void)
{
    uint8_t bite = 0;
    
    // 释放SDA，切换到输入模式
    iic_sda_in();
    delay_us(2);
    for(int i=0;i<8;i++)
    {
       iic_scl(1);						//�ͷ�SCL����������SCL�ߵ�ƽ�ڼ��ȡSDA
        // 读取SDA
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11)) {   //
            bite |= (0x80 >> i);
        }
        													
		iic_scl(0);		     //scl����ʱ�ӵ�����ʱ�ӿ����ǲ����ɵ�
    }
    // 切换回输出模式
    iic_sda_out();
    delay_us(2);
    
    return bite;
}



