#include "GSM_UART.h"
#include "led_button.h"
#include <string.h>


/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef UartHandle_X;
UART_HandleTypeDef UartHandle_PC;

uint8_t aTxBuffer[100];
uint8_t aRxBuffer[100];
uint8_t aRx_PC_Buffer[100];
__IO uint8_t ptr_tx_pc_count = 0;
__IO uint8_t ptr_rx_pc_count = 0;
__IO uint8_t aRx_PC_Buffer_dummy;
__IO uint8_t aRx_X_Buffer_dummy;
//#PC
//__IO uint8_t dataPCready = 0;
__IO ITStatus UartReady = RESET;

extern void Error_Handler(void);

void UARTPC_SaveChar(void);
void UART_Handler(UART_HandleTypeDef *UartHandle);



void GSM_ConfigComm()
{
	//HAL_Delay(2000);
//	UARTX_Transmit((uint8_t*) aTxBuffer, 2);
//	UARTX_Receive((uint8_t*) aRxBuffer, 22);
//	UARTX_Transmit((uint8_t*) aTxBuffer, 2);
//	UARTX_Receive((uint8_t*) aRxBuffer, 6);
//	UARTX_Transmit((uint8_t*) aTxBuffer, 2);
//	UARTX_Transmit((uint8_t*) aTxBuffer, 2);
//	UARTX_Receive((uint8_t*) aRxBuffer, 6);
//	UARTX_Transmit((uint8_t*) aTxBuffer, 2);
//	UARTX_Receive((uint8_t*) aRxBuffer, 6);
//	while (Buffercmp(str2comp, aRxBuffer, 6))
//	{
//		UARTX_Transmit((uint8_t*) aTxBuffer, 2);
//		UARTX_Receive((uint8_t*) aRxBuffer, 20);
//	}

}

void sendATCommand(const char *command)
{

}


void UARTX_Init()
{

	UartHandle_X.Instance = USARTx;

	UartHandle_X.Init.BaudRate = 9600;
	UartHandle_X.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle_X.Init.StopBits = UART_STOPBITS_1;
	UartHandle_X.Init.Parity = UART_PARITY_NONE;
	UartHandle_X.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UartHandle_X.Init.Mode = UART_MODE_TX_RX;
	UartHandle_X.Init.OverSampling = UART_OVERSAMPLING_16;

	if (HAL_UART_Init(&UartHandle_X) != HAL_OK)
	{
		Error_Handler();
	}

	//Init pwrkey pin
	UARTX_GSM_PinsInit();

	/*##-4- Put UART peripheral in reception process ###########################*/
	if (HAL_UART_Receive_IT(&UartHandle_X, &aRx_X_Buffer_dummy, 1) != HAL_OK)
	{
		Error_Handler();
	}
}

//PWRKEY wird gesetzt
void UARTX_GSM_PinsInit()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIOC clock */
	__HAL_RCC_GPIOE_CLK_ENABLE()
	;

	/* Configure PA0 pin as input floating */
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Pin = GPIO_PIN_4;
	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);
}



void UARTX_Transmit()
{
	/*##-2- Start the transmission process #####################################*/
	/* While the UART in reception process, user can transmit data through
	 "aTxBuffer" buffer */
//	if (HAL_UART_Transmit_IT(&UartHandle_X, aRx_PC_Buffer, ptr_tx_pc_count) != HAL_OK) // Dieser Teil sollte durch Praeproz.Direktive ausgemacht werden
	// wenn der PC keine Anweisungen an Modul schicken soll
	if (HAL_UART_Transmit_IT(&UartHandle_X, aRxBuffer, ptr_tx_pc_count) != HAL_OK)
	{
		Error_Handler();
	}
}


void UART_Handler(UART_HandleTypeDef *UartHandle)
{
	if (UartHandle->Instance == USART2)
	{
		// start again the uart receive process
		if (HAL_UART_Receive_IT(&UartHandle_PC, &aRx_PC_Buffer_dummy, 1) != HAL_OK)
		{
			Error_Handler();
		}
	} else
	{
		// start again the uart receive process
		if (HAL_UART_Receive_IT(&UartHandle_X, &aRx_X_Buffer_dummy, 1) != HAL_OK)
		{
			Error_Handler();
		}
	}
}

