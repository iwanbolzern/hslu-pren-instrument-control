/*
 * driveTelescope.c
 *
 *  Created on: 11.03.2018
 *      Author: El_Matador
 */

#include "driveTelescope.h"
#include "PPG1.h"
#include "Bit1.h"
#include "PE_Error.h"

#include "Cpu.h"
#include "Events.h"
#include "FRTOS1.h"
#include "MCUC1.h"
#include "UTIL1.h"

#define PI 3.141592654
#define d 21 //[mm]
#define Umfang (d * PI);	//[mm]
#define ONE_REVOLUTION 200
#define TICKS_PER_MM ONE_REVOLUTION/Umfang // [mm]

typedef enum {
	IDLE, SET_UP, INIT_TELE, MOVE_TELE, MEASURE, STOP
} state_t;

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

state_t state;
int nTicks;
int counterTelescope;
LDD_TDeviceData *MyPPG1Ptr;
int counterFlag;
char directionTelescope;

int runden(float x) {
	if (x > 0)
		return (int) (x + 0.5);

	return (int) (x - 0.5);
}

void setDirectionTelescope(char direction) {

	Bit1_PutVal(direction);	//PTC13

}

int getTicksToGo(int distance) {

	//calculate Ticks to go
	return distance * TICKS_PER_MM;

}
void driveTelescope(void * pvParameter) {

	char magnet;


	int param1;
	int param2;
	int temp;
	int counter_old ;
	int tickCounter;
	int distance;
	MyPPG1Ptr = NULL;
	MyPPG1Ptr = PPG1_Init(NULL);//  unter properties "enable in init. code" ankreuzen falls etwas nicht funktioniert

	// Fehlerhandling falls initialisierung fehlschlägt

	for (;;) {

		switch (state) {

		case IDLE:
			while (queue_isEmpty(driveTelescopeQueue)) {
				vTaskDelay(pdMS_TO_TICKS(20));			// 50Hz
			}
			state = SET_UP;
			break;

		case SET_UP:

			switch (queue_read(driveTelescopeQueue)) {
			case telescopeCmd_INIT_TELE:
				state = INIT_TELE;
				break;
			case telescopeCmd_DRIVE_TELE:



				distance = (queue_read(driveTelescopeQueue))*256 + queue_read(driveTelescopeQueue);


				nTicks = getTicksToGo(distance);

				directionTelescope = queue_read(driveTelescopeQueue);
				state = MOVE_TELE;
				break;
			}

			break;

		case INIT_TELE:

			setDirectionTelescope(0x00);		//einfahren

			if (PPG1_Enable(MyPPG1Ptr) == ERR_OK) {			// Error handling
		//		PPG1_SelectPeriod(MyPPG1Ptr, MODE_ULTRASLOW);
			}

			// drive Slowly (Einfahren)

			/*	while (!endschalter) {					// einfahren bis Endschalter erreicht
			 vTaskDelay(pdMS_TO_TICKS(5));
			 }
			 */
			PPG1_Disable(MyPPG1Ptr);
			state = IDLE;
			break;

		case MOVE_TELE:

			counterTelescope = 0;			// counter zurückstellen
			setDirectionTelescope(directionTelescope);


			if (PPG1_Enable(MyPPG1Ptr)== ERR_OK) {			// Error handling
		//		PPG1_SelectPeriod(MyPPG1Ptr, MODE_MEDIUM);

			}
			else{

				// error handling
			}





			// je nach nTicks eine entsprechende Geschwindigkeit festlegen oder einfach immer die selbe geschwindigkeit

			state = MEASURE;
			break;

		case MEASURE:

			counterFlag == 0;
			while (nTicks > 0) {

				vTaskDelay(pdMS_TO_TICKS(5));			//200Hz
			}

			state = STOP;

			break;

		case STOP:

			// STOP Pulsgenerator
			PPG1_Disable(MyPPG1Ptr);
			state = IDLE;
			break;

		}
	}

}

/*
 void communication(void* pvParameter) {

 (void) pvParameter;

 Init();		// Initialisierung von der asynchronverbindung

 char xPos = '0';
 char zPos = '0';
 char message = NULL;

 for (;;) {

 switch (state) {

 case IDLE:

 if (!(queue_isEmpty(xPosQueue) && (queue_isEmpty(zPosQueue)))) { // es wurde min. in eine der beiden Queues gewchrieben

 state = SEND_POS_TO_IM;

 }

 else if (RxBuf_NofElements() != 0) {

 state = READ_FROM_IM;
 }

 else {

 vTaskDelay(pdMS_TO_TICKS(20));				// 50 Hz
 }

 break;

 case SEND_POS_TO_IM:

 while (!(queue_isEmpty(xPosQueue) && (queue_isEmpty(zPosQueue)))) {

 if (queue_isEmpty(xPosQueue)) {	// xQueue ist leer, dh der Eintrag ist in der zQueue
 xPos = '0';
 zPos = queue_read(zPosQueue);

 SendChar(xPos, &deviceData);
 SendChar(zPos, &deviceData);
 }

 else if (queue_isEmpty(zPosQueue)) {// zQueue ist leer, dh der Eintrag ist in der xQueue
 zPos = '0';
 xPos = queue_read(xPosQueue);

 SendChar(xPos, &deviceData);
 SendChar(zPos, &deviceData);

 }

 else {					// beide Queues haben einen Eintrag
 zPos = queue_read(zPosQueue);
 xPos = queue_read(xPosQueue);
 SendChar(xPos, &deviceData);
 SendChar(zPos, &deviceData);
 }

 }

 state = IDLE;

 break;

 case ERROR:

 // Error Messages: SendString("blablabla went Wrong")

 state = IDLE;
 break;
 case READ_FROM_IM:

 while (RxBuf_NofElements() != 0) {
 (void) RxBuf_Get(&message);
 queue_write(commandQueue, message);

 }
 state = IDLE;
 break;

 }

 }
 }

 for (;;) {
 }
 */
