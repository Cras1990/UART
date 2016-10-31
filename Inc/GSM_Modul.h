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
void GSM_Modul_sendATCommand(void);
void GSM_Modul_Init(void);

uint8_t GSM_Modul_txctrl(void);
void GSM_Modul_rxctrl(GPIO_PinState PinState);

#endif /* GSM_MODUL_H_ */
