/*
 * Application.c
 *
 *  Created on: 26.12.2017
 *      Author: El_Matador
 */

#include "Application.h"
#include "RxBuf.h"
#include "utils/queue.h"
#include "AS1.h"

#include <stdint.h>

#include <stdlib.h>

UART_Desc deviceData;
int param1;
int param2;
int param3;

int counter;

/* User Variables from Nico*/
#define  GEAR_RATIO 70										// Getriebe�bersetzung
#define  N_TICKS_ENCODER 16									// (Ticks / Revolution)
#define  ONE_Revolution  (GEAR_RATIO * N_TICKS_ENCODER)		// eine Umdrehung -> 1120 Ticks (70 * 16)

#define  Stop 65535
#define  Fast_Decay 0
#define  MeanSpeed 30000
#define	 GEAR 42.7										//Durchmesser Antriebsrad mm
#define  PI 3.141592654
#define  CIRCUMFERENCE_GEAR (GEAR*PI)
#define  TICK_DISTANCE (CIRCUMFERENCE_GEAR / ONE_Revolution)

#define SIZE 1000

LDD_TDeviceData* MyGPIOPtr;

int counter_regulate = 0;
int startSpeed;
float v = 1.0;
float x = 0.3;
int nTicks;
int firstRunFlag = 1;

const int ONE_REV = ONE_Revolution;
const int MAX_SPEED = 0;
const int MEAN_SPEED = MeanSpeed;
const int MIN_SPEED = 0xffff;
const int STOP = Stop;
const int FAST_DECAY = Fast_Decay;

int pos_update_forward_counter;
int pos_update_reverse_counter;
int pos_update_forward_flag;
int pos_update_reverse_flag;
int pos_ready_flag;

enum direction {
	FORWARD = 0, REVERSE = 1, FAST_STOP = 2
};

enum speedMode {
	IDLE = 0, SLOW = 1, MEDIUM = 2, FAST = 3
};

float quarterPipeArray[] = { 0.3, 0.2, 0.1, 0.05, 0.05 };
int arrayIndex = 0;

typedef enum speedMode speedMode_t;
typedef enum direction direction_t;
direction_t dir;
speedMode_t spMod;

// QUEUE
QueueHandle_t positionUpdateQueue;
void del();

// Method Declarations
void setDirection(direction_t);
int calculateTicksToDrive(int);
void rollout_slow(int, int, direction_t);
void rollout_medium(int, int, direction_t);
void rollout_fast(int, int, direction_t);

void drive(direction_t, int, int);

static void SendChar(unsigned char ch, UART_Desc *desc) {
	desc->isSent = FALSE; /* this will be set to 1 once the block has been sent */
	while (AS1_SendBlock(desc->handle, (LDD_TData*) &ch, 1) != ERR_OK) {
	} /* Send char */
	while (!desc->isSent) {
	} /* wait until we get the green flag from the TX interrupt */
}

static void SendString(const unsigned char *str, UART_Desc *desc) {
	while (*str != '\0') {
		SendChar(*str++, desc);
	}
}

static void Init(void) {
	positionUpdateQueue = xQueueCreate(100, sizeof(char));

	/* initialize struct fields */
	deviceData.handle = AS1_Init(&deviceData);
	deviceData.isSent = FALSE;
	deviceData.rxChar = '\0';
	deviceData.rxPutFct = RxBuf_Put;
	/* set up to receive RX into input buffer */
	RxBuf_Init(); /* initialize RX buffer */
	/* Set up ReceiveBlock() with a single byte buffer. We will be called in OnBlockReceived() event. */
	while (AS1_ReceiveBlock(deviceData.handle, (LDD_TData *) &deviceData.rxChar,
			sizeof(deviceData.rxChar)) != ERR_OK) {
	} /* initial kick off for receiving data */
}

/**
 * sendet die Positionsupdates an das IM
 */
