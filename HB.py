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

#Logging globals 
LOGGING = True
OUTPUT_LOG = True

#Heartbeat testing globals
DEBUG = True


class Socket():
    HB = 10

    
    def __init__(self,IP = '172.217.8.164', port=80):
        self.IP =  IP
        self.port = port
        self.socket = socket.socket()
        self.connected = False

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
        end = datetime.datetime.now() + datetime.timedelta(seconds=30)        #Temp -- to stop inf loop
        now = datetime.datetime.now()                                           #temp
        while self.connected:                                    
            self.socket.sendall(b'Heartbeat')
            if DEBUG:
                self.log('Time: {}'.format(datetime.datetime.now()))
                if datetime.datetime.now() > end:                               #temp
                    break;
            time.sleep(self.HB)
        return 0
            
            
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
    
    
    
    '''New error catching block'''
    
    import select 
    
    s.cnct()
    
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
        
    '''Endif new error catching /reconnect block'''
        
        
        
    
    
    
    s.sendHB()
    s.closeSocket()
    print('Closed Socket')
    
    print(0)