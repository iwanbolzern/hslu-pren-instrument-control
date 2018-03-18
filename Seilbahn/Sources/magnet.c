#include "magnet.h"

void disableMagnet(void);
void enforceMagnet(void);
void releaseMagnet(void);

void magnetHandler(void* pvParameter) {
	for(;;) {
		if(!queue_isEmpty(magnetCmdQueue)) {
			char cmd = queue_read(magnetCmdQueue);
			switch(cmd) {
				case DISABLE:
					disableMagnet();
					break;
				case ENFORCE:
					enforceMagnet();
					break;
				case RELEASE:
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
