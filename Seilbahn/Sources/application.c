/*
 * application.c
 *
 *  Created on: 11.03.2018
 *      Author: El_Matador
 */

#include "application.h"

typedef enum imCmd {
	imCmd_INIT_TELE = 1,
	imCmd_DRIVE_DISTANCE = 2,
	imCmd_DRIVE_JOG = 3,
	imCmd_DRIVE_TO_END = 4,
	imCmd_MOVE_TELE = 5,
	imCmd_ENABLE_MAGNET = 6,
	imCmd_DISABLE_MAGNET = 7
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
				case imCmd_INIT_TELE:
					handleInitTele(++cmdStream);
					break;
				case imCmd_DRIVE_DISTANCE:
					handleDriveDistance(++cmdStream);
					break;
				case imCmd_DRIVE_JOG:
					handleDriveJog(++cmdStream);
					break;
				case imCmd_DRIVE_TO_END:
					handleDriveToEnd(++cmdStream);
					break;
				case imCmd_MOVE_TELE:
					handleMoveTele(++cmdStream);
					break;
				case imCmd_ENABLE_MAGNET:
					handleEnableMagnet(++cmdStream);
					break;
				case imCmd_DISABLE_MAGNET:
					handleDisableMagnet(++cmdStream);
					break;
			}

			vPortFree(cmdStream);
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

	char* cmdStream = pvPortMalloc(sizeof(char) * size);
	for(int i = 0; i < size; i++)
		cmdStream[i] = queue_read(commandQueue);

	return cmdStream;
}

void handleInitTele(char* cmdStream) {
	// Wenn InitTele wird 1 Byte in die driveTelescopeQueue geschrieben
	queue_write(driveTelescopeQueue, telescopeCmd_INIT_TELE);
}

void handleDriveDistance(char* cmdStream) {
	//Wenn driveDistance oder driveToEnd werden 4 Bytes in die driveQueue geschrieben
	queue_write(driveQueue, driveCmd_DRIVE_DISTANCE); // CMD Byte
	queue_write(driveQueue, cmdStream[0]); // Distance 2 Bytes
	queue_write(driveQueue, cmdStream[1]);
	queue_write(driveQueue, cmdStream[2]); // speed
	queue_write(driveQueue, cmdStream[3]); // direction
}

void handleDriveJog(char* cmdStream) {
	//Wenn driveJog werden 2 Bytes in die driveQueue geschrieben
	queue_write(driveQueue, driveCmd_DRIVE_JOG); // CMD Byte
	queue_write(driveQueue, cmdStream[0]); // speed
	queue_write(driveQueue, cmdStream[1]); // direction
}

void handleDriveToEnd(char* cmdStream) {
	//Wenn driveDistance oder driveToEnd werden 4 Bytes in die driveQueue geschrieben
	queue_write(driveQueue, driveCmd_DRIVE_TO_END); // CMD Byte
	queue_write(driveQueue, cmdStream[0]); // predicted distance 2 Bytes
	queue_write(driveQueue, cmdStream[1]);
	queue_write(driveQueue, cmdStream[2]); // speed
	queue_write(driveQueue, cmdStream[3]); // direction
}

void handleMoveTele(char* cmdStream) {
	//Wenn MoveTele werden 3 Bytes in die driveTelescopeQueue geschrieben
	queue_write(driveTelescopeQueue, telescopeCmd_DRIVE_TELE);
	queue_write(driveTelescopeQueue, cmdStream[0]); // drive distance 2 Bytes
	queue_write(driveTelescopeQueue, cmdStream[1]);
	queue_write(driveTelescopeQueue, cmdStream[2]); // direction
}

void handleEnableMagnet(char* cmdStream) {
	if(cmdStream[1] == 0)
		queue_write(magnetCmdQueue, magnetCmd_RELEASE);
	else
		queue_write(magnetCmdQueue, magnetCmd_ENFORCE);
}

void handleDisableMagnet(char* cmdSteam) {
	queue_write(magnetCmdQueue, magnetCmd_DISABLE);
}
