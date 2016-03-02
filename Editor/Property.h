#ifndef __EDITOR_PROPERTY_HEADER__
#define __EDITOR_PROPERTY_HEADER__

#include <string>
#include <functional>
#include <qvariant.h>

class Property
{
public:
	Property(const QString& name, QVariant::Type type, std::function<QVariant()> get, std::function<bool(const QVariant&)> set);

	QVariant::Type GetType();
	QString GetName();
	QVariant Get();
	bool Set(const QVariant&);

protected:
	QString mName;
	QVariant::Type mType;
	std::function<QVariant()> mGetFunc;
	std::function<bool(const QVariant&)> mSetFunc;
};

#endif
