#ifndef SOURCES_CUSTOM_QUEUE_H_
#define SOURCES_CUSTOM_QUEUE_H_

#include "FreeRTOS.h"
#include "queue.h"

QueueHandle_t queue_create(int maxElements);

void queue_write(QueueHandle_t queue, char value);

void queue_writeFromISR(QueueHandle_t queue, char value);

// returns NULL if no value is in the queue
// example usage to wait until something is available char test; while((test = queue_read(queue)) == NULL) { }
char queue_read(QueueHandle_t queue);

// returns NULL if no value is in the queue
// example usage to wait until something is available char test; while((test = queue_readFromISR(queue)) == NULL) { }
char queue_readFromISR(QueueHandle_t queue);

UBaseType_t queue_size(QueueHandle_t queue);
UBaseType_t queue_sizeFromISR(QueueHandle_t queue);

bool queue_isEmpty(QueueHandle_t queue);
bool queue_isEmptyFromISR(QueueHandle_t queue);

#endif /* SOURCES_CUSTOM_QUEUE_H_ */
