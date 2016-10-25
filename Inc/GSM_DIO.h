/*
 * GSM_DIO.h
 *
 *  Created on: 25.10.2016
 *      Author: revelo
 */

#ifndef GSM_DIO_H_
#define GSM_DIO_H_

#include "stm32f4xx_hal.h"


// ich muss ueberlegen, ob man eigene Defines fuer pins, usw. braucht.
// Momentan ist dieser Aufruf redundant, da ueber den Headerfile
// bereits die GPIO-Funktionalitaeten eingebunden sind
void GSM_DIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);



#endif /* GSM_DIO_H_ */
