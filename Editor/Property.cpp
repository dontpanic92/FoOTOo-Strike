#include "Property.h"

Property::Property(const QString& name, QVariant::Type type, std::function<QVariant()> get, std::function<bool(const QVariant&)> set)
	:mName(name), mType(type), mGetFunc(get), mSetFunc(set)
{
}

QVariant::Type Property::GetType()
{
	return mType;
}

QString Property::GetName()
{
	return mName;
}

QVariant Property::Get()
{
	return mGetFunc();
}

bool Property::Set(const QVariant& v)
{
	return mSetFunc(v);
}
