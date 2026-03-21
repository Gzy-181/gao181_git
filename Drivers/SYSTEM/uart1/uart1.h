#ifndef __USART_H__
#define __USART_H__

#include "stdio.h"
#include "sys.h"

/* ������� */
#define UART_EOK                     0      /* û�д��� */
#define UART_ERROR                   1      /* ͨ�ô��� */
#define UART_ETIMEOUT                2      /* ��ʱ���� */
#define UART_EINVAL                  3      /* �������� */

/* UART�շ������С */
#define UART1_RX_BUF_SIZE            128
#define UART1_TX_BUF_SIZE            64


void uart1_init(uint32_t baudrate);            /* ���ڳ�ʼ������ */
void uart_clear(void);
void uart_work(void);
void uart_send_frame(uint8_t data_type, float data); /* 发送符合协议的数据帧 */
#endif



