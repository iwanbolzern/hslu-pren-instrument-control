#include <stdlib.h>

#include "custom_queue.h"


QueueHandle_t queue_create(int maxElements) {
	QueueHandle_t newQueue = xQueueCreate(maxElements, sizeof(char));
	return newQueue;
}

void queue_write(QueueHandle_t queue, char value) {
	if(xQueueSend(queue, &value, (TickType_t) 0) != pdTRUE)
		exit(-1); // no more space in the queue
}

void queue_writeFromISR(QueueHandle_t queue, char value) {
	if(xQueueSendFromISR(queue, &value, NULL) != pdTRUE)
		exit(-1); // no more space in the queue
}

// returns NULL if no value is in the queue
// example usage to wait until something is available char test; while((test = queue_read(queue)) == -1) { }
char queue_read(QueueHandle_t queue) {
	char value;
	if(xQueueReceive(queue, &value, (TickType_t) 0) == pdTRUE)
		return value;
	return -1;
}

// returns NULL if no value is in the queue
// example usage to wait until something is available char test; while((test = queue_readFromISR(queue)) == -1) { }
char queue_readFromISR(QueueHandle_t queue) {
	char value;
	if(xQueueReceiveFromISR(queue, &value, NULL) == pdTRUE)
		return value;
	return -1;
}

UBaseType_t queue_size(QueueHandle_t queue) {
	return uxQueueMessagesWaiting(queue);
}

UBaseType_t queue_sizeFromISR(QueueHandle_t queue) {
	return uxQueueMessagesWaitingFromISR(queue);
}

bool queue_isEmpty(QueueHandle_t queue) {
	return queue_size(queue) <= 0;
}

bool queue_isEmptyFromISR(QueueHandle_t queue) {
	return queue_sizeFromISR(queue) <= 0;
}
