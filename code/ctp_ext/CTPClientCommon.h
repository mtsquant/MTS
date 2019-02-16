
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
#pragma once
#include "CTPClientBase.h"

template<class _Ty, class _TSpi>
class CTPClientCommon :public _TSpi, public CTPClientBase
{
public:
	CTPClientCommon(const QString& address, const QString& brokerId, const QString& userId, const QString& password)
		:CTPClientBase(address, brokerId, userId, password), _userApi(nullptr), _isLogin(false) ,_frontId(0),_sessionId(0)
	{
	}
	virtual ~CTPClientCommon() {}
	bool connectFront() {
		MTS_LOG("Connect to front:%s...\n", qPrintable(_ADDRESS));
		beforeRegisterFront();
		_userApi->RegisterFront((char*)qPrintable(_ADDRESS));
		_userApi->Init();
		return true;
	}

	void waitForExit() {
		_userApi->Join();
	}

	bool isLogin() {
		return _isLogin;
	}

protected:
	virtual void OnFrontConnected() override {
        ThreadNameRegister::instance()->registerCurrentThread(name()+"PrivateThread");
		MTS_LOG("Connect to front:%s done\n", qPrintable(_ADDRESS));
		if (!doAuthenticate()) {
			login();
		}
	}

	virtual void OnFrontDisconnected(int nReason) override {
		_isLogin = false;
		switch (nReason) {
		case  0x1001:
			MTS_ERROR("Failed to read network\n");
			break;
		case 0x1002:
			MTS_ERROR("Failed to write network");
			break;
		case 0x2001:
			MTS_ERROR("Timeout to recv heartbeat");
			break;
		case 0x2002:
			MTS_ERROR("Failed to send heartbeat");
			break;
		case 0x2003:
			MTS_ERROR("Recv invalid message");
			break;
		default:
			MTS_ERROR("Unknown error number :%d\n", nReason);
			break;
		}
	}

	virtual void OnHeartBeatWarning(int nTimeLapse) override {
	}


	virtual void OnRspUserLogout(CThostFtdcUserLogoutField * pUserLogout, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)  override {
		processRspInfoField(pRspInfo, "logout");
		_isLogin = false;
	}

	virtual void OnRspError(CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast) override {
		processRspInfoField(pRspInfo, "OnRspError");
	}

	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField * pRspUserLogin, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast) override {
		if (processRspInfoField(pRspInfo, QString("login ") + _ADDRESS)) {
			MTS_LOG("Login %s as %s, done\n", qPrintable(name()),qPrintable(_USER_ID));
			_isLogin = true;
			_frontId = pRspUserLogin->FrontID;
			_sessionId = pRspUserLogin->SessionID;
			printCThostFtdcRspUserLoginField(pRspUserLogin);
			onLoginSuccess();
			int ret = subscribe();
			if (ret != 0) {
				QString errInfo = returnValue2String(ret);
				MTS_ERROR("Failed to send subscribe/request message:%s\n", utf8Printable(errInfo));
			}
		} else {
			MTS_ERROR("Failed to login %s as %s\n", qPrintable(name()), qPrintable(_USER_ID));
		}
	}

protected:
	virtual void beforeRegisterFront() {};
	int login() {
		MTS_LOG("Login %s as %s ...\n", qPrintable(name()),qPrintable(_USER_ID));
		CThostFtdcReqUserLoginField req;
		memset(&req, 0, sizeof(CThostFtdcReqUserLoginField));
		strcpy(req.BrokerID, qPrintable(_BROKER_ID));
		strcpy(req.UserID, qPrintable(_USER_ID));
		strcpy(req.Password, qPrintable(_PASSWORD));
		int ret = _userApi->ReqUserLogin(&req, this->nextRequestId());
		if (ret != 0) {
			QString errInfo = returnValue2String(ret);
			MTS_ERROR("Failed to send login message:%s\n", utf8Printable(errInfo));
		}
		return ret;
	}

	virtual int subscribe() = 0;
	virtual void onLoginSuccess() {};
	virtual bool doAuthenticate() { return false; };
	virtual QString name() const = 0;
protected:
	_Ty* _userApi;
	bool _isLogin;
	int _frontId;
	int _sessionId;
};
