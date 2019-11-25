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

_DEBUG = True

class HB_Socket():
    _DEBUG
    
    def __init__(self,IP='172.217.8.164',port=4000,hb = 10):
        self.IP = IP
        self.port = port
        self.HB = hb
        self.socket = socket(AF_INET,SOCK_DGRAM)
        
    
    def connect(self):
        hbsocket = self.socket
        print('PyHeartBeat client sending to IP {}'.format(self.IP,self.port))
        while 1:
            hbsocket.sendto(b'HB',('127.0.0.1',self.port))
            if _DEBUG:
                print('Time: {}'.format(ctime(time())))
            sleep(self.HB)
            
    def disconnect(self):
        s = self.socket
        close = s.close()
        if self._DEBUG: print('Closing Socket {}'.format(self.IP))
        return close
        
            

if __name__ == '__main__':
    HB = HB_Socket()
    
    HB.connect() #Runs on inf loop
    sleep(30)
    HB.disconnect()

                    