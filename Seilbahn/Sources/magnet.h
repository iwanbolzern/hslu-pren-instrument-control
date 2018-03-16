#ifndef SOURCES_MAGNET_H_
#define SOURCES_MAGNET_H_

#include "custom_queue.h"

typedef enum magnet_cmd {
	DISABLE = 0,
	ENFORCE = 1,
	RELEASE = 2
} magnetCmd_t;

extern QueueHandle_t magnetCmdQueue;

void magnetHandler(void*);

#endif /* SOURCES_MAGNET_H_ */
