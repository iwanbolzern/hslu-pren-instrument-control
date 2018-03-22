/*
 * driveTelescope.h
 *
 *  Created on: 11.03.2018
 *      Author: El_Matador
 */

#ifndef SOURCES_DRIVETELESCOPE_H_
#define SOURCES_DRIVETELESCOPE_H_


#include "custom_queue.h"
#include "communication.h"
#include "PE_Types.h"

void driveTelescope(void*);

typedef enum telescopeCmd {
	telescopeCmd_INIT_TELE = 0,
	telescopeCmd_DRIVE_TELE = 1
} telesopceCmd_t;

extern QueueHandle_t driveTelescopeQueue;
extern QueueHandle_t zPosQueue;
extern QueueHandle_t endQueue;
extern int counterTelescope;
extern int nTicks;
extern int counterFlag;

#endif /* SOURCES_DRIVETELESCOPE_H_ */
