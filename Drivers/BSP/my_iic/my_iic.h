#ifndef __MY_IIC_H__
#define __MY_IIC_H__

void iic_scl_init(void);

void iic_sda_init(void);

void iic_scl(uint8_t state);

void iic_sda(uint8_t state);

void iic_sda_in(void);
void iic_sda_out(void);

void my_iic_init(void);


void iic_start(void);


void iic_stop(void);


uint8_t iic_read_sda(void);

void iic_write_bit(uint8_t data);

void iic_cpu_send_ack(uint8_t state);

uint8_t iic_cpu_receive_ack(void);

uint8_t iic_receive_bit(void);


#endif
