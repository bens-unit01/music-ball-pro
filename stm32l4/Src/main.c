/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/


#include "stm32l4xx_hal.h"
#include "main.h"
#include "uart.h"
#include <math.h>

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
DAC_HandleTypeDef hdac1;

TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DAC1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM7_Init(void);



                                    
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

bool g_audio_playback = false; 
int g_audio_sample_index = 0; 
int g_audio_note_index = 0; 

uint16_t g_deb_01 = 0; 
uint16_t g_deb_02 = 0; 
#define RAMP_SIZE   320 

uint8_t button0 = 0;
int joy[4] = {0};



static uint8_t m_app_state = STATE_IDLE; 
static int16_t m_desired_angle = 0; 
static int16_t m_current_angle= 0; 
static int16_t m_last_angle = 0; 



extern uint32_t debug[5][100];
extern uint32_t debug_ir_reading[5][1000];
extern uint16_t debug_counter[5]; 
extern bool debug_on; 

extern uint32_t g_data_table[3][4]; 
uint32_t g_data_results[3][4] = {0}; 

extern uint32_t g_monitoring_01; 
static int MAX_ANGLE = 330; 
static int MIN_ANGLE = 15; 


uint32_t g_tim4_overflow = 0; 
uint32_t g_test_timer = 0; 
uint32_t g_last_timestamp = 0; 
uint32_t g_interrupts_counter = 0; 
uint32_t g_general_counter_1 = 0; 
uint32_t g_general_counter_2 = 0; 
uint32_t g_reading_state = 0;  
uint32_t micros(void); 

uint32_t g_stop_capture = 0; 
uint32_t g_last_index = 0; 

static float ex[10] = {0};  
static float ey[10] = {0};
static float rx[10] = {0};
static float ry[10] = {0};    


int angles[3] = { 15, 45, 75}; 
int emitter[4] = { 18, 36, 54, 72}; 

const double  PI = 3.14159265;
const float radian = (float)PI / 180;

bool g_data_ready = false; 
float angle = 0, range = 0; 
float angle_old = 0; 
int timestamp_old = 0; 
int debounce_time = 50000;
//int debounce_time = 500; 
bool is_first_time = false; 


int g_max_range = 0;  
int g_max_angle = 0;  




//const uint8_t aEscalator8bit[6] = {0x0, 0x33, 0x66, 0x99, 0xCC, 0xFF  };
const uint8_t aEscalator8bit[RAMP_SIZE] = {  
234, 236, 238, 240, 238, 238, 234, 234, 232, 231,
230, 229, 228, 227, 226, 225, 224, 223, 222, 221,
220, 219, 218, 217, 216, 215, 214, 213, 212, 211,
210, 209, 208, 207, 206, 205, 204, 203, 202, 201,
200, 199, 198, 197, 196, 195, 194, 193, 192, 191,
190, 189, 188, 187, 186, 185, 184, 183, 182, 181,
180, 179, 178, 177, 176, 175, 174, 173, 172, 171,
170, 169, 168, 167, 166, 165, 164, 163, 162, 161,
160, 159, 158, 157, 156, 155, 154, 153, 152, 151,
150, 149, 148, 147, 146, 145, 144, 143, 142, 141,
140, 139, 138, 137, 136, 135, 134, 133, 132, 131,
130, 129, 128, 127, 126, 125, 124, 123, 122, 121,
120, 119, 118, 117, 116, 115, 114, 113, 112, 111,
110, 109, 108, 107, 106, 105, 104, 103, 102, 101,
100, 99, 98, 97, 96, 95, 94, 93, 92, 91,
90, 89, 88, 87, 86, 85, 84, 83, 82, 81,
80, 79, 78, 77, 76, 75, 74, 73, 72, 71,
70, 69, 68, 67, 66, 65, 64, 63, 62, 61,
60, 59, 58, 57, 56, 55, 54, 53, 52, 51,
50, 49, 48, 47, 46, 45, 44, 43, 42, 41,
40, 39, 38, 37, 36, 35, 34, 33, 32, 31,
30, 29, 28, 27, 26, 25, 24, 23, 22, 21,
20, 19, 18, 17, 16, 15, 14, 13, 12, 11,
10, 9, 8, 7, 6, 5, 4, 3, 2, 1,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
}; 

