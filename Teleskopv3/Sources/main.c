/* ###################################################################
 **     Filename    : main.c
 **     Project     : Pren1
 **     Processor   : MKL46Z256VLL4
 **     Version     : Driver 01.01
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2017-12-26, 16:07, # CodeGen: 0
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
#include "Cpu.h"
#include "Events.h"
#include "FRTOS1.h"
#include "MCUC1.h"
#include "UTIL1.h"
#include "Bit4.h"
#include "BitIoLdd4.h"
#include "LED2.h"
#include "LEDpin2.h"
#include "BitIoLdd1.h"
#include "Bit1.h"
#include "BitIoLdd6.h"
#include "Bit5.h"
#include "BitIoLdd5.h"
#include "Bit6.h"
#include "BitIoLdd7.h"
#include "AS1.h"
#include "RxBuf.h"
#include "CS1.h"
#include "PPG1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Application.h"

/* User includes (#include below this line is not maintained by Processor Expert) */



enum commandos {
	WaitForCom, InitTele, DriveDistance, DriveJog, DriveToEnd, MoveTele, EnableMagnet, DisableMagnet
};

 typedef enum commandos cmd_t;



cmd_t commando;
static cmd_t cmd;		// Kommando byte
static int param1;		// Parameter 1 vom IM mitgegeben
static int param2;		// Parameter 2 vom IM mitgegeben










mode_t mode;

int counter_flag;

static int length;			// Länge der Message
static int param1;
static int param2;
static int param3;



void PICom(void *);
void initializeASI(void*);
void initializeTasks(void*);
void startMethod(void*);
void statemachine();
extern UART_Desc deviceData;
int counter;
int magnet = 0;
int sensor = 0;
int dist_down = 0;
int dist_up = 0;
static int messageReceived = 0;
int messageFlag = 0;
int startup = 0;

#define PI 3.141592654
#define d 21.8 //[mm]						// für kürzeres Aus-/Einfahren muss d erhöht werden
#define Umfang (d * PI);	//[mm]
#define ONE_REVOLUTION 200
#define TICKS_PER_MM ONE_REVOLUTION/Umfang // [mm]

enum speed {
	INITIAL, ULTRASLOW, VERYSLOW, SLOW, MEDIUM, FAST, VERYFAST, ULTRAFAST
};

typedef enum speed speed_t;

enum direction {
	REVERSE, FORWARD
};

typedef enum direction direction_t;

/* User includes (#include below this line is not maintained by Processor Expert) */
LDD_TDeviceData *MyPPG1Ptr;
int counter = 0;
int end_value = 2000;
int nTicks = 0;
int tickFlag;
speed_t selectSpeedMode(int);
int runden(float);
void moveDistance(direction_t, int);
int startUp();
const float Ticks_per_mm = TICKS_PER_MM
;
speed_t sp;
direction_t dir;

int magnet_dir;
int positive = 0;
int negative = 0;

void moveDistance(direction_t dir, int distance) {

	/*
	 * Variable temp: beinhaltet die Anzahl Ticks, die zu erreichen sind bis das Ziel erreicht wurde
	 */
	counter = 0;

	float tick = Ticks_per_mm * distance;
	nTicks = runden(tick);

	/*
	 * gemäss dem Wert von "dir" wird entschiden ob vorwärts oder
	 * Rückwärts fahren
	 */
	if (dir == FORWARD) {
		Bit1_PutVal(TRUE);

	}

	else {
		Bit1_PutVal(FALSE);
	}

	if (nTicks != 0) {
		MyPPG1Ptr = PPG1_Init(NULL);
		PPG1_SelectPeriod(MyPPG1Ptr, 6);

		while (tickFlag != 1) {
			vTaskDelay(pdMS_TO_TICKS(1));

		}

		tickFlag = 0;
		PPG1_Disable(MyPPG1Ptr);
	}

}

int runden(float x) {
	if (x > 0)
		return (int) (x + 0.5);

	return (int) (x - 0.5);
}

/*
 ** ===================================================================

 **     Description :
 **
 **        Die Asynchrone Verbindung (UART) und die Statemachine
 **        werden hier gestarte
 **
 **
 **     Parameters  :
 **     	*pvParameters	: Pointer, wird einfach entgegengenommen
 **
 **     Returns     : Nothing

 ** ===================================================================
 */
void initialize(void *pvParameters) {
	(void) pvParameters;

	APP_Run();		// Initialisieren der UART Verbindung
	statemachine();		// Statemachin wird gestartet

	for (;;) {
	}

}








