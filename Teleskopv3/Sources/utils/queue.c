#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "queue.h"

queue_t* queue_create(void) {
	queue_t* queue = malloc(sizeof(queue_t));
	queue->pushMutex = xSemaphoreCreateMutex();
	queue->popMutex = xSemaphoreCreateMutex();
	queue->left = NULL;
	queue->right = NULL;
}

void push(queue_t* queue, void* value) {
	queueEntry_t* entry = malloc(sizeof(queueEntry_t));
	entry->previous = NULL;
	entry->value = value;

	while(xSemaphoreTake(queue->pushMutex, ( TickType_t ) 10 ) != pdTRUE) { };

	if(queue->left != NULL) {
		queue->left->previous = entry;
		queue->left = entry;
	} else {
		queue->left = entry;
		queue->right = entry;
	}

	queue->size++;

	xSemaphoreGive(queue->pushMutex);
}

void* try_pop(queue_t* queue) {
	void* value = NULL;

	while(xSemaphoreTake(queue->popMutex, ( TickType_t ) 10 ) != pdTRUE) { };

	if(queue->size > 0) {
		queueEntry_t* entry = queue->right;
		value = entry->value;

		if(entry->previous != NULL) {
			queue->right = entry->previous;
		} else {
			queue->right = NULL;
			queue->left = NULL;
		}
		queue->size--;

		//cleanup
		free(entry);
	}
	xSemaphoreGive(queue->popMutex);

	return value;
}

int queue_count(queue_t* queue) {
	return queue->size;
}