//const uint8_t aEscalator8bit[RAMP_SIZE] = {0x0, 0x33, 0x0, 0x43, 0x0, 0x53, 0x0, 0x63, 0x0  };
static DAC_ChannelConfTypeDef sConfig;


/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

uint32_t micros(void)
{	
	return ( g_tim4_overflow  * (htim4.Instance->ARR) ) + htim4.Instance->CNT; 
}


void MX_TIM4_Init(void)
{
// timer for the micros() function 

  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 83;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 0xffff;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&htim4);

}



void init_calc(void) {

	for (int i = 0; i < 4; i++)
	{
		ex[i] = (float)sin(radian * emitter[i]);
		ey[i] = (float)cos(radian * emitter[i]);
	}

	for (int i = 0; i < 3; i++)
	{
		rx[i] = (float)sin(radian * angles[i]);
		ry[i] = (float)cos(radian * angles[i]);
	}                                                                

}

static void handle_idle(void) {

	button0 = parseUARTbuffer(joy);

	switch (button0)
	{


		case DEBUG_CMD:
			if( 0 == joy[0]) {
				// turn off debug  
				LOG("debug off \r\n"); 
				debug_on = false; 
			}  
			if( 1 == joy[0]) {
				// turn on debug  
				LOG("debug on \r\n"); 
				debug_on = true; 
			}  
			if( 2 == joy[0]) {
				uint32_t tmp = 0; 
                                        LOG(" %d --    %d\t    %d    \t%d   \t%d   \t%d  \t%d \r\n" , 0, debug_ir_reading[0][0], debug_ir_reading[1][0],  debug_ir_reading[2][0], debug_ir_reading[3][0],  debug_ir_reading[4][0],  debug_ir_reading[4][0] - debug_ir_reading[4][0]); 

				for(int k = 1 ; k < 100; k++) {
				        /*
					LOG("%d --    %d\t%d\t%d", k,  debug[0][k], debug[1][k], debug[1][k] - tmp );
					if( debug[0][k] > 209 ) LOG("  ------###----------"); 
					LOG(" \r\n");    
					tmp = debug[1][k]; 
					HAL_Delay(40);
				        */ 	

                                        LOG(" %d --    %d\t    %d    \t%d   \t%d    \t%d \r\n" , 
							k, 
							debug_ir_reading[0][k],
						       	debug_ir_reading[1][k],  
							debug_ir_reading[2][k],
						       	debug_ir_reading[3][k],
						      	debug_ir_reading[4][k]); 
					HAL_Delay(20); 

				}
				LOG("data table ----------------------------- \r\n"); 



                              for(uint8_t i = 0; i < 4; i++) {
			         LOG( " %d  \t ", g_data_results[0][i]);  
			         LOG( " %d  \t ", g_data_results[1][i]);  
			         LOG( " %d  \t ", g_data_results[2][i]);  
				 LOG("\r\n"); 
			      }
				 
 
				LOG("debug - data dump : \r\n"); 
				LOG("g_general_counter_1 : %d \r\n", g_general_counter_1); 
				LOG("g_last_index : %d \r\n", g_last_index); 



					

			} 

			if( 3 == joy[0]) {   // stop capture
                          g_stop_capture = 1; 	
	                  LOG("stopping data capture ... \r\n"); 		  
			}

			if( 4 == joy[0]) {   // start capture
                          g_stop_capture = 0; 			
	                  LOG("starting data capture ... \r\n"); 		  
			}
			break; 
		default: 
			break;
	}

}

static void calc_coordinates(float* out_phi, float* out_range) {

	float sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
				     
			sum1 += ex[j] * g_data_results[i][j];  
			sum2 += ey[j] * g_data_results[i][j];  
			sum3 += rx[i] * g_data_results[i][j];
			sum4 += ry[i] * g_data_results[i][j];

		}
	}                                                         


	float Ex = sum1, Ey = sum2, Rx = sum3, Ry = sum4;                         
	float phi = (float) atan2( Ex, Ey);                                               
	float range = (float) sqrt((Rx * Rx) + (Ry * Ry));


	phi = phi / radian;
	*out_phi = phi;
	*out_range = range; 



}

