
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

#include "mts_core/OrderReport.h"
#include "PyDateTime.h"
#include "mts_core/InstrumentPropertyDb.h"

template<typename _TP>
class PyOrderReportTpl :public _TP
{
public:
	PyOrderReportTpl ( const  _TP& report)
		:_TP(report)
	{
	}

	~PyOrderReportTpl ()
	{
	}

	std::string pySymbol () const{
		return TO_UTF8_STD_STR(this->symbol());
	}
	std::string pyMtsSymbol () const{
		return TO_UTF8_STD_STR(this->mtsSymbol());
	}
	std::string pyExchId () const{
		return TO_UTF8_STD_STR(this->exchId());
	}
	PyDateTime pyCreatedDateTime ()  const{
		return PyDateTime ( this->createTicksSinceEpoch () );
	}
	std::string pyOrderExchId () const{
		return TO_UTF8_STD_STR(this->orderExchId());
	}
	std::string pyNote () const{
		return TO_UTF8_STD_STR(this->note());
	}
	std::string pyPerfNote()const
	{
		return TO_UTF8_STD_STR(this->perfNote());
	}
	std::string pyCreateSrc () const{
		return TO_UTF8_STD_STR(this->createSrc()) ;
	}
	std::string toString () const{
		return TO_UTF8_STD_STR(this->toJsonString());
	}
	std::string referenceId2String()const {
		return TO_UTF8_STD_STR(this->referenceId());
	}
};

typedef PyOrderReportTpl<mts::OrderReport> PyOrderReport;