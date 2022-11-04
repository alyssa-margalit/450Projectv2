#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
using namespace std;
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

//Server side
int main()
{
    startTCP();
    while(1)
    {
        if(!auth){
            //receive a message from the client (listen)
            cout << "Awaiting client response..." << endl;
            memset(&msg, 0, sizeof(msg));//clear the buffer
            bytesRead = recv(newSd, (char*)&msg, sizeof(msg), 0);
            cout << "The main server received the authentication for: " << msg << endl;
            string data;
            data = "0";
            memset(&msg, 0, sizeof(msg)); //clear the buffer
            strcpy(msg, data.c_str());
            //send the message to client
            bytesWritten = send(newSd, (char*)&msg, strlen(msg), 0);

        }else{
            cout<<"query time"<<endl;
            string data = "query stuff";
            strcpy(msg, data.c_str());
            bytesWritten = send(newSd, (char*)&msg, strlen(msg), 0);
        }
    }

    cout << "Connection closed..." << endl;
    return 0;   
}