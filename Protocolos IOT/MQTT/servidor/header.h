/*standard symbols */
#include <unistd.h>
#include <stdlib.h> 
/* sockets */
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <netdb.h> 
/* strings / errors*/
#include <errno.h>
#include <stdio.h> 
#include <string.h> 
/* threads */
#include <pthread.h>

#define SERVER_ADDRESS "192.168.246.131"
#define PORT            8080 
#define BACKLOG         5                 /* Max. client pending connections  */
#define BUFF_SIZE		1024

#define CONNACK_TYPE_FRAME 		(0x20)

#define TIME_KEEP_ALIVE 		(0x000A)
#define PROTOCOL_LVL 			(0x04)
#define CLEAN_SESSION 			(0x02)

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
sCONNACK createFrame_ACK();
// VARIABLES GLOBALES
int idx=0, hosts[3]={0}, idx_KA=0;
int  keepAliveList[3]={0};
struct sockaddr_in servaddr, client; 
pthread_mutex_t socketMutex = PTHREAD_MUTEX_INITIALIZER;



