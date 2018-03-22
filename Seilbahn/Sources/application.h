#ifndef SOURCES_APPLICATION_H_
#define SOURCES_APPLICATION_H_

#include "FRTOS1.h"

void application(void*);

typedef enum imCmd {
	imCmd_INIT_TELE = 1,
	imCmd_DRIVE_DISTANCE = 2,
	imCmd_DRIVE_JOG = 3,
	imCmd_DRIVE_TO_END = 4,
	imCmd_MOVE_TELE = 5,
	imCmd_ENABLE_MAGNET = 6,
	imCmd_DISABLE_MAGNET = 7
} imCmd_t;

extern QueueHandle_t commandQueue;

#endif /* SOURCES_APPLICATION_H_ */
