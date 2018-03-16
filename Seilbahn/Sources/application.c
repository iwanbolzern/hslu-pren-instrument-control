/*
 * application.c
 *
 *  Created on: 11.03.2018
 *      Author: El_Matador
 */

#include "application.h"
#include "FRTOS1.h"

typedef enum applicationState applicationState_t;
enum applicationState {
	IDLE = 1, READING = 2, WRITING = 3
};
applicationState_t applicationState = IDLE;

char commandByte[7];

void application(void* pvParameter) {
	int i = 0;

	for (;;) {
		if (!queue_isEmpty(commandQueue)) {	//Sind Inhalte in der commandQueue werden diese gelesen
			commandByte[0] = queue_read(commandQueue);
			commandByte[1] = queue_read(commandQueue);//Anhand dem zweiten Byte in der commandQueue kann die Anzahl folgender Bytes bestimmt werden
			for (i; i < commandByte[1]; i++) {//Alle restlichen zum Kommande gehörenden Bytes werden in das commandByte Array abgelegt
				commandByte[i + 2] = queue_read(commandQueue);
			}
			if ((commandByte[2] == 2) || (commandByte[2] == 4)) {//Wenn driveDistance oder driveToEnd werden 7Bytes in die driveQueue geschrieben
				for (int j = 0; j < 7; j++) {
					queue_write(driveQueue, commandByte[j]);
				}
			}
			else if (commandByte[2] == 3) {//Wenn driveJog werden 5Bytes in die driveQueue geschrieben
				for (int j = 0; j < 5; j++) {
					queue_write(driveQueue, commandByte[j]);
				}
			}
			else if ((commandByte[2] == 1) || (commandByte[2] == 7)) {//Wenn InitTele oder DisableMagnet werden 3Bytes in die driveTelescopeQueue geschrieben
				queue_write(driveTelescopeQueue, commandByte[2]);

			}
			else if (commandByte[2] == 5) {//Wenn MoveTele werden 5Bytes in die driveTelescopeQueue geschrieben
				for (int j = 2; j < 7; j++) {
					queue_write(driveTelescopeQueue, commandByte[j]);
				}
			}
			else {		//Bei EnableMagnet -> 4Bytes zu driveTelescopeQueue
				for (int j = 2; j < 4; j++) {
					queue_write(driveTelescopeQueue, commandByte[j]);
				}
			}
			vTaskDelay(pdMS_TO_TICKS(10));
		}
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}
