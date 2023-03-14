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

sKeepAlive createFrame_KeepAlive() {
	sKeepAlive connectFrame;
	
    connectFrame.bFrameType = CONNECT_TYPE_FRAME;
	
	return connectFrame;
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
	    	//sKeepAlive keepAliveFrame;
	    	// send keep alive
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
	sCONNACK connackFrame;
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
    	
	sConnect frametoSend = createFrame_Connect(argv[1], strlen(argv[1]));
	printf("Sending connect request... %lu bytes...\n", sizeof(frametoSend));
	
	len_tx = send(sockfd, &frametoSend, sizeof(frametoSend), 0);	// send connect
	sleep(1);
	printf("... %i bytes... enviados\n\n", len_tx);
	
	sleep(1);
	len_rx = recv(sockfd, &connackFrame, sizeof(connackFrame), 0);		// recv ack
	printf("[SERVER]\n");
	printf("Type: %i\n", connackFrame.bFrameType);
	printf("Size: %i\n", connackFrame.wLen);
	printf("Reservado: %i\n", connackFrame.reservado);
	printf("Return Code: %i\n\n", connackFrame.returnCode);
		
	// Esperar a finalizar el hilo de conexión para terminar el programa
	//pthread_join(tid, NULL);
	close(sockfd);
	prtinf("Connection closed.\n");
    
	return 0;
}

