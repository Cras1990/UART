/*
 * protocol.c
 *
 *  Created on: 25.10.2016
 *      Author: revelo
 */


#include "protocol.h"
#include "GSM_UART.h"
#include "GSM_DIO.h"
#include "led_button.h"



void protocol_Init()
{
	//UARTX_Init();
	UART_PC_Init();
}

void protocol_Dispatcher()
{

}

