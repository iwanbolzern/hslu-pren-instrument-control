/*
 * drive.h
 *
 *  Created on: 11.03.2018
 *      Author: El_Matador
 */

#ifndef SOURCES_DRIVE_H_
#define SOURCES_DRIVE_H_


#include "PE_Types.h"
#include "custom_queue.h"
#include "communication.h"

void drive(void*);

typedef enum driveCmd {
	driveCmd_DRIVE_DISTANCE = 2,
	driveCmd_DRIVE_JOG = 3,
	driveCmd_DRIVE_TO_END = 4,
} driveCmd_t;

typedef enum direction {
	FORWARD = 0,
	REVERSE = 1,
	FAST_STOP = 2
} direction_t;


extern QueueHandle_t driveQueue;
extern QueueHandle_t xPosQueue;
extern QueueHandle_t endDriveQueue;
extern int driveCounter;
extern direction_t direction;

#endif /* SOURCES_DRIVE_H_ */
