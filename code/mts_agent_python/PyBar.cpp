
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
#include "PyBar.h"
#include "mts_core/InstrumentPropertyDb.h"



PyBar::PyBar ( const mts::CalcBar &  bar)
	:mts::CalcBar(bar)
{
}


PyBar::~PyBar ()
{
}


std::string PyBar::pySymbol () const{
	return TO_UTF8_STD_STR(this->symbol());
}

std::string PyBar::pyMtsSymbol () const{
	return TO_UTF8_STD_STR(this->mtsSymbol());
}

std::string PyBar::pyExchId () const{
	return TO_UTF8_STD_STR(this->exchId());
}

PyDateTime PyBar::pyBeginDateTime () const{
	return PyDateTime ( this->beginTicksSinceEpoch () );
}

PyDateTime PyBar::pyEndDateTime() const {
	return PyDateTime(this->endTicksSinceEpoch());
}

std::string PyBar::toString () const{
	return TO_UTF8_STD_STR(this->toJsonString());
}
