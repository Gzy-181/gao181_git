#include "sys.h"
#include "uart1.h"
#include "stdio.h"
#include "string.h"
#include "oled.h"

uint8_t usart_rx_buff[128];
uint8_t rx_cnt=0;
uint8_t rx_pre_cnt=0;

//*********************����2����*************************

//PA2---------TX
//PA3---------RX





UART_HandleTypeDef huart2={0};

// 自定义的UART2初始化函数
void uart1_MspInit(UART_HandleTypeDef *huart)
{
    if(huart->Instance==USART2)
    {
        __HAL_RCC_USART2_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitTypeDef gpioa_initstructer;
        gpioa_initstructer.Mode=GPIO_MODE_AF_PP;
        gpioa_initstructer.Pin=GPIO_PIN_2;//TX
        gpioa_initstructer.Pull=GPIO_PULLUP;
        gpioa_initstructer.Speed=GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA,&gpioa_initstructer);
        
        gpioa_initstructer.Pin=GPIO_PIN_3;//RX
        gpioa_initstructer.Mode=GPIO_MODE_AF_INPUT;
        HAL_GPIO_Init(GPIOA,&gpioa_initstructer);
        
        HAL_NVIC_EnableIRQ(USART2_IRQn);//ʹ�ܴ����ж�
        HAL_NVIC_SetPriority(USART2_IRQn,2,2);
        __HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);//���������ж�
        __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);//���������ж�
    }
}

void uart1_init(uint32_t baudrate)
{
    huart2.Instance=USART2;
    huart2.Init.BaudRate=baudrate;
    huart2.Init.WordLength=UART_WORDLENGTH_8B;
    huart2.Init.StopBits=UART_STOPBITS_1;
    huart2.Init.Parity=UART_PARITY_NONE;
    huart2.Init.Mode=UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl=UART_HWCONTROL_NONE;
    
    // 手动调用初始化函数
    uart1_MspInit(&huart2);
    HAL_UART_Init(&huart2);
}





void uart_clear(void)
{
    memset(usart_rx_buff,0,sizeof(usart_rx_buff));
    rx_cnt=0;
}

void USART2_IRQHandler()
{
    if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_RXNE)!=RESET)//������ռĴ�����Ϊ��
    {
        uint8_t receive_data=0;
        HAL_UART_Receive(&huart2,&receive_data,1,1000);//�������ݲ��ŵ����ݻ�����
        usart_rx_buff[rx_cnt++]=receive_data;
        
    }
     if(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE) != RESET)//���������ж�
    {
        printf("recv: %s\r\n", usart_rx_buff);
        uart_clear();
        __HAL_UART_CLEAR_IDLEFLAG(&huart2);//��������жϱ�־λ
        
    }
}

/**
 * @brief 发送符合协议的数据帧
 * @param data_type 数据类型：0x01(温度), 0x02(湿度), 0x03(气压)
 * @param data 数据内容（单精度浮点数）
 */
void uart_send_frame(uint8_t data_type, float data)
{
    uint8_t frame[9];
    uint8_t checksum = 0;
    
    // 帧头
    frame[0] = 0xAA;
    
    // 数据类型
    frame[1] = data_type;
    
    // 数据长度
    frame[2] = 0x04;
    
    // 数据内容（小端格式）
    memcpy(&frame[3], &data, 4);
    
    // 计算校验和
    for (uint8_t i = 0; i < 7; i++) {
        checksum ^= frame[i];
    }
    frame[7] = checksum;
    
    // 帧尾
    frame[8] = 0x55;
    
    // 发送数据帧
    for (uint8_t i = 0; i < 9; i++) {
        while((USART2->SR & 0X40) == 0);
        USART2->DR = frame[i];
    }
}


