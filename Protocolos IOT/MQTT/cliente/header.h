/*standard symbols */
#include <unistd.h>
#include <stdlib.h> 
/* sockets */
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <netdb.h> 
/* strings / errors*/
#include <stdio.h> 
#include <string.h> 

#define SERVER_ADDRESS "192.168.246.131"
#define PORT            8080 
#define BUFF_SIZE		1024

#define CONNECT_TYPE_FRAME 		(0x10)
#define MESSAGE_TYPE_FRAME 		(0x20)

#define KEEP_ALIVE 				(0x000A) // 10
#define PROTOCOL_LVL 			(0x04)
#define CLEAN_SESSION 			(0x02)

typedef struct {
    uint8_t bFrameType;
    uint16_t wLen;
    uint8_t reservado;
    uint8_t returnCode;
}sMessage;

typedef struct {
    uint8_t bFrameType;
    uint16_t wLen;
    char sClientID[50];
    uint8_t bProtocol;
    uint8_t bCleanSession;
    uint16_t wKeepAlive;
}sConnect;

sConnect vfnCreateFrame();
