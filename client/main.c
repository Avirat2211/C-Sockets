#include "../Socketutil/src/socketutil.h"
#include<string.h>
#include<pthread.h>


void* listenAndPrintMessages(void* arg);
void startListeningAndPrintMessagesOnNewThread(int socketFD);
void readConsoleEntriesAndSendToServer(int socketFD);


int main(){
    int socketFD = createTCPIpv4Socket();
    printf("OK\n");
    if(socketFD==-1){
        printf("Errors in socket file desriptor");
        return -1;
    }
    struct sockaddr_in* address = createIpv4Address( "127.0.0.1" ,2000);
    int success = connect(socketFD,(struct sockaddr*)address,sizeof(*address));
    if (success == 0) {
        printf("Connection successful\n");
    } else {
        printf("Connection failed\n");
        free(address);  
        close(socketFD);
        return -1;
    }
    startListeningAndPrintMessagesOnNewThread(socketFD);
    readConsoleEntriesAndSendToServer(socketFD);
    close(socketFD);
    return 0;
}

void readConsoleEntriesAndSendToServer(int socketFD) {
    char *name = NULL;
    size_t nameSize= 0;
    printf("please enter your name?\n");
    ssize_t  nameCount = getline(&name,&nameSize,stdin);
    name[nameCount-1]=0;


    char *line = NULL;
    size_t lineSize= 0;
    printf("type and we will send(type exit)...\n");


    char buffer[1024];

    while(1)
    {
        ssize_t  charCount = getline(&line,&lineSize,stdin);
        line[charCount-1]=0;
        sprintf(buffer,"%s:%s",name,line);
        if(charCount>0)
        {
            if(strcmp(line,"exit")==0)
                break;
            ssize_t amountWasSent =  send(socketFD,buffer,strlen(buffer), 0);
        }
    }
}

void startListeningAndPrintMessagesOnNewThread(int socketFD){
    pthread_t id;
    pthread_create(&id, NULL, listenAndPrintMessages, (void*)&socketFD);
    pthread_detach(id); // letting the thread run independently
    
}

void* listenAndPrintMessages(void *arg){
    int socketFD = *(int *)arg;
    char buffer[1024];

    while (1) {
        int bytes_received = recv(socketFD, buffer, 1023, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            printf("%s\n", buffer);
        } 
        if (bytes_received <= 0) {
            break;
        }
    }
    close(socketFD);
    return NULL;
}