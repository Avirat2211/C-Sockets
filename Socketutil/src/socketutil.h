#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include <unistd.h> 

int createTCPIpv4Socket();
struct sockaddr_in* createIpv4Address(char *ip,int port);