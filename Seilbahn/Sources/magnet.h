#ifndef SOURCES_MAGNET_H_
#define SOURCES_MAGNET_H_

#include "FRTOS1.h"

typedef enum magnetCmd {
	magnetCmd_DISABLE = 0,
	magnetCmd_ENFORCE = 1,
	magnetCmd_RELEASE = 2
} magnetCmd_t;

extern QueueHandle_t magnetCmdQueue;

void magnetHandler(void*);

#endif /* SOURCES_MAGNET_H_ */
