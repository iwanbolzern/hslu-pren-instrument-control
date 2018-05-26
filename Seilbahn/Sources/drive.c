#include "drive.h"
#include "PWM1.h"
#include "PWM2.h"
#include "stdlib.h"

#include "GPIO1.h"
#include "end_Switch.h"
#include "timers.h"

#include "custom_queue.h"
#include "communication.h"

int calculateSpeed(char);
void setDirection(char);
char* getAppCmdStream(char);
void driveDistance(void);
void driveToEnd(void);
void driveJog(void);
void prepareForBoundedDrive(char*);
void prepareForUnboundedDrive(char*);
void decelerate(void);
void decelerateEndSwitch(void);
void accelerate(void);

// external vars
QueueHandle_t driveQueue;
xTaskHandle driveTask;

// internal vars
static LDD_TDeviceData* myGPIOPtr;
static LDD_TDeviceData* myEndSwitchPtr;

static driveCmd_t currentCmd;

static direction_t direction;
static int xEndSwitch_pressed;
static unsigned int driveCounter;
static unsigned int internTicks;
static int internSpeed;
static int currentSpeed;

const unsigned int MAX_SPEED = 0xffff;
const unsigned int STOP = 0;

void drive(void* pvParameter){
	myGPIOPtr = GPIO1_Init(NULL);
	myEndSwitchPtr = end_Switch_Init(NULL);
	PWM1_SetRatio16(STOP);
	PWM2_SetRatio16(STOP);
	PWM1_Enable();
	PWM2_Enable();

	for(;;) {
		char appCmd = queue_readInfinity(driveQueue);
		char* appCmdStream = getAppCmdStream(appCmd);				//Anhand dem zweiten Byte in der commandQueue kann die Anzahl folgender Bytes bestimmt werden

		switch(appCmd) {
		case driveCmd_DRIVE_DISTANCE:
			currentCmd = driveCmd_DRIVE_DISTANCE;
			prepareForBoundedDrive(appCmdStream);
			driveDistance();
			queue_write(endQueue, endCmd_END_DRIVE_DISTANCE);
			break;
		case driveCmd_DRIVE_JOG:
			currentCmd = driveCmd_DRIVE_JOG;
			prepareForUnboundedDrive(appCmdStream);
			driveJog();
			break;
		case driveCmd_DRIVE_TO_END:
			currentCmd = driveCmd_DRIVE_TO_END;
			prepareForBoundedDrive(appCmdStream);
			driveToEnd();
			queue_write(endQueue, endCmd_END_RUN);
			break;
		}
		vPortFree(appCmdStream);
	}
}

int calculateSpeed(char speed){
	return MAX_SPEED / 100 * speed;
}

void setSpeed(unsigned int speed) {
	if(direction == FORWARD) {
		int error = PWM2_SetRatio16(speed & 0xFFFF);
	}
	else {
		int error = PWM1_SetRatio16(speed & 0xFFFF);
	}
}

void setDirection(char d) {
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

unsigned int ticksToStop = 600;
void accelerate(void) {
	int accFac = 1000;
	while (currentSpeed < internSpeed &&
			driveCounter < internTicks - ticksToStop &&
			!xEndSwitch_pressed) {
		currentSpeed = currentSpeed + accFac <= internSpeed ? currentSpeed + accFac : internSpeed;
		setSpeed(currentSpeed);
		vTaskDelay(pdMS_TO_TICKS(30));
	}
}

void decelerate(void) {
	int decFac = 1000;
	while (((currentSpeed > 0 && driveCounter < internTicks) || driveCounter < internTicks) && !xEndSwitch_pressed) {
		currentSpeed = currentSpeed - decFac >= 0 ? currentSpeed - decFac : 0x6666;
		setSpeed(currentSpeed);
		vTaskDelay(pdMS_TO_TICKS(20));
	}
}

void decelerateEndSwitch(void) {
	int decFac = 1000;
	while (!xEndSwitch_pressed) {
		currentSpeed = currentSpeed - decFac >= 0 ? currentSpeed - decFac : 0x6666;
		setSpeed(currentSpeed);
		vTaskDelay(pdMS_TO_TICKS(20));
	}
}

void driveDistance(void) {
	accelerate();

	while(driveCounter < internTicks - ticksToStop && !xEndSwitch_pressed) {
		ulTaskNotifyTake(pdTRUE, /* Clear the notification value before exiting. */
	                 portMAX_DELAY ); /* Block indefinitely. */
	}

	decelerate();

	currentSpeed = 0;
	PWM1_SetRatio16(STOP);
	PWM2_SetRatio16(STOP);
}

void driveToEnd(void) {
	accelerate();

	while(driveCounter < internTicks - ticksToStop && !xEndSwitch_pressed) {
		ulTaskNotifyTake(pdTRUE, /* Clear the notification value before exiting. */
							 portMAX_DELAY ); /* Block indefinitely. */
	}

	decelerateEndSwitch();

	currentSpeed = 0;
	PWM1_SetRatio16(STOP);
	PWM2_SetRatio16(STOP);
}

void driveJog() {
	int accFac = 2000;
	while (currentSpeed < internSpeed && !xEndSwitch_pressed) {
		currentSpeed = currentSpeed + accFac <= internSpeed ? currentSpeed + accFac : internSpeed;
		setSpeed(currentSpeed);
		vTaskDelay(pdMS_TO_TICKS(30));
	}

	setSpeed(currentSpeed);
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
	internTicks = ((appCmdStream[0] & 0x00FF) << 8) + (appCmdStream[1] & 0x00FF);
	internSpeed = calculateSpeed(appCmdStream[2]);
	ticksToStop = appCmdStream[2] * 4;
	setDirection(appCmdStream[3]);
	driveCounter = 0;

	xEndSwitch_pressed = FALSE;
}

void prepareForUnboundedDrive(char* appCmdStream) {
	internSpeed = calculateSpeed(appCmdStream[0]);
	setDirection(appCmdStream[1]);
	driveCounter = 0;

	xEndSwitch_pressed = FALSE;
}

void drive_tickReceived(void) {
	driveCounter += 1;
	if(direction == FORWARD) {
		if(driveCounter % 8 == 0) {
			queue_writeFromISR(xPosQueue, 8);
		}
	}
	else {
		if(driveCounter % 8 == 0) {
			queue_writeFromISR(xPosQueue, -8);
		}
	}

	if ((currentCmd == driveCmd_DRIVE_DISTANCE || currentCmd == driveCmd_DRIVE_TO_END)
			&& driveCounter >= internTicks - ticksToStop) {
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		vTaskNotifyGiveFromISR(driveTask, &xHigherPriorityTaskWoken);
	}
}

void resetEndSwitch(void) {
	if(end_Switch_GetFieldValue(NULL, endSwitch) == FALSE) {
		xEndSwitch_pressed = FALSE;
	}
}

void drive_endSwitchReceived(void) {
	xEndSwitch_pressed = TRUE;

	currentSpeed = 0;
	PWM1_SetRatio16(STOP);
	PWM2_SetRatio16(STOP);

	if (currentCmd == driveCmd_DRIVE_TO_END) {
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		vTaskNotifyGiveFromISR(driveTask, &xHigherPriorityTaskWoken);
	}
}
