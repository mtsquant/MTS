
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
#include "mts_core/Quote.h"
#include "PyDateTime.h"

class PyQuote:public mts::QuoteSnapshot
{
public:
	PyQuote();
	PyQuote(const mts::QuoteSnapshot& qt);
	~PyQuote();

	std::string pySymbol() const;
	std::string pyMtsSymbol() const;
	std::string pyExchId() const;
	PyDateTime pyDateTime()  const;
	std::string toString() const;

	virtual mts::QuoteSnapshot* clone() const override;
	qint64 cppRevMicrosecond()const;
private:
	qint64 _us;
};

