#include "header.h"

sCONNACK createFrame_ACK() {
	sCONNACK ACKFrame;
	
    ACKFrame.bFrameType = CONNACK_TYPE_FRAME;
    ACKFrame.wLen = (uint16_t) 0x02;
    ACKFrame.reservado = 0x00;
    ACKFrame.returnCode = 0x00;
	
	return ACKFrame;
}

sPing createFrame_PingResponse() {
	sPing pingResponseFrame;
	
    pingResponseFrame.bFrameType = MESSAGE_TYPE_FRAME;
    pingResponseFrame.wLen = 0x0002;
    pingResponseFrame.reservado = 0x00;
    pingResponseFrame.returnCode = 0x01;
	
	return pingResponseFrame;
}

void *readWrite(void *param) {
	while(1)
	{
		for(int i=0; i<10; i++)
		{	
			if(hosts[i] != 0)
			{
				int bytes_received, bytes_send;
				sConnect connectFrame;
				sCONNACK connackFrame;
				sPing pingRequestFrame, pingResponseFrame;
				
				// read connect
				bytes_received = recv(hosts[i], &connectFrame, sizeof(connectFrame), 0);
				
				if (bytes_received == -1) {
					perror("Nothing to receive\n");
					break;
				} 

				printf("[CLIENT %i] connect\n", i+1);
				printf("Type: %i\n", connectFrame.bFrameType);
				printf("Size: %i\n", connectFrame.wLen);
				printf("Cliente ID: %s\n", connectFrame.sClientID);
				printf("Level protocol: %i\n", connectFrame.bProtocol);
				printf("Clean session: %i\n", connectFrame.bCleanSession);
				printf("Keep Alive interval: %i\n\n", connectFrame.wKeepAlive);
				
				connackFrame = createFrame_ACK();
				// send ack
				bytes_send = send(hosts[i], &connackFrame, sizeof(connackFrame), 0);
				printf("Sending connack... %i bytes... enviados\n\n", bytes_send);
				
				// read ping request
				bytes_received = recv(hosts[i], &pingRequestFrame, sizeof(pingRequestFrame), 0);
				
				if (bytes_received == -1) {
					perror("Nothing to receive\n");
					break;
				} 

				printf("[CLIENT %i] ping request\n", i+1);
				printf("Type: %i\n", pingRequestFrame.bFrameType);
				printf("Size: %i\n", pingRequestFrame.wLen);
				printf("Reserved: %i\n", pingRequestFrame.reservado);
				printf("Return code: %i\n\n", pingRequestFrame.returnCode);
				
				pingResponseFrame = createFrame_PingResponse();
				// send ack
				bytes_send = send(hosts[i], &pingResponseFrame, sizeof(pingResponseFrame), 0);
				printf("Sending ping response... %i bytes... enviados\n\n", bytes_send);
				
				close(hosts[i]);
				hosts[i] = 0;
				idx = i;		// Cambiar nuevo tamaño de lista
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
			// ADD CONECTION TO LIST
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
