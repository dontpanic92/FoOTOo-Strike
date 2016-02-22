#include <fstream>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "ConfigFile.h"

using namespace AGE;
using namespace boost::property_tree;
using namespace std;


#define GET_DEF2(retType, defaultType)\
retType ConfigFile::Get(const std::wstring& key, defaultType defaultValue)\
{\
	return d->config.get(key, defaultValue);\
}

#define GET_DEF(type) GET_DEF2(type, type)

#define SET_DEF(type)\
void ConfigFile::Set(const std::wstring& key, type value)\
{\
	d->config.put(key, value);\
}

namespace AGE
{
	struct ConfigFilePrivate
	{
		boost::property_tree::wptree config;
		std::string path;
	};
}

ConfigFile::ConfigFile():d(new ConfigFilePrivate())
{
}

ConfigFile::~ConfigFile()
{
	delete d;
}

bool ConfigFile::LoadFromFile(const std::string& path)
{
	try {
		read_xml(path, d->config);
	} catch (std::exception&) {
		return false;
	}
	d->path = path;
	return true;
}

void ConfigFile::New(const std::string& path)
{
	d->path = path;
}

bool ConfigFile::SaveToFile()
{
	write_xml(d->path, d->config);
	return true;
}

bool ConfigFile::Exist(const std::wstring& key)
{
	if (d->config.get_child_optional(key))
		return true;
	return false;
}

GET_DEF(int);
GET_DEF(long);
GET_DEF(long long);
GET_DEF(double);
GET_DEF2(std::wstring, const std::wstring&);

SET_DEF(int);
SET_DEF(long);
SET_DEF(long long);
SET_DEF(double);
SET_DEF(const wstring&);

vector<wstring> ConfigFile::Get(const wstring& key)
{
	vector<wstring> vec;
	try {
		for (wptree::value_type &v : d->config.get_child(key)) {
			vec.push_back(v.second.data());
		}
	} catch (std::exception& e) {

	}
	return vec;
}

void ConfigFile::Set(const std::wstring& key, const std::vector<std::wstring>& vector)
{
	for (auto& v : vector) {
		d->config.add(key, v);
	}
}

/*void ConfigFile::Add(const std::wstring& key, int value)
{
	d->mConfig.add(key, value);
}*/
