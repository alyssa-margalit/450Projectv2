
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
    
#define PORTEE     23555 
#define PORTM      24555
#define MAXLINE 1024

struct sockaddr_in     serverMaddr;




// Driver code 
int main() { 
    int sockfd; 
    char buffer[MAXLINE]; 
    const char *hello = "Hello from EE SERVER"; 
    struct sockaddr_in servaddr, cliaddr; 
        
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
        
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
        
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORTEE);


    memset(&serverMaddr, 0, sizeof(serverMaddr)); 
    // Filling server information 
    serverMaddr.sin_family = AF_INET; 
    serverMaddr.sin_port = htons(PORTM); 
    serverMaddr.sin_addr.s_addr = INADDR_ANY;

        
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 

        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
        
    int n; 
    socklen_t len;
    
    len = sizeof(cliaddr);  //len is value/result 
    
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
                &len); 
    buffer[n] = '\0'; 
    printf("Client : %s\n", buffer); 
    sendto(sockfd, (const char *)hello, strlen(hello),  
        MSG_CONFIRM, (const struct sockaddr *) &serverMaddr, 
            len); 
    printf("Hello message sent.\n");  
        
    return 0; 
}

////////////////
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
#define MAXLINE 1024 

int sockfd; 
char buffer[MAXLINE]; 
const char *hello = "Hello from CLIENT M"; 
struct sockaddr_in     serverCaddr; 
struct sockaddr_in     serverCSaddr; 
struct sockaddr_in     serverEEaddr;
int n;
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
}

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
            
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                MSG_WAITALL, (struct sockaddr *) &serverEEaddr, 
                &clientLen); 
    buffer[n] = '\0'; 
    printf("Server : %s\n", buffer);
    close(sockfd); 
    return 0; 
}