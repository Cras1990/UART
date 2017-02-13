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
uint8_t GSM_TIM_get_osTicks(void);
void GSM_TIM_reset_osTicks(void);
void GSM_TIM_set_osTicks(void);


#endif /* GSM_TIM_H_ */
