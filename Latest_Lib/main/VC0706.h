 #include <stdbool.h>

#define VC0706_RESET  0x26
#define VC0706_GEN_VERSION 0x11
#define VC0706_READ_FBUF 0x32
#define VC0706_GET_FBUF_LEN 0x34
#define VC0706_FBUF_CTRL 0x36
#define VC0706_DOWNSIZE_CTRL 0x54
#define VC0706_DOWNSIZE_STATUS 0x55
#define VC0706_READ_DATA 0x30
#define VC0706_WRITE_DATA 0x31
#define VC0706_COMM_MOTION_CTRL 0x37
#define VC0706_COMM_MOTION_STATUS 0x38
#define VC0706_COMM_MOTION_DETECTED 0x39
#define VC0706_MOTION_CTRL 0x42
#define VC0706_MOTION_STATUS 0x43
#define VC0706_TVOUT_CTRL 0x44
#define VC0706_OSD_ADD_CHAR 0x45

#define VC0706_STOPCURRENTFRAME 0x0
#define VC0706_STOPNEXTFRAME 0x1
#define VC0706_RESUMEFRAME 0x3
#define VC0706_STEPFRAME 0x2

#define VC0706_640x480 0x00
#define VC0706_320x240 0x11
#define VC0706_160x120 0x22

#define VC0706_MOTIONCONTROL 0x0
#define VC0706_UARTMOTION 0x01
#define VC0706_ACTIVATEMOTION 0x01

#define VC0706_SET_ZOOM 0x52
#define VC0706_GET_ZOOM 0x53

#define CAMERABUFFSIZ 100
#define CAMERADELAY 10

uint8_t  serialNum = 0;
uint8_t  camerabuff[CAMERABUFFSIZ+1];
uint8_t  bufferLen = 0;
uint16_t frameptr = 0;

void begin(uint16_t baud);
bool TVon(void);
bool TVoff(void);
bool takePicture(void);
uint8_t *readPicture(uint8_t n);
bool resumeVideo(void);
uint32_t frameLength(void);
char *getVersion(void);
uint8_t available();
uint8_t getDownsize(void);
bool setDownsize(uint8_t);
uint8_t getImageSize();
bool setImageSize(uint8_t);
bool getMotionDetect();
uint8_t getMotionStatus(uint8_t);
bool motionDetected();
bool setMotionDetect(bool f);
bool setMotionStatus(uint8_t x, uint8_t d1, uint8_t d2);
bool cameraFrameBuffCtrl(uint8_t command);
uint8_t getCompression();
bool setCompression(uint8_t c); 

void OSD(uint8_t x, uint8_t y, char *s); 
bool runCommand(uint8_t cmd, uint8_t args[], uint8_t argn, uint8_t resp, bool flushflag); 
void sendCommand(uint8_t cmd, uint8_t args[], uint8_t argn); 
uint8_t readResponse(uint8_t numbytes, uint8_t timeout);
bool verifyResponse(uint8_t command);
void printBuff(void);


void begin(uint16_t baud)
{
	USART_init(baud);
}

bool motionDetected() 
{
	if (readResponse(4, 200) != 4) {
		return 0;
	}
	if (! verifyResponse(VC0706_COMM_MOTION_DETECTED))
		return 0;

	return 1;
}


bool setMotionStatus(uint8_t x, uint8_t d1, uint8_t d2)
 {
	uint8_t args[] = {0x03, x, d1, d2};

	return runCommand(VC0706_MOTION_CTRL, args, sizeof(args), 5,1);
}


uint8_t getMotionStatus(uint8_t x) {
	uint8_t args[] = {0x01, x};

	return runCommand(VC0706_MOTION_STATUS, args, sizeof(args), 5,1);
}

bool setMotionDetect(bool flag)
 {
	if (! setMotionStatus(VC0706_MOTIONCONTROL,
	                      VC0706_UARTMOTION, VC0706_ACTIVATEMOTION))
		return 0;

	uint8_t args[] = {0x01, flag};

	runCommand(VC0706_COMM_MOTION_CTRL, args, sizeof(args), 5,1);
}

bool getMotionDetect(void) {
	uint8_t args[] = {0x0};

	if (! runCommand(VC0706_COMM_MOTION_STATUS, args, 1, 6,1))
		return 0;

	return camerabuff[5];
}

uint8_t getImageSize() {
	uint8_t args[] = {0x4, 0x4, 0x1, 0x00, 0x19};
	if (! runCommand(VC0706_READ_DATA, args, sizeof(args), 6,1))
		return -1;

	return camerabuff[5];
}

bool setImageSize(uint8_t x) {
	uint8_t args[] = {0x05, 0x04, 0x01, 0x00, 0x19, x};

	return runCommand(VC0706_WRITE_DATA, args, sizeof(args), 5,1);
}


uint8_t getDownsize(void) {
	uint8_t args[] = {0x0};
	if (! runCommand(VC0706_DOWNSIZE_STATUS, args, 1, 6,1))
		return -1;

	return camerabuff[5];
}

