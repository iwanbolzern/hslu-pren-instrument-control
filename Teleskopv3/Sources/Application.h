/*
 * Application.h
 *
 *  Created on: 26.12.2017
 *      Author: El_Matador
 */

#ifndef SOURCES_APPLICATION_H_
#define SOURCES_APPLICATION_H_

#include "PE_Types.h"
#include "PE_LDD.h"


typedef struct {
  LDD_TDeviceData *handle; /* LDD device handle */
  volatile uint8_t isSent; /* this will be set to 1 once the block has been sent */
  uint8_t rxChar; /* single character buffer for receiving chars */
  uint8_t (*rxPutFct)(uint8_t); /* callback to put received character into buffer */
} UART_Desc;


typedef enum valueMode mode_t;
enum valueMode {
	DISTANCE = 1, LETTER = 2
};
void APP_Run(void);


int getLengthOfMessage(void);



char getCmd(void);
int get2Bytes(void);
char get1Byte(void);


void initTele(void);
void driveDistance(int,int,int);
void driveJog(int,int);
void driveToEnd(int,int,int);
void moveTele(int,int);
void enableMagnet(int);
void disableMagnet(void);


int getData8(void);
int getData16(void);
void sendText(char[]);
void readOne(void);
void sendMenu(void);
int getValue(mode_t);
void sendHeader(void);
void emptyBuffer(void);



int getDecimal(unsigned int );
#endif /* SOURCES_APPLICATION_H_ */
