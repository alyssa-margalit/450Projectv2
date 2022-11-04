//tcp server for 25555
//udp server for 24555
//udp client for 21555,22555,23555



// Client side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
    
#define PORTC     21555 
#define PORTCS    22555
#define PORTEE    23555
#define PORTM     24555
#define MAXLINE 1024 
//-----------------------UDP SETUP--------------------------
int sockfd; 
char buffer[MAXLINE]; 
char bufferC[MAXLINE];
char bufferCS[MAXLINE];
char bufferEE[MAXLINE];
const char *hello = "Hello from CLIENT M"; 
struct sockaddr_in     serverCaddr; 
struct sockaddr_in     serverCSaddr; 
struct sockaddr_in     serverEEaddr;
struct sockaddr_in     serverMaddr;

int n;
int CSMessage;
int EEMessage;
socklen_t clientLen; 

void startUDPClient(){
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
    memset(&serverCaddr, 0, sizeof(serverCaddr)); 
    // Filling server information 
    serverCaddr.sin_family = AF_INET; 
    serverCaddr.sin_port = htons(PORTC); 
    serverCaddr.sin_addr.s_addr = INADDR_ANY; 

    memset(&serverCSaddr, 0, sizeof(serverCSaddr)); 
    // Filling server information 
    serverCSaddr.sin_family = AF_INET; 
    serverCSaddr.sin_port = htons(PORTCS); 
    serverCSaddr.sin_addr.s_addr = INADDR_ANY;     

    memset(&serverEEaddr, 0, sizeof(serverEEaddr)); 
    // Filling server information 
    serverEEaddr.sin_family = AF_INET; 
    serverEEaddr.sin_port = htons(PORTEE); 
    serverEEaddr.sin_addr.s_addr = INADDR_ANY;  

    memset(&serverMaddr, 0, sizeof(serverMaddr)); 
    // Filling server information 
    serverMaddr.sin_family = AF_INET; 
    serverMaddr.sin_port = htons(PORTM); 
    serverMaddr.sin_addr.s_addr = INADDR_ANY;

    if ( bind(sockfd, (const struct sockaddr *)&serverMaddr,  
            sizeof(serverMaddr)) < 0 ) 
    { 

        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
}
//__________________________________________________________________________

// Driver code 
int main() { 
    startUDPClient();


    sendto(sockfd, (const char *)hello, strlen(hello), 
        MSG_CONFIRM, (const struct sockaddr *) &serverCaddr,  
            sizeof(serverCaddr)); 
    sendto(sockfd, (const char *)hello, strlen(hello), 
        MSG_CONFIRM, (const struct sockaddr *) &serverCSaddr,  
            sizeof(serverCSaddr));
    sendto(sockfd, (const char *)hello, strlen(hello), 
        MSG_CONFIRM, (const struct sockaddr *) &serverEEaddr,  
            sizeof(serverEEaddr));
    printf("Hello message sent.\n"); 
            
    CSMessage = recvfrom(sockfd, (char *)bufferCS, MAXLINE,  
                MSG_WAITALL, (struct sockaddr *) &serverCSaddr, 
                &clientLen); 

    bufferCS[CSMessage] = '\0'; 
    printf("Server : %s\n", bufferCS);

    EEMessage = recvfrom(sockfd, (char *)bufferEE, MAXLINE,  
            MSG_WAITALL, (struct sockaddr *) &serverEEaddr, 
            &clientLen); 

    bufferEE[EEMessage] = '\0'; 
    printf("Server : %s\n", bufferEE);




    close(sockfd); 
    return 0; 
}