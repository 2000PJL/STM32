#ifndef _DELAY_H_
#define _DELAY_H_

#include "tim.h"
#include "main.h"

/* 延时所用的通用定时器 保证分频后为1Mhz*/
#define DELAY_TIME htim6

void Delay_ms(uint16_t ms);
/* 最大 65535us 没正经考虑 不太精确 反正别超过这个数 */
void Delay_us(uint16_t us);


#endif