static void ir_reading_01(void) {

  //     handle_idle(); 
       HAL_Delay(1); 
	
       calc_coordinates( &angle, &range); 

       if( angle_old == 0 && angle != 0){
               timestamp_old = micros(); 
	       g_max_range = 0;  
	       g_max_angle = 0; 
	       is_first_time = true;
//	       LOG(" - %d \r\n", (int)micros()); 
       }

       if(angle != 0 ) {
	       if( range > g_max_range) g_max_range = range;  
	       if( angle > g_max_angle) g_max_angle = angle;  

       }

       if(angle != 0 && (micros() - timestamp_old > debounce_time) && is_first_time) {
	       is_first_time = false; 
	       g_general_counter_2++; 
//	       LOG("%d %d %d\r\n", 0xFA, (int)g_max_angle, (int)g_max_range); 
	       char output_angle[4];
	       char output_range[10];
	       int n = sprintf(output_angle,"%i",g_max_angle);
	       int m = sprintf(output_range,"%i",g_max_range);
               static uint8_t output[2]; 
	          output[0] =(uint8_t)g_max_angle;   
	          output[1] ='\n';   
	       HAL_UART_Transmit(&huart2, (uint8_t *)output_angle, n, 0xFFFF);    
	       HAL_UART_Transmit(&huart2, (uint8_t *)" ", 1, 0xFFFF); 
	       HAL_UART_Transmit(&huart2, (uint8_t *)output_range, m, 0xFFFF);    
	       HAL_UART_Transmit(&huart2, (uint8_t *)"\n", 1, 0xFFFF); 
          //     g_audio_playback = true; 
	       g_audio_sample_index = 0; 
	       //HAL_UART_Transmit(&huart2,(uint8_t*)"-\n",2,1000);
	  
       }   

       if( angle_old != 0 && angle == 0)
       { 
	       g_general_counter_2 = 0; 
//	       LOG(" -- %d\t%d \r\n", g_max_range, (int)(micros() - timestamp_old)); 
       }
       angle_old = angle; 



}


static void ir_reading_02(void) {
  HAL_Delay(5); 
 
       calc_coordinates( &angle, &range); 
  //     LOG("  %d\t\t%d \r\n", (int)angle, (int)range); 
    //   if( angle != 0) 
    
//if( g_data_results[0][3] > 4000  || g_data_results[1][3] > 4000  || g_data_results[2][3] > 4000) { 

LOG("%d %d %d %d %d %d %d %d %d %d %d %d \r\n", 
		g_data_results[0][0], g_data_results[1][0], g_data_results[2][0],
		g_data_results[0][1], g_data_results[1][1], g_data_results[2][1],
		g_data_results[0][2], g_data_results[1][2], g_data_results[2][2],
		g_data_results[0][3], g_data_results[1][3], g_data_results[2][3] 
		) ; 
		 
/*} else { 

   LOG("---\r\n"); 
} 
*/ 

//if( g_data_results[0][3] > 0  || g_data_results[1][3] > 0  || g_data_results[2][3] > 0) 
//LOG("%d %d %d \r\n",  g_data_results[0][2], g_data_results[1][2], g_data_results[2][2]); 

		
}

static void ir_reading_03(void) {

	if(g_data_results[2][3] > 1700) { 
		LOG(" %d \r\n", g_data_results[2][3]); 
		HAL_Delay(20); 
	}
}

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();
  Uart_Init();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DAC1_Init();

  MX_TIM7_Init();
  HAL_TIM_Base_Start_IT(&htim7);

  MX_TIM4_Init();   // timer for the micros() function  
  HAL_TIM_Base_Start_IT(&htim4);

  init_timer_ic();   // timer2 init  
  init_external_interrupts(); 
  init_calc(); 

  
