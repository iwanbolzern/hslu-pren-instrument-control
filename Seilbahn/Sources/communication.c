/*
 * communication.c
 *
 *  Created on: 11.03.2018
 *      Author: El_Matador
 */

#include "AS1.h"
#include "RxBuf.h"
#include "communication.h"
#include "custom_queue.h"
#include "queue.h"


typedef enum {
	IDLE, GET_POS, READ_FROM_IM, SEND_POS_TO_IM, ERROR, REPORT_TO_IM
} state_t;
state_t state = IDLE;

UART_Desc deviceData;

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

void sendPositionUpdate(char x, char z) {

	SendChar(0x00, &deviceData);
	SendChar(0x05, &deviceData);
	SendChar(0x0b, &deviceData);
	SendChar(0x00, &deviceData);
	SendChar(x, &deviceData);
	SendChar(0x00, &deviceData);
	SendChar(z, &deviceData);
}

void sendReport(char cmd) {

	SendChar(0x00, &deviceData);
	SendChar(0x01, &deviceData);
	SendChar(0x0a, &deviceData);

}

void communication(void* pvParameter) {

	(void) pvParameter;

	Init();		// Initialisierung von der asynchronverbindung

	char xPos = '0';
	char zPos = '0';
	char message = NULL;

	for (;;) {

		switch (state) {

		case IDLE:

			if (!(queue_isEmpty(xPosQueue) && (queue_isEmpty(zPosQueue)))) { // es wurde min. in eine der beiden Queues geschrieben

				state = SEND_POS_TO_IM;
			}

			else if (RxBuf_NofElements() != 0) {
				state = READ_FROM_IM;
			}

			else if (!(queue_isEmpty(endMoveTeleQueue))) {

				state = REPORT_TO_IM;
			}

			else {
				vTaskDelay(pdMS_TO_TICKS(20));				// 50 Hz
			}
			break;

		case SEND_POS_TO_IM:

			while (!(queue_isEmpty(xPosQueue) && (queue_isEmpty(zPosQueue)))) {

				if (queue_isEmpty(xPosQueue)) {	// xQueue ist leer, dh der Eintrag ist in der zQueue
					xPos = '0';
					zPos = queue_read(zPosQueue);

				} else if (queue_isEmpty(zPosQueue)) {// zQueue ist leer, dh der Eintrag ist in der xQueue
					zPos = '0';
					xPos = queue_read(xPosQueue);

				} else {				// beide Queues haben einen Eintrag
					zPos = queue_read(zPosQueue);
					xPos = queue_read(xPosQueue);

				}

				sendPositionUpdate(xPos, zPos);
			}
			state = IDLE;
			break;

		case REPORT_TO_IM:

			if (!(queue_isEmpty(endMoveTeleQueue))) {

				char end =  queue_read(endMoveTeleQueue);
				sendReport(0x0a);

			}

			state = IDLE;
			break;

		case ERROR:
			// Error Messages: SendString("blablabla went Wrong")

			state = IDLE;
			break;

		case READ_FROM_IM:

			while (RxBuf_NofElements() != 0) {
				(void) RxBuf_Get(&message);
				queue_write(commandQueue, message);

			}
			state = IDLE;
			break;

		}

	}
}

