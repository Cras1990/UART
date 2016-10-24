#include "GSM_UART.h"
#include "stm32f4xx.h"
#include "led_button.h"
#include "GSM_TIM.h"
#include "string.h"

/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef UartHandle_X;
UART_HandleTypeDef UartHandle_PC;
uint8_t str2comp[] = "\r\nOK\r\n";
/* Buffer used for transmission */
__IO uint8_t aTxBuffer[100];
__IO uint8_t ptr_tx_pc_count = 0;
/* Buffer used for reception */
//uint8_t aRxBuffer[RXBUFFERSIZE+2];
__IO uint8_t aRxBuffer[100];
__IO uint8_t aRxBuffer_dummy;
__IO uint8_t ptr_rx_pc_count = 0;

__IO ITStatus UartReady = RESET;

extern void Error_Handler(void);
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2,
    uint16_t BufferLength);
static void UARTPC_SaveChar(void);

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

	UARTX_GSM_PinsInit();
}

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
	if (HAL_UART_Receive_IT(&UartHandle_PC, &aRxBuffer_dummy, 1) != HAL_OK)
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
	if (HAL_UART_Transmit_IT(&UartHandle_X, aRxBuffer, ptr_tx_pc_count) != HAL_OK)
	{
		Error_Handler();
	}

	/*##-3- Wait for the end of the transfer ###################################*/
	while (UARTX_GetStatus() != SET)
	{
	}

	/* Reset transmission flag */
	UARTX_SetStatus(RESET);
//  if(HAL_UART_Transmit(&UartHandle_X, pData, Size, 5000)!= HAL_OK)
//  {
//    Error_Handler();
//  }
//  BSP_LED_On(LED6);
}

void UARTX_Receive(uint8_t *pData, uint16_t Size)
{
	/*##-4- Put UART peripheral in reception process ###########################*/
	if (HAL_UART_Receive_IT(&UartHandle_X, pData, Size) != HAL_OK)
	{
		Error_Handler();
	}

	/*##-5- Wait for the end of the transfer ###################################*/
	while (UARTX_GetStatus() != SET)
	{

	}
	/* Reset transmission flag */
	UARTX_SetStatus(RESET);

//  if(HAL_UART_Receive(&UartHandle_X, pData, Size, 5000) != HAL_OK)
//  {
//    Error_Handler();
//  }
//
//  /* Turn LED4 on: Transfer in reception process is correct */
//  BSP_LED_On(LED4);
}

void UARTPC_Transmit()
{

//	if (HAL_UART_Transmit_IT(&UartHandle_PC, aTxBuffer, ptr_tx_pc_count) != HAL_OK)
	if (HAL_UART_Transmit_IT(&UartHandle_PC, aRxBuffer, ptr_tx_pc_count) != HAL_OK)
	{
		Error_Handler();
	}
}

void UARTPC_Handler()
{
	// start again the uart receive process
	if (HAL_UART_Receive_IT(&UartHandle_PC, &aRxBuffer_dummy, 1) != HAL_OK)
	{
		Error_Handler();
	}
}

void UARTPC_SaveChar()
{
	aRxBuffer[ptr_rx_pc_count++] = aRxBuffer_dummy;
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
		/* Set transmission flag: transfer complete */
		UARTX_SetStatus(SET);
		/* Turn LED4 on: Transfer in reception process is correct */
		BSP_LED_On(LED4);
	} else if (UartHandle->Instance == USART2)
	{

		// die Reihenfolge, wie folgende Aufrufe durchgeführt werden, spielt eine Rolle, also nicht vertauschen
		//speichere char in empfangsbuffer vom pc
		UARTPC_SaveChar();
		if (ptr_rx_pc_count == COUNTOF(aRxBuffer) || aRxBuffer_dummy == '\r')
		{
			// copy the number of bytes to send to remote device
			ptr_tx_pc_count = ptr_rx_pc_count;
			// reset the pointer to the pc_rx_buffer
			ptr_rx_pc_count = 0;
			// send the received command from pc to remote device
			UARTPC_Transmit();
			/* Turn LED4 on: Transfer in reception process is correct */
			BSP_LED_Toggle(LED4);
		}
		// reactivate on the receive process once again
		UARTPC_Handler();
	}
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
		UARTX_SetStatus(SET);
		/* Turn LED6 on: Transfer in transmission process is correct */
		BSP_LED_On(LED6);
	} else if (UartHandle->Instance == USART2) // dies wird wahrsch. nicht gebraucht. Ich sollte wahrscheinlich USART2 nur als empfaenger Einstellen
	{
		/* Turn LED4 on: Transfer in transmition process is correct */
		BSP_LED_Toggle(LED5);
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
	/* Turn LED3 on: Transfer error in reception/transmission process */
	BSP_LED_On(LED3);
}

uint8_t UARTX_GetStatus()
{
	return UartReady;
}

void UARTX_SetStatus(uint8_t status)
{
	UartReady = status;
}
