#include "GSM_UART.h"
#include "led_button.h"
#include "gsm_adapter.h"
#include <string.h>

/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef UartHandle_X;
UART_HandleTypeDef UartHandle_PC;

uint8_t aTxBuffer[100];
//uint8_t aRxBuffer[100];
extern uint8_t aRx_PC_Buffer[100];
extern uint8_t aTx_PC_Buffer[100];
__IO uint8_t ptr_tx_pc_count = 0;
__IO uint8_t ptr_rx_pc_count = 0;
__IO uint8_t aRx_PC_Buffer_dummy;
__IO uint8_t aRx_X_Buffer_dummy;
__IO uint8_t ptr_rx_x_count = 0;
__IO uint8_t ptr_tx_x_count = 0;
//#PC
//__IO uint8_t dataPCready = 0;
__IO ITStatus UartReady = RESET;

extern void Error_HandlerPC(void);
extern void Error_HandlerX(void);

void UART_Handler(UART_HandleTypeDef *UartHandle);

void UARTX_Init()
{

	UartHandle_X.Instance = USARTx;

	UartHandle_X.Init.BaudRate = 9600;
//	UartHandle_X.Init.BaudRate = 115200;
	UartHandle_X.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle_X.Init.StopBits = UART_STOPBITS_1;
	UartHandle_X.Init.Parity = UART_PARITY_NONE;
	UartHandle_X.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UartHandle_X.Init.Mode = UART_MODE_TX_RX;
	UartHandle_X.Init.OverSampling = UART_OVERSAMPLING_16;

	if (HAL_UART_Init(&UartHandle_X) != HAL_OK)
	{
		Error_HandlerX();
	}

	//Init pwrkey pin for GSM-Modul
	UARTX_GSM_PinsInit();

	/*##-4- Put UART peripheral in reception process ###########################*/
	if (HAL_UART_Receive_IT(&UartHandle_X, &aRx_X_Buffer_dummy, 1) != HAL_OK)
	{
		Error_HandlerX();
	}
}

//PWRKEY wird gesetzt
void UARTX_GSM_PinsInit()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIOC clock */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/* Configure PE4 pin as output pwrkey*/
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Pin = GPIO_PIN_4;
	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* Configure PE5 pin as output RTS*/
	GPIO_InitStructure.Pin = GPIO_PIN_5;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* Configure PD2 pin as input CTS*/
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

}

void UARTX_Transmit(uint8_t *command, uint8_t size)
{
	/*##-2- Start the transmission process #####################################*/
	/* While the UART in reception process, user can transmit data through
	 "aTxBuffer" buffer */
	if (HAL_UART_Transmit_IT(&UartHandle_X, command, size) != HAL_OK)
	{
		Error_HandlerX();
	}
}


void UART_Handler(UART_HandleTypeDef *UartHandle)
{
	if (UartHandle->Instance == USART2)
	{
		// receive or save data from GSM and start again the uart receive process
		if (HAL_UART_Receive_IT(&UartHandle_PC, &aRx_PC_Buffer_dummy, 1) != HAL_OK)
		{
			Error_HandlerPC();
		}
	} else
	{
		// receive or save data from PC and start again the uart receive process
		if (HAL_UART_Receive_IT(&UartHandle_X, &aRx_X_Buffer_dummy, 1) != HAL_OK)
		{
			Error_HandlerX();
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
		at_adapter_rx(aRx_X_Buffer_dummy);
	} else if (UartHandle->Instance == USART2)
	{
		at_adapter_tx(aRx_PC_Buffer_dummy);
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
		// do something

	} else if (UartHandle->Instance == USART2)
	{
		/* Set transmission flag: transfer complete */
		// do something
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
	if (UartHandle->Instance == USARTx)
	{
		/* Set transmission flag: transfer complete */
//		/* Turn LED5 on: Transfer in transmission process to modem is correct */
		BSP_LED_On(LED4);
	} else if (UartHandle->Instance == USART2) // dies wird wahrsch. nicht gebraucht. Ich sollte wahrscheinlich USART2 nur als empfaenger Einstellen
	{
		/* Turn LED5 on: Transfer in transmition process to PC is correct */
		BSP_LED_Toggle(LED5);
	}
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

	UartHandle_PC.Init.BaudRate = 115200;
	UartHandle_PC.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle_PC.Init.StopBits = UART_STOPBITS_1;
	UartHandle_PC.Init.Parity = UART_PARITY_NONE;
	UartHandle_PC.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UartHandle_PC.Init.Mode = UART_MODE_TX_RX;
	UartHandle_PC.Init.OverSampling = UART_OVERSAMPLING_16;

	if (HAL_UART_Init(&UartHandle_PC) != HAL_OK)
	{
		Error_HandlerPC();
	}

	/*##-4- Put UART peripheral in reception process ###########################*/
	if (HAL_UART_Receive_IT(&UartHandle_PC, &aRx_PC_Buffer_dummy, 1) != HAL_OK)
	{
		Error_HandlerPC();
	}
}

void UARTPC_Transmit(uint8_t *command, uint8_t size)
{
	// send data to PC
	HAL_UART_Transmit_IT(&UartHandle_PC, command, size);
}


