#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Nov 24 15:31:50 2019

@author: zoakes
"""

import simplefix
import socket
import sys

#GLOBALS

ORDERS = {}
SIDES = {}

#import quickfix as fix -> extend fix.Application (still doesnt work -- and I would guess it would run slow)

'''Maybe better to split Socket into it's own class?'''
class Socket():

    
    '''#REPLACE W/ FIX SOCKET -- (could use quickFix.SocketAcceptor())?'''
    def __init__(self,IP = '172.217.8.164', port=80):
        self.IP =  IP
        self.port = port
        self.socket = socket.socket()
        self.connected = False
        
        
    def cnct(self):
        s = self.socket
        try:
            s.connect((self.IP,self.port))
            print('Socket connected to {} on port {}'.format(self.IP,self.port))
            self.connected = True
            return 1
        except socket.error as err:
            print('Socket creation failed: {}'.format(err))
            return 0
        
    def closeSocket(self):
        s = self.socket
        cls = s.close()
        return cls
    
        
    def sendAll(self,msg):
        #try:
        if self.connected is False:
            self.cnct() 
        elif self.connected is True:
            try:
                print('Sending {!r} \n to socket {}'.format(msg,self.IP))
                self.socket.sendall(msg)
                
            except self.socket.error as err:
                print('Error Sending {}'.format(err))
                
            finally:
                print('Closing socket')
                self.socket.close()
                return 1
        else:
            print('Error Connecting')
            return 0
            

                
            
        
        
        
        

class SFIX():
    #ordId = 0
    testID = 0
    
    def __init__(self):
        self.FV = 'FIX.4.2'
        self.ID = 'client 1'
        self.TgtID = 'IB'
        self.ClOrdID = 0 
        self.SecType = 'CS' #Common Stock
        self.TIF = '0'
        
        self.acct_number = 'U123456' #Temp 
        
        self.IP = '172.217.8.164' #REPLACE W/ FIX SOCKET -- (could use quickFix.SocketAcceptor())?
        self.port = 80
        self.socket = socket.socket()
        
        
    def connect(self):
        s = self.socket
        try:
            s.connect((self.IP,self.port))
            txt = 'Socket connected to {} on port {}'.format(self.IP,self.port)
        except socket.error as err: 
            txt = "socket creation failed with error {}".format(err)
        print(txt) #Later Log this
        return txt
    
    def closeSocket(self):
        s = self.socket
        cls = s.close()
        print('Closing socket {}'.format(self.IP))
        return cls
    

    
    
    def baseSend(self,symbol,side,qty,ordType):
        '''
        REQD:    msg / header | 1  | 11  | 55 | 21 (handleInst)? |54 | 38 qty | 40 ordType
        Opt:     204 *? CustOrFirm Done | 100 + 15 *IF using SMART routing 
        Extras:  pkt.append_pair(21,'2')pkt.append_pair(49,'client 1') pkt.append_pair(204,'0')
        #SMART:  pkt.append_pair(100,'SMART')pkt.append_pair(15,'USD')
        '''
        pkt = simplefix.FixMessage()
        
        pkt.append_pair(8,self.FV)
        pkt.append_pair(1,self.acct_number)
        pkt.append_pair(35,'D') #Always
        pkt.append_pair(56,self.TgtID)
        
        newID = self.getClOrdId()
        #cur_ord = ordId + 1
        pkt.append_pair(11,newID) #self.getClOrdId)
        
        pkt.append_pair(55,symbol)
        pkt.append_pair(54,side)
        pkt.append_pair(38,qty)
        pkt.append_pair(40,ordType)

        buffer = pkt.encode()
        return buffer
        
    def baseBuy(self,symbol,qty):
        pkt = simplefix.FixMessage()
        
        pkt.append_pair(8,self.FV)
        pkt.append_pair(1,self.acct_number)
        pkt.append_pair(35,'D') #Always
        pkt.append_pair(56,self.TgtID)
        
        newID = self.getClOrdId()
        pkt.append_pair(11,newID)
        
        pkt.append_pair(55,symbol)
        pkt.append_pair(54,1) #buy
        pkt.append_pair(38,qty)
        pkt.append_pair(40,1) #mkt
        
        buf = pkt.encode()
        return buf
    
    def leanBuy(self,symbol,qty):
        msg = simplefix.FixMessage()
        
        self.testID = self.testID + 1
        newID = str(self.testID) 
        
        FIX_str = '8=FIX.4.2|9=51|35=D|1=U123456|56=IB|11=' + newID + \
        '|55=' + symbol + '|54=1|38=' + qty + '|40=0|10=1663'
        msg.append_string(FIX_str)
        msg.append_pair(35,'8')
        b = msg.encode()
        return b
    
    def leanSS(self,symbol,qty):
        msg = simplefix.FixMessage()
        
        self.testID = self.testID + 1
        newID = str(self.testID)
        
        FIX_str = '8=FIX.4.2|9=51|35=D|1=U123456|56=IB|11=' + newID + \
        '|55=' + symbol + '|54=5|38=' + qty + '|40=0|10=1663'
        msg.append_string(FIX_str)
        msg.append_pair(35,'8')
        b = msg.encode()        
        return b

        
    def baseSellShort(self,symbol,qty):
        pkt = simplefix.FixMessage()
        
        pkt.append_pair(8,self.FV)
        pkt.append_pair(1,self.acct_number)
        pkt.append_pair(35,'D') #Always
        pkt.append_pair(56,self.TgtID)
        
        newID = self.getClOrdId()
        pkt.append_pair(11,newID)
        
        pkt.append_pair(55,symbol)
        pkt.append_pair(54,5) #SELLSHORT
        pkt.append_pair(38,qty)
        pkt.append_pair(40,1) #mkt
        
        buf = pkt.encode()
        return buf        
    
    '''Eventually == create dicts for each field; ex: msgType[execReport] = 8'''
    def appSend(self,FixV='FIX.4.2',msgType='D',exType='E',symbol='MSFT',side='1',qty='10',ordType='1',msg=''):
        
        pkt = FixMessage()
        
        pkt.append_pair(8,FixV)
        #pkt.append_pair(9,'176') #Conf of some sort
        pkt.append_pair(35,msgType) #Msg Type (Execution report) (D is buy or sell I think?)
        pkt.append_pair(49,self.ID) #Exhchange (Sender ID)
        pkt.append_pair(56,self.TgtID) #Target ID (IB)
        
        pkt.append_pair(1,self.acct_number)
        
        pkt.append_utc_timestamp(52, precision=6, header=True) #Timestamp
        
        pkt.append_pair(11,self.getClOrdId)        #May need to run self.getClOrdId()
        pkt.append_pair(150,exType) #Exec Type  -- 'replace' 
        pkt.append_pair(55,symbol) #Symbol
        pkt.append_pair(167,self.SecType) 
        pkt.append_pair(54,side) #SIDE -- 1 = Buy
        pkt.append_pair(38,qty) #QTY
        pkt.append_pair(40,ordType) #MKT
        #if int(ordType) > 1:
        #    pkt.append_pair(44,'100') #Limit Price
        pkt.append_pair(58,msg)
        
        #pkt.append_pair(59,self.TIF) 
        #pkt.append_pair(151,qty) #This will need to calculate based on current position, add in qty chg, etc.
        #pkt.append_pair(15,'USD')
        
        #pkt.append_pair(10,1000) #Checksum -- run an assert w/ count() somehow. Added automatically
        '''This line below takes up 1/2 - 2/3 the time '''
        buf = pkt.encode()
        return buf
    
    
    def parse(self,buf):
        p = simplefix.FixParser()
        p.append_buffer(buf)
        m = p.get_message()
        print(m)
        return m
        
        
    def getClOrdId(self):
        self.testID = self.testID + 1
        return self.testID 
        
    def getLastQty(self):
        '''Temp -- replace now that we have parse'''
        p = simplefix.FixParser()
        p.append_buffer(buf)
        m = p.get_message()
        
        lastQty = m.get(32) 
            
        return lastQty


    def fieldDicts(self,field):
        '''
        Create dictionaries of any confusing fields
        Continue to add fields as you see fit.
        '''
        if field == 40:
            ordTypeD = {
                'mkt': '1',
                'lmt':'2',
                'stop':'3',
                'slmt':'4',
                'mit':'J',
                'peg':'P'
            }
            dct = ordTypeD
        elif field == 54:
            sideD = {
                'buy':'1',
                'sell':'2',
                'buy-':'3',
                'sell+':'4',
                'sellshort':'5'
            }
            dct = sideD
        return dct
    
    def superFD(self):
        ordTypeD = {
                'mkt': '1',
                'lmt':'2',
                'stop':'3',
                'slmt':'4',
                'mit':'J',
                'peg':'P'
            }
        sideD = {
                'buy':'1',
                'sell':'2',
                'buy-':'3',
                'sell+':'4',
                'sellshort':'5'
            }
        fields = {
                40:ordTypeD,
                54:sideD
                }
        return fields
    
    


if __name__ == '__main__':
    s = SFIX()

    print(s.getClOrdId())

    #s.getLastQty()
    
    buffer = s.baseSend('AMZN','0','100','0')
    s.baseSend('AAPL','0','10','0')
    
    s.parse(buffer)
    
    print(s.testID) #Definitely incrementing : ) 
    
    c = Socket()
    #c.cnct()
    
    s.connect()
    
    
    buf = s.leanBuy('MSFT','100') #Incrementing works!
    s.parse(buf)
    
    bb = s.leanSS('MSFT','100')
    s.parse(bb)
    
    b = s.leanSS('MSFT','100')
    s.parse(b)
    
    '''Test dictionary formatting '''
    print(s.fieldDicts(40))
    sides = s.fieldDicts(54)
    print('Buy ordType = ',sides['buy'])
    print('Sell ordType = ',sides['sell'])
    
    print('SellShort ordType = ',sides['sellshort'])
    
    orders = s.fieldDicts(40)
    print('Order type mkt = ',orders['mkt'])
    print('Order -- lmt = ',orders['lmt'])
    print('Order - mit = ',orders['mit'])
    ORDERS = s.fieldDicts(40)
    SIDES = s.fieldDicts(54)
    
    s.fieldDicts(40)
    
    print(ORDERS)
    print(SIDES)
    
    print(s.superFD())
    
    dct = s.superFD()
    
    print(dct[40]['mkt']) #BAM
    
    s.closeSocket()
    
    c.cnct()
    c.sendAll(buf)
    
    
    

        

    
    
    
    
    
    
    
    
    
    