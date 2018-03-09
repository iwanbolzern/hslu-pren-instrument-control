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

extern int param1;
extern int param2;
extern int param3;
extern LDD_TDeviceData* MyGPIOPtr;

typedef enum valueMode mode_t;
enum valueMode {
	DISTANCE = 1, LETTER = 2
};
void APP_Run();


int getLengthOfMessage(void);
extern int counter;


char getCmd(void);
int get2Bytes(void);
char get1Byte(void);

void insert(char);
void initTele(void *);
void driveDistance(void*);
void driveJog(void*);
void driveToEnd(void*);
void moveTele(void*);
void enableMagnet(void*);
void disableMagnet(void*);


void posUpdate(void*);
void enqueue(void);

typedef struct {
    char info;
} DATA;


typedef struct Node_t {
	DATA data;
	struct Node_t *prev;
} NODE;



/* the HEAD of the Queue, hold the amount of node's that are in the queue*/
typedef struct Queue {
	NODE *head;
	NODE *tail;
	int size;
	int limit;
} Queue;


Queue *ConstructQueue(int);
void DestructQueue(Queue *);
int Enqueue(Queue *, NODE *);
NODE *Dequeue(Queue *);
int isEmpty(Queue* );

extern Queue *pQ;
extern NODE *pN;


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
