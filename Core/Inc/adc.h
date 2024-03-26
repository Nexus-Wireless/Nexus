/**
 ******************************************************************************
 * @file    adc.h
 * @brief   This file contains all the function prototypes for
 *          the adc.c file
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported functions prototypes ---------------------------------------------*/
void MX_ADC_Init(void);

extern ADC_HandleTypeDef hadc;
extern DMA_HandleTypeDef hdma_adc;

// extern uint32_t AD_RES_BUFFER[2];

extern uint16_t AD_RES_BUFFER[2];

#ifdef __cplusplus
}
#endif
#endif /* __ADC_H__ */
