#include "driveTelescope.h"
#include "PPG1.h"
#include "Bit1.h"
#include "PE_Error.h"
#include "endSwitch_tele.h"

#include "custom_queue.h"
#include "communication.h"

#define PI 3.141592654
#define d 21 //[mm]
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
	Bit1_PutVal(direction);	//PTC13
}

int getTicksToGo(int distance) {
	return distance * TICKS_PER_MM; //calculate Ticks to go
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
	remainingTicks = distance;
	setDirectionTelescope(directionTelescope);

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

	for (;;) {
		char cmd = queue_readInfinity(driveTelescopeQueue);

		switch(cmd)  {
			case telescopeCmd_INIT_TELE:
				tele_handleInitTele();
				break;
			case telescopeCmd_DRIVE_TELE: {
				int distance = ((queue_readInfinity(driveTelescopeQueue) & 0x00FF) << 8);
				distance += queue_readInfinity(driveTelescopeQueue) & 0x00FF;


				directionTelescope = queue_readInfinity(driveTelescopeQueue);
				tele_handleDriveTele(distance, directionTelescope);
				break;
			}
		}
	}
}

void tele_tickReceived(void) {
	remainingTicks--;
	if (directionTelescope == teleDirection_RETRACT) {
		counterTelescope--;
		if ((counterTelescope % 3) == 0)
			queue_writeFromISR(zPosQueue, -3);
	} else {
		counterTelescope++;
		if ((counterTelescope % 3) == 0)
			queue_writeFromISR(zPosQueue, 3);
	}

	 if(remainingTicks <= 0) {
		queue_writeFromISR(endQueue, endCmd_END_MOVE_TELE);
	 }
}

void tele_endSwitchReceived(void) {
	zEndSwitch_pressed = TRUE;

	PPG1_Disable(myPPG1Ptr); // STOP Pulsgenerator
}
