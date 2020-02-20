//******************************************************************************
// this code allows you to use the printf function to generate USART
// Hosting of stdio functionality through USART2
// This allows using printf through USART2
//******************************************************************************
 
#include <rt_misc.h>
#include <stdio.h>
#include <stdlib.h>
#include "stm32l4xx_hal.h"
#include "uart.h"

 
#pragma import(__use_no_semihosting_swi)
 
struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;
 
int fputc(int ch, FILE *f)
{
    static int last;
 
    if ((ch == (int)'\n') && (last != (int)'\r'))
    {
        last = (int)'\r';			
				HAL_UART_Transmit(&huart2,(uint8_t*)&last,1,1000);

    }
    else
        last = ch;
		
		HAL_UART_Transmit(&huart2,(uint8_t*)&last,1,1000);
 
  return(ch);
}
