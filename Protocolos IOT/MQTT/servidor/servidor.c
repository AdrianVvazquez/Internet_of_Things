#include "header.h"

sCONNACK createFrame_ACK() {
	sCONNACK ACKFrame;
	
    ACKFrame.bFrameType = CONNACK_TYPE_FRAME;
    ACKFrame.wLen = (uint16_t) 0x02;
    ACKFrame.reservado = 0x00;
    ACKFrame.returnCode = 0x00;
	
	return ACKFrame;
}

void *readWrite(void *param) {
	while(1)
	{
		for(int i=0; i<2; i++)
		{	
			if(hosts[i] != 0)
			{
		    	pthread_mutex_lock(&socketMutex);

		    	
				sConnect connectFrame;
				int bytes_received = recv(hosts[i], &connectFrame, sizeof(connectFrame), 0);	// read connect
				if (bytes_received == -1) {
					perror("Nothing to receive\n");
					
					break;
				}
							
				printf("[CLIENT %i]\n", i+1);
				printf("Type: %i\n", connectFrame.bFrameType);
				printf("Size: %i\n", connectFrame.wLen);
				printf("Cliente ID: %s\n", connectFrame.sClientID);
				printf("Level protocol: %i\n", connectFrame.bProtocol);
				printf("Clean session: %i\n", connectFrame.bCleanSession);
				printf("Keep Alive interval: %i\n\n", connectFrame.wKeepAlive);
				
				keepAliveList[idx_KA] = (int)connectFrame.wKeepAlive;						// Add keep alive to list
			
				sCONNACK connackFrame = createFrame_ACK();
				int bytes_send = send(hosts[i], &connackFrame, sizeof(connackFrame), 0);	// send connack
				printf("Sending connack... %i bytes...\n", bytes_send);
				
				close(hosts[i]);
				hosts[i] = 0;
				idx = i;		// Cambiar nuevo tamaño de lista
				idx_KA++;
				
		    	pthread_mutex_unlock(&socketMutex);
			}
		}
	}	

	// pthread_exit(0);
}


int main(int32_t argc, char *argv[]){
	/* Identifier */
	pthread_t tid;
	/* Attributes */
	pthread_attr_t attr;
    /* thread init */
    pthread_attr_init(&attr);
    
    int sockfd ,connfd;
    struct sockaddr_in servaddr;
   	
    // SOCKET
    /* Socket creation */
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) 
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
    
    /* Bind socket */
    if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) 
    { 
        printf("[SERVER-error]: socket bind failed. %d: %s\n", errno, strerror(errno));
        return -1;
    } 
    else
    {
        printf("Status:  Socket successfully binded. \n");
    }
    
    // LISTEN
    if ((listen(sockfd, BACKLOG)) != 0) 
    { 
        printf("[SERVER-error]: socket listen failed. %d: %s\n", errno, strerror(errno));
        return -1;
    } 
    else
    {
        printf("Status: Listening on SERV_PORT %d \n", ntohs(servaddr.sin_port) ); 
    }
    
    unsigned int sock_len = sizeof(client);		/* length of client address */
    
	// HILO READ/WRITE
	pthread_create(&tid, &attr, readWrite, NULL);
	
	// ACCEPT loop
	while(1)
	{
		if ((connfd = accept(sockfd, (struct sockaddr *)&client, &sock_len)) < 0)
		{ 
			printf("[SERVER-error]: connection not accepted. %d: %s\n", errno, strerror(errno));
			return -1;
		} 
		else
		{    
			printf("\n\t[NEW] Connection accepted. \n");
			// ADD CONNECTION TO LIST
			if (hosts[idx] == 0) 
			{
				hosts[idx] = connfd;
				idx++;
				printf("\t[CONECTIONS]: %i\n\n", idx);
			}
					
		}
    } 
        
	
	return 0;
}
