#ifndef __E2PROM_H__
#define __E2PROM_H__

void e2prom_init(void);

void e2_write_bit(uint8_t address,uint8_t data);

uint8_t e2_read_data(uint8_t address);


#endif
