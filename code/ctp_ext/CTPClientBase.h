
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
#pragma once
#include <QtCore/QString>
#include <iostream>

#ifdef _WIN32
#include "../../api/win64/CTP/ThostFtdcUserApiStruct.h"
#include "../../api/win64/CTP/ThostFtdcTraderApi.h"
#include "../../api/win64/CTP/ThostFtdcMdApi.h"
#else
#include "../../api/linux64/CTP/ThostFtdcUserApiStruct.h"
#include "../../api/linux64/CTP/ThostFtdcTraderApi.h"
#include "../../api/linux64/CTP/ThostFtdcMdApi.h"
#include <float.h>
#endif

#include "base/MtsLog.h"
#include "base/MtsUtils.h"
#include "ctp_ext_api.h"


class CTP_EXT_API CTPClientBase
{
public:
	CTPClientBase(const QString& address, const QString& brokerId, const QString& userId, const QString& password);
	virtual ~CTPClientBase();

	static QString returnValue2String(int ret);
	static bool processQryRet(const QString& queryName, int ret);
	static bool processRspInfoField(CThostFtdcRspInfoField* rspInfo, const QString& action);
	static void printCThostFtdcRspUserLoginField(CThostFtdcRspUserLoginField* loginRsp);
protected:
	const QString _ADDRESS;
	const QString _BROKER_ID;
	const QString _USER_ID;
	const QString _PASSWORD;

	int nextRequestId();
private:
	int _requestId;

};

