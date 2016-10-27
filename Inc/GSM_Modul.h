/*
 * GSM_Modul.h
 *
 *  Created on: 27.10.2016
 *      Author: revelo
 */

#ifndef GSM_MODUL_H_
#define GSM_MODUL_H_

#include "stm32f4xx_hal.h"

void GSM_Modul_ConfigComm(void);
void sendATCommand(const uint8_t *command);
void GSM_Modul_Init(void);


#endif /* GSM_MODUL_H_ */
