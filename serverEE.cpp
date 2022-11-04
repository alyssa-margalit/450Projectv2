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


//----------------------UDP SETUP-------------------------  
#define PORTEE     23555 
#define PORTM      24555
#define MAXLINE 1024

using namespace::std;

struct sockaddr_in     serverMaddr;
int sockfd; 
char buffer[MAXLINE]; 
const char *hello = "Hello from EE SERVER"; 
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
    servaddr.sin_port = htons(PORTEE);

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
//______________________________________________________
//---------------read text file------------------------
int size;

vector<string> classes;
vector<string> courseNumber;
vector<string> credit;
vector<string> professor;
vector<string> days;
vector<string> courseName;
vector<string> temp2;
void getClasses(){
    ifstream in("ee.txt");
    string temp;
    while(getline(in,temp)){
        if(temp.size()>0){
            classes.push_back(temp);
            stringstream ss(temp);
            while(ss.good()){
                string substr;
                getline(ss,substr,',');
                temp2.push_back(substr);
            }
            courseNumber.push_back(temp2.at(0));
            credit.push_back(temp2.at(1));
            professor.push_back(temp2.at(2));
            days.push_back(temp2.at(3));
            courseName.push_back(temp2.at(4));
            temp2.clear();
        }
    }

    //   for(size_t i = 0; i<courseNumber.size();i++){
    //       cout<<courseNumber.at(i)<<endl;
    //   }
}
string courseQuery = "EE520";
string tempCourse;

void checkMatch(){
     for(size_t i = 0; i<classes.size();i++){
        tempCourse = courseNumber.at(i);
        size_t index = tempCourse.find(courseQuery);
        cout<<tempCourse<<endl;
        if(index != string::npos){
            cout<<classes.at(i)<<endl;
        }
        
     }  
}
//___________________________________________________



// Driver code 
int main() { 
    startUDP(); 
    getClasses();  
    checkMatch();   
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