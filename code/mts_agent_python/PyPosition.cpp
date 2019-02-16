
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
#include "PyPosition.h"
#include "mts_core/InstrumentPropertyDb.h"


PyPosition::PyPosition() 
{
}

PyPosition::PyPosition(const mts::Position& pos)
	:mts::Position(pos)
{
}


PyPosition::~PyPosition() 
{
}


std::string PyPosition::pySymbol() const {
	return TO_UTF8_STD_STR(this->symbol());
}

std::string PyPosition::pyMtsSymbol() const {
	return TO_UTF8_STD_STR(this->mtsSymbol());
}

std::string PyPosition::pyExchId() const {
	return TO_UTF8_STD_STR(this->exchId());
}

std::string PyPosition::toString() const {
	return TO_UTF8_STD_STR(this->toJsonString());
}

PyInstrumentProperty PyPosition::pyAttribute() const {
	auto* p = dynamic_cast<const mts::InstrumentProperty*>(this->attribute());
	if (p) {
		return PyInstrumentProperty(*p);
	} else {
		return PyInstrumentProperty();
	}
}
