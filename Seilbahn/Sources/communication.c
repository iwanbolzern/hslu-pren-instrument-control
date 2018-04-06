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

bool handleEndQueue() {
	if(!queue_isEmpty(endQueue)) {
		char endCmd = queue_read(endQueue);
		AS1_SendChar(0x01);
		AS1_SendChar(endCmd);
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
	AS1_SendChar(0x03);
	AS1_SendChar(0x0b);
	AS1_SendChar(x);
	AS1_SendChar(z);
}

void communication(void* pvParameter) {
	AS1_ClearRxBuf();

	for (;;) {
		// handle input stream
		while (AS1_GetCharsInRxBuf() > 0) {
			char msg;
			AS1_RecvChar(&msg);
			queue_write(commandQueue, msg);
		}

		bool moreWork = TRUE;
		while(moreWork = handlePostionQueue()) { };

		moreWork |= handleEndQueue();

		if(!moreWork) {
			vTaskDelay(pdMS_TO_TICKS(10));
		}
	}
}

