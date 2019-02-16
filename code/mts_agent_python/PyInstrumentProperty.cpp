
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
#include "PyInstrumentProperty.h"

PyInstrumentProperty::PyInstrumentProperty ()
	:mts::InstrumentProperty()
{
}

PyInstrumentProperty::PyInstrumentProperty(const mts::InstrumentProperty & p)
	:mts::InstrumentProperty(p) {

}

std::string PyInstrumentProperty::pyName() const {
	return TO_UTF8_STD_STR(mts::InstrumentBaseProperty::name());
}

std::string PyInstrumentProperty::pyMtsSymbol() const {
	return TO_UTF8_STD_STR(mts::InstrumentBaseProperty::mtsSymbol());
}

std::string PyInstrumentProperty::toString() const {
	return TO_UTF8_STD_STR(mts::InstrumentBaseProperty::toJsonString());
}

std::string PyInstrumentProperty::pySymbol() const {
	return TO_UTF8_STD_STR(mts::InstrumentBaseProperty::instrumentId().symbol);
}

