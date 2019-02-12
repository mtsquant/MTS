#!/usr/bin/env python
# encoding: utf-8

import signal
import sys
#import pandas as pd
#import numpy as np

def setGlobals(g):
    #print globals()
    globals().update(g)
    #print globals()

def exit():
    mtsExit(0)

def quit(signum):
    sys.exit(signum)

def quitNow(signum,frame):
    quit(signum)

def initialize(params,p2=None):
    signal.signal(signal.SIGINT, quitNow)
    signal.signal(signal.SIGTERM, quitNow)
    if (p2==None):
        return mtsInitialize(params)
    else:
        return mtsInitialize(params,p2)

def execute():
    mtsExecute()

def info(*tp):
    return mtsInfo(tp)

def log(*tp):
    return mtsLog(tp)

def warn(*tp):
    return mtsWarn(tp)

def error(*tp):
    return mtsError(tp)

def file(*tp):
    return mtsFile(tp)
	
def genMtsStratgyClass():
    class Strategy(StrategyBase):
        def __init__(self,name, strategyId):
            super(Strategy, self).__init__(name,strategyId)
            
        def newDirectOrder(self,symbol,price,volume,direction,offsetFlag):
            return self.newOrder({"type":1,"symbol":symbol,"price":price,"volume":volume,"direction":direction,"offsetFlag":offsetFlag})
            
        def newNetOrder(self,symbol,price,volume):
            return self.newOrder({"type":2,"symbol":symbol,"price":price,"volume":volume})
            
        def newBoxOrder(self,symbol,price,volume):
            return self.newOrder({"type":3,"symbol":symbol,"price":price,"volume":volume})

        # def history(self,symbol,count):
        #     pf = pd.DataFrame(self.getHistory(symbol, count),
        #                       columns=['date', 'open', 'high', 'low', 'close', 'volume', 'vwap'])
        #     pf['date'] = pd.to_datetime(pf['date'],format='%Y%m%d %H%M%S')
        #     return pf.set_index('date').astype(np.float64)
        #
        # def dailyHistory(self, symbol, count):
        #     pf = pd.DataFrame(self.getDailyHistory(symbol, count),
        #                       columns=['date', 'open', 'high', 'low', 'close', 'volume', 'vwap'])
        #     pf['date'] = pd.to_datetime(pf['date'], format='%Y%m%d %H%M%S')
        #     return pf.set_index('date').astype(np.float64)
            
    return Strategy