#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Nov 25 11:01:00 2019

@author: zoakes
"""

from socket import socket, AF_INET, SOCK_DGRAM
from time import time, ctime, sleep
import sys
import os
import time
import datetime
import simplefix

import threading
import concurrent.futures

#GLOBALS 
_DEBUG = False
HB_SIM = False #Runs continuous loop in sendHB -- (non threading version)

###############################################################################


def every(delay, task):
  '''Helper function for HB, not needed'''
  next_time = time.time() + delay
  while True:
    time.sleep(max(0, next_time - time.time()))
    try:
      task()
    except Exception:
      print('Error threading!')
    next_time += (time.time() - next_time) // delay * delay + delay

def test():
  print("Threadtest", time.time())
  
    
###############################################################################
  
class HB_Socket():
    _DEBUG
    
    def __init__(self,IP='127.0.0.1',port=4000,hb = 30):
        self.IP = IP
        self.port = port
        self.HB = hb
        self.socket = socket() #AF_INET,SOCK_DGRAM)
        self.connected = False
        
    def run(self):
        if not self.connected: self.cnct()
        now = time.time() 
        hb_tm = now + 30
        if self.connected: 
            print('Connected')
            on = self.logOn()
            self.send(on)
            print('LogOn message Sent')
        else:
            print('Error Connecting')
        while self.connected:
            
            
            if time.time() >= hb_tm:
                hb = self.heartBeat()
                self.send(hb) 
                print(f'HB: {time.time()}')
                now = time.time()
                hb_tm = now + self.HB
                
            if datetime.datetime.now().time().hour >= 16:
                off = self.logOff()
                self.send(off)
                print('LogOff message sent')
                break
            
            #Temp vars -- new addits
            long_entry = False #Eventually, make these instance vars/dict (inits)
            short_entry = False
            ticker, qty = 'AMD', 1 #Make these run args
            self.qty = 0
            
            #Entry logic
            if long_entry:
                buy = self.createOrder(ticker,1,qty) 
                self.send(buy)
                print(f'Buying {qty} share of {ticker} at {time.time()}') #and args here
                #self.qty = qty
            
            if short_entry:
                ss = self.createOrder(ticker,5,qty)
                self.send(ss)
                print(f'Selling {qty} shares of {ticker} at {time.time()}')
                #self.qty = -qty
            
            #Exit Logic 
            if self.qty != 0:
                print('Checking Stop/Target/Trail -- replace w/ real')

            
            #LogBackOn? error catching?
            
            #End New Addits
        
        print('Closed for the day...')
        self.disconnect() 
        return 0
            
        

        
    def cnct(self):
        s = self.socket
        #s.connect((self.IP,self.port))                                          
        try:  
            s.connect((self.IP,self.port))
            print('Socket connected to {} on port {}'.format(self.IP,self.port))
            
            on = self.logOn()
            self.send(on)
            print('LogOn message sent')
            
            self.connected = True
            return 1
        except:
            print('Socket creation failed: {}')
            return 0
        
    def logOn(self):
        '''create logOn msg -- returns bytes'''
        msg = simplefix.FixMessage()
        
        fix_str = '8=FIX.4.2'
        
        msg.append_pair(49,'qafix67',header=True)
        msg.append_pair(56,'IB',header=True)
        msg.append_pair(34,'30',header=True)
        
        msg.append_string(fix_str)
        msg.append_pair(35,'A')
        msg.append_pair(108,'30')
        
        msg.append_utc_timestamp(52, precision=6, header=True)
        msg.append_pair(98,0)
        
        b = msg.encode()
        return b
    
    def logOff(self):
        '''create logOff msg -- returns bytes'''
        msg = simplefix.FixMessage()
        
        fix_str = '8=FIX.4.2'
        
        msg.append_pair(49,'qafix67',header=True)
        msg.append_pair(56,'IB',header=True)
        msg.append_pair(34,'30',header=True)

        
        msg.append_string(fix_str)
        #msg.append_pair(9,'54') #duplicate
        msg.append_pair(35,5)
        msg.append_utc_timestamp(52, precision=6, header=True)
        
        b = msg.encode()
        return b
    
    def heartBeat(self):
        '''Create heartbeat msg -- returns bytes'''
        msg = simplefix.FixMessage()
        
        fix_str = '8=FIX.4.2' # 1=U01067 Moved out of header                   
        
        msg.append_string(fix_str)

        msg.append_pair(49,'qafix67',header=True)
        msg.append_pair(56,'IB',header=True)
        msg.append_pair(34,31,header=True)
        msg.append_utc_timestamp(52,precision=6,header=True)
        msg.append_pair(1,'U01067')

        msg.append_pair(35,0)
        
        
        b = msg.encode()
        return b
    
    def createOrder(self,ticker,side,qty):
        '''
        create Buy / Sell order msg in bytes
        Side: Buy = 1; SellShort = 5
        '''
        msg = simplefix.FixMessage()
        
        #fix_str = '8=FIX.4.2 1=U01067'
        msg.append_pair(8,'FIX.4.2') #These may need header=True
        msg.append_pair(1,'U01067')
        
        msg.append_pair(49,'qafix67',header=True)
        msg.append_pair(56,'IB',header=True)
        msg.append_pair(34, 31, header=True)
        msg.append_utc_timestamp(52, precision=6, header=True)
        
        msg.append_pair(35,'D')
        msg.append_pair(55,ticker)
        msg.append_pair(54,side) #1 Buy, 5 = Sellshort
        msg.append_pair(38,qty)
        
        b = msg.encode()
        
        return b
        

        
    def sendHB(self):
        '''
        Can recursively call HB, if HB_SIM True
        Otherwise, just a send w/ msg = heartBeat
        '''
        hbsocket = self.socket
        print('PyHeartBeat client sending to IP {}'.format(self.IP,self.port))
        
        hb = self.heartBeat()
        hbsocket.sendto(hb,('127.0.0.1',self.port))
        
        if HB_SIM:
            sleep(self.HB)
            if datetime.datetime.now().time().hour >= 16:                      
                self.disconnect()
                return
            self.sendHB()
         
                 
            
    def send(self,msg):
        hbsocket = self.socket
        print('FIX client sending to IP {}'.format(self.IP,self.port))
        #while 1:
        hbsocket.sendto(msg,('127.0.0.1',self.port))
        if _DEBUG:
            print('Time: {}'.format(ctime(time())))

            
    def disconnect(self):
        s = self.socket
        off = self.logOff()
        self.send(off)
        close = s.close()
        if _DEBUG: print('Closing Socket {}'.format(self.IP))
        return close
    
    @staticmethod
    def parse(buf):
        p = simplefix.FixParser()
        p.append_buffer(buf)
        m = p.get_message()
        print(m)
        return m
        
            

if __name__ == '__main__':
    HB = HB_Socket()
    
    HB_MSG = HB.heartBeat() 
    print(HB_MSG)

    HB.cnct()              
    print('Connected:',HB.connected)
    
    '''Recursion / threading tests'''
    
    #threading.Thread(target=lambda: every(30,test)).start() #Thread test
    
    #Recursive HB testing...
    #HB_SIM = True                                                              
    #HB.sendHB()       #RUNS RECURSIVELY  
    
    #threading.Thread(target=lambda: every(30,HB.sendHB),daemon=True).start()  
   
    #HB_SIM = True #Recursive on 
    #dmn = threading.Thread(target=HB.sendHB(), daemon=True)
    #dmn.start()
    
    HB_SIM = False
    #NON RECURSIVELY -- thread 
    #threading.Thread(target=every,args=(30,HB.sendHB)).start()                 
    

    '''Regular version ! -- (run)'''
    
    #HB.run()                                                                   #WORKS !! 
    #Typically, ^^ this is how it should be ran without threading.
    
    
    '''Message format testing''' 
    #WORKS ! 
    lo = HB.logOn()
    #8=FIX.4.2|9=68|35=A|49=qafix67|56=IB|34=30|52=20191219-23:11:38.836563|108=30|98=0|10=239
    
    lf = HB.logOff()
    #8=FIX.4.2|9=56|35=5|49=qafix67|56=IB|34=30|52=20191219-23:11:38.836563|10=199
    
    hbt = HB.heartBeat()
    #8=FIX.4.2|9=65|35=0|49=qafix67|56=IB|34=31|52=20191219-23:11:38.836563|1=U01067|10=133
    
    t1 = time.time_ns()
    
    buy = HB.createOrder('AMD',1,1)
    sell = HB.createOrder('AMD',5,1)

    t2 = time.time_ns()
    print('Order time in ns: {}'.format((t2-t1)))  #0? Not working?
    
    
    #Admin
    HB.parse(lo)
    HB.parse(hbt)
    HB.parse(lf)
    
    #OMS
    HB.parse(buy)
    HB.parse(sell)
    
    print('All messages work perfectly !!')
    
    
    
    
    '''** Threading **'''
    #threading.Thread(target=HB.run).start()                                  #Works -- less safe
    #Daemon version ? 
    threading.Thread(target=HB.run,daemon=True).start()                       #Works! 
    
    
    '''Changes -- per IB
    Pipes removed,
    8098 removed (added auto)
    49 header
    1 not in login (Removed from header to normal positon)
    34 header (already was, ensure in header -- check to override order)
    
    
    '''

    
    
    
    
    
    

                    