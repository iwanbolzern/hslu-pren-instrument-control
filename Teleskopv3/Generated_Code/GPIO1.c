/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : GPIO1.c
**     Project     : Teleskopv3
**     Processor   : MKL46Z256VLL4
**     Component   : GPIO_LDD
**     Version     : Component 01.128, Driver 01.08, CPU db: 3.00.000
**     Repository  : Kinetis
**     Compiler    : GNU C Compiler
**     Date/Time   : 2018-03-02, 10:49, # CodeGen: 6
**     Abstract    :
**         The HAL GPIO component will provide a low level API for unified
**         access to general purpose digital input/output pins across
**         various device designs.
**
**         RTOS drivers using HAL GPIO API will be simpler and more
**         portable to various microprocessors.
**     Settings    :
**          Component name                                 : GPIO1
**          Port                                           : PTA
**          Port width                                     : 32 bits
**          Mask of allocated pins                         : 0x30000
**          Interrupt service/event                        : Enabled
**            Interrupt                                    : INT_PORTA
**            Interrupt priority                           : medium priority
**          Bit fields                                     : 2
**            Bit field                                    : 
**              Field name                                 : InputA
**              Pins                                       : 1
**                Pin                                      : 
**                  Pin                                    : PTA16/SPI0_MOSI/SPI0_MISO/I2S0_RX_FS/I2S0_RXD0
**                  Pin signal                             : 
**                  Initial pin direction                  : Input
**                  Initial pin event                      : Rising edge
**            Bit field                                    : 
**              Field name                                 : InputB
**              Pins                                       : 1
**                Pin                                      : 
**                  Pin                                    : PTA17/SPI0_MISO/SPI0_MOSI/I2S0_MCLK
**                  Pin signal                             : 
**                  Initial pin direction                  : Input
**                  Initial pin event                      : Disabled
**          Initialization                                 : 
**            Auto initialization                          : no
**            Event mask                                   : 
**              OnPortEvent                                : Enabled
**     Contents    :
**         Init            - LDD_TDeviceData* GPIO1_Init(LDD_TUserData *UserDataPtr);
**         SetFieldValue   - void GPIO1_SetFieldValue(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField...
**         GetFieldValue   - GPIO1_TFieldValue GPIO1_GetFieldValue(LDD_TDeviceData *DeviceDataPtr,...
**         ClearFieldBits  - void GPIO1_ClearFieldBits(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField...
**         SetFieldBits    - void GPIO1_SetFieldBits(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField...
**         ToggleFieldBits - void GPIO1_ToggleFieldBits(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField...
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
** @file GPIO1.c
** @version 01.08
** @brief
**         The HAL GPIO component will provide a low level API for unified
**         access to general purpose digital input/output pins across
**         various device designs.
**
**         RTOS drivers using HAL GPIO API will be simpler and more
**         portable to various microprocessors.
*/         
/*!
**  @addtogroup GPIO1_module GPIO1 module documentation
**  @{
*/         

/* MODULE GPIO1. */

#include "Events.h"
#include "GPIO1.h"
#include "FreeRTOS.h" /* FreeRTOS interface */
#include "IO_Map.h"

#ifdef __cplusplus
extern "C" {
#endif 

typedef struct {
  GPIO1_TPortValue EventFlags;         /* Holds event flags */
  LDD_TUserData *UserData;             /* RTOS device data structure */
} GPIO1_TDeviceData, *GPIO1_TDeviceDataPtr; /* Device data structure type */
/* {FreeRTOS RTOS Adapter} Static object used for simulation of dynamic driver memory allocation */
static GPIO1_TDeviceData DeviceDataPrv__DEFAULT_RTOS_ALLOC;
/* {FreeRTOS RTOS Adapter} Global variable used for passing a parameter into ISR */
static GPIO1_TDeviceData * INT_PORTA__BAREBOARD_RTOS_ISRPARAM;
/*
** ===================================================================
**     Method      :  GPIO1_Init (component GPIO_LDD)
*/
/*!
**     @brief
**         This method initializes the associated peripheral(s) and the
**         component internal variables. The method is called
**         automatically as a part of the application initialization
**         code.
**     @param
**         UserDataPtr     - Pointer to the RTOS device
**                           structure. This pointer will be passed to
**                           all events as parameter.
**     @return
**                         - Pointer to the dynamically allocated private
**                           structure or NULL if there was an error.
*/
/* ===================================================================*/
LDD_TDeviceData* GPIO1_Init(LDD_TUserData *UserDataPtr)
{
  /* Allocate LDD device structure */
  GPIO1_TDeviceData *DeviceDataPrv;

  /* {FreeRTOS RTOS Adapter} Driver memory allocation: Dynamic allocation is simulated by a pointer to the static object */
  DeviceDataPrv = &DeviceDataPrv__DEFAULT_RTOS_ALLOC;
  /* Save RTOS Device structure */
  DeviceDataPrv->EventFlags = 0x00U;   /* Clears stored events */
  DeviceDataPrv->UserData = UserDataPtr; /* Store the RTOS device structure */
  /* Interrupt vector(s) allocation */
  /* {FreeRTOS RTOS Adapter} Set interrupt vector: IVT is static, ISR parameter is passed by the global variable */
  INT_PORTA__BAREBOARD_RTOS_ISRPARAM = DeviceDataPrv;
  /* GPIOA_PDDR: PDD&=~0x00030000 */
  GPIOA_PDDR &= (uint32_t)~(uint32_t)(GPIO_PDDR_PDD(0x00030000));
  /* Initialization of Port Control registers */
  /* PORTA_PCR16: ISF=0,MUX=1 */
  PORTA_PCR16 = (uint32_t)((PORTA_PCR16 & (uint32_t)~(uint32_t)(
                 PORT_PCR_ISF_MASK |
                 PORT_PCR_MUX(0x06)
                )) | (uint32_t)(
                 PORT_PCR_MUX(0x01)
                ));
  /* PORTA_PCR17: ISF=0,MUX=1 */
  PORTA_PCR17 = (uint32_t)((PORTA_PCR17 & (uint32_t)~(uint32_t)(
                 PORT_PCR_ISF_MASK |
                 PORT_PCR_MUX(0x06)
                )) | (uint32_t)(
                 PORT_PCR_MUX(0x01)
                ));
  /* PORTA_PCR16: ISF=1,IRQC=9 */
  PORTA_PCR16 = (uint32_t)((PORTA_PCR16 & (uint32_t)~(uint32_t)(
                 PORT_PCR_IRQC(0x06)
                )) | (uint32_t)(
                 PORT_PCR_ISF_MASK |
                 PORT_PCR_IRQC(0x09)
                ));
  /* NVIC_IPR7: PRI_30=0x80 */
  NVIC_IPR7 = (uint32_t)((NVIC_IPR7 & (uint32_t)~(uint32_t)(
               NVIC_IP_PRI_30(0x7F)
              )) | (uint32_t)(
               NVIC_IP_PRI_30(0x80)
              ));
  /* NVIC_ISER: SETENA|=0x40000000 */
  NVIC_ISER |= NVIC_ISER_SETENA(0x40000000);
  /* Registration of the device structure */
  PE_LDD_RegisterDeviceStructure(PE_LDD_COMPONENT_GPIO1_ID,DeviceDataPrv);
  return ((LDD_TDeviceData *)DeviceDataPrv);
}

/*
** ===================================================================
**     Method      :  GPIO1_SetFieldValue (component GPIO_LDD)
*/
/*!
**     @brief
**         This method sets the output data value of the specified bit
**         field.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @param
**         Field           - Bit field to write. Bit fields are
**                           defined during design time and for each bit
**                           field there is a generated constant.
**     @param
**         Value           - Aligned data value to writting to the
**                           specified bit field. The bit 0 corresponds
**                           with the pin which has index 0 within the
**                           given bit field, the bit 1 corresponds with
**                           the pin which has index 1 within the given
**                           bit field, etc.
*/
/* ===================================================================*/
void GPIO1_SetFieldValue(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField Field, GPIO1_TFieldValue Value)
{
  (void)DeviceDataPtr;                 /* Parameter is not used, suppress unused argument warning */
  switch (Field) {                     /* no break */
    case InputA: {                     /* bit field #0 */
      GPIO_PDD_SetPortDataOutput(GPIO1_MODULE_BASE_ADDRESS,
        (
          GPIO_PDD_GetPortDataOutput(GPIO1_MODULE_BASE_ADDRESS)
          & ((GPIO1_TPortValue)(~((GPIO1_TPortValue)GPIO1_InputA_MASK)))
        )
        | (
          ((GPIO1_TPortValue)(Value << GPIO1_InputA_START_BIT))
          & ((GPIO1_TPortValue)GPIO1_InputA_MASK)
        )
      );
      break;
    }
    case InputB: {                     /* bit field #1 */
      GPIO_PDD_SetPortDataOutput(GPIO1_MODULE_BASE_ADDRESS,
        (
          GPIO_PDD_GetPortDataOutput(GPIO1_MODULE_BASE_ADDRESS)
          & ((GPIO1_TPortValue)(~((GPIO1_TPortValue)GPIO1_InputB_MASK)))
        )
        | (
          ((GPIO1_TPortValue)(Value << GPIO1_InputB_START_BIT))
          & ((GPIO1_TPortValue)GPIO1_InputB_MASK)
        )
      );
      break;
    }
    default:
      break;                           /* Invalid Field is not treated, result is undefined */
  } /* switch (Field) */
}

/*
** ===================================================================
**     Method      :  GPIO1_GetFieldValue (component GPIO_LDD)
*/
/*!
**     @brief
**         This method returns the current input data of the specified
**         field.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @param
**         Field           - Bit field to reading. Bit fields are
**                           defined during design time and for each bit
**                           field there is a generated constant.
**     @return
**                         - Aligned current port input value masked for
**                           allocated pins of the field. The bit 0
**                           corresponds with the pin which has index 0
**                           within the given bit field, the bit 1
**                           corresponds with the pin which has index 1
**                           within the given bit field, etc.
*/
/* ===================================================================*/
GPIO1_TFieldValue GPIO1_GetFieldValue(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField Field)
{
  (void)DeviceDataPtr;                 /* Parameter is not used, suppress unused argument warning */
  switch (Field) {                     /* no break */
    case InputA: {                     /* bit field #0 */
      return
        (GPIO1_TFieldValue)(
          (
            GPIO_PDD_GetPortDataInput(GPIO1_MODULE_BASE_ADDRESS)
            & (GPIO1_TPortValue)GPIO1_InputA_MASK
          )
          >> GPIO1_InputA_START_BIT
        );
    }
    case InputB: {                     /* bit field #1 */
      return
        (GPIO1_TFieldValue)(
          (
            GPIO_PDD_GetPortDataInput(GPIO1_MODULE_BASE_ADDRESS)
            & (GPIO1_TPortValue)GPIO1_InputB_MASK
          )
          >> GPIO1_InputB_START_BIT
        );
    }
    default:
      break;                           /* Invalid BitField is not treated, result is undefined */
  } /* switch (Field) */
  return (GPIO1_TFieldValue)0U;
}

/*
** ===================================================================
**     Method      :  GPIO1_ClearFieldBits (component GPIO_LDD)
*/
/*!
**     @brief
**         This method drives the specified bits of the specified bit
**         field to the inactive level.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @param
**         Field           - Bit field to write. Bit fields are
**                           defined during design time and for each bit
**                           field there is a generated constant.
**     @param
**         Mask            - Aligned mask of bits to setting the
**                           inactive level. Each field pin has
**                           corresponding bit in the mask. Bit value 0
**                           means not selected bit, bit value 1 means
**                           selected bit. The bit 0 corresponds with
**                           the pin which has index 0 within the given
**                           bit field, the bit 1 corresponds with the
**                           pin which has index 1 within the given bit
**                           field, etc.
*/
/* ===================================================================*/
void GPIO1_ClearFieldBits(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField Field, GPIO1_TFieldValue Mask)
{
  (void)DeviceDataPtr;                 /* Parameter is not used, suppress unused argument warning */
  switch (Field) {                     /* no break */
    case InputA: {                     /* bit field #0 */
      GPIO_PDD_ClearPortDataOutputMask(GPIO1_MODULE_BASE_ADDRESS,
        ((GPIO1_TPortValue)GPIO1_InputA_MASK)
        & ((GPIO1_TPortValue)(Mask << GPIO1_InputA_START_BIT))
      );
      break;
    }
    case InputB: {                     /* bit field #1 */
      GPIO_PDD_ClearPortDataOutputMask(GPIO1_MODULE_BASE_ADDRESS,
        ((GPIO1_TPortValue)GPIO1_InputB_MASK)
        & ((GPIO1_TPortValue)(Mask << GPIO1_InputB_START_BIT))
      );
      break;
    }
    default:
      break;                           /* Invalid Field is not treated, result is undefined */
  } /* switch (Field) */
}

/*
** ===================================================================
**     Method      :  GPIO1_SetFieldBits (component GPIO_LDD)
*/
/*!
**     @brief
**         This method drives the specified bits of the specified bit
**         field to the active level.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @param
**         Field           - Bit field to write. Bit fields are
**                           defined during design time and for each bit
**                           field there is a generated constant.
**     @param
**         Mask            - Aligned mask of bits to setting the
**                           active level. Each field pin has
**                           corresponding bit in the mask. Bit value 0
**                           means not selected bit, bit value 1 means
**                           selected bit. The bit 0 corresponds with
**                           the pin which has index 0 within the given
**                           bit field, the bit 1 corresponds with the
**                           pin which has index 1 within the given bit
**                           field, etc.
*/
/* ===================================================================*/
void GPIO1_SetFieldBits(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField Field, GPIO1_TFieldValue Mask)
{
  (void)DeviceDataPtr;                 /* Parameter is not used, suppress unused argument warning */
  switch (Field) {                     /* no break */
    case InputA: {                     /* bit field #0 */
      GPIO_PDD_SetPortDataOutputMask(GPIO1_MODULE_BASE_ADDRESS,
        ((GPIO1_TPortValue)GPIO1_InputA_MASK)
        & ((GPIO1_TPortValue)(Mask << GPIO1_InputA_START_BIT))
      );
      break;
    }
    case InputB: {                     /* bit field #1 */
      GPIO_PDD_SetPortDataOutputMask(GPIO1_MODULE_BASE_ADDRESS,
        ((GPIO1_TPortValue)GPIO1_InputB_MASK)
        & ((GPIO1_TPortValue)(Mask << GPIO1_InputB_START_BIT))
      );
      break;
    }
    default:
      break;                           /* Invalid Field is not treated, result is undefined */
  } /* switch (Field) */
}

/*
** ===================================================================
**     Method      :  GPIO1_ToggleFieldBits (component GPIO_LDD)
*/
/*!
**     @brief
**         This method inverts the specified bits of the specified bit
**         field to other level.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @param
**         Field           - Bit field to write. Bit fields are
**                           defined during design time and for each bit
**                           field there is a generated constant.
**     @param
**         Mask            - Aligned mask of bits to inverting the
**                           current level. Each field pin has
**                           corresponding bit in the mask. Bit value 0
**                           means not selected bit, bit value 1 means
**                           selected bit. The bit 0 corresponds with
**                           the pin which has index 0 within the given
**                           bit field, the bit 1 corresponds with the
**                           pin which has index 1 within the given bit
**                           field, etc.
*/
/* ===================================================================*/
void GPIO1_ToggleFieldBits(LDD_TDeviceData *DeviceDataPtr, LDD_GPIO_TBitField Field, GPIO1_TFieldValue Mask)
{
  (void)DeviceDataPtr;                 /* Parameter is not used, suppress unused argument warning */
  switch (Field) {                     /* no break */
    case InputA: {                     /* bit field #0 */
      GPIO_PDD_TogglePortDataOutputMask(GPIO1_MODULE_BASE_ADDRESS,
        ((GPIO1_TPortValue)GPIO1_InputA_MASK)
        & ((GPIO1_TPortValue)(Mask << GPIO1_InputA_START_BIT))
      );
      break;
    }
    case InputB: {                     /* bit field #1 */
      GPIO_PDD_TogglePortDataOutputMask(GPIO1_MODULE_BASE_ADDRESS,
        ((GPIO1_TPortValue)GPIO1_InputB_MASK)
        & ((GPIO1_TPortValue)(Mask << GPIO1_InputB_START_BIT))
      );
      break;
    }
    default:
      break;                           /* Invalid Field is not treated, result is undefined */
  } /* switch (Field) */
}

/*
** ===================================================================
**     Method      :  GPIO1_Interrupt (component GPIO_LDD)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes event(s) of the component.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(GPIO1_Interrupt)
{
  /* {FreeRTOS RTOS Adapter} ISR parameter is passed through the global variable */
  GPIO1_TDeviceDataPtr DeviceDataPrv = INT_PORTA__BAREBOARD_RTOS_ISRPARAM;
  GPIO1_TPortValue State;

  State = (GPIO1_TPortValue)(PORT_PDD_GetInterruptFlags(GPIO1_PORTCONTROL_BASE_ADDRESS)
          & ((GPIO1_TPortValue)GPIO1_ALLOCATED_PINS_MASK)); /* Retrieve flags */
  DeviceDataPrv->EventFlags |= State;
  PORT_PDD_ClearInterruptFlags(GPIO1_PORTCONTROL_BASE_ADDRESS, State); /* Clears flags */
  GPIO1_OnPortEvent(DeviceDataPrv->UserData); /* Call OnPortEvent event */
}

/* END GPIO1. */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

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