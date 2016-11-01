/*
 * GSM_Modul.c
 *
 *  Created on: 27.10.2016
 *      Author: revelo
 */

#include "GSM_Modul.h"
#include "GSM_DIO.h"
#include "GSM_UART.h"
#include "led_button.h"

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
	BSP_LED_Toggle(LED5);
}

void GSM_Modul_sendATCommand()
{
	UARTX_Transmit(aRx_PC_Buffer,ptr_tx_x_count);
}

void GSM_Modul_Init()
{
	UARTX_Init();
	UART_PC_Init();
}

uint8_t GSM_Modul_txctrl()
{
  if ( GSM_DIO_ReadPin(GPIOD, GPIO_PIN_2) )
      return 0;
  else
      return 1;
}

void GSM_Modul_rxctrl(GPIO_PinState PinState)
{
  if ( PinState )
  	GSM_DIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET);
  else
  	GSM_DIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);
}
