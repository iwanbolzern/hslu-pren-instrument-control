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
#include "queue.h"

void drive(void*);

typedef enum direction direction_t;
enum direction {
	FORWARD = 0, REVERSE = 1, FAST_STOP = 2
};

extern QueueHandle_t driveQueue;
extern QueueHandle_t xPosQueue;
extern int driveCounter;
extern direction_t direction;

#endif /* SOURCES_DRIVE_H_ */