static void move(void* pvParameters) {

	(void) pvParameters;
	int down = 0;
	int up = 0;
	up = dist_up;
	down = dist_down;

	moveDistance(FORWARD, down);

	switch (magnet) {

	case 'p':
		Bit5_PutVal(TRUE);
		Bit6_PutVal(FALSE);

		break;
	case 'n':
		// turn magnetic field on the other side compared to case 'p
		Bit5_PutVal(FALSE);
		Bit6_PutVal(TRUE);

		break;
	case 'i':
		Bit5_PutVal(FALSE);
		Bit6_PutVal(FALSE);
		break;
	}

	vTaskDelay(pdMS_TO_TICKS(1000));

	switch (sensor) {

	case 'e':
		// enable sensor surveillance, create new Task
		break;
	case 'a':
		// diseable sensor surveillance
		break;

	}

	vTaskDelay(pdMS_TO_TICKS(1000));
	moveDistance(REVERSE, up);

	// Magnetpolung wieder auf idle
	Bit5_PutVal(FALSE);
	Bit6_PutVal(FALSE);

	sendText(
			"\n\n\n##################    Testlauf abgeschlossen      #########################");

	sendMenu();
	sendText("\n\nAusfahren [mm]:   FORMAT \"dddd\"\n\n");
	vTaskSuspend(NULL);

}




/*
 ** ===================================================================

 **     Description :
 **
 **        Statemachine, welche die entsprechenden Methoden aufruft,
 **        respektive Tasks generiert für die Methoden
 **
 **
 **     Parameters  : Nothing
 **
 **
 **     Returns     : Nothing

 ** ===================================================================
 */





void statemachine() {

	for (;;) {

		switch (commando) {



		case WaitForCom:


			if (messageReceived) {
				messageFlag = 1;
				(void) getMessage();
				commando = cmd;
				messageFlag = 0;

			}

			vTaskDelay(pdMS_TO_TICKS(5));	// muss noch evaluiert werden ob notwendig

			break;



		case InitTele:

			if (FRTOS1_xTaskCreate(
					initTele, /* pointer to the task */
					(signed portCHAR *)"Initialisierungsprozess", /* task name for kernel awareness debugging */
					configMINIMAL_STACK_SIZE, /* task stack size */
					(void*)NULL, /* optional task startup argument */
					tskIDLE_PRIORITY, /* initial priority */
					(xTaskHandle*)NULL /* optional task handle to create */
			) != pdPASS) {
				for (;;) {
				}; /* Out of heap memory? */
			}

			commando = WaitForCom;

			break;



		case DriveDistance:

			if (FRTOS1_xTaskCreate(
					driveDistance, /* pointer to the task */
					(signed portCHAR *)"DriveDistance", /* task name for kernel awareness debugging */
					configMINIMAL_STACK_SIZE, /* task stack size */
					(void*)NULL, /* optional task startup argument */
					tskIDLE_PRIORITY, /* initial priority */
					(xTaskHandle*)NULL /* optional task handle to create */
			) != pdPASS) {
				for (;;) {
				}; /* Out of heap memory? */
			}

			commando = WaitForCom;

			break;



		case DriveJog:

			if (FRTOS1_xTaskCreate(
					driveJog, /* pointer to the task */
					(signed portCHAR *)"DriveJog", /* task name for kernel awareness debugging */
					configMINIMAL_STACK_SIZE, /* task stack size */
					(void*)NULL, /* optional task startup argument */
					tskIDLE_PRIORITY, /* initial priority */
					(xTaskHandle*)NULL /* optional task handle to create */
			) != pdPASS) {
				for (;;) {
				}; /* Out of heap memory? */
			}

			commando = WaitForCom;

			break;




		case DriveToEnd:

			if (FRTOS1_xTaskCreate(
					driveToEnd, /* pointer to the task */
					(signed portCHAR *)"DriveToEnd", /* task name for kernel awareness debugging */
					configMINIMAL_STACK_SIZE, /* task stack size */
					(void*)NULL, /* optional task startup argument */
					tskIDLE_PRIORITY, /* initial priority */
					(xTaskHandle*)NULL /* optional task handle to create */
			) != pdPASS) {
				for (;;) {
				}; /* Out of heap memory? */
			}

			commando = WaitForCom;

			break;



		case MoveTele:

			if (FRTOS1_xTaskCreate(
					moveTele, /* pointer to the task */
					(signed portCHAR *)"MoveTele", /* task name for kernel awareness debugging */
					configMINIMAL_STACK_SIZE, /* task stack size */
					(void*)NULL, /* optional task startup argument */
					tskIDLE_PRIORITY, /* initial priority */
					(xTaskHandle*)NULL /* optional task handle to create */
			) != pdPASS) {
				for (;;) {
				}; /* Out of heap memory? */
			}

			commando = WaitForCom;

			break;

		case EnableMagnet:

			if (FRTOS1_xTaskCreate(
					enableMagnet, /* pointer to the task */
					(signed portCHAR *)"EnableMagnet", /* task name for kernel awareness debugging */
					configMINIMAL_STACK_SIZE, /* task stack size */
					(void*)NULL, /* optional task startup argument */
					tskIDLE_PRIORITY, /* initial priority */
					(xTaskHandle*)NULL /* optional task handle to create */
			) != pdPASS) {
				for (;;) {
				}; /* Out of heap memory? */
			}

			commando = WaitForCom;

			break;


		case DisableMagnet:

			if (FRTOS1_xTaskCreate(
					disableMagnet, /* pointer to the task */
					(signed portCHAR *)"DisableMagnet", /* task name for kernel awareness debugging */
					configMINIMAL_STACK_SIZE, /* task stack size */
					(void*)NULL, /* optional task startup argument */
					tskIDLE_PRIORITY, /* initial priority */
					(xTaskHandle*)NULL /* optional task handle to create */
			) != pdPASS) {
				for (;;) {
				}; /* Out of heap memory? */
			}

			commando = WaitForCom;

			break;





		}
	}

}




