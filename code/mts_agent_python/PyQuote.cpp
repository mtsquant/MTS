
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
#include "PyQuote.h"
#include "mts_core/InstrumentPropertyDb.h"
#include <base/MtsLog.h>


PyQuote::PyQuote() 
:mts::QuoteSnapshot (), _us(0)
{
#ifdef MTS_LOG_DISABLED
	_us = DateTime::nowToUTCMicrosecsSinceEpoch();
#endif
}

PyQuote::PyQuote(const mts::QuoteSnapshot& qt)
:mts::QuoteSnapshot (qt), _us(0)
{
#ifdef MTS_LOG_DISABLED
	_us = DateTime::nowToUTCMicrosecsSinceEpoch();
#endif
}


PyQuote::~PyQuote() 
{
}

std::string PyQuote::pySymbol() const {
	return TO_UTF8_STD_STR(this->symbol());
}

std::string PyQuote::pyMtsSymbol() const {
	return TO_UTF8_STD_STR(this->mtsSymbol());
}

std::string PyQuote::pyExchId() const {
	return TO_UTF8_STD_STR(this->exchId());
}

PyDateTime PyQuote::pyDateTime() const {
	return PyDateTime(this->ticksSinceEpoch());
}

std::string PyQuote::toString() const {
	return TO_UTF8_STD_STR(this->toJsonString());
}

mts::QuoteSnapshot * PyQuote::clone() const {
	return nullptr;
}

qint64 PyQuote::cppRevMicrosecond() const
{
	return _us;
}