/**
 * @brief  Rx Transfer completed callback
 * @param  UartHandle: UART handle
 * @note   This example shows a simple way to report end of IT Rx transfer, and
 *         you can add your own implementation.
 * @retval None
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	if (UartHandle->Instance == USARTx)
	{
		BSP_LED_Toggle(LED4);
	} else if (UartHandle->Instance == USART2)
	{
		// die Reihenfolge, wie folgende Aufrufe durchgeführt werden, spielt eine Rolle, also nicht vertauschen
		//speichere char in empfangsbuffer vom pc
		UARTPC_SaveChar();
		if (ptr_rx_pc_count == COUNTOF(aRx_PC_Buffer) || aRx_PC_Buffer_dummy == '\r')
		{
			// copy the number of bytes to send to remote device
			ptr_tx_pc_count = ptr_rx_pc_count;
			// reset the pointer to the pc_rx_buffer
			ptr_rx_pc_count = 0;
			// send the received command from pc to remote device
			UARTPC_Transmit();
			memcpy(aRxBuffer,aRx_PC_Buffer,ptr_tx_pc_count);
			UARTX_Transmit();
			/* Turn LED3 on: Transfer in reception process is correct */
			BSP_LED_Toggle(LED3);
			// data ready to be readen
			UART_SetStatus(SET);
		}
//		UART_Handler(UartHandle);
	}
	// reactivate on the receive process once again
	UART_Handler(UartHandle);
}

/**
 * @brief  Tx Transfer completed callback
 * @param  UartHandle: UART handle.
 * @note   This example shows a simple way to report end of IT Tx transfer, and
 *         you can add your own implementation.
 * @retval None
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	if (UartHandle->Instance == USARTx)
	{
		/* Set transmission flag: transfer complete */
//		UARTX_SetStatus(SET);
//		/* Turn LED5 on: Transfer in transmission process to modem is correct */
//		BSP_LED_On(LED5);
	} else if (UartHandle->Instance == USART2) // dies wird wahrsch. nicht gebraucht. Ich sollte wahrscheinlich USART2 nur als empfaenger Einstellen
	{
		/* Turn LED5 on: Transfer in transmition process to PC is correct */
		//BSP_LED_Toggle(LED5);
	}
}

/**
 * @brief  UART error callbacks
 * @param  UartHandle: UART handle
 * @note   This example shows a simple way to report transfer error, and you can
 *         add your own implementation.
 * @retval None
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
	/* Turn LED6 on: Transfer error in reception/transmission process */
	BSP_LED_On(LED6);
}

uint8_t UART_GetStatus()
{
	return UartReady;
}

void UART_SetStatus(uint8_t status)
{
	UartReady = status;
}

//###################PC

void UART_PC_Init()
{

	UartHandle_PC.Instance = USART2;

	UartHandle_PC.Init.BaudRate = 9600;
	UartHandle_PC.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle_PC.Init.StopBits = UART_STOPBITS_1;
	UartHandle_PC.Init.Parity = UART_PARITY_NONE;
	UartHandle_PC.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UartHandle_PC.Init.Mode = UART_MODE_TX_RX;
	UartHandle_PC.Init.OverSampling = UART_OVERSAMPLING_16;

	if (HAL_UART_Init(&UartHandle_PC) != HAL_OK)
	{
		Error_Handler();
	}
	//MX_TIM2_Init();

	/*##-4- Put UART peripheral in reception process ###########################*/
	if (HAL_UART_Receive_IT(&UartHandle_PC, &aRx_PC_Buffer_dummy, 1) != HAL_OK)
	{
		Error_Handler();
	}
}

void UARTPC_Transmit()
{
	// hierbei gehe ich davon aus, dass der buffer aRxBuffer
	// sowohl fürs Abspeichern von einkommenden Daten vom
	// PC als auch vom Modem
	if (HAL_UART_Transmit_IT(&UartHandle_PC, aRx_PC_Buffer, ptr_tx_pc_count)
	    != HAL_OK)
	{
		Error_Handler();
	}
}

void UARTPC_SaveChar()
{
	aRx_PC_Buffer[ptr_rx_pc_count++] = aRx_PC_Buffer_dummy;
}

//void UART_PC_set_dataready()
//{
//	UartReady = SET;
//}
//
//uint8_t UART_PC_get_dataready()
//{
//
//}
