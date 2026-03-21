#ifndef __DHT22_H__
#define __DHT22_H__

#include "sys.h"

/* 返回值定义 */
#define DHT22_OK                 0
#define DHT22_ERR_RESPONSE     -1 /* DHT 未响应或响应超时 */
#define DHT22_ERR_DATA_TIMEOUT -2 /* 读取数据位超时 */
#define DHT22_ERR_CHECKSUM     -3 /* 校验和错误 */
#define DHT22_ERR_READ_FAIL    -4 /* 读取失败（通用） */

/* 采样标志 */
extern uint8_t dht22_sample_flag;

/* 初始化（配置引脚为输入上拉） */
int dht22_init(void);

/* 初始化定时器（2秒中断一次） */
void dht22_timer_init(void);

/* 读取温湿度，temperature (℃)，humidity (%%)，返回 DHT22_OK 或 负错误码 */
int dht22_read(float *temperature, float *humidity);

/* 直接读取5字节原始返回数据（湿度高8位 湿度低8位 温度高8位 温度低8位 校验） */
int dht22_read_raw_bytes(uint8_t buf[5]);

/* 把错误码转换为可读字符串（指针为静态存储） */
const char *dht22_strerror(int err);

/* 调试：采样 DHT 数据引脚若干次并把 0/1 写入 out（需要 pre-allocated buffer），
   返回实际采样数或负值 on error。out 最多写入 maxlen 字符，加上终止符。 */
int dht22_dump_pin(char *out, int maxlen, int samples, int interval_us);

/* 高层封装：读取温湿度并更新 OLED/串口（只输出结果或错误） */
void dht22_update_display(void);

#endif
