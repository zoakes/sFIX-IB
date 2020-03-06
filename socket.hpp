//
//  socket.hpp
//  sFIX
//
//  Created by Zachary Oakes on 1/8/20.
//  Copyright © 2020 Zachary Oakes. All rights reserved.
//

#ifndef socket_hpp
#define socket_hpp

#include <stdio.h>
#include <iostream>

#include <netdb.h>
#include <iomanip>
#include <ctype.h>
#include <fstream>


#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

//MACROS !!
#define PORT 4004
#define ADDR "127.0.0.1"


using namespace std;

class Socket{
private:
    int sockfd, connfd, res, out;
    struct sockaddr_in servaddr;
    char buffer[1024];
    
    //int port = 8004;
    //string IP = "127.0.0.1"; Created macros instead 
    
    friend class sFix;
    
public:
    int sock = 0;
    //Socket *sock_ptr;
    
    
    //https://www.google.com
    Socket(const char *addr = "www.google.com", int port = 4000){
        
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr(addr);
        servaddr.sin_port = htons(port);
        
        sockfd = socket(AF_INET,SOCK_STREAM,0);
        if(sockfd == -1){
            cout << "Socket creation failed";
            exit(0);
        }
        else
            cout << "Socket created succesfully!";
        
        //Clear buffer + Connect to socket !
        bzero(&servaddr,sizeof(servaddr)); //Clear the buffer
        
        res = connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));

        cout << "Connecting..." << endl;
        if(res != 0){
            cout << "Connected ! " << endl;
            //exit(1); //Do I want this here? It cuts the program... or just the class? no deconstruct...
        }
        else if(out == 0){
            cout << "Not Connected.. please try again." << endl;
        }
        cout << "Out : " << res << endl;
    }
    
    //WHY is the destructor being called while still in scope? Obviously RUN is still going...?
    //Maybe initialize in constructor of FIX?
    ~Socket(){
        cout << "Disconnecting ... " << endl;
        close(sockfd);
        //auto out = disconnect(); Maybe include close(sockfd) in disconnect (+ logOff) etc.
        cout << " / Deconstructing Socket -- Out: " << res;
    }
    
    //test function
    void print_shit(int x); //Practicing splitting between two sheets!
    
    int cnct(string ip);
    //int connect(string addr); -- THERES A BUILTIN -- so don't use this name!
    int disconnect();
    int send_str(string msg); //Not functional !
    int send_ptr(const char * const& msg);
    
    //Helper function
    int force_connect();
    int check_connect();
    
    
};



#endif /* socket_hpp */
