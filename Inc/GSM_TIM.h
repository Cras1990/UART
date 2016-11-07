/*
 * GSM_TIM.h
 *
 *  Created on: 18.10.2016
 *      Author: revelo
 */

#ifndef GSM_TIM_H_
#define GSM_TIM_H_

#include "stm32f4xx_hal.h"

void MX_TIM2_Init(void);
void MX_TIM3_Init(void);
uint8_t get_osTicks(void);
void reset_osTicks(void);


#endif /* GSM_TIM_H_ */
