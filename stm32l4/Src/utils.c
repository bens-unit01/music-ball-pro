

//#include "stm32f4xx_hal.h"
#include "stm32l4xx_hal.h"
#include "stdbool.h"
uint32_t debug[5][100];
int i = 0,  j = 0; 
extern TIM_HandleTypeDef htim2;
extern uint32_t g_reading_state; 
extern uint32_t g_data_table[3][4]; 
extern uint32_t g_data_results[3][4]; 
int g_input_capture = 0; 
uint32_t g_ref_time = 0; 
uint32_t micros(void); 


extern uint32_t debug_ir_reading[5][1000];

void init_timer_ic(void)
{
  

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_IC_InitTypeDef sConfigIC;

  htim2.Instance = TIM2;
  //htim2.Init.Prescaler = 48000;
  htim2.Init.Prescaler = 84000;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 100000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htim2);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);

  HAL_TIM_IC_Init(&htim2);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 15;
  HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1);
 

  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1); 


}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if( htim->Instance == TIM2) 
	{

		g_input_capture = __HAL_TIM_GetCompare(&htim2, TIM_CHANNEL_1); 
		if( j > 100) j = 0;  
		debug[0][j] = g_input_capture;  
		debug[1][j] = micros();  

		g_reading_state = 1; 	
		g_ref_time = debug[1][j]; 

		for(uint8_t i = 0; i < 4; i++) {
			g_data_results[0][i] = g_data_table[0][i]; 
			g_data_results[1][i] = g_data_table[1][i]; 
			g_data_results[2][i] = g_data_table[2][i]; 
			g_data_table[0][i] = 0; 
			g_data_table[1][i] = 0; 
			g_data_table[2][i] = 0; 
		}




		j++; 

		__HAL_TIM_SetCounter( &htim2, 0);  
	}


}




void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */

  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}












