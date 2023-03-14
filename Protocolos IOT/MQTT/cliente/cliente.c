#include "header.h"

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

sMessage createFrame_Mesasage() {
	sMessage messageFrame;
	
    messageFrame.bFrameType = MESSAGE_TYPE_FRAME;
    messageFrame.wLen = (uint16_t) 0x02;
    messageFrame.returnCode = 0x00;
	
	return messageFrame;
}


int main(int32_t argc, char *argv[]){
    int sockfd, len_tx, timer=60;
    struct sockaddr_in servaddr; 
    // char buf_rx[BUFF_SIZE];
   	
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
    
    while(1)
    {
    	
    	if (timer <= 0)
    	{
    		printf("Timer is 0. Closing connection ...\n");
    		break;
    	}
    	
    	if (timer%KEEP_ALIVE == 0)
    	{
    		printf("5 segundos\n");	
    		
    		sConnect frametoSend = createFrame_Connect(argv[1], strlen(argv[1]));
			
			printf("Type: : %i\n", frametoSend.bFrameType);
			printf("Size: %i\n", frametoSend.wLen);
			printf("Cliente ID: %s\n", frametoSend.sClientID);
			printf("Level protocol: %i\n", frametoSend.bProtocol);
			printf("Clean session: %i\n", frametoSend.bCleanSession);
			printf("Keep Alive interval: %i\n", frametoSend.wKeepAlive);
			
			printf("\nEnviando... %lu bytes...\n", sizeof(frametoSend));
			
			sleep(5);
			len_tx = send(sockfd, &frametoSend, sizeof(frametoSend), 0);
			printf("... %i bytes... enviados\n", len_tx);
    		
    		
    	}
    	
    	timer--;
    }
    
    close(sockfd);
    
	return 0;
}

