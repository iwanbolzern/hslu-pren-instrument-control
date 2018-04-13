#include "drive.h"
#include "PWM1.h"
#include "PWM2.h"
#include "IN1.h"
#include "IN2.h"
#include "stdlib.h"

#include "GPIO1.h"
#include "end_Switch.h"
#include "timers.h"

#include "custom_queue.h"
#include "communication.h"

int calculateTicksToDrive(int);
int calculateSpeed(int);
void setDirection(direction_t);
char* getAppCmdStream(char);
void driveDistance(void);
void driveToEnd(void);
void driveJog(void);
void prepareForBoundedDrive(char*);
void prepareForUnboundedDrive(char*);
void decelerate(void);
void accelerate(void);

// external vars
QueueHandle_t driveQueue;
xTaskHandle driveTask;

// internal vars
static LDD_TDeviceData* myGPIOPtr;
static LDD_TDeviceData* myEndSwitchPtr;

static direction_t direction;
static int xEndSwitch_pressed;
static unsigned int driveCounter;
static unsigned int internTicks;
static int internSpeed;
const int MAX_SPEED = 0xffff;
const unsigned int STOP = 0;

void drive(void* pvParameter){
	myGPIOPtr = GPIO1_Init(NULL);
	myEndSwitchPtr = end_Switch_Init(NULL);
	setDirection(FORWARD);
	PWM1_SetRatio16(STOP);
	PWM2_SetRatio16(STOP);
	PWM1_Enable();
	PWM2_Enable();

	for(;;) {
		char appCmd = queue_readInfinity(driveQueue);
		char* appCmdStream = getAppCmdStream(appCmd);				//Anhand dem zweiten Byte in der commandQueue kann die Anzahl folgender Bytes bestimmt werden

		switch(appCmd) {
		case driveCmd_DRIVE_DISTANCE:
			prepareForBoundedDrive(appCmdStream);
			driveDistance();
			queue_write(endQueue, endCmd_END_DRIVE_DISTANCE);
			break;
		case driveCmd_DRIVE_JOG:
			prepareForUnboundedDrive(appCmdStream);
			driveJog();
			break;
		case driveCmd_DRIVE_TO_END:
			prepareForBoundedDrive(appCmdStream);
			driveToEnd();
			queue_write(endQueue, endCmd_END_RUN);
			break;
		}
		vPortFree(appCmdStream);
	}
}

int calculateTicksToDrive(int distance) {
	int l = (distance / (0.12));
	return l;
}

int calculateSpeed(int speed){
	return MAX_SPEED / 100 * speed;
}

void setDirection(direction_t d) {
	if (d == FORWARD) {
		PWM1_SetRatio16(STOP);
		direction = FORWARD;
	} else if (d == REVERSE) {
		PWM2_SetRatio16(STOP);
		direction = REVERSE;
	} else if (d == FAST_STOP) {
		PWM1_SetRatio16(STOP);
		PWM2_SetRatio16(STOP);
		direction = FAST_STOP;
	}
}

int ticksToStop = 400;
void accelerate(void) {
	int accFac = 1000;
	int speed = 0;
	while (speed < internSpeed && driveCounter < internTicks - ticksToStop) {
		speed = speed + accFac <= internSpeed ? speed + accFac : internSpeed;
		if(direction == FORWARD) {
			int error = PWM2_SetRatio16(speed & 0xFFFF);
		}
		else {
			int error = PWM1_SetRatio16(speed & 0xFFFF);
		}
		vTaskDelay(pdMS_TO_TICKS(30));
	}
	internSpeed = speed;
}

void decelerate(void) {
	int decFac = 1000;
	int speed = internSpeed;
	while ((speed > 0 && driveCounter < internTicks) || driveCounter < internTicks) {
		speed = speed - decFac >= 0 ? speed - decFac : 0x2fff;
		if(direction == FORWARD) {
			int error = PWM2_SetRatio16(speed & 0xFFFF);
		}
		else {
			int error = PWM1_SetRatio16(speed & 0xFFFF);
		}
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}

void driveDistance(void) {
	accelerate();

	ulTaskNotifyTake(pdTRUE, /* Clear the notification value before exiting. */
	                 portMAX_DELAY ); /* Block indefinitely. */

	decelerate();

	PWM1_SetRatio16(STOP);
	PWM2_SetRatio16(STOP);
}

void driveToEnd(void){
	int speedWasSet = 0;
	while(!xEndSwitch_pressed) {
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
	int size = 4;
	if(appCmd == driveCmd_DRIVE_JOG) {
		size = 2;
	}

	char* cmdStream = pvPortMalloc(sizeof(char) * size);
	for(int i = 0; i < size; i++) {
		cmdStream[i] = queue_readInfinity(driveQueue);
	}

	return cmdStream;
}

void prepareForBoundedDrive(char* appCmdStream) {
	unsigned int distance = ((appCmdStream[0] & 0x00FF) << 8) + (appCmdStream[1] & 0x00FF);

	internSpeed = calculateSpeed(appCmdStream[2]);
	internTicks = calculateTicksToDrive(distance);
	direction = appCmdStream[3];
	setDirection(direction);
	driveCounter = 0;
}

void prepareForUnboundedDrive(char* appCmdStream) {
	internSpeed = calculateSpeed(appCmdStream[0]);
	direction = appCmdStream[1];
	setDirection(direction);
	driveCounter = 0;
}

void drive_tickReceived(void) {
	driveCounter += 1;
	if(direction == FORWARD) {
		if(driveCounter % 25 == 0) {
			queue_writeFromISR(xPosQueue, 3);
		}
	}
	else {
		if(driveCounter % 25 == 0) {
			queue_writeFromISR(xPosQueue, -3);
		}
	}

	if (driveCounter >= internTicks - ticksToStop) {
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		vTaskNotifyGiveFromISR(driveTask, &xHigherPriorityTaskWoken);
	}
}

void drive_endSwitchReceived(void) {
	xEndSwitch_pressed = TRUE;
}
