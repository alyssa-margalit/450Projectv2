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
#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

//------------------------------Set up UDP--------------------  
#define PORTC     21555 
#define PORTM      24555
#define MAXLINE 1024

struct sockaddr_in     serverMaddr;
int sockfd; 
char buffer[MAXLINE]; 
const char *hello = "Hello from C SERVER"; 
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

    // Define address of serverC
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORTC);

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
//___________________________________________________________________
vector<string> credentials1;
vector<string> username;
vector<string> password;
int size;
void getCredentials(){
    ifstream in("cred.txt");
    string temp;
    while(getline(in,temp)){
        if(temp.size()>0){
            credentials1.push_back(temp);
        }
    }
    for(size_t i = 0; i<credentials1.size();i++){
        cout<<credentials1.at(i)<<endl;
    }
}
void splitCredentials(){
    for(size_t i = 0; i<credentials1.size();i++){
        vector<string> split;
        string temp = credentials1.at(i);
        // cout<<"temp: "<<temp<<endl;
        stringstream x(temp);
        while(x.good()){
            string y;
            getline(x,y,',');
            split.push_back(y);
        }
        // cout<<"username"<<i<<": "<<split.at(0)<<endl;
        // cout<<"password"<<i<<": "<<split.at(1)<<endl;
        username.push_back(split.at(0));
        password.push_back(split.at(1));
    }
}

// Driver code 
int main() { 
    getCredentials();
    splitCredentials();
    startUDP();
      
    int n; 
    socklen_t len;
    
    len = sizeof(serverMaddr);  //len is value/result 
    while(1){
        memset(&serverMaddr, 0, sizeof(serverMaddr));
        n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
            MSG_WAITALL, ( struct sockaddr *) &serverMaddr, 
            &len);   
    }
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