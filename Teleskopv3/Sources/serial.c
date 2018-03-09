
void initSerialPort() {
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

int getLengthOfMessage(void) {
	int temp = 0;
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

char getCmd(void) {

	char ch = 0;

	(void) RxBuf_Get(&ch);

	return ch;

}

int get2Bytes(void) {

	unsigned char ch1 = 0;
	unsigned char ch2 = 0;
	int temp1 = 0;
	int temp2 = 0;
	int temp3 = 0;
	int value16e2 = 256;
	int result = 0;

	(void) RxBuf_Get(&ch1);

	temp1 = ch1;

	(void) RxBuf_Get(&ch2);

	temp3 = temp1 * value16e2;

	temp2 = ch2;
	result = temp3 + temp2;

	return result;

}
