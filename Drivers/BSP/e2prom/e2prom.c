#include "sys.h"
#include "my_iic.h"
#include "oled.h"
void e2prom_init(void)
{
    my_iic_init();
}

void e2_write_bit(uint8_t address,uint8_t data)
{
    iic_start();
    iic_write_bit(0xA0);
    if(iic_cpu_receive_ack()==0)
    {
        iic_write_bit(address);
        iic_cpu_receive_ack();
        iic_write_bit(data);
        iic_stop();
    }
    else
    {
        oled_show_string(3,3,"write_error",12);
        return;
    }
}

uint8_t e2_read_data(uint8_t address)
{
    uint8_t  data1;
    iic_start();
    iic_write_bit(0xA0);
    if(iic_cpu_receive_ack()==0)
    {
        
        iic_write_bit(address);
        iic_cpu_receive_ack();
        iic_start();
        iic_write_bit(0xA1);
        data1=iic_receive_bit();
        iic_stop();
        return data1;
    }
    else{
        oled_show_string(3,3,"read_error",12);
        return 1;
    }
}

