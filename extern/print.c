

#include "gpio.h"


int put(int ch)
//��ʹ��`printf`�ȱ�׼�������ʱ��`printf`����õײ��`fputc`�������������ַ���
// ���`fputc`������`printf`����ַ�������ʵ�֣������ַ����͵�ָ����Ӳ���豸��������USART1���ڣ���
{
    while((USART1->SR & 0X40) == 0);
        
    USART1->DR = (uint8_t)ch;
    return ch;
}

void print(const char* str)
{
    while(*str)
    {
        put(*str++);
    }
}

void print_int(int value) {
    char buffer[20]; // �㹻�洢һ���������ַ�����ʾ
    snprintf(buffer, sizeof(buffer), "%d", value); // ��ʽ������Ϊ�ַ���
    for (int i = 0; buffer[i] != '\0'; i++) {
        put(buffer[i]); // ����ַ����
    }
}

void print_float(float value) {
    char buffer[20]; // �㹻�洢һ�����������ַ�����ʾ
    snprintf(buffer, sizeof(buffer), "%.2f", value); // ��ʽ��������Ϊ�ַ�����������λС��
    for (int i = 0; buffer[i] != '\0'; i++) {
        put(buffer[i]); // ����ַ����
    }
}
