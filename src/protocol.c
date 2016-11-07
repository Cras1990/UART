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
#include "GSM_Modul.h"
#include "GSM_TIM.h"
#include "gsm_adapter.h"

extern uint8_t aRx_PC_Buffer[100];

static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2,
    uint16_t BufferLength);

void protocol_Init()
{
	GSM_Modul_Init();
	MX_TIM2_Init();
	MX_TIM3_Init();
}

void protocol_Dispatcher()
{
	if (!Buffercmp("InitModul\r", aRx_PC_Buffer, COUNTOF("InitModul\r")))
	{
		at_adapter_init();
		BSP_LED_Toggle(LED5);
	}
	else
	{
		GSM_Modul_sendATCommand();
	}
}

void protocol_Handler()
{
	// wenn ein Commando vom PC aus an den uC fertig ausgelesen wurde, dann
	// kuemmert sich der Dispatcher darum, zu entscheiden, was
	// gemacht werden soll
	if (UART_GetStatus())
	{
		protocol_Dispatcher();
		UART_SetStatus(RESET);
	}
}

/**
 * @brief  Compares two buffers.
 * @param  pBuffer1, pBuffer2: buffers to be compared.
 * @param  BufferLength: buffer's length
 * @retval 0  : pBuffer1 identical to pBuffer2
 *         >0 : pBuffer1 differs from pBuffer2
 */
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2,
    uint16_t BufferLength)
{
	while (BufferLength--)
	{
		if ((*pBuffer1) != *pBuffer2)
		{
			return BufferLength;
		}
		pBuffer1++;
		pBuffer2++;
	}

	return 0;
}
