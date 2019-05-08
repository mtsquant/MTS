
/*****************************************************************************
* Copyright [2017-2019] [MTSQuant]
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




    /*static void asyncQueueCallback(uv_async_t* handle) {
        AsyncActiveQueue* queue = (AsyncActiveQueue*)(handle->data);
        queue->processAllData();
    }*/

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
