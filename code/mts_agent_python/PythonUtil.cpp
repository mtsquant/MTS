
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
#include "PythonUtil.h"


void PythonUtil::dict2vmap(const boost::python::dict & params, QVariantMap * paramMap) {
	boost::python::list keys = params.keys();
	int keySz = boost::python::len(keys);
	for (int i = 0; i < keySz; ++i) {
		QString key = pyExtract(keys[i]).toString();
		QVariant val = pyExtract(params[key.toStdString()]);
		paramMap->insert(key, val);
	}
}

void PythonUtil::printDict(const boost::python::dict & params) {
	QVariantMap tmp;
	dict2vmap(params, &tmp);
	for (auto i = tmp.begin(), iend = tmp.end(); i != iend; ++i) {
		printf("%s=%s\t", qPrintable(i.key()), qPrintable(i.value().toString()));
	}
	printf("\n");
}


boost::python::list PythonUtil::toObject(const QStringList&  strs) {
	boost::python::list list;
	for (int i = 0; i < strs.size(); ++i) {
		list.append<std::string>(strs.at(i).toStdString());
	}
	return list;
}

boost::python::dict PythonUtil::globals() {
	auto main = boost::python::import("__main__");
	return  boost::python::extract<boost::python::dict>(main.attr("__dict__"));
}

boost::python::object PythonUtil::addObject(const QString& objName, const boost::python::object& obj) {
	globals()[qPrintable(objName)] = obj;
	return obj;
}

bool PythonUtil::hasObject(const QString & objName) {
	return globals().has_key(qPrintable(objName));
}


int PythonUtil::attrSize(const boost::python::object& obj) {
	boost::python::dict result = boost::python::extract<boost::python::dict>(obj.attr("__dict__"));
	return boost::python::len(result);
}

bool PythonUtil::hasAttr(const boost::python::object& obj, const QString& attrName) {
	boost::python::dict result = boost::python::extract<boost::python::dict>(obj.attr("__dict__"));
	return result.has_key(qPrintable(attrName));

}

boost::python::object PythonUtil::getObject(const QString& objName) {
	if (!hasObject(objName)) {
		return boost::python::object();
	}
	return globals()[qPrintable(objName)];
}


