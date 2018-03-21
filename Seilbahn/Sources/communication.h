/*
 * communication.h
 *
 *  Created on: 11.03.2018
 *      Author: El_Matador
 */

#ifndef SOURCES_COMMUNICATION_H_
#define SOURCES_COMMUNICATION_H_


#include "PE_Types.h"
#include "PE_LDD.h"



void communication(void*);


typedef struct {
  LDD_TDeviceData *handle; /* LDD device handle */
  volatile uint8_t isSent; /* this will be set to 1 once the block has been sent */
  uint8_t rxChar; /* single character buffer for receiving chars */
  uint8_t (*rxPutFct)(uint8_t); /* callback to put received character into buffer */
} UART_Desc;

typedef enum endCmd_ {
	END_INIT_TELE = 8,
	END_DRIVE_DISTANCE = 9,
	END_MOVE_TELE = 10
} endCmd_t;

extern QueueHandle_t commandQueue;
extern QueueHandle_t xPosQueue;
extern QueueHandle_t zPosQueue;
extern QueueHandle_t endQueue;



#endif /* SOURCES_COMMUNICATION_H_ */
