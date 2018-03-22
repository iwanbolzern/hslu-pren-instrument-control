#include "magnet.h"
#include "Magnet1.h"
#include "Magnet2.h"
#include "custom_queue.h"

void disableMagnet(void);
void enforceMagnet(void);
void releaseMagnet(void);

// external vars
QueueHandle_t magnetCmdQueue;

void magnetHandler(void* pvParameter) {
	for(;;) {
		if(!queue_isEmpty(magnetCmdQueue)) {
			char cmd = queue_read(magnetCmdQueue);
			switch(cmd) {
				case magnetCmd_DISABLE:
					disableMagnet();
					break;
				case magnetCmd_ENFORCE:
					enforceMagnet();
					break;
				case magnetCmd_RELEASE:
					releaseMagnet();
					break;
				default:
					// no idea how to make exception handling in c
					break;
			}
		} else {
			vTaskDelay(pdMS_TO_TICKS(20));
		}
	}
}

void disableMagnet(void) {
	Magnet1_PutVal(0);
	Magnet2_PutVal(0);
}

void enforceMagnet(void) {
	Magnet1_PutVal(0);
	Magnet2_PutVal(1);
}

void releaseMagnet(void) {
	Magnet1_PutVal(1);
	Magnet2_PutVal(0);
}
