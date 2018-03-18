/*
 * application.c
 *
 *  Created on: 11.03.2018
 *      Author: El_Matador
 */

#include "application.h"

typedef enum imCmd {
	INIT_TELE = 1,
	DRIVE_DISTANCE = 2,
	DRIVE_JOG = 3,
	DRIVE_TO_END = 4,
	MOVE_TELE = 5,
	ENABLE_MAGNET = 6,
	DISABLE_MAGNET = 7
} imCmd_t;

char* getCmdStream(char size);

// handle functions
void handleInitTele(char*);
void handleDriveDistance(char*);
void handleDriveJog(char*);
void handleDriveToEnd(char*);
void handleMoveTele(char*);
void handleEnableMagnet(char*);
void handleDisableMagnet(char*);

void application(void* pvParameter) {
	for (;;) {
		if (!queue_isEmpty(commandQueue)) {	//Sind Inhalte in der commandQueue werden diese gelesen
			char cmdSize = queue_read(commandQueue);
			char* cmdStream = getCmdStream(cmdSize);

			switch(cmdStream[0]) { // you need ++ to skip command byte
				case INIT_TELE:
					handleInitTele(++cmdStream);
					break;
				case DRIVE_DISTANCE:
					handleDriveDistance(++cmdStream);
					break;
				case DRIVE_JOG:
					handleDriveJog(++cmdStream);
					break;
				case DRIVE_TO_END:
					handleDriveToEnd(++cmdStream);
					break;
				case MOVE_TELE:
					handleMoveTele(++cmdStream);
					break;
				case ENABLE_MAGNET:
					handleEnableMagnet(++cmdStream);
					break;
				case DISABLE_MAGNET:
					handleDisableMagnet(++cmdStream);
					break;
			}

			free(cmdStream);
		} else {
			vTaskDelay(pdMS_TO_TICKS(10));
		}
	}
}

char* getCmdStream(char size) {
	// wait until queue has all bytes
	while(queue_size(commandQueue) < size) {
		vTaskDelay(pdMS_TO_TICKS(10));
	}

	char* cmdStream = malloc(sizeof(char) * size);
	for(int i = 0; i < size; i++)
		cmdStream[i] = queue_read(commandQueue);

	return cmdStream;
}

void handleInitTele(char* cmdStream) {
	// Wenn InitTele wird 1 Byte in die driveTelescopeQueue geschrieben
	queue_write(driveTelescopeQueue, cmdStream[2]);
}

void handleDriveDistance(char* cmdStream) {
	//Wenn driveDistance oder driveToEnd werden 4 Bytes in die driveQueue geschrieben
	for (int i = 0; i < 4; i++) {
		queue_write(driveQueue, cmdStream[i]);
	}
}

void handleDriveJog(char* cmdStream) {
	//Wenn driveJog werden 2 Bytes in die driveQueue geschrieben
	for (int i = 0; i < 2; i++) {
		queue_write(driveQueue, cmdStream[i]);
	}
}

void handleDriveToEnd(char* cmdStream) {
	//Wenn driveDistance oder driveToEnd werden 4 Bytes in die driveQueue geschrieben
	for (int i = 0; i < 4; i++) {
		queue_write(driveQueue, cmdStream[i]);
	}
}

void handleMoveTele(char* cmdStream) {
	//Wenn MoveTele werden 3 Bytes in die driveTelescopeQueue geschrieben
	for (int i = 0; i < 3; i++) {
		queue_write(driveTelescopeQueue, cmdStream[i]);
	}
}

void handleEnableMagnet(char* cmdStream) {
	if(cmdStream[1] == 0)
		queue_write(magnetCmdQueue, RELEASE);
	else
		queue_write(magnetCmdQueue, RELEASE);
}

void handleDisableMagnet(char* cmdSteam) {
	queue_write(magnetCmdQueue, DISABLE);
}
