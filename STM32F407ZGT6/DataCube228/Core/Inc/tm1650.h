/*
 * tm1650.h
 *
 *  Created on: Sep 26, 2021
 *      Author: TanXY
 */

#ifndef INC_TM1650_H_
#define INC_TM1650_H_

#include "stdint.h"
#include "iic.h"

void TM1650_Wr_RAM(uint8_t Address, uint8_t Data);
void open_display(void);
void set_number(int num);

#endif /* INC_TM1650_H_ */
