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
/* threads */
#include <pthread.h>

#define SERVER_ADDRESS "192.168.246.131"
#define PORT            8080 
#define BUFF_SIZE		1024

#define CONNECT_TYPE_FRAME 		(0x10)
#define KEEP_ALIVE 				(0x000A) // 10
#define PROTOCOL_LVL 			(0x04)
#define CLEAN_SESSION 			(0x02)

typedef struct {
    uint8_t bFrameType;
    uint16_t wLen;
    uint8_t reservado;
    uint8_t returnCode;
}sKeepAlive;

typedef struct {
    uint8_t bFrameType;
    uint16_t wLen;
    char sClientID[50];
    uint8_t bProtocol;
    uint8_t bCleanSession;
    uint16_t wKeepAlive;
}sConnect;

typedef struct {
    uint8_t bFrameType;
    uint16_t wLen;
    uint8_t reservado;
    uint8_t returnCode;
}sCONNACK;

// FUNCIONES
sConnect createFrame_Connect(char *argv, uint16_t size);
sKeepAlive createFrame_KeepAlive();
void *keepAliveFunc();

// VARIABLES GLOBALES
int sockfd, len_tx, len_rx;
struct sockaddr_in servaddr;

