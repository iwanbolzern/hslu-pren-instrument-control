/*
 * driveTelescope.h
 *
 *  Created on: 11.03.2018
 *      Author: El_Matador
 */

#ifndef SOURCES_DRIVETELESCOPE_H_
#define SOURCES_DRIVETELESCOPE_H_

#include "FRTOS1.h"

void driveTelescope(void*);
void tele_tickReceived(void);
void tele_endSwitchReceived(void);

typedef enum telescopeCmd {
	telescopeCmd_INIT_TELE = 0,
	telescopeCmd_DRIVE_TELE = 1
} telesopceCmd_t;

typedef enum teleDirection {
	teleDirection_RETRACT = 1,
	teleDirection_EXTEND = 0
} teleDirection_t;

enum speedmode {
	MODE_HYPERSLOW = 1,
	MODE_ULTRASLOW,
	MODE_VERYSLOW,
	MODE_SLOW,
	MODE_MEDIUM,
	MODE_FAST,
	MODE_VERYFAST,
	MODE_ULTRAFAST,
	MODE_HYPERFAST
};

extern QueueHandle_t driveTelescopeQueue;

#endif /* SOURCES_DRIVETELESCOPE_H_ */
