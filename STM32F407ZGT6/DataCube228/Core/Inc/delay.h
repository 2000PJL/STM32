/*
 * delay.h
 *
 *  Created on: Sep 27, 2021
 *      Author: TanXY
 */

#ifndef INC_DELAY_H_
#define INC_DELAY_H_

#include "tim.h"
#include "stdint.h"

#define DELAY_TIME htim6

void delay_us(uint16_t us);
void delay_ms(uint16_t ms);

#endif /* INC_DELAY_H_ */
