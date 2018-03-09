
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
	return get2Bytes();
}

char getCmd(void) {
	return get1Byte();
}

char get1Byte(void) {
	char ch = 0;
	(void) RxBuf_Get(&ch);
	return ch;
}

int get2Bytes(void) {
	unsigned char ch1 = 0;
	int result = 0;

	// first byte and shift
	(void) RxBuf_Get(&ch1);
	result = ch1;
	result << 8;

	// second byte and shift
	(void) RxBuf_Get(&ch);
	result = result | ch;

	return result;
}
