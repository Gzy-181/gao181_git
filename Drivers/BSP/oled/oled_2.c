#include "oled_2.h"
#include "sys.h"
void oled_gpio_init(void)
{
    GPIO_InitTypeDef gpio_initstruct;

    OLED_I2C_SCL_CLK();
    OLED_I2C_SDA_CLK();
    
    gpio_initstruct.Pin = OLED_I2C_SCL_PIN;          
    gpio_initstruct.Mode = GPIO_MODE_OUTPUT_OD;             
    gpio_initstruct.Pull = GPIO_PULLUP;                     
    gpio_initstruct.Speed = GPIO_SPEED_FREQ_HIGH;           
    HAL_GPIO_Init(OLED_I2C_SCL_PORT, &gpio_initstruct);
    
    gpio_initstruct.Pin = OLED_I2C_SDA_PIN;          
    HAL_GPIO_Init(OLED_I2C_SDA_PORT, &gpio_initstruct);
}

void oled_i2c_start(void)
{
    OLED_SCL_SET();
    OLED_SDA_SET();
    OLED_SDA_RESET();
    OLED_SCL_RESET();
}

void oled_i2c_stop(void)
{
    OLED_SCL_SET();
    OLED_SDA_RESET();
    OLED_SDA_SET();
}

void oled_i2c_ack(void)
{
    OLED_SCL_SET();
    OLED_SCL_RESET();
}

void oled_i2c_write_byte(uint8_t data)
{
    uint8_t i, tmp;
    tmp = data;
    
    for(i = 0; i < 8; i++)
    {
        if((tmp & 0x80) == 0x80)
            OLED_SDA_SET();
        else
            OLED_SDA_RESET();
        tmp = tmp << 1;
        OLED_SCL_SET();
        OLED_SCL_RESET();
    }
}

void oled_write_cmd(uint8_t cmd)
{
    oled_i2c_start();
    oled_i2c_write_byte(0x78);
    oled_i2c_ack();
    oled_i2c_write_byte(0x00);
    oled_i2c_ack();
    oled_i2c_write_byte(cmd);
    oled_i2c_ack();
    oled_i2c_stop();
}

void oled_write_data(uint8_t data)
{
    oled_i2c_start();
    oled_i2c_write_byte(0x78);
    oled_i2c_ack();
    oled_i2c_write_byte(0x40);
    oled_i2c_ack();
    oled_i2c_write_byte(data);
    oled_i2c_ack();
    oled_i2c_stop();
}

void oled_init(void)
{
    oled_gpio_init();
    
    delay_ms(100);
    
    oled_write_cmd(0xAE);    //������ʾ����/�رգ�0xAE�رգ�0xAF����

    oled_write_cmd(0xD5);    //������ʾʱ�ӷ�Ƶ��/����Ƶ��
    oled_write_cmd(0x80);    //0x00~0xFF

    oled_write_cmd(0xA8);    //���ö�·������
    oled_write_cmd(0x3F);    //0x0E~0x3F

    oled_write_cmd(0xD3);    //������ʾƫ��
    oled_write_cmd(0x00);    //0x00~0x7F

    oled_write_cmd(0x40);    //������ʾ��ʼ�У�0x40~0x7F

    oled_write_cmd(0xA1);    //�������ҷ���0xA1������0xA0���ҷ���

    oled_write_cmd(0xC8);    //�������·���0xC8������0xC0���·���

    oled_write_cmd(0xDA);    //����COM����Ӳ������
    oled_write_cmd(0x12);

    oled_write_cmd(0x81);    //���öԱȶ�
    oled_write_cmd(0xCF);    //0x00~0xFF

    oled_write_cmd(0xD9);    //����Ԥ�������
    oled_write_cmd(0xF1);

    oled_write_cmd(0xDB);    //����VCOMHȡ��ѡ�񼶱�
    oled_write_cmd(0x30);

    oled_write_cmd(0xA4);    //����������ʾ��/�ر�

    oled_write_cmd(0xA6);    //��������/��ɫ��ʾ��0xA6������0xA7��ɫ

    oled_write_cmd(0x8D);    //���ó���
    oled_write_cmd(0x14);

    oled_write_cmd(0xAF);    //������ʾ
}

void oled_set_cursor(uint8_t x, uint8_t y)
{
    oled_write_cmd(0xB0 + y);
    oled_write_cmd((x & 0x0F) | 0x00);
    oled_write_cmd(((x & 0xF0) >> 4) | 0x10);
}

void oled_fill(uint8_t data)
{
    uint8_t i, j;
    for(i = 0; i < 8; i++)
    {
        oled_set_cursor(0, i);
        for(j = 0; j < 128; j++)
            oled_write_data(data);
    }
}