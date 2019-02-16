
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
#include "CalcBar.h"
#include "base/MtsLog.h"

#include <QtCore/QJsonDocument>
#include <assert.h>
#include "mts_core/InstrumentPropertyDb.h"

namespace mts{

CalcBar::CalcBar(const InstrumentId& id, int interval, qint64 endTicks)
	:InstrumentObject(id),_bar(new Bar()), _interval(interval)//, _isUpdating(false)
{
	assert(interval>1000);
	_bar->setEndTicksSinceEpoch(endTicks);
}

CalcBar::CalcBar(BarPtr bar, const InstrumentId& id, int interval)
	:InstrumentObject(id), _bar(bar), _interval(interval)//, _isUpdating(false)
{
	assert(interval>1000);
}

CalcBar::CalcBar(int interval)
	:InstrumentObject()
	,_bar(new Bar())
	,_interval(interval)//, _isUpdating(false)
{
	assert(interval==0 || interval>1000);
}


CalcBar::~CalcBar() 
{
}

qint64 CalcBar::beginTicksSinceEpoch () const{ //开始时间
	return _bar->endTicksSinceEpoch() - _interval;
}


bool CalcBar::updateQuote(QuoteSnapshot* qt) {
	return _bar->updateQuote(qt, _interval);
}

bool CalcBar::updateTime(qint64 ticks) {
	if (ticks >= _bar->endTicksSinceEpoch()) {
		return true;
	} else {
		return false;
	}
}

bool CalcBar::fromJsonString(const QString & jsonStr) {
	QJsonParseError  error;
	QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonStr.toLocal8Bit(), &error);
	if (error.error != QJsonParseError::NoError) {
		MTS_ERROR("Failed to parse bar json object '%s'\n", qPrintable(jsonStr));
		return false;
	}

	QJsonObject jsonObj = jsonDoc.object();
	if (!_bar->fromJson(jsonObj)) {
		return false;
	}

	QString symbol = jsonObj.value("symbol").toString();
	QString typeNameStr = jsonObj.value("instrumentType").toString();
	QString exchIdStr = jsonObj.value("exchId").toString();

	InstrumentId id(symbol, instrumentType(qPrintable(typeNameStr)), mts::exchId(qPrintable(exchIdStr)));
	this->setInstrumentId(id);
	this->setInterval(jsonObj.value("interval").toInt());
	return true;
}

QString CalcBar::toJsonString() const {
	QJsonObject barObj=_bar->toJson();
	barObj.insert("symbol", this->instrumentId().symbol);
	barObj.insert("instrumentType", instrumentTypeName(this->instrumentId().typeId));
	barObj.insert("exchId", exchIdName(this->instrumentId().exchId));
	barObj.insert("interval", _interval);
	//barObj.insert("isUpdating", _isUpdating);
	QJsonDocument jsonDoc(barObj);
	return jsonDoc.toJson().simplified();
}

BarPtr CalcBar::coreBar() const {
	return _bar;
}


bool CalcBar::fromCsvString(const QString & line, QChar sep/* = ','*/) {
	return this->_bar->fromCsvString(line.section(sep,2), sep);
}

//std::string CalcBar::toCsvString() const
//{
//	return this->symbol()+"," this->_bar->toCsvString().toStdString();
//}

void CalcBar::mergeFrom ( const CalcBar & bar){
	_bar->mergeFrom ( *bar._bar );
	this->setInterval ( this->interval () + bar.interval () );
}

CalcBar * CalcBar::clone() const {
	return new CalcBar(BarPtr(_bar->clone()), instrumentId(), _interval);
}

void CalcBar::registerMetaType() {
	qRegisterMetaType<CalcBarPtr>("CalcBarPtr");
	qRegisterMetaType<CalcBar>("CalcBar");
}

double CalcBar::vwap () const{
	auto * property = InstrumentPropertyDb::instance ()->find ( this->instrumentId () );
	return this->_bar->vwap (property->volumeMultiple());
}

}