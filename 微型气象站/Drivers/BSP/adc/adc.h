#ifndef __ADC_H
#define __ADC_H	

#include "sys.h"


#define Battery_Ch 8		//Í¨µÀ8
#define u16 uint16_t
#define u8 uint8_t


void MX_ADC1_Init(void);
u16 Get_Adc(u8 ch);
float Get_battery_volt(void);  





#endif 


