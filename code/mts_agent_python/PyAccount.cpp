
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
#include "PyAccount.h"



PyAccount::PyAccount (const mts::Account& account, const boost::python::list& activeOrders, const boost::python::list& positions)
	:mts::Account(account),_activeOrders(activeOrders),_positions(positions)
{
}


PyAccount::~PyAccount ()
{
}

PyInstrumentProperty PyAccount::findInstrumentProperty ( const std::string & symbol ){
	auto* ip=dynamic_cast<mts::InstrumentProperty*>(this->find ( symbol.c_str () ));
	if ( ip == nullptr ){
		return PyInstrumentProperty ();
	} else{
		return PyInstrumentProperty ( *ip );
	}
}

boost::python::list PyAccount::allActiveOrders() const {
	return _activeOrders;
}

boost::python::list PyAccount::allPositions() const {
	return _positions;
}