// debug gpio used with the oscilloscope   
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_10; 
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    GPIO_InitStruct.Alternate = GPIO_A;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);



  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    HAL_DAC_Start(&hdac1,DAC_CHANNEL_1);
  while (1)
  {
   button0 = parseUARTbuffer(joy); 
   if( 0 != button0) { 
      LOG("-- %d \r\n", g_audio_note_index); 
      g_audio_playback = true; 
      HAL_Delay(150); 
      g_audio_playback = false; 
      g_audio_note_index++; 
      g_audio_note_index %= 12;
      g_audio_sample_index = 0;   
   }
   
  ir_reading_02(); 
   //handle_idle(); 
   HAL_Delay(20); 

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/



void TIM4_IRQHandler(void)
{
  /* USER CODE BEGIN TIM16_IRQn 0 */

  /* USER CODE END TIM16_IRQn 0 */
 
//  extern TIM_HandleTypeDef htim4;
  HAL_TIM_IRQHandler(&htim4);
  /* USER CODE BEGIN TIM16_IRQn 1 */

  /* USER CODE END TIM16_IRQn 1 */
}

void SystemClock_Config_old(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* MSI is enabled after System reset, activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}




void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
		while(1); 
  //  _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
		while(1); 
   //_Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the main internal regulator output voltage 
    */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
		while(1); 
  //  _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}



static void MX_DAC1_Init(void)
{

   hdac1.Instance = DAC1;

  if (HAL_DAC_Init(&hdac1) != HAL_OK)
  {
    /* Initialization Error */
		while(1); 
    //Error_Handler();
  }

  /*##-1- DAC channel1 Configuration #########################################*/

  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
 

  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    /* Channel configuration Error */
    while(1);
  }
}


void MX_TIM7_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig;

  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 0;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 1813;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    while(1);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
		while(1); 
    //Error_Handler();
  }

}



/* TIM2 init function */
static void MX_TIM2_Init(void)
{

  static TIM_HandleTypeDef  htim;
  TIM_MasterConfigTypeDef sMasterConfig;

  /*##-1- Configure the TIM peripheral #######################################*/
  /* Time base configuration */
  htim.Instance = TIM2;

  //htim.Init.Period            = 0x7FF;
  htim.Init.Period            = 0x7FF;
  htim.Init.Prescaler         = 0;
  htim.Init.ClockDivision     = 0;
  htim.Init.CounterMode       = TIM_COUNTERMODE_UP;
  htim.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&htim);

  /* TIM6 TRGO selection */
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig);

  /*##-2- Enable TIM peripheral counter ######################################*/
  HAL_TIM_Base_Start(&htim);


}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
     PA2   ------> USART2_TX
     PA3   ------> USART2_RX
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : B1_Pin */
 /* GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);
*/
  /*Configure GPIO pins : USART_TX_Pin USART_RX_Pin */
 /* GPIO_InitStruct.Pin = USART_TX_Pin|USART_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	*/

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}


void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}



void TIM7_IRQHandler(void)
{
  /* USER CODE BEGIN TIM7_IRQn 0 */

  /* USER CODE END TIM7_IRQn 0 */
  HAL_TIM_IRQHandler(&htim7);
  /* USER CODE BEGIN TIM7_IRQn 1 */

  /* USER CODE END TIM7_IRQn 1 */
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM7) //metronome timer ... half beat timer
	{ 

		//HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_10); 	
		static uint16_t nextSample = 32768;
		static int16_t signedSample = 0; 
		// extern const unsigned char do_01[18432];  
		extern address_map_t piano[];  
//		static int16_t * audioBuf; 

		if(g_audio_playback) { 

//			audioBuf =  piano[g_audio_note_index].address_audio;  
			HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_L, nextSample); //output it to the dac first to ensure good timing

			g_audio_sample_index++; 

			if(g_audio_sample_index >= piano[g_audio_note_index].num_samples) {
				g_audio_playback = false; 
				g_audio_sample_index = 0; 
				nextSample = 0;                 
			}

                        signedSample = piano[g_audio_note_index].address_audio[g_audio_sample_index]; 
//			signedSample = audioBuf[g_audio_sample_index];  

			if(signedSample > 32767) 
			{
				signedSample = 32767; //force clip (16 bit)
			}
			else if( signedSample < -32767) 
			{
				signedSample = -32767; //force clip (16 bit)
			}	

			nextSample = signedSample + 32767; 
			g_deb_01 = nextSample; 
			g_deb_02 = g_audio_sample_index;
		} 	
		

	}else if(htim->Instance == TIM4)
	{
		g_tim4_overflow++; //increments the timer 16 overflow counter
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_10);
	}


}



#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
