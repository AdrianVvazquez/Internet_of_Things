#include "header.h"

int timer=60;

sConnect createFrame_Connect(char *argv, uint16_t size) {
	sConnect connectFrame;
	
    connectFrame.bFrameType = CONNECT_TYPE_FRAME;
    connectFrame.wLen = size;
    strcpy(connectFrame.sClientID, argv);
    connectFrame.bProtocol = PROTOCOL_LVL;
    connectFrame.bCleanSession = CLEAN_SESSION;
    connectFrame.wKeepAlive = KEEP_ALIVE;
	
	return connectFrame;
}

sPing createFrame_PingRequest() {
	sPing pingRequestFrame;
	
    pingRequestFrame.bFrameType = MESSAGE_TYPE_FRAME;
    pingRequestFrame.wLen = 0x0002;
    pingRequestFrame.reservado = 0x00;
    pingRequestFrame.returnCode = 0x00;
	
	return pingRequestFrame;
}

void *keepAliveFunc(void *param) {
	
	while(1)
	{
    	if (timer == 0)
    	{
    		printf("Timer is 0. Closing connection ...\n");	
			pthread_exit(0);
    	}
    	
    	if (timer%KEEP_ALIVE == 0)
    	{
    		printf("¡¡%i segundos!!\n", timer);	
    	} 
    	else 
    	{
			printf("%i segundos\n", timer);
    	}
		sleep(1);
		timer--;
    }	
}

int main(int32_t argc, char *argv[]){ 
	/* Identifier */
	//pthread_t tid;
	/* Attributes */
	pthread_attr_t attr;
	/* Set the default attributes of the thread*/
	pthread_attr_init(&attr);
	
    /* Socket creation */
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd < 0) 
    { 
        printf("[Error]: socket creation failed...\n"); 
        return -1;  
    } 
    else
    {
        printf("Status: Socket successfully created..\n"); 
    }
    
    memset(&servaddr, 0, sizeof(servaddr));
    /* assign IP, PORT */
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr( SERVER_ADDRESS );
    servaddr.sin_port = htons(PORT); 
    
  	// CONNECT
    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) 
    { 
        printf("[Error]: Connection with the server failed...\n");
        return -1;
    } 
    printf("Status: Connected to the server..\n\n");
    
    //pthread_create(&tid, &attr, keepAliveFunc, NULL);
    	
	sConnect connectFrame = createFrame_Connect(argv[1], strlen(argv[1]));
	printf("Sending connect request... %lu bytes...\n", sizeof(connectFrame));
	
	len_tx = send(sockfd, &connectFrame, sizeof(connectFrame), 0);	// send connect
	sleep(1);
	printf("... %i bytes... enviados\n\n", len_tx);
	
	sCONNACK connackFrame;
	len_rx = recv(sockfd, &connackFrame, sizeof(connackFrame), 0);		// recv ack
	printf("[SERVER] connack\n");
	printf("Type: %i\n", connackFrame.bFrameType);
	printf("Size: %i\n", connackFrame.wLen);
	printf("Reservado: %i\n", connackFrame.reservado);
	printf("Return Code: %i\n\n", connackFrame.returnCode);
		
	sPing pingRequestFrame = createFrame_PingRequest();
	printf("Sending ping request... %lu bytes...\n", sizeof(pingRequestFrame));
	
	len_tx = send(sockfd, &pingRequestFrame, sizeof(pingRequestFrame), 0);	// send ping request
	sleep(1);
	printf("... %i bytes... enviados\n\n", len_tx);
	
	sPing pingResponseFrame;
	len_rx = recv(sockfd, &pingResponseFrame, sizeof(pingResponseFrame), 0);		// recv ack
	printf("[SERVER] ping response\n");
	printf("Type: %i\n", pingResponseFrame.bFrameType);
	printf("Size: %i\n", pingResponseFrame.wLen);
	printf("Reservado: %i\n", pingResponseFrame.reservado);
	printf("Return Code: %i\n\n", pingResponseFrame.returnCode);
	//pthread_join(tid, NULL);
    
	return 0;
}

