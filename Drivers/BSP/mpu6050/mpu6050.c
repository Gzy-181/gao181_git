#include "sys.h"
#include "my_iic.h"
#include "mpu6050.h"
#include "mpu6050_register.h"


#define MPU6050_ADDRESS     0xD0

#define MPU6050_IIC_PLUS

void mpu6050_write_single(uint8_t address,uint8_t data)
{
    iic_start();
    iic_write_bit(MPU6050_ADDRESS);
    iic_cpu_receive_ack();
    iic_write_bit(address);
    iic_cpu_receive_ack();
    iic_write_bit(data);
    iic_cpu_receive_ack();
    iic_stop();
}



void mpu6050_init(void)
{
    my_iic_init();
    										//�ȳ�ʼ���ײ��I2C
	
	/*MPU6050�Ĵ�����ʼ������Ҫ����MPU6050�ֲ�ļĴ����������ã��˴��������˲�����Ҫ�ļĴ���*/
	mpu6050_write_single(MPU6050_PWR_MGMT_1, 0x01);		//��Դ�����Ĵ���1��ȡ��˯��ģʽ��ѡ��ʱ��ԴΪX��������
	mpu6050_write_single(MPU6050_PWR_MGMT_2, 0x00);		//��Դ�����Ĵ���2������Ĭ��ֵ0���������������
	mpu6050_write_single(MPU6050_SMPLRT_DIV, 0x09);		//�����ʷ�Ƶ�Ĵ��������ò�����
	mpu6050_write_single(MPU6050_CONFIG, 0x06);			//���üĴ���������DLPF
	mpu6050_write_single(MPU6050_GYRO_CONFIG, 0x10);	//�����ǽ��ٶ��������üĴ�����ѡ������Ϊ��X��/s
	mpu6050_write_single(MPU6050_ACCEL_CONFIG, 0x10);   //���ٶ��������üĴ�����ѡ������Ϊ��N*g��/s
}



uint8_t mpu6050_read_single(uint8_t address)
{
    uint8_t datas;
    iic_start();
    iic_write_bit(MPU6050_ADDRESS);
    iic_cpu_receive_ack();//�ӻ�Ӧ��λ�����ɺ���
    iic_write_bit(address);
    iic_cpu_receive_ack();
    
    iic_start();
    iic_write_bit(0xD1);
    iic_cpu_receive_ack();
    datas=iic_receive_bit();
    iic_cpu_send_ack(1);
    iic_stop();
    return datas;
}

void mpu6050_getdata(int16_t *accx,int16_t *accy,int16_t *accz,int16_t *g_x,int16_t *g_y,int16_t *g_z)
{
    int16_t temp1,temp2;
    temp1=mpu6050_read_single(MPU6050_ACCEL_XOUT_H);
    temp2=mpu6050_read_single(MPU6050_ACCEL_XOUT_L);
    *accx=(temp1<<8)|temp2;
    temp1=mpu6050_read_single(MPU6050_ACCEL_YOUT_H);
    temp2=mpu6050_read_single(MPU6050_ACCEL_YOUT_L);
    *accy=(temp1<<8)|temp2;
    
    temp1=mpu6050_read_single(MPU6050_ACCEL_ZOUT_H);
    temp2=mpu6050_read_single(MPU6050_ACCEL_ZOUT_L);
    *accz=(temp1<<8)|temp2;
    
    temp1=mpu6050_read_single(MPU6050_GYRO_XOUT_H);
    temp2=mpu6050_read_single(MPU6050_GYRO_XOUT_L);
    *g_x=(temp1<<8)|temp2;
    
    temp1=mpu6050_read_single(MPU6050_GYRO_YOUT_H);
    temp2=mpu6050_read_single(MPU6050_GYRO_YOUT_L);
    *g_y=(temp1<<8)|temp2;
    
    temp1=mpu6050_read_single(MPU6050_GYRO_ZOUT_H);
    temp2=mpu6050_read_single(MPU6050_GYRO_ZOUT_L);
    *g_z=(temp1<<8)|temp2;
}


void mpu_get_only_acc_data(int16_t *accx,int16_t *accy,int16_t *accz)
{
	int16_t temp1,temp2;
    temp1=mpu6050_read_single(MPU6050_ACCEL_XOUT_H);
    temp2=mpu6050_read_single(MPU6050_ACCEL_XOUT_L);
    *accx=(temp1<<8)|temp2;
    temp1=mpu6050_read_single(MPU6050_ACCEL_YOUT_H);
    temp2=mpu6050_read_single(MPU6050_ACCEL_YOUT_L);
    *accy=(temp1<<8)|temp2;
    
    temp1=mpu6050_read_single(MPU6050_ACCEL_ZOUT_H);
    temp2=mpu6050_read_single(MPU6050_ACCEL_ZOUT_L);
    *accz=(temp1<<8)|temp2;
}


void mpu_get_only_gcc_data(int16_t *g_x,int16_t *g_y,int16_t *g_z)
{
	int16_t temp1,temp2;
	temp1=mpu6050_read_single(MPU6050_GYRO_XOUT_H);
    temp2=mpu6050_read_single(MPU6050_GYRO_XOUT_L);
    *g_x=(temp1<<8)|temp2;
    
    temp1=mpu6050_read_single(MPU6050_GYRO_YOUT_H);
    temp2=mpu6050_read_single(MPU6050_GYRO_YOUT_L);
    *g_y=(temp1<<8)|temp2;
    
    temp1=mpu6050_read_single(MPU6050_GYRO_ZOUT_H);
    temp2=mpu6050_read_single(MPU6050_GYRO_ZOUT_L);
    *g_z=(temp1<<8)|temp2;
}

//new added-----by gzy
#ifdef MPU6050_IIC_PLUS
//һ����д����Ĵ���
void mpu6050_write_some_registers(uint8_t address,uint8_t nums,uint8_t*data)
{
    uint8_t i=0;
    iic_start();
    iic_write_bit(MPU6050_ADDRESS);
    iic_cpu_receive_ack();
    iic_write_bit(address);
    iic_cpu_receive_ack();
    
    for(i=0;i<nums;i++)
    {
    iic_write_bit(data[i]);
    iic_cpu_receive_ack();
    iic_stop();
    }

}


uint8_t* mpu6050_read_some_registers(uint8_t address,uint8_t nums)
{
    uint8_t* datas;
    iic_start();
    iic_write_bit(MPU6050_ADDRESS);
    iic_cpu_receive_ack();//�ӻ�Ӧ��λ�����ɺ���
    iic_write_bit(address);
    iic_cpu_receive_ack();
    
    iic_start();
    iic_write_bit(0xD1);
    iic_cpu_receive_ack();
    
    for(int i=0;i<nums;i++)
    {
        datas[i]=iic_receive_bit();
        iic_cpu_send_ack(0);
        
    }
    iic_cpu_send_ack(1);
    iic_stop();
    return datas;
}

#endif
