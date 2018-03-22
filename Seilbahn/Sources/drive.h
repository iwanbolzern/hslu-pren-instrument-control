#ifndef SOURCES_DRIVE_H_
#define SOURCES_DRIVE_H_

#include "FRTOS1.h"

void drive(void*);
void drive_tickReceived(void);
void drive_endSwitchReceived(void);

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

#endif /* SOURCES_DRIVE_H_ */
