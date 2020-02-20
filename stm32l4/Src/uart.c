


#include "uart.h"
#include "stdbool.h"
#define PACKET_SIZE      4 
uint8_t  RX6Data[PACKET_SIZE] = {0};						
uint8_t  RX2Data[PACKET_SIZE] = {0};						
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart6;
uint32_t timestamp6 = 0;
uint32_t timestamp2 = 0;

bool debug_on = true; 


uint32_t millis(void) {
  return HAL_GetTick();
}

 

void reset_IT(UART_HandleTypeDef *UartHandle)
{

	if(UartHandle == &huart6)
	{
		HAL_UART_Receive_IT(UartHandle, RX6Data, PACKET_SIZE);
		timestamp6 = millis();
	}


	if(UartHandle == &huart2)
	{
		HAL_UART_Receive_IT(UartHandle, RX2Data, PACKET_SIZE);
		timestamp2 = millis();
	}


}



void USART6_IRQHandler(void)
{
	/* USER CODE BEGIN USART2_IRQn 0 */
	/* USER CODE END USART2_IRQn 0 */

	HAL_UART_IRQHandler(&huart6);
	/* USER CODE BEGIN USART2_IRQn 1 */
	//   LOG("irq3 %03x\t%03x\t%03x\t%d... \n", RX3Data[0], RX3Data[1], RX3Data[2], huart3.RxXferCount);  

	/* USER CODE END USART2_IRQn 1 */
}



void USART2_IRQHandler(void)
{
	/* USER CODE BEGIN USART2_IRQn 0 */
	/* USER CODE END USART2_IRQn 0 */

	HAL_UART_IRQHandler(&huart2);
	/* USER CODE BEGIN USART2_IRQn 1 */
	//   LOG("irq3 %03x\t%03x\t%03x\t%d... \n", RX3Data[0], RX3Data[1], RX3Data[2], huart3.RxXferCount);  

	/* USER CODE END USART2_IRQn 1 */
}




void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

  GPIO_InitTypeDef GPIO_InitStruct;
 
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
	  
 

}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *mUartHandle)
{
  reset_IT(mUartHandle);
}


 void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
	 
	   reset_IT(UartHandle);
}


/**
* @brief  Initializes uart communications
* @param  None
* @retval None
*/
void Uart_Init(void){
  
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();
  
  /* Enable USARTx clock */
  USARTx_CLK_ENABLE(); 
  
  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = USARTx_TX_AF;
  
  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);
  
  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = USARTx_RX_PIN;
  GPIO_InitStruct.Alternate = USARTx_RX_AF;
  
  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct); 
  huart2.Instance          = USARTx;  
  huart2.Init.BaudRate     = 115200;
  huart2.Init.WordLength   = UART_WORDLENGTH_8B;
  huart2.Init.StopBits     = UART_STOPBITS_1;
  huart2.Init.Parity       = UART_PARITY_NONE;
  huart2.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  huart2.Init.Mode         = UART_MODE_TX_RX;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  
  HAL_UART_Init(&huart2);
  HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(USART2_IRQn);
  HAL_UART_Receive_IT(&huart2 , RX2Data, PACKET_SIZE);
  
   __GPIOC_CLK_ENABLE(); 
	 /*
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart6);
  HAL_UART_Receive_IT(&huart6, RX6Data, PACKET_SIZE);

*/

}



// parse UART0 buffer and set command variables

uint8_t parseUARTbuffer(int joy[])
{
	uint8_t button = 0;

	float m = 0;
	joy[0] = 0;
	joy[1] = 0;
	joy[2] = 0;


	m = (millis() - timestamp2);
	if( m < 0) { 
		RX2Data[0] = 0;
	} else if ( m < UART_TIMEOUT)
	{
//0xFA 0x41 0x02 0x00        -> debug dump 

           if( RX2Data[0] == START_BYTE) {

             joy[0] = RX2Data[2]; 
	     button = DEBUG_CMD; 
	     RX2Data[0] = 0; 
	          
	     LOG("----   %d %d %d %d \r\n", RX2Data[0], RX2Data[1], RX2Data[2], RX2Data[3] );
	   }
	}



	return button; 
}	





