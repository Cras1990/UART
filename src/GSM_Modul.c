/*
 * GSM_Modul.c
 *
 *  Created on: 27.10.2016
 *      Author: revelo
 */

#include "GSM_Modul.h"
#include "GSM_DIO.h"
#include "GSM_UART.h"

extern uint8_t aRx_PC_Buffer[100];
extern __IO uint8_t ptr_tx_x_count;


void GSM_Modul_ConfigComm()
{
	//PWRKEY wird dadurch angesteuert
	GSM_DIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_Delay(100);
	GSM_DIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_Delay(100);
	GSM_DIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_Delay(12500);
}

void sendATCommand()
{
	UARTX_Transmit(aRx_PC_Buffer,ptr_tx_x_count);
}

void GSM_Modul_Init()
{
	UARTX_Init();
	UART_PC_Init();
}
