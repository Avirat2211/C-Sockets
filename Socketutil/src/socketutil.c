#include "socketutil.h"

int createTCPIpv4Socket(){
    return socket(AF_INET,SOCK_STREAM,0);// returns socket file descriptor
    //AF_INET = address family inet == IPV4
    //SOCK_STREAM = requesting for sockets
    // 0 = protocol layer
}


struct sockaddr_in* createIpv4Address(char *ip,int port){
    struct sockaddr_in *address = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
    if (address == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    address->sin_port=htons(port);
    address->sin_family=AF_INET;
    if(strlen(ip)==0){
        address->sin_addr.s_addr=INADDR_ANY;
    }
    else{
        inet_pton(AF_INET,ip,&address->sin_addr.s_addr);
    }
    return address;
}