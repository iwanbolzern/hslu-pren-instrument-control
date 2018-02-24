/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : PPG1.h
**     Project     : Teleskopv3
**     Processor   : MKL46Z256VLL4
**     Component   : PPG_LDD
**     Version     : Component 01.015, Driver 01.03, CPU db: 3.00.000
**     Repository  : Kinetis
**     Compiler    : GNU C Compiler
**     Date/Time   : 2018-02-23, 08:54, # CodeGen: 0
**     Abstract    :
**          This component implements a programmable pulse generator that
**          generates signal with variable duty and variable cycle (period).
**     Settings    :
**          Component name                                 : PPG1
**          Module name                                    : TPM2
**          Counter                                        : TPM2_CNT
**          Period device                                  : TPM2_MOD
**          Duty device                                    : TPM2_C0V
**          Output pin                                     : LCD_P2/ADC0_SE12/TSI0_CH7/PTB2/I2C0_SCL/TPM2_CH0
**          Output pin signal                              : 
**          Interrupt service/event                        : Enabled
**            Interrupt                                    : INT_TPM2
**            Interrupt priority                           : maximal priority
**            Iterations before action/event               : 1
**          Period                                         : 100 ms
**          Starting pulse width                           : 25 ms
**          Initial polarity                               : low
**          Initialization                                 : 
**            Enabled in init. code                        : yes
**            Auto initialization                          : no
**            Event mask                                   : 
**              OnEnd                                      : Enabled
**          CPU clock/configuration selection              : 
**            Clock configuration 0                        : This component enabled
**            Clock configuration 1                        : This component disabled
**            Clock configuration 2                        : This component disabled
**            Clock configuration 3                        : This component disabled
**            Clock configuration 4                        : This component disabled
**            Clock configuration 5                        : This component disabled
**            Clock configuration 6                        : This component disabled
**            Clock configuration 7                        : This component disabled
**     Contents    :
**         Init         - LDD_TDeviceData* PPG1_Init(LDD_TUserData *UserDataPtr);
**         Enable       - LDD_TError PPG1_Enable(LDD_TDeviceData *DeviceDataPtr);
**         Disable      - LDD_TError PPG1_Disable(LDD_TDeviceData *DeviceDataPtr);
**         SetEventMask - LDD_TError PPG1_SetEventMask(LDD_TDeviceData *DeviceDataPtr, LDD_TEventMask...
**         SelectPeriod - LDD_TError PPG1_SelectPeriod(LDD_TDeviceData *DeviceDataPtr, PPG1_TClockList...
**         SetRatio16   - LDD_TError PPG1_SetRatio16(LDD_TDeviceData *DeviceDataPtr, uint16_t Ratio);
**         SetDutyMS    - LDD_TError PPG1_SetDutyMS(LDD_TDeviceData *DeviceDataPtr, uint16_t Time);
**
**     Copyright : 1997 - 2015 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file PPG1.h
** @version 01.03
** @brief
**          This component implements a programmable pulse generator that
**          generates signal with variable duty and variable cycle (period).
*/         
/*!
**  @addtogroup PPG1_module PPG1 module documentation
**  @{
*/         

#ifndef __PPG1_H
#define __PPG1_H

/* MODULE PPG1. */

/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* Include inherited beans */

#include "TPM_PDD.h"
#include "Cpu.h"

#ifdef __cplusplus
extern "C" {
#endif 

/* These constants are used to specify several distinctive modes of timing defined in the Period property,
   if the Runtime settings is set to "from list of values".
   These constants may be used as a parameter for SelectPeriod method, value is an index of selected user mode.
   The SP_[Timing] constant name denotes a value of period as it is specified by the user, value without spaces and "."
   is replaced by "_", e.g. for example 16_384ms for 16.384 ms. */
#ifndef __BWUserType_PPG1_TClockList
#define __BWUserType_PPG1_TClockList
  typedef enum {
    PPG1_SP_100ms          = 0U,       /* Constant for predefined period "100ms" selection. */
    PPG1_SP_50ms           = 1U,       /* Constant for predefined period "50ms" selection. */
    PPG1_SP_30ms           = 2U,       /* Constant for predefined period "30ms" selection. */
    PPG1_SP_15ms           = 3U,       /* Constant for predefined period "15ms" selection. */
    PPG1_SP_5ms            = 4U,       /* Constant for predefined period "5ms" selection. */
    PPG1_SP_3ms            = 5U,       /* Constant for predefined period "3ms" selection. */
    PPG1_SP_1ms            = 6U,       /* Constant for predefined period "1ms" selection. */
    PPG1_SP_0_5ms          = 7U,       /* Constant for predefined period "0_5ms" selection. */
    PPG1_SP_0_1ms          = 8U,       /* Constant for predefined period "0_1ms" selection. */
    PPG1_SP_0_05ms         = 9U,       /* Constant for predefined period "0_05ms" selection. */
    PPG1_SP_6ms            = 10U,      /* Constant for predefined period "6ms" selection. */
    PPG1_SP_7ms            = 11U,      /* Constant for predefined period "7ms" selection. */
    PPG1_SP_8ms            = 12U,      /* Constant for predefined period "8ms" selection. */
    PPG1_SP_9ms            = 13U,      /* Constant for predefined period "9ms" selection. */
    PPG1_SP_11ms           = 14U,      /* Constant for predefined period "11ms" selection. */
    PPG1_SP_12ms           = 15U       /* Constant for predefined period "12ms" selection. */
  } PPG1_TClockList;                   /* Type for list of predefined timings. */
#endif
#define PPG1_PERIOD_TICKS  0x00UL      /* Initial period value in ticks of the timer. */
#define PPG1_PERIOD_TICKS_0 0x00UL     /* Period value in ticks of the timer in clock configuration 0. */
#define PPG1_PERIOD_COUNT 0x10U        /* Count of predefined counter input frequencies */
/*! Peripheral base address of a device allocated by the component. This constant can be used directly in PDD macros. */
#define PPG1_PRPH_BASE_ADDRESS  0x4003A000U
  
/* Methods configuration constants - generated for all enabled component's methods */
#define PPG1_Init_METHOD_ENABLED       /*!< Init method of the component PPG1 is enabled (generated) */
#define PPG1_Enable_METHOD_ENABLED     /*!< Enable method of the component PPG1 is enabled (generated) */
#define PPG1_Disable_METHOD_ENABLED    /*!< Disable method of the component PPG1 is enabled (generated) */
#define PPG1_SetEventMask_METHOD_ENABLED /*!< SetEventMask method of the component PPG1 is enabled (generated) */
#define PPG1_SelectPeriod_METHOD_ENABLED /*!< SelectPeriod method of the component PPG1 is enabled (generated) */
#define PPG1_SetRatio16_METHOD_ENABLED /*!< SetRatio16 method of the component PPG1 is enabled (generated) */
#define PPG1_SetDutyMS_METHOD_ENABLED  /*!< SetDutyMS method of the component PPG1 is enabled (generated) */

/* Events configuration constants - generated for all enabled component's events */
#define PPG1_OnEnd_EVENT_ENABLED       /*!< OnEnd event of the component PPG1 is enabled (generated) */



/*
** ===================================================================
**     Method      :  PPG1_Init (component PPG_LDD)
*/
/*!
**     @brief
**         Initializes the device. Allocates memory for the device data
**         structure, allocates interrupt vectors and sets interrupt
**         priority, sets pin routing, sets timing, etc. If the
**         property ["Enable in init. code"] is set to "yes" value then
**         the device is also enabled (see the description of the
**         [Enable] method). In this case the [Enable] method is not
**         necessary and needn't to be generated. This method can be
**         called only once. Before the second call of Init the [Deinit]
**         must be called first.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer will be
**                           passed as an event or callback parameter.
**     @return
**                         - Pointer to the dynamically allocated private
**                           structure or NULL if there was an error.
*/
/* ===================================================================*/
LDD_TDeviceData* PPG1_Init(LDD_TUserData *UserDataPtr);

/*
** ===================================================================
**     Method      :  PPG1_Enable (component PPG_LDD)
*/
/*!
**     @brief
**         Enables the component - it starts the signal generation.
**         Events may be generated (see SetEventMask).
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - The component does not work in
**                           the active clock configuration
*/
/* ===================================================================*/
LDD_TError PPG1_Enable(LDD_TDeviceData *DeviceDataPtr);

/*
** ===================================================================
**     Method      :  PPG1_Disable (component PPG_LDD)
*/
/*!
**     @brief
**         Disables the component - it stops signal generation and
**         events calling.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - The component does not work in
**                           the active clock configuration
*/
/* ===================================================================*/
LDD_TError PPG1_Disable(LDD_TDeviceData *DeviceDataPtr);

/*
** ===================================================================
**     Method      :  PPG1_SetEventMask (component PPG_LDD)
*/
/*!
**     @brief
**         Enables/disables event(s). The events contained within the
**         mask are enabled. Events not contained within the mask are
**         disabled. The component event masks are defined in the
**         PE_Types.h file. Note: Event that are not generated (See the
**         "Events" tab in the Component inspector) are not handled by
**         this method. In this case the method returns ERR_PARAM_MASK
**         error code. See also method [GetEventMask].
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @param
**         EventMask       - Event mask
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - The component does not work in
**                           the active clock configuration
**                           ERR_PARAM_MASK - Event mask is not valid
*/
/* ===================================================================*/
LDD_TError PPG1_SetEventMask(LDD_TDeviceData *DeviceDataPtr, LDD_TEventMask EventMask);

/*
** ===================================================================
**     Method      :  PPG1_SelectPeriod (component PPG_LDD)
*/
/*!
**     @brief
**         Sets new period (setting prescaler a clock source, not an
**         external source such as pin). Up to 16 different values can
**         be predefined in designed-time. This method is enabled only
**         if ["Runtime setting type"] property is set to "from list of
**         values" and if a list of possible periods settings is
**         specified at design time. Each of these settings constitutes
**         a _/period/_ and Processor Expert assigns them a _/period
**         identifier/_. The prescaler corresponding to each mode are
**         calculated at design time. Modes can be switched at runtime
**         just by referring to a mode identifier. No run-time
**         calculations are performed, all the calculations are
**         performed at design time. The periods and periods
**         identifiers may be found in the include file *.h.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @param
**         Period          - New output period as enumeration
**                           value.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active clock configuration
**                           ERR_PARAM_VALUE - Invalid parameter Period
*/
/* ===================================================================*/
LDD_TError PPG1_SelectPeriod(LDD_TDeviceData *DeviceDataPtr, PPG1_TClockList Period);

/*
** ===================================================================
**     Method      :  PPG1_SetRatio16 (component PPG_LDD)
*/
/*!
**     @brief
**         This method sets a new duty-cycle ratio. Ratio is expressed
**         as a 16-bit unsigned integer number. 0 - FFFF value is
**         proportional to ratio 0 - 100%. The method is available
**         only if it is not selected list of predefined values in
**         [Starting pulse width] property. 
**         Note: Calculated duty depends on the timer possibilities and
**         on the selected period.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @param
**         Ratio           - Ratio to set. 0 - 65535 value is
**                           proportional to ratio 0 - 100%
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - The component does not work in
**                           the active clock configuration
*/
/* ===================================================================*/
LDD_TError PPG1_SetRatio16(LDD_TDeviceData *DeviceDataPtr, uint16_t Ratio);

/*
** ===================================================================
**     Method      :  PPG1_SetDutyMS (component PPG_LDD)
*/
/*!
**     @brief
**         This method sets the new duty value of the output signal.
**         The duty is expressed in milliseconds as a 16-bit unsigned
**         integer number. The method is available only if it is not
**         selected list of predefined values in [Starting pulse width]
**         property.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @param
**         Time            - Duty to set [in milliseconds]
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - The component does not work in
**                           the active clock configuration
**                           ERR_MATH - Overflow during evaluation
**                           ERR_PARAM_RANGE - Parameter out of range
*/
/* ===================================================================*/
LDD_TError PPG1_SetDutyMS(LDD_TDeviceData *DeviceDataPtr, uint16_t Time);

/*
** ===================================================================
**     Method      :  PPG1_SetClockConfiguration (component PPG_LDD)
**
**     Description :
**         This method changes the clock configuration. During a clock 
**         configuration change the component changes it's setting 
**         immediately upon a request.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void PPG1_SetClockConfiguration(LDD_TDeviceData *DeviceDataPtr, LDD_TClockConfiguration ClockConfiguration);

/*
** ===================================================================
**     Method      :  PPG1_Interrupt (component PPG_LDD)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes event(s) of the component.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
/* {FreeRTOS RTOS Adapter} ISR function prototype */
PE_ISR(PPG1_Interrupt);

/* END PPG1. */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __PPG1_H */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
