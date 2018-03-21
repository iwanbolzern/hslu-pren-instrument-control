/*
 * drive.c
 *
 *  Created on: 11.03.2018
 *      Author: El_Matador
 */

#include "drive.h"
#include "PWM1.h"
#include "IN1.h"
#include "IN2.h"
#include "stdlib.h"
#include "FreeRTOS.h"
#include "GPIO1.h"
#include "end_Switch.h"
#include "timers.h"

int calculateTicksToDrive(int);
int calculateSpeed(int);
void setDirection(direction_t);
char* getAppCmdStream(char);
void driveDistance(unsigned int, unsigned int);
void driveToEnd();
void driveJog();
void prepareForBoundedDrive(char*);
void prepareForUnboundedDrive(char*);

LDD_TDeviceData* MyGPIOPtr;
LDD_TDeviceData* MyEndSwitchPtr;
direction_t direction;

extern int endSwitch_pressed;
int driveCounter = 0;
unsigned int distance;
unsigned int speedFromIm;
unsigned int x = 30;
unsigned int v = 100;
unsigned int internTicks;
unsigned int internSpeed;
const int MIN_SPEED = 0xffff;
const int STOP = 0xffff;



typedef enum appCmd {
	DRIVE_DISTANCE = 2,
	DRIVE_JOG = 3,
	DRIVE_TO_END = 4,
} appCmd_t;

void drive(void* pvParameter){
	MyGPIOPtr = GPIO1_Init(NULL);
	MyEndSwitchPtr = end_Switch_Init(NULL);
	PWM1_SetRatio16(STOP);
	PWM1_Enable();
	for(;;){
		if(!queue_isEmpty(driveQueue)){							//Sind Inhalte in der commandQueue werden diese gelesen
			char appCmd = queue_read(driveQueue);
			char* appCmdStream = getAppCmdStream(appCmd);				//Anhand dem zweiten Byte in der commandQueue kann die Anzahl folgender Bytes bestimmt werden

			switch(appCmd){
			case DRIVE_DISTANCE:
				prepareForBoundedDrive(appCmdStream);
				driveDistance(x,v);
				queue_write(endQueue, END_DRIVE_DISTANCE);
				vTaskDelay(pdMS_TO_TICKS(20));
				break;
			case DRIVE_JOG:
				prepareForUnboundedDrive(appCmdStream);
				driveJog();
				vTaskDelay(pdMS_TO_TICKS(20));
				break;
			case DRIVE_TO_END:
				prepareForBoundedDrive(appCmdStream);
				driveToEnd();
				queue_write(endQueue, END_RUN);
				vTaskDelay(pdMS_TO_TICKS(20));
				break;
			}
			free(appCmdStream);
		}
		else{
			vTaskDelay(pdMS_TO_TICKS(20));
		}
	}
}

int calculateTicksToDrive(int distance) {
	int l = (distance / (0.12));
	return l;
}

int calculateSpeed(int speed){
	return ((speed * MIN_SPEED)/100);
}

void setDirection(direction_t d) {
	if (d == FORWARD) {
		IN1_PutVal(FALSE);
		IN2_PutVal(TRUE);
		direction = FORWARD;
	} else if (d == REVERSE) {
		IN1_PutVal(TRUE);
		IN2_PutVal(FALSE);
		direction = REVERSE;
	} else if (d == FAST_STOP) {
		IN1_PutVal(TRUE);
		IN2_PutVal(TRUE);
		direction = FAST_STOP;
	}
}

void driveDistance(unsigned int x, unsigned int v) {
	int speedWasSet = 0;
	while (driveCounter < internTicks) {
		while (driveCounter < ((internTicks * x)/100)) {
			if (!speedWasSet) {
				PWM1_SetRatio16((internSpeed * v)/100);
				speedWasSet = 1;
			}
		}
		x = x + 10;
		v = v + 15;
		if (((internSpeed * v)/100) < ((MIN_SPEED * 90)/100)) {
			driveDistance(x, v);
		}
	}
	PWM1_SetRatio16(STOP);
// Fast Stop initialisierung
//	setDirection(REVERSE);
//	PWM1_SetRatio16(0xffff * 0.3);
//	setDirection(FAST_STOP);
}

void driveToEnd(){
int speedWasSet = 0;
	while(!endSwitch_pressed){
		if (!speedWasSet) {
			PWM1_SetRatio16(internSpeed);
			speedWasSet = 1;
		}
		vTaskDelay(pdMS_TO_TICKS(20));
	}
	PWM1_SetRatio16(STOP);
}

void driveJog(){
	PWM1_SetRatio16(internSpeed);
}

char* getAppCmdStream(char appCmd) {
	// wait until queue has all bytes
	int size = 0;
	if(appCmd == 3){
		size = 2;
	}
	else{
		size = 4;
	}
	while(queue_size(driveQueue) < size) {
		vTaskDelay(pdMS_TO_TICKS(10));
	}

	char* cmdStream = malloc(sizeof(char) * size);
	for(int i = 0; i < size; i++)
		cmdStream[i] = queue_read(driveQueue);

	return cmdStream;
}

void prepareForBoundedDrive(char* appCmdStream){
	distance = (appCmdStream[0]+appCmdStream[1]);
	speedFromIm = appCmdStream[2];
	internSpeed = calculateSpeed(speedFromIm);
	internTicks = calculateTicksToDrive(distance);
	direction = appCmdStream[3];
	setDirection(direction);
	driveCounter = 0;
}

void prepareForUnboundedDrive(char* appCmdStream){
	speedFromIm = appCmdStream[0];
	internSpeed = calculateSpeed(speedFromIm);
	direction = appCmdStream[1];
	setDirection(direction);
	driveCounter = 0;
}
