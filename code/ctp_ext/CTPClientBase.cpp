
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
#include "CTPClientBase.h"
#include <iostream>
#include "base/enum_ext.h"


QString CTPClientBase::returnValue2String(int ret) {
	if (ret == 0) {
		return "发送成功";
	}	if (ret == -1) {
		return "因网络原因发送失败";
	} else if (ret == -2) {
		return "未处理请求队列总数量超限";
	} else if (ret == -3) {
		return "每秒发送请求数量超限";
	} else if (ret==-9){
		return  "Not found valid data";
	} else {
		return "Unknown error";
	}
}

bool CTPClientBase::processQryRet(const QString& queryName, int ret)
{
	if (ret != 0) {
		MTS_ERROR("Error:%s-%s,code:%d\n" ,qPrintable(queryName), utf8Printable(returnValue2String(ret)),ret);
		return false;
	}
	return true;
}

bool CTPClientBase::processRspInfoField(CThostFtdcRspInfoField * rspInfo, const QString & action) {
	if (rspInfo == 0) {
		return true;
	}

	if ( rspInfo->ErrorID == 0) {
		MTS_LOG("Succeed to %s\n",qPrintable(action));
		return true;
	} else {
		MTS_LOG("Failed to %s:%s\n",qPrintable(action), MtsUtils::toUtf8(rspInfo->ErrorMsg).constData());
		return false;
	}
}

void CTPClientBase::printCThostFtdcRspUserLoginField(CThostFtdcRspUserLoginField * loginRsp) {

	QString info = QString(
		"TradingDay:%1\n"
		"LoginTime:%2\n"
		"BrokId:%3\n"
		"UserID:%4\n"
		"SystemName:%5\n"
		"FrontID:%6\n"
		"SessionID:%7\n"
		"MaxOrderRef:%8\n"
		"SHFETime:%9\n"
		"DCETime:%10\n"
		"CZCETime:%11\n"
		"FFEXTime:%12\n"
		"INETime:%13"
		)
		.arg(LS(loginRsp->TradingDay))
		.arg(LS(loginRsp->LoginTime))
		.arg(loginRsp->BrokerID)
		.arg(loginRsp->UserID)
		.arg(LS(loginRsp->SystemName))
		.arg(loginRsp->FrontID)
		.arg(loginRsp->SessionID)
		.arg(loginRsp->MaxOrderRef)
		.arg(LS(loginRsp->SHFETime))
		.arg(LS(loginRsp->DCETime))
		.arg(LS(loginRsp->CZCETime))
		.arg(LS(loginRsp->FFEXTime))
		.arg(LS(loginRsp->INETime))
		;
	MTS_LOG("%s\n", utf8Printable(info));
}

CTPClientBase::CTPClientBase(const QString& address, const QString& brokerId, const QString& userId, const QString& password) 
	:_ADDRESS(address), _BROKER_ID(brokerId), _USER_ID(userId), _PASSWORD(password),
	_requestId(0)
{
}


CTPClientBase::~CTPClientBase() 
{
}

int CTPClientBase::nextRequestId() {
	return _requestId++;
}
