/**
  ******************************************************************************
  * @file    sys_app.c
  * @author  Soran Ismail
  * @brief   Initializes HW and SW system entities (not related to the radio)
  ******************************************************************************

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "platform.h"
#include "sys_app.h"
#include "stm32_seq.h"
#include "stm32_systime.h"
#include "stm32_lpm.h"
#include "timer_if.h"
#include "utilities_def.h"
#include "sys_debug.h"


/* External variables ---------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define MAX_TS_SIZE (int) 16


/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static uint8_t SYS_TimerInitialisedFlag = 0;

/* Private function prototypes -----------------------------------------------*/
/**
  * @brief Returns sec and msec based on the systime in use
  * @param buff to update with timestamp
  * @param size of updated buffer
  */
static void TimestampNow(uint8_t *buff, uint16_t *size);

/**
  * @brief  it calls UTIL_ADV_TRACE_VSNPRINTF
  */
static void tiny_snprintf_like(char *buf, uint32_t maxsize, const char *strFormat, ...);


/* Exported functions ---------------------------------------------------------*/
void SystemApp_Init(void)
{

  /* Ensure that MSI is wake-up system clock */
  __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_MSI);

  /*Initialize timer and RTC*/
  UTIL_TIMER_Init();
  SYS_TimerInitialisedFlag = 1;
  /* Initializes the SW probes pins and the monitor RF pins via Alternate Function */
  DBG_Init();

  /*Initialize the terminal */
  UTIL_ADV_TRACE_Init();
  UTIL_ADV_TRACE_RegisterTimeStampFunction(TimestampNow);

  /*Set verbose LEVEL*/
  UTIL_ADV_TRACE_SetVerboseLevel(VERBOSE_LEVEL);

  /*Init low power manager*/
  UTIL_LPM_Init();
  /* Disable Stand-by mode */
  UTIL_LPM_SetOffMode((1 << CFG_LPM_APPLI_Id), UTIL_LPM_DISABLE);

#if defined (LOW_POWER_DISABLE) && (LOW_POWER_DISABLE == 1)
  /* Disable Stop Mode */
  UTIL_LPM_SetStopMode((1 << CFG_LPM_APPLI_Id), UTIL_LPM_DISABLE);
#elif !defined (LOW_POWER_DISABLE)
#error LOW_POWER_DISABLE not defined
#endif /* LOW_POWER_DISABLE */

}

/**
  * @brief redefines __weak function in stm32_seq.c such to enter low power
  */
void UTIL_SEQ_Idle(void)
{
  UTIL_LPM_EnterLowPower();
}

/* Private functions ---------------------------------------------------------*/

static void TimestampNow(uint8_t *buff, uint16_t *size)
{
  SysTime_t curtime = SysTimeGet();
  tiny_snprintf_like((char *)buff, MAX_TS_SIZE, "%ds%03d:", curtime.Seconds, curtime.SubSeconds);
  *size = strlen((char *)buff);
}

/* Disable StopMode when traces need to be printed */
void UTIL_ADV_TRACE_PreSendHook(void)
{
  UTIL_LPM_SetStopMode((1 << CFG_LPM_UART_TX_Id), UTIL_LPM_DISABLE);
}
/* Re-enable StopMode when traces have been printed */
void UTIL_ADV_TRACE_PostSendHook(void)
{
  UTIL_LPM_SetStopMode((1 << CFG_LPM_UART_TX_Id), UTIL_LPM_ENABLE);
}

static void tiny_snprintf_like(char *buf, uint32_t maxsize, const char *strFormat, ...)
{
  va_list vaArgs;
  va_start(vaArgs, strFormat);
  UTIL_ADV_TRACE_VSNPRINTF(buf, maxsize, strFormat, vaArgs);
  va_end(vaArgs);
}

/* HAL overload functions ---------------------------------------------------------*/

/**
  * @note This function overwrites the __weak one from HAL
  */
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
  /*Don't enable SysTick if TIMER_IF is based on other counters (e.g. RTC) */
  return HAL_OK;
}

/**
  * @note This function overwrites the __weak one from HAL
  */
uint32_t HAL_GetTick(void)
{
  uint32_t ret = 0;
  /* TIMER_IF can be based on other counter the SysTick e.g. RTC */
  if (SYS_TimerInitialisedFlag == 0)
  {
    /* TIMER_IF_GetTimerValue should be used only once UTIL_TIMER_Init() is initialized */
    /* If HAL_Delay or a TIMEOUT countdown is necessary during initialization phase */
    /* please use temporarily another timebase source (SysTick or TIMx), which implies also */
    /* to rework the above function HAL_InitTick() and to call HAL_IncTick() on the timebase IRQ */
    /* Note: when TIMER_IF is based on RTC, stm32wlxx_hal_rtc.c calls this function before TimeServer is functional */
    /* RTC TIMEOUT will not expire, i.e. if RTC has an hw problem it will keep looping in the RTC_Init function */
  }
  else
  {
    ret = TIMER_IF_GetTimerValue();
  }
  return ret;
}

/**
  * @note This function overwrites the __weak one from HAL
  */
void HAL_Delay(__IO uint32_t Delay)
{
  /* TIMER_IF can be based on other counter the SysTick e.g. RTC */
  TIMER_IF_DelayMs(Delay);
}
