/**
  ******************************************************************************
  * @file    app_subghz_phy.c
  * @author  MCD Application Team
  * @brief   Application of the SubGHz_Phy Middleware
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "app_subghz_phy.h"
#include "subghz_phy_app.h"
#include "sys_app.h"
#include "stm32_seq.h"

/* External variables ---------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

void MX_SubGHz_Phy_Init(void)
{
  SystemApp_Init();
  SubghzApp_Init();
}

void MX_SubGHz_Phy_Process(void)
{
  UTIL_SEQ_Run(UTIL_SEQ_DEFAULT);
}

/* Private Functions Definition -----------------------------------------------*/