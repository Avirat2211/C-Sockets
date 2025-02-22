#include "../Socketutil/src/socketutil.h"
#include<pthread.h>

struct AcceptedSocket{
    int acceptedSocketFD;
    struct sockaddr_in address;
    int error;
    int success;
};

struct AcceptedSocket* acceptIncomingConnection(int serverSocketFD);
void* recieveAndPrintIncomingData(void* arg);
void* acceptNewConnectionAndRecieveAndPrintItsData(void* arg);
void recieveAndPrintIncomingDataOnSeparateThread(struct AcceptedSocket* clientSocket);
void sendRecievedMessageToOtherClients(int socketFD,char *buffer);


struct AcceptedSocket acceptedSockets[10];
int acceptedCount = 0;

int main(){

    
    int serverSocketFD = createTCPIpv4Socket();
    struct sockaddr_in* serverAddress = createIpv4Address("0.0.0.0",2000);

    int success = bind(serverSocketFD, (struct sockaddr*)serverAddress, sizeof(struct sockaddr_in));

    if(success==0){
        printf("Server socket is bound successfully\n");
    }else{
        printf("Binding failed\n");
    }
    
    
    int listen_result = listen(serverSocketFD,10);
    if(listen_result==0){
        printf("Listening successfully\n");
    }
    else{
        printf("Listening failed\n");
    }

    pthread_t id;
    pthread_create(&id, NULL, acceptNewConnectionAndRecieveAndPrintItsData, (void*)&serverSocketFD);
    
    pthread_join(id, NULL);
    
    
    shutdown(serverSocketFD,SHUT_RDWR);
    
    return 0;
}


struct AcceptedSocket* acceptIncomingConnection(int serverSocketFD){
    struct sockaddr_in clientAddress;
    int clientAddressSize = sizeof(struct sockaddr_in);
    int clientSocketFD = accept(serverSocketFD, (struct sockaddr*)&clientAddress, &clientAddressSize);
    struct AcceptedSocket* aS = malloc(sizeof(struct AcceptedSocket));
    aS->acceptedSocketFD = clientSocketFD;
    aS->address = clientAddress;
    aS->success = clientSocketFD>0;
    if(!aS->success){
        aS->error = clientSocketFD;
    }
    return aS;
}

void* acceptNewConnectionAndRecieveAndPrintItsData(void* arg) {
    int serverSocketFD = *(int*)arg;
    
    while (1) {
        struct AcceptedSocket* clientSocket = acceptIncomingConnection(serverSocketFD);
        if (clientSocket->acceptedSocketFD < 0) {
            printf("Accept failed\n");
            free(clientSocket);
            continue;
        }

        if (acceptedCount < 10) { 
            acceptedSockets[acceptedCount++] = *clientSocket;
        } else {
            printf("Max clients reached. Connection refused.\n");
            close(clientSocket->acceptedSocketFD);
            free(clientSocket);
            continue;
        }

        recieveAndPrintIncomingDataOnSeparateThread(clientSocket);
    }
}

void recieveAndPrintIncomingDataOnSeparateThread(struct AcceptedSocket* clientSocket) {
    pthread_t id;
    pthread_create(&id, NULL, recieveAndPrintIncomingData, (void*)clientSocket);
    pthread_detach(id); 
}

void* recieveAndPrintIncomingData(void* arg) {
    struct AcceptedSocket* clientSocket = (struct AcceptedSocket*)arg;
    char buffer[1024];

    while (1) {
        int bytes_received = recv(clientSocket->acceptedSocketFD, buffer, 1023, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            printf("Received: %s\n", buffer);
            fflush(stdout);
            sendRecievedMessageToOtherClients(clientSocket->acceptedSocketFD,buffer);
        } 
        else if (bytes_received == 0) {
            printf("Client disconnected: FD=%d\n", clientSocket->acceptedSocketFD);
            break;
        }
        else {
            perror("recv error"); 
            break;
        }
    }
    close(clientSocket->acceptedSocketFD);
    for (int i = 0; i < acceptedCount; i++) {
        if (acceptedSockets[i].acceptedSocketFD == clientSocket->acceptedSocketFD) {
            acceptedSockets[i] = acceptedSockets[acceptedCount - 1]; 
            acceptedCount--;
            break;
        }
    }
    free(clientSocket);
    return NULL;
}


void sendRecievedMessageToOtherClients(int socketFD,char *buffer){
    for(int i=0;i<acceptedCount;i++){
        if(acceptedSockets[i].acceptedSocketFD!=socketFD){
            send(acceptedSockets[i].acceptedSocketFD,buffer,strlen(buffer),0);
        }
    }
}