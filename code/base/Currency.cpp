#include "./Currency.h"
#include <QtCore/QHash>
#include <QtCore/QString>

const char * currencyIdName(CurrencyId currency) {
	static const char* names[] = {
		CURRENCY_ID_ENUM(SELECT_2_AND_COMMA_IN_3)
	};
	if (currency >= 0 && currency < ARRAY_SIZE(names)) {
		return names[currency];
	} else {
		return names[0];
	}
}

CurrencyId currencyId(const char * currencyName) {
	static QHash<QString, CurrencyId> map;
	if (map.isEmpty()) {
		CURRENCY_ID_ENUM(DEFINE_NAME_ENUM_MAP3_KEY_2);
	}
	return map[currencyName];
	return CurrencyId();
}

