#include "sys.h"
#include "uart1.h"
#include "stdio.h"
#include "string.h"
#include "oled.h"

uint8_t usart_rx_buff[128];
uint8_t rx_cnt=0;
uint8_t rx_pre_cnt=0;

//*********************串口2接线*************************

//PA2---------TX
//PA3---------RX





UART_HandleTypeDef uart_handle={0};
void uart1_init(uint32_t baudrate)
{
    uart_handle.Instance=USART2;
    uart_handle.Init.BaudRate=baudrate;
    uart_handle.Init.WordLength=UART_WORDLENGTH_8B;
    uart_handle.Init.StopBits=UART_STOPBITS_1;
    uart_handle.Init.Parity=UART_PARITY_NONE;
    uart_handle.Init.Mode=UART_MODE_TX_RX;
    uart_handle.Init.HwFlowCtl=UART_HWCONTROL_NONE;
    HAL_UART_Init(&uart_handle);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
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
        
        HAL_NVIC_EnableIRQ(USART2_IRQn);//使能串口中断
        HAL_NVIC_SetPriority(USART2_IRQn,2,2);
        __HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);//开启接收中断
        __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);//开启空闲中断
    }
}





void uart_clear(void)
{
    memset(usart_rx_buff,0,sizeof(usart_rx_buff));
    rx_cnt=0;
}

void USART2_IRQHandler()
{
    if(__HAL_UART_GET_FLAG(&uart_handle,UART_FLAG_RXNE)!=RESET)//如果接收寄存器不为零
    {
        uint8_t receive_data=0;
        HAL_UART_Receive(&uart_handle,&receive_data,1,1000);//接收数据并放到数据缓冲区
        usart_rx_buff[rx_cnt++]=receive_data;
        
    }
     if(__HAL_UART_GET_FLAG(&uart_handle, UART_FLAG_IDLE) != RESET)//触发空闲中断
    {
        printf("recv: %s\r\n", usart_rx_buff);
        uart_clear();
        __HAL_UART_CLEAR_IDLEFLAG(&uart_handle);//清除空闲中断标志位
        
    }
}


