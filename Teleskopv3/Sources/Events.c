/* ###################################################################
 **     Filename    : Events.c
 **     Project     : Pren1
 **     Processor   : MKL46Z256VLL4
 **     Component   : Events
 **     Version     : Driver 01.00
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2017-12-26, 16:07, # CodeGen: 0
 **     Abstract    :
 **         This is user's event module.
 **         Put your event handler code here.
 **     Contents    :
 **         Cpu_OnNMIINT - void Cpu_OnNMIINT(void);
 **
 ** ###################################################################*/
/*!
 ** @file Events.c
 ** @version 01.00
 ** @brief
 **         This is user's event module.
 **         Put your event handler code here.
 */
/*!
 **  @addtogroup Events_module Events module documentation
 **  @{
 */
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"
#include "Application.h"

#ifdef __cplusplus
extern "C" {
#endif 

/* User includes (#include below this line is not maintained by Processor Expert) */

int nTicks;
int counter;
static int finish = 0;
int mode = 0;
int counter_flag;
int tickFlag;
LDD_TDeviceData *MyPPG1Ptr;

enum speedmode {

	MODE_HYPERSLOW = 1,
	MODE_ULTRASLOW,
	MODE_VERYSLOW,
	MODE_SLOW,
	MODE_MEDIUM,
	MODE_FAST,
	MODE_VERYFAST,
	MODE_ULTRAFAST,
	MODE_HYPERFAST
};

/*
 ** ===================================================================
 **     Event       :  Cpu_OnNMIINT (module Events)
 **
 **     Component   :  Cpu [MKL46Z256LL4]
 */
/*!
 **     @brief
 **         This event is called when the Non maskable interrupt had
 **         occurred. This event is automatically enabled when the [NMI
 **         interrupt] property is set to 'Enabled'.
 */
/* ===================================================================*/
void Cpu_OnNMIINT(void) {
	/* Write your code here ... */
}

/*
 ** ===================================================================
 **     Event       :  FRTOS1_vApplicationStackOverflowHook (module Events)
 **
 **     Component   :  FRTOS1 [FreeRTOS]
 **     Description :
 **         if enabled, this hook will be called in case of a stack
 **         overflow.
 **     Parameters  :
 **         NAME            - DESCRIPTION
 **         pxTask          - Task handle
 **       * pcTaskName      - Pointer to task name
 **     Returns     : Nothing
 ** ===================================================================
 */
void FRTOS1_vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName) {
	/* This will get called if a stack overflow is detected during the context
	 switch.  Set configCHECK_FOR_STACK_OVERFLOWS to 2 to also check for stack
	 problems within nested interrupts, but only do this for debug purposes as
	 it will increase the context switch time. */
	(void) pxTask;
	(void) pcTaskName;
	taskDISABLE_INTERRUPTS();
	/* Write your code here ... */
	for (;;) {
	}
}

/*
 ** ===================================================================
 **     Event       :  FRTOS1_vApplicationTickHook (module Events)
 **
 **     Component   :  FRTOS1 [FreeRTOS]
 **     Description :
 **         If enabled, this hook will be called by the RTOS for every
 **         tick increment.
 **     Parameters  : None
 **     Returns     : Nothing
 ** ===================================================================
 */
void FRTOS1_vApplicationTickHook(void) {
	/* Called for every RTOS tick. */
	/* Write your code here ... */
}

/*
 ** ===================================================================
 **     Event       :  FRTOS1_vApplicationIdleHook (module Events)
 **
 **     Component   :  FRTOS1 [FreeRTOS]
 **     Description :
 **         If enabled, this hook will be called when the RTOS is idle.
 **         This might be a good place to go into low power mode.
 **     Parameters  : None
 **     Returns     : Nothing
 ** ===================================================================
 */
void FRTOS1_vApplicationIdleHook(void) {
	/* Called whenever the RTOS is idle (from the IDLE task).
	 Here would be a good place to put the CPU into low power mode. */
	/* Write your code here ... */
}

/*
 ** ===================================================================
 **     Event       :  FRTOS1_vApplicationMallocFailedHook (module Events)
 **
 **     Component   :  FRTOS1 [FreeRTOS]
 **     Description :
 **         If enabled, the RTOS will call this hook in case memory
 **         allocation failed.
 **     Parameters  : None
 **     Returns     : Nothing
 ** ===================================================================
 */
void FRTOS1_vApplicationMallocFailedHook(void) {
	/* Called if a call to pvPortMalloc() fails because there is insufficient
	 free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	 internally by FreeRTOS API functions that create tasks, queues, software
	 timers, and semaphores.  The size of the FreeRTOS heap is set by the
	 configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
	taskDISABLE_INTERRUPTS();
	/* Write your code here ... */
	for (;;) {
	}
}

/*
 ** ===================================================================
 **     Event       :  AS1_OnBlockReceived (module Events)
 **
 **     Component   :  AS1 [Serial_LDD]
 */
/*!
 **     @brief
 **         This event is called when the requested number of data is
 **         moved to the input buffer.
 **     @param
 **         UserDataPtr     - Pointer to the user or
 **                           RTOS specific data. This pointer is passed
 **                           as the parameter of Init method.
 */
/* ===================================================================*/
void AS1_OnBlockReceived(LDD_TUserData *UserDataPtr) {
	UART_Desc *ptr = (UART_Desc*) UserDataPtr;

	(void) ptr->rxPutFct(ptr->rxChar); /* but received character into buffer */
	(void) AS1_ReceiveBlock(ptr->handle, (LDD_TData *) &ptr->rxChar,
			sizeof(ptr->rxChar));
}

