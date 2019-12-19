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


_DEBUG = False
HB_SIM = False #Runs continuous loop in sendHB -- (non threading version)

#GLOBALS 
ID = 0
newID = ID + 1
symbol = 'AMD'
qty = 10
msg = 'HB'


if msg == 'HB':
    msgType = '0'
    symbol = '0'
    side = '0'
    qty = '0'
    checkSum = '76'
    
elif msg == 'BUY':
    msgType = 'D'
    symbol = symbol
    side = '1'
    qty = qty
    checkSum = '76'

elif msg == 'SELL':
    msgType = 'D'
    symbol = symbol
    side = '5'
    qty = qty
    checkSum = '76'
    
elif msg == 'ON':
    msgType = 'A'
    
    
else:
    print('Error -- msg must be HB, BUY SELL or EOD')
    
fix_list = ['8=','FIX.4.2','|9=','51','|35=',msgType,'|1=','U01067','|56=','IB','|11=',str(newID),'|55=',
            symbol,'|54=',side,'|38=',str(qty),'|40=','1','|204=0','|10=',checkSum]

FIX_MSG = ("".join(fix_list)).encode()

#THIS IS WHERE THE BUG IS, ^^^ IN THIS MESS W/ JOIN -- THAT HB THROWS ERROR.

def every(delay, task):
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
                #self.sendHB()
                hb = self.heartBeat()
                #self.send(hb) #THIS is whats disconnecting me !
                #self.send(on) #Works with ON!
                self.send(hb) #Try with HB! WORKS w/ CHANGED heartBeat() (to match logOn)
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
        self.disconnect() #Remove temporarily, just to make sure not catching.
        return 0
            
        

        
    def cnct(self):
        s = self.socket
        #s.connect((self.IP,self.port))                                         #MOVED THIS 
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
        '''If ISSUE STILL PRESENT (|'s) -- append_pair for ALL'''
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
        '''If ISSUE STILL PRESENT (|'s) -- append_pair for ALL'''
        msg = simplefix.FixMessage()
        
        fix_str = '8=FIX.4.2'
        
        msg.append_pair(49,'qafix67',header=True)
        msg.append_pair(56,'IB',header=True)
        msg.append_pair(34,'30',header=True)

        
        msg.append_string(fix_str)
        #msg.append_pair(9,'54') #Was 65, duplicate
        msg.append_pair(35,5)
        msg.append_utc_timestamp(52, precision=6, header=True)
        
        b = msg.encode()
        return b
    
    def heartBeat(self):
        '''If ISSUE STILL PRESENT (|'s) -- append_pair for ALL'''
        msg = simplefix.FixMessage()
        
        fix_str = '8=FIX.4.2' # 1=U01067 Moved out of header                   #New Addit (bc on works)
        
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
        '''Side: Buy = 1; SellShort = 5'''
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
        '''Can recursively call HB, if HB_SIM True
        Otherwise, just a send w/ msg = heartBeat
        '''
        hbsocket = self.socket
        print('PyHeartBeat client sending to IP {}'.format(self.IP,self.port))
        
        hb = self.heartBeat()
        hbsocket.sendto(hb,('127.0.0.1',self.port))
        
        if HB_SIM:
            sleep(self.HB)
            if datetime.datetime.now().time().hour >= 16:                      #ADDIts
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
            #sleep(self.HB)
            #self.send(FIX_MSG)                                                #NEW ADDIT ! (Recursive) 
            #self.disconnect() #JUST TO CLOSE -- DELETE
            
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
    
    
    
    HB_MSG = HB.heartBeat() #FIX_MSG
    print(HB_MSG)
    

    
    
    #threading.Thread(target=lambda: every(30,test)).start() Works 
    
    HB.cnct()             #COMMENTED TEMPORARILY 
    print('Connected:',HB.connected)
    
    HB_SIM = True                                                              #Recursive solution.
    #HB.sendHB()       #RUNS RECURSIVELY  -- did work?
    

    
    #Maybe a dict with MSGS ? FIX['BUY'] = ...
    print('HB Thread started...')
    '''
    #threading.Thread(target=lambda: every(30,HB.sendHB),daemon=True).start()    #Works, but stops?
   
    
    HB_SIM = True #Recursive 
    dmn = threading.Thread(target=HB.sendHB(), daemon=True)
    dmn.start()
    '''
    HB_SIM = False
    #NON RECURSIVELY -- thread 
    #threading.Thread(target=every,args=(30,HB.sendHB)).start()                 #BINGO!


    #every(30,HB.send(HB_MSG)) #Alone gets error threading? 
    
    print(datetime.datetime.now().time().hour)
    
    #HB.send(HB_MSG)  #THIS DISCONNECTS THE SOCKET EACH TIME!
    #HB.sendHB()   #AS DOES THIS !
    

    '''Bingo -- regular version !'''
    
    
    
    #HB.run()                                        #WORKS !! 
    #Typically, ^^ this is how it could be ran without threading.
    
    #Testing message format and parse staticmethod 
    
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
    #threading.Thread(target=HB.run).start()                                  #Works (after hours)
    #Daemon version ? 
    threading.Thread(target=HB.run,daemon=True).start()                       #Works! 
    
    
    '''Changes -- per IB
    Pipes removed,
    8098 removed (added auto)
    49 header
    1 not in login (Removed from header to normal positon)
    34 header (already was, ensure in header -- check to override order)
    
    
    '''

    
    
    
    
    
    

                    
