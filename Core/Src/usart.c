
#include "usart.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

UART_HandleTypeDef huart={0};

/* USART2 init function */



void MX_USART2_UART_Init(void)
{
    huart.Instance=USART2;
    huart.Init.BaudRate=115200;
    huart.Init.WordLength=UART_WORDLENGTH_8B;
    huart.Init.StopBits=UART_STOPBITS_1;
    huart.Init.Parity=UART_PARITY_NONE;
    huart.Init.Mode=UART_MODE_TX_RX;
    huart.Init.HwFlowCtl=UART_HWCONTROL_NONE;
  if (HAL_UART_Init(&huart) != HAL_OK)
  {
    Error_Handler();
      
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{


  if(uartHandle->Instance==USART2)
  {


    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
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

  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}



/* USER CODE BEGIN 1 */


#include "gpio.h"
#include "stdio.h"
#include "stm32f1xx_hal.h"

//*********************USART2����*************************

//PA2---------TX
//PA3---------RX

/* USER CODE BEGIN 1 */

// 禁用半主机模式，防止触发 BKPT 断点
#pragma import(__use_no_semihosting)

// 定义标准库需要的 FILE 结构
struct __FILE {
  int handle;
};
FILE __stdout;
FILE __stdin;
FILE __stderr;

// 实现 _sys_open 桩函数（直接返回成功，避免 BKPT）
int _sys_open(const char *name, int openmode) {
  return 1; // 返回非负数表示“文件打开成功”
}

// 实现 _sys_close 桩函数
int _sys_close(int fh) {
  return 0;
}

// 实现 _sys_read 桩函数
int _sys_read(int fh, char *buf, size_t count) {
  return 0;
}

// 实现 _sys_lseek 桩函数
int _sys_lseek(int fh, long offset, int whence) {
  return 0;
}

// 实现 _sys_istty 桩函数
int _sys_istty(int fh) {
  return 1; // 告诉标准库这是终端设备
}

// 实现 _sys_flen 桩函数
int _sys_flen(int fh) {
  return 0;
}

// 实现 _sys_remove 桩函数
int _sys_remove(const char *name) {
  return -1;
}

// 实现 _sys_rename 桩函数
int _sys_rename(const char *oldname, const char *newname) {
  return -1;
}

// 实现 _sys_exit 桩函数（死循环防止程序跑飞）
void _sys_exit(int status) {
  while (1);
}

// 你的原有串口输出函数（保留不动）
int put(int ch)
{
    while((USART2->SR & 0X40) == 0);
    USART2->DR = (uint8_t)ch;
    return ch;
}

// 重写 fputc，让 printf 直接调用你的 put 函数
int fputc(int ch, FILE *f)
{
    return put(ch);
}

void print(const char* str)
{
    while(*str)
    {
        put(*str++);
    }
}

void print_int(int value) {
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%d", value);
    for (int i = 0; buffer[i] != '\0'; i++) {
        put(buffer[i]);
    }
}

void print_float(float value) {
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%.2f", value);
    for (int i = 0; buffer[i] != '\0'; i++) {
        put(buffer[i]);
    }
}

/* USER CODE END 1 */
