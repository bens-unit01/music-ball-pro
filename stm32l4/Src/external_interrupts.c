
//#include "cube_hal.h"
#include "stm32l4xx_hal.h"
#include "stdbool.h" 
#include "main.h" 
uint32_t micros(void); 
uint32_t debug_ir_reading[5][1000];  
uint32_t g_data_table[3][4] = {0}; 
uint32_t g_data_table_old[3][4] = {0}; 

extern uint32_t g_data_results[3][4]; 

uint32_t g_ir_reading_r0_prev = 0; 
uint32_t g_ir_reading_r1_prev = 0; 
uint32_t g_ir_reading_r2_prev = 0; 
extern uint32_t g_reading_state;  
extern uint32_t g_general_counter_1; 
extern uint32_t g_stop_capture; 
extern uint32_t g_last_index; 
extern uint32_t g_ref_time; 


extern bool debug_on; 

uint32_t g_monitoring_01 = 0; 

extern bool g_data_ready; 

static int j = 0; 
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{


        uint32_t ir_reading_r0; 
        uint32_t ir_reading_r1; 
        uint32_t ir_reading_r2; 
	extern uint32_t g_interrupts_counter; 


//	if(g_reading_state){

		switch(GPIO_Pin)
		{
			case GPIO_PIN_1:

               	                ir_reading_r0 = micros(); 
                                uint32_t ir_strengh_r0 = ir_reading_r0 - g_ir_reading_r0_prev; 

				if ( (( ir_reading_r0 - g_ref_time ) > 8000)  && ( GPIO_PIN_SET == HAL_GPIO_ReadPin( GPIOB, GPIO_PIN_1) )) {
					/*debug_ir_reading[3][j] = ir_strengh_r0;  // = micros()  
					debug_ir_reading[4][j] = ir_reading_r0;  // = micros()  
					debug_ir_reading[0][j] = g_ref_time; 
					debug_ir_reading[1][j] =   HAL_GPIO_ReadPin( GPIOB, GPIO_PIN_1); 

					debug_ir_reading[2][j] = ir_reading_r0 - g_ref_time; 
					j++;
					g_last_index = j; 
					if(j > 100) j = 0; 

					 */ 

					uint32_t index  = (( g_ir_reading_r0_prev - g_ref_time) - 7000)/ 8000;   // index calculation 

					if( index <= 3 && ir_strengh_r0 < 8000)  {
						g_data_table[0][index] =  ir_strengh_r0; 
					}
					
				}
				g_ir_reading_r0_prev = ir_reading_r0; 

				break; 
			case GPIO_PIN_2: 

				ir_reading_r1 = micros(); 
                                uint32_t ir_strengh_r1 = ir_reading_r1 - g_ir_reading_r1_prev; 

				if ( (( ir_reading_r1 - g_ref_time ) > 8000)  && ( GPIO_PIN_SET == HAL_GPIO_ReadPin( GPIOB, GPIO_PIN_2) )) {
					/*
					debug_ir_reading[3][j] = ir_strengh_r1;  // = micros()  
					debug_ir_reading[4][j] = ir_reading_r1;  // = micros()  
					debug_ir_reading[0][j] = g_ref_time; 
					debug_ir_reading[1][j] =   HAL_GPIO_ReadPin( GPIOB, GPIO_PIN_2); 

					debug_ir_reading[2][j] = ir_reading_r1 - g_ref_time; 
					j++;
					g_last_index = j; 
					if(j > 100) j = 0; 

					*/ 
				
					uint32_t index  = (( g_ir_reading_r1_prev - g_ref_time) - 7000)  / 8000;   // index calculation 

					  if( index <= 3 ) {
						  g_data_table[1][index] =  ir_strengh_r1 ; 
					  }
				}
				g_ir_reading_r1_prev = ir_reading_r1; 
				break; 

			case GPIO_PIN_3: 
					ir_reading_r2 = micros(); 
					uint32_t ir_strengh_r2 = ir_reading_r2 - g_ir_reading_r2_prev; 
	
					debug_ir_reading[0][j] = ir_reading_r2; 
					debug_ir_reading[1][j] = HAL_GPIO_ReadPin( GPIOB, GPIO_PIN_3);  
					debug_ir_reading[2][j] = ir_reading_r2 - g_ref_time; 
					debug_ir_reading[3][j] = g_ref_time; 
					//debug_ir_reading[4][j] = ir_reading_r2 - g_ir_reading_r2_prev; 
					debug_ir_reading[4][j] = g_ir_reading_r2_prev - g_ref_time; 
					j++;
			//		j %= 100; 
					g_last_index = j; 
				        if(j > 100) j = 0; 
		
			           if ( (( ir_reading_r2 - g_ref_time ) > 8000)  && ( GPIO_PIN_SET == HAL_GPIO_ReadPin( GPIOB, GPIO_PIN_3) )) {

			
					uint32_t index  = ((g_ir_reading_r2_prev - g_ref_time) -7000) / 8000;   // index calculation 

					  if( index <= 3 ) {
						  g_data_table[2][index] =  ir_strengh_r2 ; 
					  }
				     }
				    g_ir_reading_r2_prev = ir_reading_r2; 
				    break; 

		}
		//	}

}



void EXTI1_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}


void EXTI2_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}


void EXTI3_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}





void init_external_interrupts(void){

  GPIO_InitTypeDef GPIO_InitStruct;

  __GPIOB_CLK_ENABLE();


  GPIO_InitStruct.Pin = GPIO_PIN_1 |GPIO_PIN_2 | GPIO_PIN_3 ;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);



  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);



}










