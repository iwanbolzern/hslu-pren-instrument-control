#include "FRTOS1.h"
#include "MCUC1.h"
#include "UTIL1.h"
#include "AS1.h"
#include "ASerialLdd1.h"
#include "RxBuf.h"
#include "CS1.h"
#include "PPG1.h"
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
#include "Magnet1.h"
#include "BitIoLdd4.h"
#include "Magnet2.h"
#include "BitIoLdd5.h"
#include "end_Switch.h"
#include "endSwitch_tele.h"
/* Including shared modules, which are used for whole project */
#include "Cpu.h"
#include "Events.h"
#include "FRTOS1.h"
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */
#include "application.h"
#include "drive.h"
#include "driveTelescope.h"
#include "communication.h"
#include "custom_queue.h"
#include "magnet.h"

#define MAXQUEUESIZE 2000

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */
	driveQueue =  queue_create(100);
	driveTelescopeQueue =  queue_create(100);
	xPosQueue =  queue_create(3000);
	zPosQueue =  queue_create(3000);
	commandQueue = queue_create(1000);
	magnetCmdQueue = queue_create(100);
	endQueue = queue_create(100);

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
			tskIDLE_PRIORITY+2, /* initial priority */
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

	if (FRTOS1_xTaskCreate(
				magnetHandler, /* pointer to the task */
				(signed portCHAR *) "Magnet Task", /* task name for kernel awareness debugging */
				configMINIMAL_STACK_SIZE, /* task stack size */
				(void*) NULL, /* optional task startup argument */
				tskIDLE_PRIORITY + 1, /* initial priority */
				(xTaskHandle*) NULL /* optional task handle to create */
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
