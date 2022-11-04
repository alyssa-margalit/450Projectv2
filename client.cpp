//communicates with serverM over tcp with dynamic local port assignment
//input a username and password, send to serverM @ tcp port 25555
//5 to 50 characters
//3 attempts to get the password right

//phase 2
//enter course code to query
//enter category
//send request to tcp server

//serverC serves on UDP port 21555
//serverCS serves on UDP port 22555
//serverEE serves on UDP port 23555
//serverM serves on UDP port 24555 and TCP port 25555
//Client serves on nothing, client for TCP port 2555


//to run
// g++ client.cpp -o client
// ./client

//send over tcp to port 25555

#include <iostream>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <bits/stdc++.h>

#define PORT 25556
using namespace std;

int debug = 1;
bool auth = false;
int tries = 1;
char total[101];
char username[50];
char password[50];

int sock = 0, valread, client_fd;
struct sockaddr_in serv_addr;
const char* hello = "0,Hello from client";
char buffer[1024] = { 0 };
void inputCreds(){
        cout<<"Please enter username: ";
        cin.getline(username,50);
        cout<<"Please enter password: ";
        cin.getline(password,50); 
        cout<<"username: "<<username<<endl;
        
        cout<<"password: "<<password<<endl;

        strcpy(total,username);
        strcat(total,",");
        strcat(total,password);
        cout<<"total "<<total<<endl;

        send(sock,total,strlen(total),0);
        cout<<username<<" sent an autherntication request to the main server"<<endl;
        
        
}

char courseCode[30];
char category[30];
char totalQuery[60];

void inputCourseQuery(){
    cout<<"please enter course code to query: "<<endl;
    cin.getline(courseCode,30);
    cout<<"please enter category (Credit/Professor/Days/CourseName):"<<endl;
    cin.getline(category,30);
    strcpy(totalQuery,courseCode);
    strcat(totalQuery,",");
    strcat(totalQuery,category);
    send(sock,totalQuery,strlen(totalQuery),0);

}
int startClient(){

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
 
    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }
 
    if ((client_fd
         = connect(sock, (struct sockaddr*)&serv_addr,
                   sizeof(serv_addr)))
        < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    return 0;
}


int main(){
    int x = startClient();
    if(x){
        cout<<"socket failed"<<endl;
    }else{
        cout<<"The client is up and running"<<endl;
    }
    char msg[1500];
    int n;
    string msgS;
    while(1){
        memset(&msg, 0, sizeof(msg));//clear the buffer
        if(!auth){
            if(tries<=3){
                inputCreds();
                tries++;

            }else{
                cout<<"Authentication Failed for 3 attempts. Client will shut down."<<endl;
                close(client_fd);
                return 0;
            }
            n = recv(sock, (char*)&msg, sizeof(msg), 0);
            msgS = msg;
            if(msgS=="0"){
                cout<<username<<"received the result of authentication using TCP over port "<<PORT<<" Authentication successful"<<endl;
                auth = true;
            }else if(msgS=="1"){
                cout<<username<<"received the result of authentication using TCP over port "<<PORT<<" Authentication failed: Username Does not exist"<<endl<<endl;
                cout<<"Attempts remaining: "<<3-tries<<endl;           
            }else if(msgS=="2"){
                cout<<username<<"received the result of authentication using TCP over port "<<PORT<<" Authentication failed: Password does not match"<<endl<<endl;
                cout<<"Attempts remaining: "<<3-tries<<endl;           
            }else{
                cout<<"something weird"<<endl;
            }
        }else{
            inputCourseQuery();
            n = recv(sock, (char*)&msg, sizeof(msg), 0);

        }
}


}