/*
 ** ===================================================================
 **     Event       :  AS1_OnBlockSent (module Events)
 **
 **     Component   :  AS1 [Serial_LDD]
 */
/*!
 **     @brief
 **         This event is called after the last character from the
 **         output buffer is moved to the transmitter.
 **     @param
 **         UserDataPtr     - Pointer to the user or
 **                           RTOS specific data. This pointer is passed
 **                           as the parameter of Init method.
 */
/* ===================================================================*/
void AS1_OnBlockSent(LDD_TUserData *UserDataPtr) {
	UART_Desc *ptr = (UART_Desc*) UserDataPtr;

	ptr->isSent = TRUE; /* set flag so sender knows we have finished */
}

/*
 ** ===================================================================
 **     Event       :  PPG1_OnEnd (module Events)
 **
 **     Component   :  PPG1 [PPG_LDD]
 */
/*!
 **     @brief
 **         This event is called when the specified number of cycles has
 **         been generated. Component and OnEnd event must be enabled.
 **         See [SetEventMask] and [GetEventMask] methods. This event is
 **         available only if a [Interrupt service/event] is enabled.
 **     @param
 **         UserDataPtr     - Pointer to the user or
 **                           RTOS specific data. The pointer passed as
 **                           the parameter of Init method.
 */
/* ===================================================================*/

void PPG1_OnEnd(LDD_TUserData *UserDataPtr) {

	/*

	 if ((nTicks == 10)) {
	 PPG1_SelectPeriod(MyPPG1Ptr, 0);
	 }

	 else if ((counter == 10) || (nTicks == 100)) {
	 PPG1_SelectPeriod(MyPPG1Ptr, MODE_HYPERSLOW);
	 }

	 else if ((counter == 100) || (nTicks == 200)) {
	 PPG1_SelectPeriod(MyPPG1Ptr, MODE_ULTRASLOW);
	 }

	 else if ((counter == 200) || (nTicks == 400)) {
	 PPG1_SelectPeriod(MyPPG1Ptr, MODE_VERYSLOW);
	 }

	 else if ((counter == 400) || (nTicks == 700)) {
	 PPG1_SelectPeriod(MyPPG1Ptr, MODE_SLOW);
	 }

	 else if ((counter == 700) || (nTicks == 900)) {
	 PPG1_SelectPeriod(MyPPG1Ptr, MODE_MEDIUM);
	 }

	 else if ((counter == 900) || (nTicks == 1500)) {
	 PPG1_SelectPeriod(MyPPG1Ptr, MODE_FAST);
	 }

	 else if ((counter == 1500) || (nTicks == 2000)) {
	 PPG1_SelectPeriod(MyPPG1Ptr, MODE_VERYFAST);
	 }

	 else if ((counter == 2000) || (nTicks == 3000)) {
	 PPG1_SelectPeriod(MyPPG1Ptr, MODE_ULTRAFAST);
	 }

	 else if (counter == 3000)  {
	 PPG1_SelectPeriod(MyPPG1Ptr, MODE_HYPERFAST);
	 }
	 */

	LED2_Neg();
	counter = counter + 1;
	nTicks = nTicks - 1;

	if (nTicks == 0) {
		tickFlag = 1;

		//PPG1_SelectPeriod(MyPPG1Ptr, 1);

	}

}

/*
 ** ===================================================================
 **     Event       :  GPIO1_OnPortEvent (module Events)
 **
 **     Component   :  GPIO1 [GPIO_LDD]
 */
/*!
 **     @brief
 **         Called if defined event on any pin of the port occured.
 **         OnPortEvent event and GPIO interrupt must be enabled. See
 **         SetEventMask() and GetEventMask() methods. This event is
 **         enabled if [Interrupt service/event] is Enabled and disabled
 **         if [Interrupt service/event] is Disabled.
 **     @param
 **         UserDataPtr     - Pointer to RTOS device
 **                           data structure pointer.
 */
/* ===================================================================*/
LDD_TDeviceData* MyGPIOPtr;
int counter;
int pos_update_forward_counter;
int pos_update_reverse_counter;
int pos_update_forward_flag;
int pos_update_reverse_flag;
Queue *pQ;
NODE *pN;
extern int counter_regulate;
int queue_error;
int count_to_ten = 0;
int insertFlag;

//extern enum direction {REVERSE = 0, FORWARD = 1, NO_ROTATION = 2};

void GPIO1_OnPortEvent(LDD_TUserData *UserDataPtr) {

	if (GPIO1_GetFieldValue(MyGPIOPtr, InputB)) {		// Motor dreht vorwärts

		counter = counter - 1;
		count_to_ten = count_to_ten - 1;

	} else {											// Motor dreht rückwärts

		counter = counter + 1;
		count_to_ten = count_to_ten + 1;

	}


	if (count_to_ten = 9) {
		insertFlag = 1;
		char* positionUpdate = (char*) malloc(sizeof(char));
		insert(1);
		insertFlag = 0;

		count_to_ten = 0;
	}



	else if(count_to_ten = -9){
		insertFlag = 1;
		insert(-1);
		insertFlag = 0;

		count_to_ten = 0;
	}

}

/* END Events */

#ifdef __cplusplus
} /* extern "C" */
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
