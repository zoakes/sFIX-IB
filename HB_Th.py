#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Nov 25 11:11:54 2019

@author: zoakes
"""

import socket
import time
import datetime
import sys
import logging
import select
import threading 
import calendar

import numpy as np


#Logging globals 
LOGGING = True
OUTPUT_LOG = True

#Heartbeat testing globals
DEBUG = False


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
  




class Socket():
    HB = 10


    
    def __init__(self,IP = '172.217.8.164', port=80):
        self.IP =  IP
        self.port = port
        self.socket = socket.socket()
        
        #sc = s.socket  -- Doesnt work here.
        #self.in_error = select.select([sc,],[sc,],[], 5)
        self.connected = False #self.in_error

        self.logger = logging.getLogger(__name__)
        self.handler = logging.FileHandler('sFIX_Socket.log')
        self.formatter = logging.Formatter(
            '%(asctime)s - %(name)s - %(levelname)s - %(message)s')
        self.handler.setFormatter(self.formatter)
        self.logger.addHandler(self.handler)
        self.logger.info('Starting log at {}'.format(datetime.datetime.now()))
        
        
    def log(self,msg=''):
        '''Add Log to Output file'''
        if LOGGING: self.logger.info(msg)
        if OUTPUT_LOG: print(msg)
        
        
    def cnct(self):
        s = self.socket
        try:
            s.connect((self.IP,self.port))
            self.log('Socket connected to {} on port {}'.format(self.IP,self.port))
            self.connected = True
            return 1
        except socket.error as err:
            self.log('Socket creation failed: {}'.format(err))
            return 0
        
    def sendHB(self):
        print('Sending HB to {}'.format(self.IP))
        end = datetime.datetime.now() + datetime.timedelta(seconds=100)        #Temp -- to stop inf loop
        if self.connected:   
            if DEBUG and datetime.datetime.now() < end: #Temp                                 
                self.socket.sendall(b'Heartbeat')
                self.log('Time: {}'.format(datetime.datetime.now()))
        return 0
    
    def leanHB(self):
        if self.connected:
            self.log('Sending {} HB to {}'.format(datetime.datetime.now(),self.IP))
            self.socket.sendall(b'HB')
        return 0
        
    
    
    
    def run(self):
        '''Works, but repetitive -- and bloated'''
        now = datetime.datetime.now().time()

        EOD = datetime.time(hour=18) 
        BOD = datetime.time(hour=9,minute=30)
        
        #Begin HB / Market Day
        if BOD < now < EOD and self.connected:
            #threading.Thread(target=lambda: every(30,test)).start() #Works ! -- using in main instead
            #threading.Thread(target=lambda: self.leanHB()).start()  SHOULD work, but doesnt inside class?
            self.log('Market Open for day ...')

        while self.connected:
            
            #Check market hours + HB (Could do 2 while loops? inner break outer brk)
            if BOD > now > EOD:
                print('Market Closed for day...')
                sys.exit()
                self.closeSocket()
                self.connected = False
                break
            
            if DEBUG:
                print('Running')
                
                go_long, go_short = False, False
                if np.random.randint(-100,100) > 0:
                    go_long = True
                else:
                    go_short = True
                
                trailcat = np.random.randint(-100,100) > 50 and (go_long or go_short)
    
                if go_long:
                    self.log('Long Entry')
                    self.leanSend('buy_test')
                    
                if go_short:
                    self.log('Short Entry')
                    self.leanSend('sell_test')
                    
                if trailcat:
                    self.log('Trail / Cat')
                    break #To not run endlessly 
                
        self.log('System closed for day')
                
            
            
    
                
            
            
    def closeSocket(self):
        s = self.socket
        cls = s.close()
        self.connected = False
        return cls
    
    
    def sendAll(self,msg):
        #try:
        if self.connected is False:
            self.cnct() 
        elif self.connected is True:
            try:
                self.log('Sending {!r} \n to socket {}'.format(msg,self.IP))
                self.socket.sendall(msg)
                
            except self.socket.error as err:
                self.log('Error Sending {}'.format(err))
               
            #WILL NEED TO REMOVE THIS -- destructor would be nice.
            finally:
                self.log('Closing socket')
                self.socket.close()
                self.connected = False
                return 1
        else:
            self.log('Error Connecting')
            return 0
        
        
    def leanSend(self,msg):
        try:
            print('Sending {}'.format(msg))
            self.socket.sendall(msg)
            
        except self.socket.error as err:
            self.log('Error {}'.format(err))
            
        finally:
            self.socket.close()
            return 'Closing Socket'
    
    
if __name__ == '__main__':
    s = Socket()
    
    
    
    
    
    
    s.cnct()
    
    #Heartbeat! -- (difficult to benchmark now! but oh well)
    threading.Thread(target=lambda: every(30,s.leanHB)).start()
    
    s.run()
    
    
    
    '''
    #New error catching block (To reconnect !)
    
    try:
        sc = s.socket
        in_error = select.select([sc,],[sc,],[], 5)
        print('End select')
        
    except select.error:
        sc.shutdown(2)
        s.closeSocket() #Or sc.close()
        print('Connection error')
    finally:
        s.cnct()
        print('Re-Connected')
        
    #Endif new error catching /reconnect block
       
        
    '''    
    
    
    
    #s.sendHB()
    s.closeSocket()
    print('Closed Socket')
    
    print(0)
    
    '''
    def run(self):
       #Works, but repetitive -- and bloated
        now = datetime.datetime.now().time()

        hb30 = now.second == 1 or now.second == 31
        EOD = datetime.time(hour=17)
        BOD = datetime.time(hour=9,minute=30)
        
        time_ref = calendar.timegm(time.strptime(
                now.strftime('%Y-%m-%d') + ' 13:30:00', '%Y-%m-%d %H:%M:%S'))
        
        
        
        while self.connected:
            
            HBCt = 0
            #Reset HBCt every minute
            time_since_open = time.time() - time_ref
            if 60-(time_since_open%60) <= 1: 
                HBCt = 0
            
            threading.Thread(target=lambda: self.every(30,foo)).start()
            #Check market hours + HB (Could do 2 while loops? inner break outer brk)
            if BOD < now < EOD:
                secs = datetime.datetime.now().time().second
                if secs == 1 or secs == 31:
                    if HBCt < 3:
                        continue
                        HBCt += 1
                        print('HB')
                        self.leanSend('HB')
                        #continue
                        #break #This may need to be continue? WHY is there no DO functions
                        
            else:
                print('Market Closed for day...')
                self.closeSocket()
                self.connected = False
                break
            
            #self.run()
            
            
            #if go_long:
            #    leanSend('buy_test')
                
            #if go_short:
                leanSend('sell_test')
                
            #if trailStop or catStop:
            #    print('Trail / Cat')
            
    
    
    '''
