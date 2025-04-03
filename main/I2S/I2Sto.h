#ifndef _I2STO_H_
#define _I2STO_H_

#include "driver/i2s_std.h"

//事件处理函数
void my_i2s_init();
esp_err_t my_i2s_read(int16_t *buffer, int length, size_t *bytes_read) ;
#endif