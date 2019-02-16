
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
#include "OrderReport.h"
#include <QtCore/QDateTime>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#include "base/DateTime.h"

namespace mts {

	OrderReport::OrderReport(OrderReportType type)
		:OrderActionNew(type)
	{
	}

	OrderReport::OrderReport(const OrderReport& rpt)
		: OrderActionNew(rpt)
	{
		_ordExchId = rpt._ordExchId;
		_note = rpt._note;
	}
	
	OrderReport::~OrderReport() {
	}

	QString OrderReport::orderExchId() const {
		return _ordExchId;
	}

	void OrderReport::setOrderExchId(const QString& id) {
		_ordExchId = id;
	}

	OrderReport * OrderReport::clone() const {
		return new OrderReport(*this);
	}

	QJsonObject& OrderReport::toJson(QJsonObject& jsonObj) const
	{
		OrderActionNew::toJson(jsonObj);
		jsonObj.insert("orderExchId", this->orderExchId());
		jsonObj.insert("note", this->note());
		return jsonObj;
	}

	void OrderReport::registerMetaType() {
		qRegisterMetaType<OrderReportPtr>("OrderReportPtr");
	}

	const QString& OrderReport::note() const
	{
		return _note;
	}

	void OrderReport::setNote(const QString& errorMsg)
	{
		_note = errorMsg;
	}

	OrderReportNewDone::OrderReportNewDone()
		:OrderReport(OrderReportType::ORT_NEW_DONE)
	{

	}

	OrderReportNewDone::OrderReportNewDone(OrderReportType type)
		: OrderReport(type)
	{
	}


	OrderReportNewDone::OrderReportNewDone(const OrderReportNewDone& done)
		: OrderReport(done)
	{

	}

	OrderReportNewDone::~OrderReportNewDone()
	{

	}

	OrderReportNewDone * OrderReportNewDone::clone() const {
		return new OrderReportNewDone(*this);
	}

	void OrderReportNewDone::registerMetaType() {
		qRegisterMetaType<OrderReportNewDone>("OrderReportNewDone");
		qRegisterMetaType<OrderReportNewDonePtr>("OrderReportNewDonePtr");
	}

	
	OrderReportReject::OrderReportReject()
		:OrderReport(OrderReportType::ORT_NEW_REJECT)
	{

	}

	OrderReportReject::OrderReportReject(OrderReportType type)
		: OrderReport(type)
	{
	}


	OrderReportReject::OrderReportReject(const OrderReportReject& rjt)
		: OrderReport(rjt)
	{
	}

	OrderReportReject::~OrderReportReject()
	{

	}
	
	OrderReportReject * OrderReportReject::clone() const {
		return new OrderReportReject(*this);
	}


	void OrderReportReject::copyFrom(OrderActionNew* action)
	{
		setTradingAccountId(action->tradingAccountId());
		setInstrumentId(action->instrumentId());
		setBook(action->book());
		setTradingDay(action->tradingDay());

		setVolume(action->volume());
		setPrice(action->price());
		setPriceType(action->priceType());
		setDirectonSide(action->directionSide());
		setTimeCondition(action->timeCondition());
		setCreateSrc(action->createSrc());
		setInstanceId(action->instanceId());
	}

	OrderReportNewReject::OrderReportNewReject()
		:OrderReportReject(OrderReportType::ORT_NEW_REJECT)
	{
	}

	OrderReportNewReject::OrderReportNewReject(const OrderReportNewReject& rjt)
		: OrderReportReject(rjt)
	{

	}

	OrderReportNewReject::~OrderReportNewReject() {
	}

	OrderReportNewReject * OrderReportNewReject::clone() const {
		return new OrderReportNewReject(*this);
	}

	void OrderReportNewReject::registerMetaType() {
		qRegisterMetaType<OrderReportNewReject>("OrderReportNewReject");
		qRegisterMetaType<OrderReportNewRejectPtr>("OrderReportNewRejectPtr");
	}


	OrderReportFill::OrderReportFill()
		:OrderReport(OrderReportType::ORT_FILL), _fillTicksSinceEpoch(DateTime::now().toUTCMillisecsSinceEpoch ())
	{

	}

	OrderReportFill::OrderReportFill(const OrderReportFill& filrpt)
		: OrderReport(filrpt)
	{
		_fillId = filrpt._fillId;
		_fillTicksSinceEpoch = filrpt._fillTicksSinceEpoch;

	}

	OrderReportFill::~OrderReportFill()
	{

	}

	double OrderReportFill::leftVolume() const //TODO check double
	{
		return 0;
	}

	const QString& OrderReportFill::fillId() const
	{
		return _fillId;
	}

	void OrderReportFill::setFillId(const QString& id)
	{
		_fillId = id;
	}

	qint64 OrderReportFill::fillTicksSinceEpoch() const
	{
		return _fillTicksSinceEpoch;
	}

	void OrderReportFill::setFillTicksSinceEpoch(qint64 ticks)
	{
		_fillTicksSinceEpoch = ticks;
	}

	OrderReportFill * OrderReportFill::clone() const {
		return new OrderReportFill(*this);
	}
	
	QJsonObject& OrderReportFill::toJson(QJsonObject& jsonObj) const
	{
		OrderReport::toJson(jsonObj);
		jsonObj.insert("fillId",this->fillId());
		jsonObj.insert("leftVolume", this->leftVolume());
		jsonObj.insert("fillTicksSinceEpoch", this->fillTicksSinceEpoch());
		return jsonObj;
	}

	void OrderReportFill::registerMetaType() {
		qRegisterMetaType<OrderReportFill>("OrderReportFill");
		qRegisterMetaType<OrderReportFillPtr>("OrderReportFillPtr");
	}


	OrderReportCancelDone::OrderReportCancelDone()
		:OrderReport(OrderReportType::ORT_CANCEL_DONE)
	{

	}

	OrderReportCancelDone::OrderReportCancelDone(const OrderReportCancelDone& done)
		: OrderReport(done)
	{

	}

	OrderReportCancelDone::~OrderReportCancelDone() {

	}

	OrderReportCancelDone * OrderReportCancelDone::clone() const {
		return new OrderReportCancelDone(*this);
	}

	void OrderReportCancelDone::registerMetaType() {
		qRegisterMetaType<OrderReportCancelDone>("OrderReportCancelDone");
		qRegisterMetaType<OrderReportCancelDonePtr>("OrderReportCancelDonePtr");
	}
	
	
	OrderReportCancelReject::OrderReportCancelReject()
		:OrderReportReject(OrderReportType::ORT_CANCEL_REJECT)
	{

	}

	OrderReportCancelReject::OrderReportCancelReject(const OrderReportCancelReject& rjt)
		:OrderReportReject(rjt)
	{

	}

	OrderReportCancelReject::~OrderReportCancelReject()
	{

	}

	OrderReportCancelReject * OrderReportCancelReject::clone() const {
		return new OrderReportCancelReject(*this);
	}

	void OrderReportCancelReject::registerMetaType() {
		qRegisterMetaType<OrderReportCancelReject>("OrderReportCancelReject");
		qRegisterMetaType<OrderReportCancelRejectPtr>("OrderReportCancelRejectPtr");
	}

}