void posUpdate(void* pvParameters) {
	for (;;) {
		char ch;
		while(!xQueueReceive(positionUpdateQueue, &ch, (TickType_t)10)) { };

		SendChar(ch, &deviceData);
		free(ch);
	}
}

int calculateTicksToDrive(int distance) {

	int l = 0;

	l = (distance / (0.121));

	return l;
//return (uint8)(distance/TICK_DISTANCE);
}

/*
 ** ===================================================================

 **     Description :
 **
 **        Liest die L�ngenangabe der Message
 **
 **
 **     Parameters  : Nothing
 **
 **
 **     Returns     : L�nge der Message als int

 ** ===================================================================
 */
int getLengthOfMessage(void) {
	int temp = 0;
	int value16e1 = 0;
	int value16e0 = 0;
	char ch = 0;

	(void) RxBuf_Get(&ch);
	temp = ch;
	value16e1 = 16 * temp;
	(void) RxBuf_Get(&ch);
	temp = ch;
	value16e0 = temp;

	return value16e1 + value16e0;

}

/*
 ** ===================================================================

 **     Description :
 **
 **        Liest das Kommando ID
 **
 **
 **     Parameters  : Nothing
 **
 **
 **     Returns     : Kommando-ID

 ** ===================================================================
 */

char getCmd(void) {

	char ch = 0;

	(void) RxBuf_Get(&ch);

	return ch;

}

/*
 ** ===================================================================

 **     Description :
 **
 **     Liest das Kommando ID
 **
 **
 **     Parameters  : Nothing
 **
 **
 **     Returns     : 2 Bytes aus dem RxBuffer

 ** ===================================================================
 */

int get2Bytes(void) {

	unsigned char ch1 = 0;
	unsigned char ch2 = 0;
	int temp1 = 0;
	int temp2 = 0;
	int temp3 = 0;
	int value16e2 = 256;
	int result = 0;

	(void) RxBuf_Get(&ch1);

	temp1 = ch1;

	(void) RxBuf_Get(&ch2);

	temp3 = temp1 * value16e2;

	temp2 = ch2;
	result = temp3 + temp2;

	return result;

}

void rollOut(float x, float v) {
	int speedWasSet = 0;

	while (counter < nTicks) {

		while (counter < (nTicks * x)) {

			if (!speedWasSet) {
				LED1_Neg();
				PWM1_SetRatio16(startSpeed * v);
				speedWasSet = 1;
			}

		}

		x = x + 0.1;
		v = v + 0.15;

		if ((startSpeed * v) < (MIN_SPEED * 0.9)) {
			rollOut(x, v);
		}

	}

}

void drive(direction_t direction, int ticks, int speed) {
	PWM1_Enable();
	setDirection(direction);
	vTaskDelay(pdMS_TO_TICKS(10));
	int internTicks = ticks;
	counter = 0;
	int newSpeed = 0;
	int internSpeed = (1 - ((float) speed / 100)) * MIN_SPEED;

	newSpeed = internSpeed;

	int ticksToGo = ticks;

//Try Nico
	startSpeed = internSpeed;
	nTicks = ticks;

	LED1_On();				// Debug

	rollOut(x, v);

// Fast STop initialisierung
	setDirection(REVERSE);
	PWM1_SetRatio16(0xffff * 0.3);
	vTaskDelay(pdMS_TO_TICKS(30));
	setDirection(FAST_STOP);

//

}