bool setDownsize(uint8_t newsize) {
	uint8_t args[] = {0x01, newsize};

	return runCommand(VC0706_DOWNSIZE_CTRL, args, 2, 5,1);
}

char * getVersion(void) {
	uint8_t args[] = {0x01};

	sendCommand(VC0706_GEN_VERSION, args, 1);
	// get reply
	if (!readResponse(CAMERABUFFSIZ, 200))
		return 0;
	camerabuff[bufferLen] = '\0';  // end it!
	return (char *)camerabuff;  // return it!
}

void OSD(uint8_t x, uint8_t y, char *str) {
	if (strlen(str) > 14) {
		str[13] = 0;
	}

	uint8_t args[17] = {strlen(str), strlen(str)-1, (y & 0xF) | ((x & 0x3) << 4)};

	for (uint8_t i=0; i<strlen(str); i++) {
		char c = str[i];
		if ((c >= '0') && (c <= '9')) {
			str[i] -= '0';
		} else if ((c >= 'A') && (c <= 'Z')) {
			str[i] -= 'A';
			str[i] += 10;
		} else if ((c >= 'a') && (c <= 'z')) {
			str[i] -= 'a';
			str[i] += 36;
		}

		args[3+i] = str[i];
	}

	runCommand(VC0706_OSD_ADD_CHAR, args, strlen(str)+3, 5,1);
	printBuff();
}

bool setCompression(uint8_t c) {
	uint8_t args[] = {0x5, 0x1, 0x1, 0x12, 0x04, c};
	return runCommand(VC0706_WRITE_DATA, args, sizeof(args), 5,1);
}

uint8_t getCompression(void) {
	uint8_t args[] = {0x4, 0x1, 0x1, 0x12, 0x04};
	runCommand(VC0706_READ_DATA, args, sizeof(args), 6,1);
	printBuff();
	return camerabuff[5];
}

bool takePicture() {
	frameptr = 0;
	return cameraFrameBuffCtrl(VC0706_STOPCURRENTFRAME);
}

bool resumeVideo() {
	return cameraFrameBuffCtrl(VC0706_RESUMEFRAME);
}

bool TVon() {
	uint8_t args[] = {0x1, 0x1};
	return runCommand(VC0706_TVOUT_CTRL, args, sizeof(args), 5,1);
}
bool TVoff() {
	uint8_t args[] = {0x1, 0x0};
	return runCommand(VC0706_TVOUT_CTRL, args, sizeof(args), 5,1);
}

bool cameraFrameBuffCtrl(uint8_t command) {
	uint8_t args[] = {0x1, command};
	return runCommand(VC0706_FBUF_CTRL, args, sizeof(args), 5,1);
}

uint32_t frameLength(void) {
	uint8_t args[] = {0x01, 0x00};
	if (!runCommand(VC0706_GET_FBUF_LEN, args, sizeof(args), 9,1))
		return 0;

	uint32_t len;
	len = camerabuff[5];
	len <<= 8;
	len |= camerabuff[6];
	len <<= 8;
	len |= camerabuff[7];
	len <<= 8;
	len |= camerabuff[8];

	return len;
}


uint8_t available(void) {
	return bufferLen;
}

uint8_t * readPicture(uint8_t n) {
	uint8_t args[] = {0x0C, 0x0, 0x0A,
	                  0, 0, frameptr >> 8, frameptr & 0xFF,
	                  0, 0, 0, n,
	                  CAMERADELAY >> 8, CAMERADELAY & 0xFF
	                 };

	if (! runCommand(VC0706_READ_FBUF, args, sizeof(args), 5, 0))
		return 0;


	// read into the buffer PACKETLEN!
	if (readResponse(n+5, CAMERADELAY) == 0)
		return 0;
	frameptr += n;
	return camerabuff;
}

bool runCommand(uint8_t cmd, uint8_t *args, uint8_t argn,uint8_t resplen, bool flushflag)
 {
	if (flushflag) {
		readResponse(100, 10);
	}

	sendCommand(cmd, args, argn);
	if (readResponse(resplen, 200) != resplen)
		return 0;
	if (! verifyResponse(cmd))
		return 0;
	return 1;
}

void sendCommand(uint8_t cmd, uint8_t args[] , uint8_t argn) 
{
		USART_putnumber(86);
		USART_putnumber(serialNum);
		USART_putnumber(cmd);

		for (uint8_t i=0; i<argn; i++) 
			USART_putnumber(args[i]);
}

uint8_t readResponse(uint8_t numbytes, uint8_t timeout)
 {
	bufferLen = 0;
	bool response = 0;
	
	while ((response != 0) && (bufferLen != numbytes)) 
	response = USART_receive_timeout(timeout,camerabuff[bufferLen]);	
	
	return bufferLen;
}

bool verifyResponse(uint8_t command) {
	if ((camerabuff[0] != 0x76) ||
	        (camerabuff[1] != serialNum) ||
	        (camerabuff[2] != command) ||
	        (camerabuff[3] != 0x0))
		return 0;
	return 1;

}

void printBuff() {
	for (uint8_t i = 0; i< bufferLen; i++) 
		USART_putnumber(camerabuff[i]);
}