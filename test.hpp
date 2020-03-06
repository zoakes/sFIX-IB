//
//  test.hpp
//  sFIX
//
//  Created by Zachary Oakes on 1/5/20.
//  Copyright © 2020 Zachary Oakes. All rights reserved.
//

#ifndef test_hpp
#define test_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <list>
#include <vector>

//MACROS !!

using namespace std;

#define DBUG true

#include "socket.hpp"



class FIX: private Socket{
private:
    int HB = 30;
    FIX *cls_ptr;
    Socket *s_ptr;
    

    friend class Socket;
    //Socket *s_ptr;
protected:
    list<string> logOn = {};
    list<string> logOff = {};
    list<string> hbMsg = {};
    list<string>::iterator i;
    bool Connected = false;
    vector<char> bits = {};
    
public:
    
    FIX(){
        cout << "Instance created" << endl;
        Socket ss;
        //Socket::sock_ptr = &ss;
        s_ptr = &ss;
        
    }
    
    ~FIX(){
        cout << "Fix instance destructed" << endl;
    }
    
    //static string getTS();
    //time_t get_time(int H_M_S);
    //string heartBeat();
    //string make_logOn();
    //string make_logOff();
    /*logOn ={"8=","FIX.4.2"," 9=","65"," 35=",msgType," 49=",sendCompID," 56=",tgtCompID," 34=", "30"," 52=",timestmp," 1=",acct," 108=",hbInt," 10=","239"};*/
    //string make_order();
    //int cnct();
    //int disconnect(); //include logoff?
    //int send(string msg);
    
    int test_function();
    int test();
    
    //friend class Socket; WHY IS FRIEND NOT WORKING? 
    //friend int Socket::send_str(string msg);
    //friend int Socket::send_ptr(const char * const& msg);

    /*DEFINED IN CPP FILE ! Utilize Socket class...*/
    void run(int BOD = 8, int EOD = 15);
    int fix_cnct();
    int fix_disconnect();
    //Uses Socket -- thus must be in c++ file ! 

    
    static vector<char> string_to_bytes(string msg){
        std::vector<char> bytes(msg.begin(), msg.end());
        return bytes;
    }
    
    static int count_bytes(string msg){
        auto v = FIX::string_to_bytes(msg); //THIS IS HOW YOU CALL FUCKING STATICS INSIDE CLASS!
        int res = sizeof(v);
        if(res)
            return res;
        return 0;
    }
    
    static unsigned long len_bytes(string msg){
        auto v = FIX::string_to_bytes(msg);
        auto res = v.size();
        if(res) // != NULL
            return res;
        return 0;
    }
    
    
    time_t get_time(int H_M_S){
        time_t now = time(0);
        tm *ltm = localtime(&now);
        
        time_t res;
        switch(H_M_S){
            case 'H':
                res = (ltm->tm_hour);
                break;
            case 'M':
                res = (ltm->tm_min);
                break;
            case 'S':
                res = (ltm->tm_sec);
                break;
            default:
                res = (ltm->tm_hour);
                cout << "Error: Must use 'H', 'M' or 'S' as input" << endl;
        }
        return res;
    }
    
    string getTS(){
      time_t now = time(0);
      tm *ltm = localtime(&now);
      auto y = to_string(1900 + ltm->tm_year);
      auto m = to_string(ltm->tm_mon);
      auto d =  to_string(ltm->tm_mday);
      auto h = to_string(ltm->tm_hour);
      auto mn = to_string(ltm->tm_min);
      auto s = to_string(ltm->tm_sec);
      string time = y +  m  + d + "-" + h  + ":" + mn + ":" + s;
      return time;
    }
    
    string make_logOn(){
        string msgType = "A";
        string sendCompID = "qafix67";
        string tgtCompID = "IB";
        string timestmp = getTS();
        string hbInt = to_string(HB);
        
        string MSG = "";
        logOn = {"8=","FIX.4.2"," 9=","65"," 35=",msgType," 49=",sendCompID," 56=",tgtCompID," 34=", "30"," 52=",timestmp," 108=",hbInt," 10=","239"};
        for(auto m : logOn){
            //cout << m;
            MSG += m;
        }
        //std::stringstream buffer;
        //buffer << MSG << endl;
        return MSG;
    }
    
    string make_msg(string msgType){
        string sendCompID = "qafix67";
        string tgtCompID = "IB";
        string timestmp = getTS();
        string hbInt = to_string(HB);
        
        string MSG = "";
        logOn = {"8=","FIX.4.2"," 9=","65"," 35=",msgType," 49=",sendCompID," 56=",tgtCompID," 34=", "30"," 52=",timestmp," 108=",hbInt," 10=","239"};
        for(auto m : logOn){
            if(DBUG) {cout << m;}
            MSG += m;
        }
        //std::stringstream buffer;
        //buffer << MSG << endl;
        return MSG;
    }
    

    
    string make_logOff(){
        auto MSG = make_msg("5");
        return MSG;
    }
    
    
    string heartBeat(){
        string msgType = "0";
        string acct = "U01067";
        string sendCompID = "qafix67";
        string tgtCompID = "IB";
        string timestmp = getTS();
        string hbInt = to_string(HB);
        
        string MSG = "";
        logOn ={"8=","FIX.4.2"," 9=","65"," 35=",msgType," 49=",sendCompID," 56=",tgtCompID," 34=", "30"," 52=",timestmp," 1=",acct," 108=",hbInt," 10=","239"};
        
        for(auto m : logOn){
            MSG += m;
        }
        return MSG;
    }
    
    //TODO: Create order logic ! (+ Connect, Send, Disconnect! -- ALL socket shit!)
    string make_order(){
        cout << "Order Created!";
        return "0";
    }
    

    

    //Temporary / Filler method, until Socket send is built (and to test!)
    int send(string msg){
        //Convert to bytes
        cout << "Sending" << endl;
        if(DBUG){
            cout << msg << endl;
        }
        return 0;
    }
    

    
    
    
    
    list<string> fix_msg(string msgType){
        //Returns a list ! (can be swapped out -- if need to customize
        string acct = "U01067";
        string sendCompID = "qafix67";
        string tgtCompID = "IB";
        string timestmp = getTS();
        string hbInt = to_string(HB);
        
        string MSG = "";
        logOn ={"8=","FIX.4.2"," 9=","65"," 35=",msgType," 49=",sendCompID," 56=",tgtCompID," 34=", "30"," 52=",timestmp," 1=",acct," 108=",hbInt," 10=","239"};

        
        return logOn;
        
    }
    
    string hb(){
        auto msg = fix_msg("0");
        string MSG = "";

        for(auto m : msg){
            MSG = MSG + m;
        }
        return MSG;
    }

    
    
    
};




/*
string FIX::test_func(){
    cout << "Testing proper format of C++ .h / .cpp files" << endl;
    if(DBUG){
        return 0;
    }
    else{
        return 1;
    }
    
}*/


//NON CLASS DECLARATIONS



#endif /* test_hpp */