void rollout_slow(int speed, int ticks, direction_t direction) {

	int initialSpeed = speed;
	int ticksToGo = ticks;
	int flag = 0;
	float speedFactor = 1.0;
	float ticksToGoFactor = 0.3;
	int fullSpeedFlag = 0;
	int noMoreSpeedIncreaseFlag = 0;

	while (counter < ticksToGo) {

		while (counter < (ticksToGo * ticksToGoFactor)) {

			if (flag == 0) {
				PWM1_SetRatio16(initialSpeed * speedFactor);
				flag = 1;
			}

		}

		flag = 0;

		if ((speedFactor * initialSpeed) > MIN_SPEED * 0.8) {

			PWM1_SetRatio16(MIN_SPEED * 0.9);
			noMoreSpeedIncreaseFlag = 1;

			speedFactor = 0;

		} else {

			if (noMoreSpeedIncreaseFlag == 0) {
				speedFactor = speedFactor + 0.15;
				ticksToGoFactor = ticksToGoFactor + 0.1;
			}

		}

		vTaskDelay(pdMS_TO_TICKS(10));
	}

	setDirection(FAST_STOP);

	speedFactor = speedFactor * 100;
	ticksToGoFactor = ticksToGoFactor * 100;
	vTaskDelay(pdMS_TO_TICKS(3000));
	int k = 878;

}

void rollout_medium(int speed, int ticks, direction_t direction) {

	int initialSpeed = speed;
	int ticksToGo = ticks;
	int flag = 0;
	float speedFactor = 1.0;
	float ticksToGoFactor = 0.3;
	int fullSpeedFlag = 0;
	int noMoreSpeedIncreaseFlag = 0;

	while (counter < ticksToGo) {

		while (counter < (ticksToGo * ticksToGoFactor)) {

			if (flag == 0) {
				PWM1_SetRatio16(initialSpeed * speedFactor);
				flag = 1;
			}

		}

		flag = 0;

		if ((speedFactor * initialSpeed) > MIN_SPEED * 0.8) {

			PWM1_SetRatio16(MIN_SPEED * 0.9);
			noMoreSpeedIncreaseFlag = 1;

			speedFactor = 0;

		} else {

			if (noMoreSpeedIncreaseFlag == 0) {
				speedFactor = speedFactor + 0.15;
				ticksToGoFactor = ticksToGoFactor + 0.1;
			}

		}

		vTaskDelay(pdMS_TO_TICKS(1));
	}

	setDirection(FAST_STOP);

	speedFactor = speedFactor * 100;
	ticksToGoFactor = ticksToGoFactor * 100;
	vTaskDelay(pdMS_TO_TICKS(3000));
	int k = 878;

}

void rollout_fast(int speed, int ticks, direction_t direction) {

	int initialSpeed = speed;
	int ticksToGo = ticks;
	int flag = 0;
	float speedFactor = 1.0;
	float ticksToGoFactor = 0.3;
	int fullSpeedFlag = 0;
	int noMoreSpeedIncreaseFlag = 0;

	while (counter < ticksToGo) {

		while (counter < (ticksToGo * ticksToGoFactor)) {

			if (flag == 0) {
				PWM1_SetRatio16(initialSpeed * speedFactor);
				flag = 1;
			}

		}

		flag = 0;

		if ((speedFactor * initialSpeed) > MIN_SPEED * 0.8) {

			PWM1_SetRatio16(MIN_SPEED * 0.9);
			noMoreSpeedIncreaseFlag = 1;

			speedFactor = 0;

		} else {

			if (noMoreSpeedIncreaseFlag == 0) {
				speedFactor = speedFactor + 0.15;
				ticksToGoFactor = ticksToGoFactor + 0.1;
			}

		}

		vTaskDelay(pdMS_TO_TICKS(1));
	}

	setDirection(FAST_STOP);

	speedFactor = speedFactor * 100;
	ticksToGoFactor = ticksToGoFactor * 100;
	vTaskDelay(pdMS_TO_TICKS(3000));
	int k = 878;

}

void setDirection(direction_t d) {
	if (d == FORWARD) {
		IN1_PutVal(FALSE);
		IN2_PutVal(TRUE);
		dir = FORWARD;
	} else if (d == REVERSE) {
		IN1_PutVal(TRUE);
		IN2_PutVal(FALSE);
		dir = REVERSE;
	} else if (d == FAST_STOP) {
		IN1_PutVal(TRUE);
		IN2_PutVal(TRUE);
		dir = FAST_STOP;
	}

}

