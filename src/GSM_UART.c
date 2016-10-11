#include "GSM_UART.h"
#include "stm32f4xx.h"
#include "led_button.h"

/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef UartHandle;
__IO ITStatus UartReady = RESET;

extern void Error_Handler(void);

void UARTX_Init()
{

	UartHandle.Instance = USARTx;

	UartHandle.Init.BaudRate = 9600;
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits = UART_STOPBITS_1;
	UartHandle.Init.Parity = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode = UART_MODE_TX_RX;
	UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

	if (HAL_UART_Init(&UartHandle) != HAL_OK)
	{
		Error_Handler();
	}
}

void UARTX_Transmit(uint8_t *pData, uint16_t Size)
{
  /*##-2- Start the transmission process #####################################*/
  /* While the UART in reception process, user can transmit data through
     "aTxBuffer" buffer */
  if(HAL_UART_Transmit_IT(&UartHandle, pData, Size)!= HAL_OK)
  {
    Error_Handler();
  }
}

void UARTX_Receive(uint8_t *pData, uint16_t Size)
{
  /*##-4- Put UART peripheral in reception process ###########################*/
  if(HAL_UART_Receive_IT(&UartHandle, pData, Size) != HAL_OK)
  {
    Error_Handler();
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
  /* Set transmission flag: transfer complete */
  //UartReady = SET;
	UARTX_SetStatus(SET);

  /* Turn LED6 on: Transfer in transmission process is correct */
  BSP_LED_On(LED6);
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
  /* Set transmission flag: transfer complete */
	UARTX_SetStatus(SET);

  /* Turn LED4 on: Transfer in reception process is correct */
  BSP_LED_On(LED4);
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