/*
 ** ===================================================================

 **     Description :
 **
 **        Liest die Kommando-ID und die Parameter aus
 **
 **
 **     Parameters  : Nothing
 **
 **
 **     Returns     : Nothing

 ** ===================================================================
 */


void getMessage(){

char c = 0;
length = getLengthOfMessage();
c = getCmd();



switch(c){

case 1:

	cmd = InitTele;
	break;

case 2:

	cmd = DriveDistance;
	// param1, param2 und param3 bestimmen
	break;

case 3:

	cmd = DriveJog;
	// param1 und param2 bestimmen
	break;

case 4:

	cmd = DriveToEnd;
	// param1, param2 und oaram3 bestimmen
	break;

case 5:

	cmd = MoveTele;
	// param1 und param2 bestimmen
	break;

case 6:

	cmd = EnableMagnet;
	// param1 bestimmen
	break;

case 7:

	cmd = DisableMagnet;
	break;
}




	//if( length = ...){

	//cmd =
   // param1 = ...
//	param2 = ...
	//}
//	else if(length = ..){
//		cmd = ..
//		param1 = ...

//	}

//	else {
//		cmd = ...
//	}





}























/*
 ** ===================================================================

 **     Description :
 **
 **        Methode, welche im 5ms abfragt ob eine Message vom IM
 **        gesendet wurde.
 **
 **
 **
 **
 **
 **     Parameters  :
 **     	*pvParameters	: Pointer, wird einfach entgegengenommen
 **     	messageReceived : Flag das gesetzt wird sobald eine Message
 **     				      empfangen wurde
 **     	messageFlag		: wird in der Statemachine generiert sobald
 **     				      begonne wurde die Nachricht zu entschlüsseln
 **     				      Wurde die nachricht entgegengenommen und
 **     				      ausgewertet, wird das Flag wieder auf 0
 **     				      gesetzt und das messageReceived Flag kann
 **     				      wieder den Wert 1 annehmen sobald eine Nachricht
 **     				      generiert wird vom IM
 **
 **     Returns     : Nothing
 ** ===================================================================
 */

void PICom(void *pvParameters) {
	(void) pvParameters;

	for (;;) {
		if ((RxBuf_NofElements() != 0) && (messageFlag == 0)) {

			messageReceived = 1;

		}

		else {
			messageReceived = 0;
		}
		vTaskDelay(pdMS_TO_TICKS(5));
	}
}




/*
 ** ===================================================================

 **     Description :
 **
 **        Methode, welche die Tasks erstellt, die am Anfng benötigt
 **        werden.
 **        - initialize :	Task, welcher die notwendigen Komponenten
 **        						initialisiert.
 **
 **        - PiCommuniction:Task für die permanente Abfrage ob eine
 **        					Nachricht vom IM empfangen wurde
 **
 **
 **     Parameters  :
 **     	*pvParameters	: Pointer, wird einfach entgegengenommen
 **
 **     Returns     : Nothing
 **
 **
 **     Hinweis: Weitere Tasks könne hier generiert werden, zB zum senden
 **     	     der Encoder-Werte
 ** ===================================================================
 */

void initializeTasks(void* pvParameters) {
	(void) pvParameters;

	/* Initialisierungstask*/

	if (FRTOS1_xTaskCreate(
			initialize, /* pointer to the task */
			(signed portCHAR *)"initialize", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY, /* initial priority */
			(xTaskHandle*)NULL /* optional task handle to create */
	) != pdPASS) {
		for (;;) {
		}; /* Out of heap memory? */
	}



	/* Task erstellen, welcher permanent den Empfangsbuffer abfragt*/



	if (FRTOS1_xTaskCreate(
			PICom, /* pointer to the task */
			(signed portCHAR *)"PiCommunication", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY, /* initial priority */
			(xTaskHandle*)NULL /* optional task handle to create */
	) != pdPASS) {
		for (;;) {
		}; /* Out of heap memory? */
	}

	for (;;) {

	}

}





/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/* Write your local variable definition here */

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/*
	 * Generiert einen Task, der die anderen Tasks generiert
	 */
	if (FRTOS1_xTaskCreate(
			initializeTasks, /* pointer to the task */
			(signed portCHAR *)"tasks initialisieren", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY, /* initial priority */
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
}
