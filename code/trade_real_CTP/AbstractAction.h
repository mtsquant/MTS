/*****************************************************************************
* Copyright [2018-2019] [3fellows]
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*  http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*****************************************************************************/
#ifndef ABSTRACTACTION_H
#define ABSTRACTACTION_H
#pragma once

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QQueue>
#include <QtCore/QMutex>
#include <QtCore/QPair>

#ifdef _WIN32
#include "../../api/win64/CTP/ThostFtdcTraderApi.h"
#else
#include "../../api/linux64/CTP/ThostFtdcTraderApi.h"
#include <float.h>
#endif
#include "base/MtsLog.h"
#include "mts_core/const.h"

//#include "../uv8ext/AsyncActiveQueue.h"

//template<typename _Ty>
//class AsyncActiveQueue
//{
//public:
//	AsyncActiveQueue() {
        //uv_async_init(uv_default_loop(), &_async, asyncQueueCallback);
        //_async.data = (void*)this;
    //}
    //virtual ~AsyncActiveQueue() {
    //}

    //call these functions on work thread
    //void push(const _Ty& data) {
    //	QMutexLocker l(&_queueLock);
    //	_queue.push_back(data);
    //}
    //void notifyProcess() {
    //	this->processAllData();
        //uv_async_send(&_async);
    //} //notify the thread that the AsyncQueue was created on it to process the data pushed

//protected:
//	virtual void doProcess(const _Ty&) = 0;//it will be called on the thread that the AsyncQueue was created
//
//private:
    //uv_async_t _async;
    /*static void asyncQueueCallback(uv_async_t* handle) {
        AsyncActiveQueue* queue = (AsyncActiveQueue*)(handle->data);
        queue->processAllData();
        //uv_close((uv_handle_t*)&handle, nullptr);	//Èç¹ûasyncûÓйرգ¬ÏûϢ¶ÓÁÐÊǻá×èÈûµÄ
    }*/
//	void processAllData() {
//		QList<_Ty> dataList;
//		{
//			QMutexLocker l(&_queueLock);
//			dataList.swap(_queue);
//		}
//		foreach(const _Ty& data, dataList) {
//			this->doProcess(data);
//		}
//	}
//
//	QMutex _queueLock;
//	QList<_Ty> _queue;
//};

//1. emit signals to main thread in callback on CTP thread
//2. split all kinds of action callbacks into its classes
class AbstractAction:public QObject, public CThostFtdcTraderSpi//,private AsyncActiveQueue<QPair<int/*result id*/, QByteArray/*result data*/> >
{
    Q_OBJECT
 public:
    AbstractAction();
    virtual ~AbstractAction();

    int request(CThostFtdcTraderApi*,int requestId);
    int requestId() const;

Q_SIGNALS:
    void actionError(int  errorId, const QString& errorMsg, OwnerLevel);

protected Q_SLOTS:
    virtual void onProcessResult(int actionId, const QByteArray& data) = 0;

protected:
    virtual int doRequest(CThostFtdcTraderApi* tradeApi,int requestId) = 0;
    void emitActionError(CThostFtdcRspInfoField* pRspInfo, OwnerLevel);
private:
    int _reqId;

 };

#endif // ABSTRACTACTION_H
