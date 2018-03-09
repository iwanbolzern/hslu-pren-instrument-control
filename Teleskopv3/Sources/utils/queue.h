#ifndef HL_QUEUE_H
#define HL_QUEUE_H

#include "FRTOS1.h"

typedef struct _queueEntry {
	struct _queueEntry* previous;
	void* value;
} queueEntry_t;


typedef struct _queue {
	queueEntry_t* left;
	queueEntry_t* right;

	int size; // just for info
	SemaphoreHandle_t pushMutex;
	SemaphoreHandle_t popMutex;
} queue_t;

/**
 * @brief Create a new queue
 * @return a newly allocated and initialized queue
 */
queue_t *queue_create();

/**
 * @brief Append a new value to the queue (left)
 * @param q : A valid pointer to a queue_t structure
 * @param val : The value to store in the tail of the queue
 */
void queue_push(queue_t *q, void *val);

/**
 * @brief Remove fist inserted value from the queue
 * @param q : A valid pointer to a queue_t structure
 * @return The value previously stored in the tail of the queue
 */
void* queue_try_pop(queue_t *q);

/**
 * @brief Return the total count of items in the queue
 * @param q : A valid pointer to a queue_t structure
 * @return the actual number of items stored in the queue
 */
int queue_count(queue_t *q);

#endif
