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
#include "FRTOS1.h"

#define  GEAR_RATIO 70										// Getriebeuebersetzung
#define  N_TICKS_ENCODER 16									// (Ticks / Revolution)
#define  ONE_Revolution  1120								// eine Umdrehung -> 1120 Ticks (70 * 16)
#define  Stop 65535
#define  Fast_Decay 0
#define  MeanSpeed 30000
#define	 GEAR 42.7										//Durchmesser Antriebsrad mm
#define  PI 3.141592654
#define  CIRCUMFERENCE_GEAR 134
#define  TICK_DISTANCE 0.12

const int ONE_REV = ONE_Revolution;
const int MAX_SPEED = 0;
const int MEAN_SPEED = MeanSpeed;
const int MIN_SPEED = 0xffff;
const int STOP = Stop;
const int FAST_DECAY = Fast_Decay;

char commandByte[7];
int i = 0;

//Variables for use in driveDistance and rollOut Method
int driveCounter = 0;
LDD_TDeviceData* MyGPIOPtr;
int x = 30;
int v = 100;
int internTicks;
int internSpeed;
direction_t direction;

int endSwitch = 0;

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

void driveDistance(float x, float v) {
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
	PWM1_SetRatio16(Stop);
// Fast Stop initialisierung
//	setDirection(REVERSE);
//	PWM1_SetRatio16(0xffff * 0.3);
//	setDirection(FAST_STOP);
}

void driveToEnd(){
int speedWasSet = 0;
	while(!endSwitch){
		if (!speedWasSet) {
			PWM1_SetRatio16(internSpeed);
			speedWasSet = 1;
		}
		vTaskDelay(pdMS_TO_TICKS(20));
	}
	PWM1_SetRatio16(STOP);
}

void driveJog(){
//Interpretierung und Aufbereitung der Parameter für DriveJog
int speedFromIm = commandByte[3];
int speed = calculateSpeed(speedFromIm);
direction = commandByte[4];
setDirection(direction);
internSpeed = speed;
driveCounter = 0;
}

void drive(void* pvParameter){
	MyGPIOPtr = GPIO1_Init(NULL);
	PWM1_SetRatio16(STOP);
	PWM1_Enable();
	for(;;){
		if(!queue_isEmpty(driveQueue)){							//Sind Inhalte in der commandQueue werden diese gelesen
			commandByte[0] = queue_read(driveQueue);
			commandByte[1] = queue_read(driveQueue);				//Anhand dem zweiten Byte in der commandQueue kann die Anzahl folgender Bytes bestimmt werden
			for(i; i < commandByte[1]; i++){						//Alle restlichen zum Kommande gehörenden Bytes werden in das commandByte Array abgelegt
				commandByte[i+2] = queue_read(driveQueue);
			}
		// Hier habe ich versucht, dass es nach einem Abarbeiten des DriveDistances auch aufhört und nicht anhand des Array wieder auslöst   }
		//Interpretierung und Aufbereitung der Parameter für DriveDistance und DriveToEnd
			if(commandByte[2] != 3){
				int distance = commandByte[3]+commandByte[4];
				int speedFromIm = commandByte[5];
				int speed = calculateSpeed(speedFromIm);
				int ticksToDrive = 	calculateTicksToDrive(distance);
				direction = commandByte[6];
				setDirection(direction);
				internTicks = ticksToDrive;
				internSpeed = speed;
				driveCounter = 0;
			}
			if(commandByte[2] == 2){
				driveDistance(x,v);
				vTaskDelay(pdMS_TO_TICKS(20));
			}
			else if(commandByte[2] == 4){
				driveToEnd();
				vTaskDelay(pdMS_TO_TICKS(20));
			}
			else{
				driveJog();
				vTaskDelay(pdMS_TO_TICKS(20));
			}
		}
	}
}
