//udp server for 23555
//udp client for 24555

// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
    
#define PORTCS     22555 
#define PORTM      24555
#define MAXLINE 1024

struct sockaddr_in     serverMaddr;
int sockfd; 
char buffer[MAXLINE]; 
const char *hello = "Hello from CS SERVER"; 
struct sockaddr_in servaddr, cliaddr; 

void startUDP(){
    printf("hello");
    // Create socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
    //make room for incoming and outgoing messages
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 

    // Define address of serverEE
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORTCS);

    //make room for messages about serverM
    memset(&serverMaddr, 0, sizeof(serverMaddr)); 
    // Define address of serverM
    serverMaddr.sin_family = AF_INET; 
    serverMaddr.sin_port = htons(PORTM); 
    serverMaddr.sin_addr.s_addr = INADDR_ANY;


    // Bind socket file descriptor to serverEE port 23555
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 

        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
}

// Driver code 
int main() { 
    startUDP();      
    int n; 
    socklen_t len;
    
    len = sizeof(serverMaddr);  //len is value/result 
    
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                MSG_WAITALL, ( struct sockaddr *) &serverMaddr, 
                &len); 
    buffer[n] = '\0'; 
    printf("Client : %s\n", buffer); 
    sendto(sockfd, (const char *)hello, strlen(hello),  
        MSG_CONFIRM, (const struct sockaddr *) &serverMaddr, 
            len); 
    printf("Hello message sent.\n");  
        
    return 0; 
}