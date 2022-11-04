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
    
#include <iostream>
#include <string>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
using namespace std;



//-----------------------UDP SETUP--------------------------
#define PORTC     21555 
#define PORTCS    22555
#define PORTEE    23555
#define PORTM     24555
#define MAXLINE 1024 

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

bool courseCode = false;

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
//--------------------------TCP SETUP---------------------------------
#define TCPPORT 25555

bool auth = false;
//buffer to send and receive messages with
char msg[1500];
sockaddr_in servAddr, newSockAddr;
int serverSd;
int bindStatus;
int newSd;
socklen_t newSockAddrSize;
//also keep track of the amount of data sent as well
int bytesRead, bytesWritten = 0;


void startTCP(){
    cout<<"starting TCP"<<endl;
    //clear space in memory and define address
    memset((char*)&servAddr, 0,sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(TCPPORT);
    serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSd < 0)
    {
        cerr << "Error establishing the server socket" << endl;
        exit(0);
    }
    //bind the socket to its local address
    bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, 
        sizeof(servAddr));
    if(bindStatus < 0)
    {
        cerr << "Error binding socket to local address" << endl;
        exit(0);
    }
    cout << "Waiting for a client to connect..." << endl;
    listen(serverSd, 5);
    newSockAddrSize = sizeof(newSockAddr);
    //accept, create a new socket descriptor to 
    //handle the new connection with client
    newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
    if(newSd < 0)
    {
        cerr << "Error accepting request from client!" << endl;
        exit(1);
    }
    cout << "The main server is up and running" << endl;
}
//________________________________________________________________________
//-----------------------------------process credentials----------------------------
char substr;
char unencrypted[102];
char encrypted[102];

void encrypt(char* message,char* outMessage){
   // cout<<"original message: "<<message<<endl;
   cout<<"ABOUT TO ENCRYPT"<<endl;
    for(int i = 0; i<(unsigned)strlen(message);i++){
        substr = message[i];
        if(isalpha(substr)){
            int asciiVal = int(substr);
            if(asciiVal>=97){
                //is lowercase
                message[i]-=97;
                message[i]+=4;
                message[i]=message[i]%26;
                message[i]+=97;
            }else{
                //is uppercase
                message[i]-=65;
                message[i]+=4;
                message[i]=message[i]%26;
                message[i]+=65;
            }
        }else if(isdigit(substr)){
            //is digit
            message[i]-=48;
            message[i]+=4;
            message[i]=message[i]%10;
            message[i]+=48;
        }
     }
     cout<<"encryped value: "<< message<<endl;
    // for(int i = 2; i<(unsigned)strlen(message);i++){
    //     outMessage[i-2]=message[i];
    //     //cout<<message[i]<<endl;
    // }
    //w,x,y, and z should wrap
    // //6,7,8,9 should wrap
    // char x = 'W';
    // int y = int(x);
    // x= x+4-65;
    // cout<<"x+4-65 is "<<int(x)<<endl;
    // x=x%26;
    // cout<<"x mod 26 is "<<int(x)<<endl;
    // x+=65;
    // cout<<"new x is "<<int(x)<<endl;
}
//____________________________________________________________________





// Driver code 
int main() { 
    startUDPClient();
    startTCP();
    while(1){
        //if still waiting for password
        if(!auth){
            //recieving authentication request from client
            cout<<"waiting for authentication request";
            memset(&msg, 0, sizeof(msg));//clear the buffer
            bytesRead = recv(newSd, (char*)&msg, sizeof(msg), 0);
            cout << "The main server received the authentication for: " << msg << endl;
            encrypt(msg,encrypted);

        }else{//if password has been received and authenticated
            if(!courseCode){//if still waiting for course code query

            }else{//if got course code and waiting for category request

            }

        }
    }


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




//close udp socket
    close(sockfd); 
    return 0; 
}