/*
 * Application.c
 *
 *  Created on: 26.12.2017
 *      Author: El_Matador
 */

#include "Application.h"
#include "RxBuf.h"
#include "AS1.h"

UART_Desc deviceData;

static void SendChar(unsigned char ch, UART_Desc *desc) {
	desc->isSent = FALSE; /* this will be set to 1 once the block has been sent */
	while (AS1_SendBlock(desc->handle, (LDD_TData*) &ch, 1) != ERR_OK) {
	} /* Send char */
	while (!desc->isSent) {
	} /* wait until we get the green flag from the TX interrupt */
}

static void SendString(const unsigned char *str, UART_Desc *desc) {
	while (*str != '\0') {
		SendChar(*str++, desc);
	}
}




static void Init(void) {
	/* initialize struct fields */
	deviceData.handle = AS1_Init(&deviceData);
	deviceData.isSent = FALSE;
	deviceData.rxChar = '\0';
	deviceData.rxPutFct = RxBuf_Put;
	/* set up to receive RX into input buffer */
	RxBuf_Init(); /* initialize RX buffer */
	/* Set up ReceiveBlock() with a single byte buffer. We will be called in OnBlockReceived() event. */
	while (AS1_ReceiveBlock(deviceData.handle, (LDD_TData *) &deviceData.rxChar,
			sizeof(deviceData.rxChar)) != ERR_OK) {
	} /* initial kick off for receiving data */
}




int hexTodec(unsigned int hex) {

	switch (hex) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
		return hex - 55;
		break;

	default:
		return hex - 48;

	}

}

int getDecimal(unsigned int value) {

	return value - 0x30;

}

/*
 * Methode, welche die nöchsten 8 Bits ausliest
 */
int getData8() {

	int counter = 2;
	int value16e0 = 0;
	int value16e1 = 0;
	unsigned char ch;

	while (counter > 0) { // Datengrösse ist sowieso nie grösser als 255 Byte (0xFF)

		(void) RxBuf_Get(&ch);
		vTaskDelay(pdMS_TO_TICKS(10));
		counter = counter - 1;
	}

	(void) RxBuf_Get(&ch);

	value16e1 = 16 * hexTodec(ch);

	(void) RxBuf_Get(&ch);

	value16e0 = hexTodec(ch);

	return (value16e1 + value16e0);

	//SendChar(dataLength, &deviceData);

}

void readOne() {

	unsigned char ch;
	(void) RxBuf_Get(&ch);

}

int getData16() {

	int counter = 2;
	int value16e0 = 0;
	int value16e1 = 0;
	int value16e2 = 0;
	int value16e3 = 0;

	unsigned char ch;

	while (counter > 0) { // Datengrösse ist sowieso nie grösser als 255 Byte (0xFF)

		(void) RxBuf_Get(&ch);
		vTaskDelay(pdMS_TO_TICKS(10));
		counter = counter - 1;
	}

	(void) RxBuf_Get(&ch);

	value16e3 = 0x1000 * hexTodec(ch);				// 0x1000 = 16^3

	(void) RxBuf_Get(&ch);

	value16e2 = 0x100 * hexTodec(ch);			// 0x100 = 16^2

	counter = 2;

	while (counter > 0) { // Datengrösse ist sowieso nie grösser als 255 Byte (0xFF)

		(void) RxBuf_Get(&ch);
		vTaskDelay(pdMS_TO_TICKS(10));
		counter = counter - 1;
	}

	(void) RxBuf_Get(&ch);

	value16e1 = 0x10 * hexTodec(ch);				// 0x10 = 16^1

	(void) RxBuf_Get(&ch);

	value16e0 = 0x1 * hexTodec(ch);			// 0x1 = 16^0

	return (value16e3 + value16e2 + value16e1 + value16e0);

	//SendChar(dataLength, &deviceData);

}





/*
 ** ===================================================================

 **     Description :
 **
 **        Liest die Längenangabe der Message
 **
 **
 **     Parameters  : Nothing
 **
 **
 **     Returns     : Länge der Message als int

 ** ===================================================================
 */
int getLengthOfMessage(void) {
int temp= 0;
int value16e1 = 0;
int value16e0 = 0;
char ch = 0;

(void) RxBuf_Get(&ch);
temp = ch;
value16e1 = 16 * temp;
(void) RxBuf_Get(&ch);
temp = ch;
value16e0 = temp;

	return value16e1 + value16e0;

}



/*
 ** ===================================================================

 **     Description :
 **
 **        Liest das Kommando ID
 **
 **
 **     Parameters  : Nothing
 **
 **
 **     Returns     : Kommando-ID

 ** ===================================================================
 */


char getCmd(void){

char ch = 0;

(void) RxBuf_Get(&ch);

return ch;

}



void initTele(void){



}
void driveDistance(int distance,int speed,int direction){

}
void driveJog(int speed,int direction){

}
void driveToEnd(int predDistance,int speed,int direction){

}
void moveTele(int distance,int direction){

}
void enableMagnet(int direction){

}
void disableMagnet(void){

}

void APP_Run(void){

	Init();

}













void sendText(char text[]) {

	SendString((unsigned char*) text, &deviceData);

}



void emptyBuffer(void) {

	unsigned char ch;
	while (RxBuf_NofElements() != 0) {
		(void) RxBuf_Get(&ch);
		vTaskDelay(pdMS_TO_TICKS(3));

	}

}
int getValue(mode_t modus) {

	unsigned char ch;
	unsigned char t;

	unsigned char a;
	unsigned char b;
	unsigned char c;
	unsigned char d;

	unsigned char e;

	int value10e3 = 0;
	int value10e2 = 0;
	int value10e1 = 0;
	int value10e0 = 0;
	int letter = 0;

	if (modus == DISTANCE) {

		(void) RxBuf_Get(&ch);
		a = ch;
		vTaskDelay(pdMS_TO_TICKS(2));
		value10e3 = getDecimal(ch) * 1000;

		vTaskDelay(pdMS_TO_TICKS(2));
		(void) RxBuf_Get(&ch);
		b = ch;
		value10e2 = getDecimal(ch) * 100;

		vTaskDelay(pdMS_TO_TICKS(2));
		(void) RxBuf_Get(&ch);
		c = ch;
		value10e1 = getDecimal(ch) * 10;

		vTaskDelay(pdMS_TO_TICKS(2));
		(void) RxBuf_Get(&ch);
		d = ch;
		value10e0 = getDecimal(ch);

		(void) RxBuf_Get(&ch);

		if ((a == 'x') || (b == 'x') || (c == 'x') || (d == 'x')) {
			return -1;
		} else {
			return value10e3 + value10e2 + value10e1 + value10e0;
		}
	}

	else {
		(void) RxBuf_Get(&ch);
		t = ch;
		if (t == 'x') {
			return -1;
		} else {
			(void) RxBuf_Get(&ch);
			return t;

		}
	}

	// zum /0 aus dem Buffer nehmen

}

void sendHeader(void) {

	sendText(
			"#####################################################\n"
					"#"
					"                                                                         "
					"                                              #\n#              TELESKOP TESTER"
					"                                                              #\n#"
					"                                                                         "
					"                                              #\n#####################################################");

}

void sendMenu(void) {

	sendText(
			"\n\nsjdasjdlkjdlkasjdklda\n\n");



}


