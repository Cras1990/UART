/* Include core modules */
#include "stm32fxxx_hal.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32_disco.h"
#include "tm_stm32_delay.h"
#include "tm_stm32_usart.h"

/* Buffer array */
char mybuffer[100];
int16_t number = 0;

int main(void) {
	/* Init system clock for maximum system speed */
	TM_RCC_InitSystem();

	/* Init HAL layer */
	HAL_Init();

	/* Init leds */
	TM_DISCO_LedInit();

	/* Init button */
	TM_DISCO_ButtonInit();

	/* Init USART, TX: PC6, RX: PC7, 921600 bauds */
	TM_USART_Init(USART2, TM_USART_PinsPack_1, 115200);

	/* Put test string */
	TM_USART_Puts(USART2, "Hello world\n");

	while (1) {
		/* Check if string received */
		/* Waiting for \n at the end of string */
		if (TM_USART_Gets(USART2, mybuffer, sizeof(mybuffer))) {
			/* Send string back */
//			TM_USART_Puts(USART2, mybuffer);
			number = atoi(mybuffer);
			sprintf(mybuffer, "Number writen: %d \n", number);
			TM_USART_Puts(USART2, mybuffer);
		}

	}
}
