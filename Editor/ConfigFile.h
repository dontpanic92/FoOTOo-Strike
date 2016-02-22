#ifndef __AGE_CONFIGFILE_HEADER__
#define __AGE_CONFIGFILE_HEADER__

#include <string>
#include <vector>
#include <functional>


#define CONF_GET_DECL2(retType, defaultType) retType Get(const std::wstring& key, defaultType defaultValue)
#define CONF_GET_DECL(type) CONF_GET_DECL2(type, type)
#define CONF_SET_DECL(type) void Set(const std::wstring& key, type value)

namespace AGE
{
	class ConfigFilePrivate;
	class ConfigFile
	{
	public:
		ConfigFile();
		~ConfigFile();

		bool LoadFromFile(const std::string& path);
		void New(const std::string& path);
		bool SaveToFile();

		CONF_GET_DECL(int);
		CONF_GET_DECL(long);
		CONF_GET_DECL(long long);
		CONF_GET_DECL(double);
		CONF_GET_DECL2(std::wstring, const std::wstring&);

		CONF_SET_DECL(int);
		CONF_SET_DECL(long);
		CONF_SET_DECL(long long);
		CONF_SET_DECL(double);
		CONF_SET_DECL(const std::wstring&);

		void Set(const std::wstring& key, const std::vector<std::wstring>& vector);

		std::vector<std::wstring> Get(const std::wstring& key);

		bool Exist(const std::wstring& key);

	private:
		void Iterate(const std::wstring& key, std::function<void(void*)>);

		//void Add(const std::wstring& key, int value);

		ConfigFilePrivate* d;
	};
}

#endif