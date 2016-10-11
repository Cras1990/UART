/*
 * GSM_UART.h
 *
 *  Created on: 11.10.2016
 *      Author: revelo
 */

#ifndef GSM_UART_H_
#define GSM_UART_H_

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

/* User can use this section to tailor USARTx/UARTx instance used and associated
 resources */
/* Definition for USARTx clock resources */
#define USARTx                           USART2
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_2
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_TX_AF                     GPIO_AF7_USART2
#define USARTx_RX_PIN                    GPIO_PIN_3
#define USARTx_RX_GPIO_PORT              GPIOA
#define USARTx_RX_AF                     GPIO_AF7_USART2

/* Definition for USARTx's NVIC */
#define USARTx_IRQn                      USART2_IRQn
#define USARTx_IRQHandler                USART2_IRQHandler
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Size of Transmission buffer */
#define TXBUFFERSIZE                      (COUNTOF(aTxBuffer) - 1)
/* Size of Reception buffer */
#define RXBUFFERSIZE                      TXBUFFERSIZE



/* Global function prototypes -----------------------------------------------*/
void UARTX_Init(void);
void UARTX_Transmit(uint8_t *pData, uint16_t Size);
void UARTX_Receive(uint8_t *pData, uint16_t Size);
uint8_t UARTX_GetStatus(void);
void UARTX_SetStatus(uint8_t status);


#endif /* GSM_UART_H_ */
