#ifndef __MPU6050_H__
#define __MPU6050_H__



void mpu6050_init(void);


void mpu6050_write_single(uint8_t address,uint8_t data);


uint8_t mpu6050_read_single(uint8_t address);

void mpu6050_getdata(int16_t *accx,int16_t *accy,int16_t *accz,int16_t *g_x,int16_t *g_y,int16_t *g_z);

void mpu_get_only_acc_data(int16_t *accx,int16_t *accy,int16_t *accz);


void mpu_get_only_gcc_data(int16_t *g_x,int16_t *g_y,int16_t *g_z);


#endif
