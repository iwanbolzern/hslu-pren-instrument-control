/*
 * application.h
 *
 *  Created on: 11.03.2018
 *      Author: El_Matador
 */

#ifndef SOURCES_APPLICATION_H_
#define SOURCES_APPLICATION_H_

#include "PE_Types.h"

#include "custom_queue.h"
#include "queue.h"


void application(void*);


extern QueueHandle_t driveQueue;
extern QueueHandle_t driveTelescopeQueue;
extern QueueHandle_t commandQueue;


#endif /* SOURCES_APPLICATION_H_ */