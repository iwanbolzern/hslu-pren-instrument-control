#include "communication.h"
#include "AS1.h"
#include "RxBuf.h"
#include "custom_queue.h"


void sendPositionUpdate(char, char);

// external vars
QueueHandle_t commandQueue;
QueueHandle_t xPosQueue;
QueueHandle_t zPosQueue;
QueueHandle_t endQueue;

// internal vars
static UART_Desc deviceData;

static void SendChar(unsigned char ch, UART_Desc *desc) {
	desc->isSent = FALSE; /* this will be set to 1 once the block has been sent */
	while (AS1_SendBlock(desc->handle, (LDD_TData*) &ch, 1) != ERR_OK) {
	} /* Send char */
	while (!desc->isSent) {
	} /* wait until we get the green flag from the TX interrupt */
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

bool handleEndQueue() {
	if(!queue_isEmpty(endQueue)) {
		char endCmd = queue_read(endQueue);
		SendChar(0x01, &deviceData);
		SendChar(endCmd, &deviceData);
	}
	return !queue_isEmpty(endQueue);
}

bool handlePostionQueue(void) {
	if(!queue_isEmpty(xPosQueue) || !queue_isEmpty(zPosQueue)) {
		char x, z = 0;
		if(!queue_isEmpty(xPosQueue))
			x = queue_read(xPosQueue);
		if(!queue_isEmpty(zPosQueue))
			z = queue_read(zPosQueue);
		sendPositionUpdate(x, z);
	}
	return !queue_isEmpty(xPosQueue) || !queue_isEmpty(zPosQueue);
}

void sendPositionUpdate(char x, char z) {
	SendChar(0x03, &deviceData);
	SendChar(0x0b, &deviceData);
	SendChar(x, &deviceData);
	SendChar(z, &deviceData);
}

void communication(void* pvParameter) {
	Init();		// Initialisierung von der asynchronverbindung

	for (;;) {
		// handle input stream
		while (RxBuf_NofElements() != 0) {
			char msg = NULL;
			(void) RxBuf_Get(&msg);
			queue_write(commandQueue, msg);
		}

		bool moreWork = TRUE;
		while(moreWork = handlePostionQueue()) { };

		moreWork |= handleEndQueue();

		if(!moreWork)
			vTaskDelay(pdMS_TO_TICKS(10));
	}
}

