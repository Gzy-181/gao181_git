#include "oled.h"
#include "delay.h"
#include "FreeRTOS.h"
#include "task.h"
#include "oled_font.h"
#include "math.h"
void oled_gpio_init(void)
{
    GPIO_InitTypeDef gpio_initstruct;

    OLED_I2C_SCL_CLK();
    OLED_I2C_SDA_CLK();
    
    gpio_initstruct.Pin = OLED_I2C_SCL_PIN;          
    gpio_initstruct.Mode = GPIO_MODE_OUTPUT_PP;             
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
    taskENTER_CRITICAL();
    uint8_t i, j;
    for(i = 0; i < 8; i++)
    {
        oled_set_cursor(0, i);
        for(j = 0; j < 128; j++)
            oled_write_data(data);
    }
    taskEXIT_CRITICAL();
}


//size: 12:�����СΪ6*12
//      16:�����СΪ8*16  
//      24:�����СΪ12*24


void oled_show_char(uint8_t x, uint8_t y, uint8_t num, uint8_t size)
{
    uint8_t i, j, page;
    
    num = num - ' ';
    page = size / 8;
    if(size % 8)
        page++;
    
    for(j = 0; j < page; j++)
    {
        oled_set_cursor(x, y + j);
        for(i = size / 2 * j; i < size /2 * (j + 1); i++)
        {
            if(size == 12)
                oled_write_data(ascii_6X12[num][i]);
            else if(size == 16)
                oled_write_data(ascii_8X16[num][i]);
            else if(size == 24)
                oled_write_data(ascii_12X24[num][i]);
                
        }
    }
}

void oled_show_string(uint8_t x, uint8_t y, char *p, uint8_t size)
{
    taskENTER_CRITICAL();
    while(*p != '\0')
    {
        oled_show_char(x, y, *p, size);
        x += size/2;
        p++;
    }
    taskEXIT_CRITICAL();
}

void oled_show_chinese(uint8_t x, uint8_t y, uint8_t N, uint8_t size)
{
    uint16_t i, j;
    for(j = 0; j < size/8; j++)
    {
        oled_set_cursor(x, y + j);
        for(i = size *j; i < size * (j + 1); i++)
        {
            if(size == 16)
                oled_write_data(chinese_16x16[N][i]);
            else if(size == 24)
                oled_write_data(chinese_24x24[N][i]);
        }
    }
}

// 清除指定行的内容
void oled_clear_line(uint8_t y, uint8_t size)
{
    taskENTER_CRITICAL();
    uint8_t i, j, page;
    
    page = size / 8;
    if(size % 8)
        page++;
    
    for(j = 0; j < page; j++)
    {
        oled_set_cursor(0, y + j);
        for(i = 0; i < 128; i++)
        {
            oled_write_data(0x00);
        }
    }
    taskEXIT_CRITICAL();
}

void oled_show_image(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t *bmp)
{
    uint8_t i, j;
    for(j = 0; j < height; j++)
    {
        oled_set_cursor(x, y + j);
        for(i = 0; i < width; i++)
            oled_write_data(bmp[width * j + i]);
    }
}

//��׼����ֲ

void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	oled_write_cmd(0xB0 | Y);					//����Yλ��
	oled_write_cmd(0x10 | ((X & 0xF0) >> 4));	//����Xλ�ø�4λ
	oled_write_cmd(0x00 | (X & 0x0F));			//����Xλ�õ�4λ
}


void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//���ù��λ�����ϰ벿��
	for (i = 0; i < 8; i++)
	{
		oled_write_data(OLED_F8x16[Char - ' '][i]);			//��ʾ�ϰ벿������
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//���ù��λ�����°벿��
	for (i = 0; i < 8; i++)
	{
		oled_write_data(OLED_F8x16[Char - ' '][i + 8]);		//��ʾ�°벿������
	}
}

uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}


void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}

void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

void OLED_ShowFloatNum(uint8_t Line, uint8_t Column, float Number, uint8_t IntLength, uint8_t FraLength)
{
    // �����������ֺ�С������
    int32_t IntPart = (int32_t)Number;  // ��������
    float FraPart = Number - IntPart;   // С������

    // ��ʾ��������
    OLED_ShowSignedNum(Line, Column, IntPart, IntLength);

    // ��ʾС����
    OLED_ShowChar(Line, Column + IntLength+1, '.');

    // ��ʾС������
    uint32_t FraPartScaled = (uint32_t)(fabs(FraPart) * OLED_Pow(10, FraLength) + 0.5);  // С�����ַŴ���������

    // ����С�����ֵ���ʾλ��
    uint8_t FraPartColumn = Column + IntLength + 2;  // С������һλ����ʼ��

    // ��ʾС�����ֵ�ÿһλ
    for (uint8_t i = 0; i < FraLength; i++)
    {
        uint8_t digit = (FraPartScaled / OLED_Pow(10, FraLength - i - 1)) % 10;  // ��ȡ��ǰλ������
        OLED_ShowChar(Line, FraPartColumn + i, digit + '0');  // ��ʾ��ǰλ������
    }
}
