//
//  socket.cpp
//  sFIX
//
//  Created by Zachary Oakes on 1/8/20.
//  Copyright © 2020 Zachary Oakes. All rights reserved.
//

#include "test.hpp"
#include "socket.hpp"

//#include <stdio.h> //Don't think I need these in here ! (JUST the header import ! 
//#include <iostream>

using namespace std;
//why does no std shit work?



void Socket::print_shit(int x){
    cout << x << endl;
}


/*Created 2 Connect functions -- just to test different ways of doing it*/

int Socket::cnct(string ip = ADDR){
    cout << "Connected" << endl;
    return out;
}


int Socket::disconnect(){
    cout << "Disconnected" << endl;
    return 0;
}

int Socket::send_str(string msg){
    cout << "Sending " << msg << endl;
    //res = send(sockfd,msg,msg.size(),0); //THIS won't work, need ptr !
    return res;
}

int Socket::send_ptr(const char * const& msg){
    const char* msgFmt = msg;
    res = send(sockfd, msgFmt, strlen(msg), 0);
    if(res != 0){
        cout << "Sent :" << msg;
        return 1;
    }
    return 0;
    
}



int Socket::force_connect(){
    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != -1) //MAKE SURE THIS IS CORRECT (0 FOR SUCCESS)
        cout << "connection failed ..." << endl;
    else
        cout << "connection success !" << endl;
    return res;
}

int Socket::check_connect(){
    out = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    return out;
}


/*There's a builtin connect -- DO NOT USE METHOD NAME
int Socket::connect(string ip = ADDR){
    cout << "Connected" << endl;
    return out;
}
 */
