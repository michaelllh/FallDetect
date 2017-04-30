/**
  ******************************************************************************
  * @file    osx_motion_id.h
  * @author  VMA Application Team
  * @version V1.0.0
  * @date    14-October-2016
  * @brief   Header for osx_motion_id module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Licensed under AMS-ST OpenSoftwareX Limited License Agreement, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        ..\OpenSoftwareX_LLA_evaluation_5Nov2014.pdf
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
  ********************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __OSX_MOTION_ID_H
#define __OSX_MOTION_ID_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup OSX_MOTION_ID OSX_MOTION_ID
  * @{
  */
    

/** @defgroup OSX_MOTION_ID_Exported_Types OSX_MOTION_ID_Exported_Types
 * @{
 */  
/* Exported types ------------------------------------------------------------*/
 typedef struct
{
  float AccX;           /*  acc x axes [g]  */
  float AccY;           /*  acc y axes [g]  */
  float AccZ;           /*  acc z axes [g]  */
} osx_MID_input_t;  

typedef enum 
{
    OSX_MID_0           = 0x00,  /* on desk */ 
    OSX_MID_1           = 0x01,  /* bed, couch, pillow */ 
    OSX_MID_2           = 0x02,  /* light movment */ 
    OSX_MID_3           = 0x03,  /* biking */ 
    OSX_MID_4           = 0x04,  /* TYPING_WRITING */ 
    OSX_MID_5           = 0x05,  /* HI_TYPING__SLOW_WALKING */ 
    OSX_MID_6           = 0x06,  /* WASHING_HANDS_WALKING */ 
    OSX_MID_7           = 0x07,  /* FWALKING */ 
    OSX_MID_8           = 0x08,  /* FWALKING_JOGGING */ 
    OSX_MID_9           = 0x09,  /* FJOGGING_BRUSHING */ 
    OSX_MID_10          = 0x0A   /* SPRINTING */ 
} osx_MID_output_t;


/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup OSX_MOTION_ID_Exported_Functions OSX_MOTION_ID_Exported_Functions
 * @{
 */

/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Initialize the MotionID engine
 * @param  None
 * @retval 1 in case of success, 0 otherwise
 */
uint8_t osx_MotionID_Initialize(void);

/**
 * @brief  Run Intensity Detection Algorithm
 * @param  data_in: pointer to the osx_MID_input_t structure
 * @retval intensity index
 */
osx_MID_output_t osx_MotionID_Update(osx_MID_input_t *data_in);

/**
 * @brief  Reset Libray
 * @param  None
 * @retval None
 */
void osx_MotionID_ResetLib(void);

/**
 * @brief  Get the library version
 * @param  version pointer to an array of 35 char
 * @retval Number of characters in the version string
 */
uint8_t osx_MotionID_GetLibVersion(char *version);


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __OSX_MOTION_ID_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
