#ifndef __OLED_H__
#define __OLED_H__

#include "sys.h"

/*�˲���ֵ���������жϣ��������ڼ�������ַ�ƫ�ƣ�Ĭ��ֵΪ�������ؿ���*/
#define OLED_8X16				8
#define OLED_6X8				6

/*IsFilled������ֵ*/
#define OLED_UNFILLED			0
#define OLED_FILLED				1

#define OLED_I2C_SCL_CLK()      __HAL_RCC_GPIOB_CLK_ENABLE();
#define OLED_I2C_SCL_PORT       GPIOB
#define OLED_I2C_SCL_PIN        GPIO_PIN_8

#define OLED_I2C_SDA_CLK()      __HAL_RCC_GPIOB_CLK_ENABLE();
#define OLED_I2C_SDA_PORT       GPIOB
#define OLED_I2C_SDA_PIN        GPIO_PIN_9

#define OLED_SCL_RESET()        HAL_GPIO_WritePin(OLED_I2C_SCL_PORT, OLED_I2C_SCL_PIN, GPIO_PIN_RESET)
#define OLED_SCL_SET()          HAL_GPIO_WritePin(OLED_I2C_SCL_PORT, OLED_I2C_SCL_PIN, GPIO_PIN_SET)

#define OLED_SDA_RESET()        HAL_GPIO_WritePin(OLED_I2C_SDA_PORT, OLED_I2C_SDA_PIN, GPIO_PIN_RESET)
#define OLED_SDA_SET()          HAL_GPIO_WritePin(OLED_I2C_SDA_PORT, OLED_I2C_SDA_PIN, GPIO_PIN_SET)

void oled_init(void);
void oled_write_cmd(uint8_t cmd);
void oled_write_data(uint8_t data);
void oled_fill(uint8_t data);
void oled_show_char(uint8_t x, uint8_t y, uint8_t num, uint8_t size);
void oled_show_string(uint8_t x, uint8_t y, char *p, uint8_t size);
void oled_show_chinese(uint8_t x, uint8_t y, uint8_t N, uint8_t size);
void oled_show_image(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t *bmp);
//**********
void OLED_SetCursor(uint8_t Y, uint8_t X);

void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);

void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);


uint32_t OLED_Pow(uint32_t X, uint32_t Y);



void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

void OLED_ShowFloatNum(uint8_t Line, uint8_t Column, float Number, uint8_t IntLength, uint8_t FraLength);

void oled_clear_line(uint8_t y, uint8_t size);

#endif

