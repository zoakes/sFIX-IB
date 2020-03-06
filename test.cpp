//
//  test.cpp
//  sFIX
//
//  Created by Zachary Oakes on 1/5/20.
//  Copyright © 2020 Zachary Oakes. All rights reserved.
//

#include "test.hpp"
#include "socket.hpp"


//extern void run();
    //cout << "Test"; //Don't know if needed?

//extern int test_func();
//extern int test(); //This still plays ? , don't know the point.

/*  FIX CLASS FUNCTIONS !! */

/*
FIX::FIX(){
    cout << "Instance created" << endl;
    //Socket ss;
    
}*/

void FIX::run(int BOD, int EOD){
    auto ss = s_ptr;
    time_t now = time(0);
    time_t hb_tm = time(0) + 30;
    time_t hr = get_time('H');
    if(Connected == false){
        ss->cnct("127.0.0.1"); //Connect socket,
        fix_cnct(); //Connect FIX
    }
    else{
        cout << "Socket Connected: " << Connected << endl;
    }
    if(BOD < hr < EOD){
        cout << "Market open for day" << endl;
    }
    while(1){
        if(hr < BOD){
            cout << "Waiting for open..." << endl;
        }
        else if(hr > EOD){
            cout << "Market closing for session" << endl;
            break;

        }
        
        if(time(0) >= hb_tm){
            cout << "HB: " << time(0) << endl;
            string hb = heartBeat();
            ss->send_str(hb);
            now = time(0);
            hb_tm = now + HB;
        }
        
        //Log Back on ?
        
        //Entries, Exits, etc logic...
        
    }
    cout << "Trading Session Over, disconnecting..." << endl;
    fix_disconnect();
    ss->disconnect();
}

int FIX::fix_cnct(){
    auto ss = s_ptr;
    cout << "Connecting ... " << endl;
    string on = make_logOn();
    int out = ss->send_str(on);
    Connected = true;
    return out;
}

int FIX::fix_disconnect(){
    //Socket ss;
    auto ss = s_ptr;
    cout << "Disconnecting" << endl;
    string off = make_logOff();
    int out = ss->send_str(off);
    Connected = false;
    return out;
}



unsigned long new_func(vector<int> arr){
    //auto L,H = 0, arr.size();
    auto LOW = 0;
    auto HIGH = arr.size();
    return HIGH + LOW;
    //Socket ss;
    //ss.print_shit(10);
}

int FIX::test(){
    cout << "Testing -- 1 -- 2 ";
    return 0;
}


 

int FIX::test_function(){
    cout << "Testing";
    return 0;
}

/*
 Version for testing, prior to implementation of Socket 
 void FIX::run(int BOD, int EOD){
     //Socket ss;
     time_t now = time(0);
     time_t hb_tm = time(0) + 30;
     time_t hr = get_time('H');
     if(Connected == false){
         //ss.cnct("127.0.0.1"); //Connect socket,
         fix_cnct(); //Connect FIX
     }
     else{
         cout << "Socket Connected: " << Connected << endl;
     }
     if(BOD < hr < EOD){
         cout << "Market open for day" << endl;
     }
     while(1){
         if(hr < BOD){
             cout << "Waiting for open..." << endl;
         }
         else if(hr > EOD){
             cout << "Market closing for session" << endl;
             break;

         }
         
         if(time(0) >= hb_tm){
             cout << "HB: " << time(0) << endl;
             string hb = heartBeat();
             //ss.send_str(hb);
             send(hb);
             now = time(0);
             hb_tm = now + HB;
         }
         
         //Log Back on ?
         
         //Entries, Exits, etc logic...
         
     }
     cout << "Trading Session Over, disconnecting..." << endl;
     fix_disconnect();
     //ss.disconnect();
 }
     
 };
 static string getTS();
time_t get_time(int H_M_S);
void test_switch();
string heartBeat();
string make_logOn();
string make_lofOff();
*/
