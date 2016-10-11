/* Include core modules */
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "led_button.h"
#include "GSM_UART.h"
/* Include my libraries here */
//#include "defines.h"
//#include "tm_stm32_disco.h"
//#include "tm_stm32_delay.h"
//#include "tm_stm32_usart.h"
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Buffer used for transmission */
uint8_t aTxBuffer[] =
    " ****UART_TwoBoards_ComIT****  ****UART_TwoBoards_ComIT****  ****UART_TwoBoards_ComIT**** ";
//uint8_t aTxBuffer[] = "";
/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
void Error_Handler(void);


int main(void)
{
	/* Init system clock for maximum system speed */
//	TM_RCC_InitSystem();
	/* STM32F4xx HAL library initialization:
	 - Configure the Flash prefetch, instruction and Data caches
	 - Configure the Systick to generate an interrupt each 1 msec
	 - Set NVIC Group Priority to 4
	 - Global MSP (MCU Support Package) initialization
	 */
	HAL_Init();

  /* Configure LED3, LED4, LED5 and LED6 */
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED5);
  BSP_LED_Init(LED6);

	/* Configure the system clock to 168 MHz */
	SystemClock_Config();

	/*##-1- Configure the UART peripheral ######################################*/
	/* Put the USART peripheral in the Asynchronous mode (UART Mode) */
	/* UART1 configured as follow:
	 - Word Length = 8 Bits
	 - Stop Bit = One Stop bit
	 - Parity = None
	 - BaudRate = 9600 baud
	 - Hardware flow control disabled (RTS and CTS signals) */
	UARTX_Init();

	/* Configure USER Button */
	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);

	/* Wait for USER Button press before starting the Communication */
	while (BSP_PB_GetState(BUTTON_KEY) == RESET)
	{
		/* Toggle LED3 waiting for user to press button */
		BSP_LED_Toggle(LED3);
		HAL_Delay(40);
	}

	/* Wait for USER Button release before starting the Communication */
	while (BSP_PB_GetState(BUTTON_KEY) == SET)
	{
	}

	/* Turn LED3 off */
	BSP_LED_Off(LED3);

	UARTX_Transmit((uint8_t*)aTxBuffer, TXBUFFERSIZE);

  /*##-3- Wait for the end of the transfer ###################################*/
  while (UARTX_GetStatus() != SET)
  {
  }

  /* Reset transmission flag */
  UARTX_SetStatus(RESET);

  UARTX_Receive((uint8_t*)aRxBuffer, 5);

  /*##-5- Wait for the end of the transfer ###################################*/
  while (UARTX_GetStatus() != SET)
  {
  }

  /* Reset transmission flag */
  UARTX_SetStatus(RESET);

	/* Infinite loop */
	while (1)
	{
	}

}



/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 168000000
 *            HCLK(Hz)                       = 168000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 4
 *            APB2 Prescaler                 = 2
 *            HSE Frequency(Hz)              = 8000000
 *            PLL_M                          = 8
 *            PLL_N                          = 336
 *            PLL_P                          = 2
 *            PLL_Q                          = 7
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale1 mode
 *            Flash Latency(WS)              = 5
 * @param  None
 * @retval None
 */
static void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE()
	;

	/* The voltage scaling allows optimizing the power consumption when the device is
	 clocked below the maximum system frequency, to update the voltage scaling value
	 regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
	    | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		Error_Handler();
	}

	/* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
	if (HAL_GetREVID() == 0x1001)
	{
		/* Enable the Flash prefetch */
		__HAL_FLASH_PREFETCH_BUFFER_ENABLE();
	}
}



/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
void Error_Handler(void)
{
	/* Turn LED5 on */
	BSP_LED_On(LED5);
	while (1)
	{
	}
}
