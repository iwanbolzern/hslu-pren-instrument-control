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

typedef enum teleDirection {
	teleDirection_RETRACT = 0,
	teleDirection_EXTEND = 1
} teleDirection_t;

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

int nTicks;
int counterTelescope;
LDD_TDeviceData *MyPPG1Ptr;
char directionTelescope;
extern bool zEndSwitch_pressed;


void setDirectionTelescope(char direction) {
	Bit1_PutVal(direction);	//PTC13
}

int getTicksToGo(int distance) {
	//calculate Ticks to go
	return distance * TICKS_PER_MM;
}

void tele_handleInitTele(void) {
	setDirectionTelescope(teleDirection_RETRACT);

	if (PPG1_Enable(MyPPG1Ptr) == ERR_OK) {			// Error handling
//		PPG1_SelectPeriod(MyPPG1Ptr, MODE_ULTRASLOW);
	}

	while (!zEndSwitch_pressed) {					// einfahren bis Endschalter erreicht
		vTaskDelay(pdMS_TO_TICKS(5));
	}
	PPG1_Disable(MyPPG1Ptr);
}

void tele_handleDriveTele(int distance, char direction) {
	nTicks = getTicksToGo(distance);
	counterTelescope = 0;			// counter zurückstellen
	setDirectionTelescope(directionTelescope);


	if (PPG1_Enable(MyPPG1Ptr)== ERR_OK) {			// Error handling
//		PPG1_SelectPeriod(MyPPG1Ptr, MODE_MEDIUM);

	}
	else {
		// error handling
	}
	while (nTicks > 0) {

					vTaskDelay(pdMS_TO_TICKS(5));			//200Hz
				}
	// STOP Pulsgenerator
				PPG1_Disable(MyPPG1Ptr);
}

LDD_TDeviceData* zEndSwitchPtr;
void driveTelescope(void * pvParameter) {
	zEndSwitchPtr = endSwitch_tele_Init(NULL);
	MyPPG1Ptr = NULL;
	MyPPG1Ptr = PPG1_Init(NULL);//  unter properties "enable in init. code" ankreuzen falls etwas nicht funktioniert

	// Fehlerhandling falls initialisierung fehlschlägt

	for (;;) {
		while (queue_isEmpty(driveTelescopeQueue)) {
			vTaskDelay(pdMS_TO_TICKS(20));			// 50Hz
		}
		char cmd = queue_read(driveTelescopeQueue);

		switch(cmd)  {
			case telescopeCmd_INIT_TELE:
				tele_handleInitTele();
				break;
			case telescopeCmd_DRIVE_TELE: {
				int distance = queue_read(driveTelescopeQueue);
				distance <<= 8;
				distance += queue_read(driveTelescopeQueue);
				directionTelescope = queue_read(driveTelescopeQueue);
				tele_handleDriveTele(distance, directionTelescope);
				break;
			}
		}
	}
}
