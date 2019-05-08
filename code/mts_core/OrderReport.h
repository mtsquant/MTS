
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
#include <QtCore/QSharedPointer>
#include "OrderAction.h"

namespace mts {
	class MTS_CORE_API OrderReport :public OrderActionCancel
	{
	public:
		OrderReport(OrderReportType type);
		OrderReport(const OrderReport&);
		virtual ~OrderReport();
		const QString& note()const;
		void setNote(const QString&);
		virtual OrderReport* clone() const;
		virtual QJsonObject& toJson(QJsonObject& jsonObj) const override;
		static void registerMetaType();
	private:
		QString _note;
	};

	typedef QSharedPointer<OrderReport> OrderReportPtr;


	class MTS_CORE_API OrderReportNewDone:public OrderReport
	{
	public:
		OrderReportNewDone();
		OrderReportNewDone(const OrderReportNewDone&);
		virtual ~OrderReportNewDone();
		virtual OrderReportNewDone* clone() const override;
		static void registerMetaType();
	protected:
		OrderReportNewDone(OrderReportType type);
	};
	
	typedef QSharedPointer<OrderReportNewDone> OrderReportNewDonePtr;


	class MTS_CORE_API OrderReportReject :public OrderReport
	{
	public:
		OrderReportReject();
		OrderReportReject(const OrderReportReject&);
		virtual ~OrderReportReject();

		virtual OrderReportReject* clone() const override;
		void copyFrom(OrderActionNew*);
	protected:
		OrderReportReject(OrderReportType type);
	};


	class MTS_CORE_API OrderReportNewReject :public OrderReportReject
	{
	public:
		OrderReportNewReject();
		OrderReportNewReject(const OrderReportNewReject&);
		virtual ~OrderReportNewReject();
		virtual OrderReportNewReject* clone() const override;
		static void registerMetaType();
	};

	typedef QSharedPointer<OrderReportNewReject> OrderReportNewRejectPtr;


	class MTS_CORE_API OrderReportFill :public OrderReport
	{
	public:
		OrderReportFill();
		OrderReportFill(const OrderReportFill&);
		virtual ~OrderReportFill();

		const QString& fillId()const;
		qint64 fillTicksSinceEpoch() const;

		void setFillId(const QString&);
		void setFillTicksSinceEpoch(qint64);
		virtual OrderReportFill* clone() const override;
		virtual QJsonObject& toJson(QJsonObject& jsonObj) const override;
		static void registerMetaType();
	protected:
		virtual double leftVolume() const; //TODO check double
		QString _fillId;
		qint64 _fillTicksSinceEpoch;
	};

	typedef QSharedPointer<OrderReportFill> OrderReportFillPtr;


	class MTS_CORE_API OrderReportCancelDone:public OrderReport
	{
	public:
		OrderReportCancelDone();
		OrderReportCancelDone(const OrderReportCancelDone&);
		virtual ~OrderReportCancelDone();

		virtual OrderReportCancelDone* clone() const override;
		static void registerMetaType();
	};

	typedef QSharedPointer<OrderReportCancelDone> OrderReportCancelDonePtr;


	class MTS_CORE_API OrderReportCancelReject :public OrderReportReject
	{
	public:
		OrderReportCancelReject();
		OrderReportCancelReject(const OrderReportCancelReject&);
		virtual ~OrderReportCancelReject();

		virtual OrderReportCancelReject* clone() const override;
		static void registerMetaType();
	};

	typedef QSharedPointer<OrderReportCancelReject> OrderReportCancelRejectPtr;

	/*
	class OrderReportReplaceAck:public OrderReport
	{
	public:
		OrderReportReplaceAck(char type = OrderStatus::OS_REPLACE_ACK);
		virtual ~OrderReportReplaceAck();
	};
		
	class OrderReportReplaceReject:public OrderRepoprtNewReject
	{
	public:
		OrderReportReplaceReject(char type = OrderStatus::OS_REPLACE_REJECT);
		virtual ~OrderReportReplaceReject();
	};
	*/
}

Q_DECLARE_METATYPE(mts::OrderReportPtr);
Q_DECLARE_METATYPE(mts::OrderReportNewDonePtr);
Q_DECLARE_METATYPE(mts::OrderReportNewRejectPtr);
Q_DECLARE_METATYPE(mts::OrderReportFillPtr);
Q_DECLARE_METATYPE(mts::OrderReportCancelDonePtr);
Q_DECLARE_METATYPE(mts::OrderReportCancelRejectPtr);
