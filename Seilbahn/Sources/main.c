/* ###################################################################
 **     Filename    : main.c
 **     Project     : Seilbahn
 **     Processor   : MKL46Z256VLL4
 **     Version     : Driver 01.01
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2018-03-11, 16:57, # CodeGen: 0
 **     Abstract    :
 **         Main module.
 **         This module contains user's application code.
 **     Settings    :
 **     Contents    :
 **         No public methods
 **
 ** ###################################################################*/
/*!
 ** @file main.c
 ** @version 01.01
 ** @brief
 **         Main module.
 **         This module contains user's application code.
 */
/*!
 **  @addtogroup main_module main module documentation
 **  @{
 */
/* MODULE main */

/* Including needed modules to compile this module/procedure */
#include "application.h"
#include "drive.h"
#include "driveTelescope.h"
#include "Communication.h"
#include "custom_queue.h"



#include "Cpu.h"
#include "Events.h"
#include "FRTOS1.h"
#include "MCUC1.h"
#include "UTIL1.h"
#include "AS1.h"
#include "RxBuf.h"
#include "CS1.h"
#include "PPG1.h"
//#include "PPG1.h"			nur noch im driveTelescope.c vorhanden
#include "PWM1.h"
#include "PwmLdd1.h"
#include "GPIO1.h"
#include "IN1.h"
#include "BitIoLdd1.h"
#include "IN2.h"
#include "BitIoLdd2.h"
#include "TU1.h"
#include "Bit1.h"
#include "BitIoLdd3.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */

#define MAXQUEUESIZE 1000



QueueHandle_t driveQueue;
QueueHandle_t driveTelescopeQueue;
QueueHandle_t commandQueue;
QueueHandle_t xPosQueue;
QueueHandle_t zPosQueue;





/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/* Write your local variable definition here */

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */
	/* For example: for(;;) { } */



	driveQueue =  queue_create(MAXQUEUESIZE);
	driveTelescopeQueue =  queue_create(MAXQUEUESIZE);
	xPosQueue =  queue_create(MAXQUEUESIZE);
	zPosQueue =  queue_create(MAXQUEUESIZE);
	commandQueue = queue_create(MAXQUEUESIZE);





	if (FRTOS1_xTaskCreate(
			application, /* pointer to the task */
			(signed portCHAR *)"Applikation Task", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY+1, /* initial priority */
			(xTaskHandle*)NULL /* optional task handle to create */
	) != pdPASS) {
		for (;;) {
		}; /* Out of heap memory? */
	}

	if (FRTOS1_xTaskCreate(
			drive, /* pointer to the task */
			(signed portCHAR *)"Drive Task", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY+1, /* initial priority */
			(xTaskHandle*)NULL /* optional task handle to create */
	) != pdPASS) {
		for (;;) {
		}; /* Out of heap memory? */
	}

	if (FRTOS1_xTaskCreate(
			communication, /* pointer to the task */
			(signed portCHAR *)"Kommunikations Task", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY+1, /* initial priority */
			(xTaskHandle*)NULL /* optional task handle to create */
	) != pdPASS) {
		for (;;) {
		}; /* Out of heap memory? */
	}

	if (FRTOS1_xTaskCreate(
			driveTelescope, /* pointer to the task */
			(signed portCHAR *)"Teleskop Task", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY+1, /* initial priority */
			(xTaskHandle*)NULL /* optional task handle to create */
	) != pdPASS) {
		for (;;) {
		}; /* Out of heap memory? */
	}

	/*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
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
