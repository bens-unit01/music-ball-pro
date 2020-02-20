/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include "stm32l4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>


#define BLINKM_ADDR  19

#define  START_BYTE                 0xFA
#define  DRIVE                      0x81
#define	 GOTO_ANGLE                 0x40
#define  WAKE_WORD_DETECTED         0x42
#define  DEBUG_CMD                  0x41

#define  ESTOP                      0x65
#define  CLEAR_ESTOP                0x66
#define  DP_REACH_BEACON            0x43
#define  DP_NORDIC_MB_TEST          0x44
#define	 NOTF_NORDIC_MB_TEST        0x45
#define	 NOTF_DP_TARGET_REACHED     0x46
#define  NOTF_VOICE_RECORD          0x50
#define  DP_GET_CLOSEST_BEACON      0x48
#define  NOTF_DP_CLOSEST_BEACON     0x47
#define  DP_CHANGE_RANGE            0x49
#define  NOTF_MCU_UP                0x55
#define  ACK_BYTE                   0xFD

#define DEBUG_DUMP                  0x35
#define DEBUG_STATE                 0x36
#define DEBUG_ST_STATE              0x37
#define DEBUG_TEST_01               0x38

// BlinkM RGB commands

#define RGB_CTRL_COMMAND   0x54

    // RGB supported values

/*
1) Active Internet connection - Blue
2) No Internet connection - Red
3) Listening – Amber / orange
4) Thinking Green long pulse
5) Speaking? Green flashing (with intonation of voice?)
6) resting state – green solid (if none of the above present)

  */

#define RGB_OFF               0x00
#define RGB_SOLID_RED         0x01
#define RGB_SOLID_BLUE        0x02
#define RGB_SOLID_AMBER       0x03
#define RGB_SOLID_GREEN       0x04
#define RGB_FLASH_GREEN_SLOW  0x05
#define RGB_FLASH_GREEN_QUICK 0x06
#define RGB_FLASH_BLUE_SLOW   0x07
#define RGB_FLASH_BLUE_QUICK  0x08


#define DEBUG_MODE    1

#ifdef DEBUG_MODE
#define LOG printf 
#define PGM_LOG(...) 
#else
#define LOG(...)
#define PGM_LOG(...)
#endif

enum APP_STATE {
    STATE_IDLE = 0, 
    STATE_GOTO_ANGLE,
    STATE_ANGLE_REACHED

}; 


typedef struct
{
	uint32_t  num_samples;
	int16_t * address_audio; 
} address_map_t;


 
#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