/*
 ** ===================================================================

 **     Description :
 **
 **     Liest das Kommando ID
 **
 **
 **     Parameters  : Nothing
 **
 **
 **     Returns     : 1 Bytes aus dem RxBuffer

 ** ===================================================================
 */

char get1Byte(void) {

	unsigned char ch = 0;

	(void) RxBuf_Get(&ch);

	return ch;
}

void initTele(void *pvParameters) {

	(void) pvParameters;
}

void driveDistance(void *pvParameters) {

	(void) pvParameters;

	MyGPIOPtr = GPIO1_Init(NULL);
	int distance = param1;
	int speed = param2;
	direction_t (direction) = param3;
	int ticks;

	ticks = calculateTicksToDrive(distance);
	drive(direction, ticks, speed);

	vTaskDelete(NULL);

}

void driveJog(void *pvParameters) {

	(void) pvParameters;

	int speed = param1;
	int direction = param2;

}

void driveToEnd(void *pvParameters) {

	(void) pvParameters;
	int distance = param1;
	int speed = param2;
	int direction = param3;

	for (;;) {

		LED2_Neg();
		vTaskDelay(pdMS_TO_TICKS(500));

	}

}

void moveTele(void *pvParameters) {

	(void) pvParameters;
	int distance = param1;
	int direction = param2;

}

void enableMagnet(void *pvParameters) {

	(void) pvParameters;
	int direction = param1;

}

void disableMagnet(void *pvParameters) {

	(void) pvParameters;
}

void APP_Run(void) {

	Init();

}

void sendText(char text[]) {

	SendString((unsigned char*) text, &deviceData);

}

void emptyBuffer(void) {

	unsigned char ch;
	while (RxBuf_NofElements() != 0) {
		(void) RxBuf_Get(&ch);
		vTaskDelay(pdMS_TO_TICKS(3));

	}

}

int getValue(mode_t modus) {

	unsigned char ch;
	unsigned char t;

	unsigned char a;
	unsigned char b;
	unsigned char c;
	unsigned char d;

	unsigned char e;

	int value10e3 = 0;
	int value10e2 = 0;
	int value10e1 = 0;
	int value10e0 = 0;
	int letter = 0;

	if (modus == DISTANCE) {

		(void) RxBuf_Get(&ch);
		a = ch;
		vTaskDelay(pdMS_TO_TICKS(2));
		value10e3 = getDecimal(ch) * 1000;

		vTaskDelay(pdMS_TO_TICKS(2));
		(void) RxBuf_Get(&ch);
		b = ch;
		value10e2 = getDecimal(ch) * 100;

		vTaskDelay(pdMS_TO_TICKS(2));
		(void) RxBuf_Get(&ch);
		c = ch;
		value10e1 = getDecimal(ch) * 10;

		vTaskDelay(pdMS_TO_TICKS(2));
		(void) RxBuf_Get(&ch);
		d = ch;
		value10e0 = getDecimal(ch);

		(void) RxBuf_Get(&ch);

		if ((a == 'x') || (b == 'x') || (c == 'x') || (d == 'x')) {
			return -1;
		} else {
			return value10e3 + value10e2 + value10e1 + value10e0;
		}
	}

	else {
		(void) RxBuf_Get(&ch);
		t = ch;
		if (t == 'x') {
			return -1;
		} else {
			(void) RxBuf_Get(&ch);
			return t;

		}
	}

// zum /0 aus dem Buffer nehmen

}

void sendHeader(void) {

	sendText(
			"#####################################################\n"
					"#"
					"                                                                         "
					"                                              #\n#              TELESKOP TESTER"
					"                                                              #\n#"
					"                                                                         "
					"                                              #\n#####################################################");

}

void sendMenu(void) {

	sendText("\n\nsjdasjdlkjdlkasjdklda\n\n");

}

