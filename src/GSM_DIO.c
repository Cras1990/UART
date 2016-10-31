/*
 * GSM_DIO.c
 *
 *  Created on: 25.10.2016
 *      Author: revelo
 */

#include "GSM_DIO.h"

void GSM_DIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
}


uint8_t GSM_DIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	return HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
}

