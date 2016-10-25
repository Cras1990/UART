#include "GSM_TIM.h"
#include "led_button.h"

TIM_HandleTypeDef htim2;
__IO uint8_t osTicks;
extern void Error_Handler();

/** TIM2 init function
 **/
void MX_TIM2_Init() {

	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;

	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 0;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 84000;                    // Timer eingestellt bei 1ms
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	if(HAL_TIM_Base_Init(&htim2) != HAL_OK)
	{
    /* Initialization Error */
    Error_Handler();
	}

//	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
//	HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);
//
//	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
//	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//	HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);

	if (HAL_TIM_Base_Start_IT(&htim2) != HAL_OK)
	{ // Initialisierung Timer schon bevor Messung startet
		/* Counter Enable Error */
		Error_Handler();
	}

}

/**
 * @brief  TIM callbacks.
 * 		   Methode wird zum Entprellen des, falls Button betaetigt wird.
 * 		   Ausserdem wird durch den TIM2 eine Zeiterfassung durchgefuehrt
 * @param  TIM_HandleTypeDef: Specifies the timer pointer
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM2)
	{
		osTicks++; //ueberlauf, wenn ich dies nicht handle
		//BSP_LED_Toggle(LED3);
	}

}

uint8_t get_osTicks()
{
	if(osTicks > 0)
		return 1;
	else
		return 0;
}

void reset_osTicks()
{
	osTicks--;
}
