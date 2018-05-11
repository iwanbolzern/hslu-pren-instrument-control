#include "driveTelescope.h"
#include "PPG1.h"
#include "DirectionTele.h"
#include "StepMode_M0.h"
#include "StepMode_M1.h"
#include "StepMode_M2.h"
#include "PE_Error.h"
#include "endSwitch_tele.h"

#include "custom_queue.h"
#include "communication.h"

#define PI 3.141592654
#define d 19 //[mm]
#define Umfang (d * PI);	//[mm]
#define ONE_REVOLUTION 200
#define TICKS_PER_MM ONE_REVOLUTION/Umfang // [mm]

// external vars
QueueHandle_t driveTelescopeQueue;

// internal vars
static LDD_TDeviceData* myPPG1Ptr;
static LDD_TDeviceData* zEndSwitchPtr;
static long remainingTicks;
static long counterTelescope;
static char directionTelescope;
static bool zEndSwitch_pressed;

void setDirectionTelescope(char direction) {
	if(direction == teleDirection_RETRACT) {
		DirectionTele_PutVal(TRUE);	//PTC13
	} else {
		DirectionTele_PutVal(FALSE);	//PTC13
	}
}

// M2 	M1 	M0
// 0 	0 	0 	Full step (2-phase excitation) with 71% current
// 0 	0 	1 	1/2 step (1-2 phase excitation)
// 0 	1 	0 	1/4 step (W1-2 phase excitation)
// 0 	1 	1 	8 microsteps/step
// 1 	0 	0 	16 microsteps/step
// 1 	0 	1 	32 microsteps/step
// 1 	1 	0 	32 microsteps/step
// 1 	1 	1 	32 microsteps/step
void setStepMode() {
	StepMode_M0_PutVal(FALSE);
	StepMode_M1_PutVal(TRUE);
	StepMode_M2_PutVal(FALSE);
}

int getTicksToGo(int distance) {
	return distance * 4 * TICKS_PER_MM; //calculate Ticks to go
}

void tele_handleInitTele(void) {
	zEndSwitch_pressed = FALSE;
	setDirectionTelescope(teleDirection_RETRACT);

	PPG1_Enable(myPPG1Ptr);

	while (!zEndSwitch_pressed) {					// einfahren bis Endschalter erreicht
		vTaskDelay(pdMS_TO_TICKS(5));
	}

	PPG1_Disable(myPPG1Ptr);
	counterTelescope = 0;			// counter zurückstellen
}

void tele_handleDriveTele(int distance, char direction) {
	zEndSwitch_pressed = FALSE;
	remainingTicks = getTicksToGo(distance);
	setDirectionTelescope(direction);

	PPG1_Enable(myPPG1Ptr);

	while (remainingTicks > 0) {
		vTaskDelay(pdMS_TO_TICKS(10));			//200Hz
	}
	PPG1_Disable(myPPG1Ptr); // STOP Pulsgenerator
}

void driveTelescope(void * pvParameter) {
	zEndSwitchPtr = endSwitch_tele_Init(NULL);
	myPPG1Ptr = PPG1_Init(NULL);//  unter properties "enable in init. code" ankreuzen falls etwas nicht funktioniert
	PPG1_SetRatio16(myPPG1Ptr, 0x7FFF);
	setStepMode();

	for (;;) {
		char cmd = queue_readInfinity(driveTelescopeQueue);

		switch(cmd)  {
			case telescopeCmd_INIT_TELE:
				tele_handleInitTele();
				queue_write(endQueue, endCmd_END_INIT_TELE);
				break;
			case telescopeCmd_DRIVE_TELE: {
				int distance = ((queue_readInfinity(driveTelescopeQueue) & 0x00FF) << 8);
				distance += queue_readInfinity(driveTelescopeQueue) & 0x00FF;


				directionTelescope = queue_readInfinity(driveTelescopeQueue);
				tele_handleDriveTele(distance, directionTelescope);
				queue_write(endQueue, endCmd_END_MOVE_TELE);
				break;
			}
		}
	}
}

void tele_tickReceived(void) {
	remainingTicks--;
	if (directionTelescope == teleDirection_RETRACT) {
		counterTelescope--;
	} else {
		counterTelescope++;
	}

	if ((counterTelescope % 40) == 0) {
		if(counterTelescope > 0)
			queue_writeFromISR(zPosQueue, 3);
		else
			queue_writeFromISR(zPosQueue, -3);
		counterTelescope = 0;
	}

	if(remainingTicks <= 0) {
		queue_writeFromISR(endQueue, endCmd_END_MOVE_TELE);
	}
}

void tele_endSwitchReceived(void) {
	zEndSwitch_pressed = TRUE;

	PPG1_Disable(myPPG1Ptr); // STOP Pulsgenerator
}
