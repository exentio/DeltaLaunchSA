#include <iostream>
#include <vector>
#include <stdexcept>
#include <Windows.h>
#include "SimpleIni.h"

void openApplication(const char* path)
{
	ShellExecute(NULL, "open", path, NULL, NULL, SW_SHOW);
}

std::string quoter(std::string value)
{
	value.insert(0, "\"");
	value += "\"";
	return value;
}

int main()
{
	int c;
	char* selection;
	CSimpleIniA ini;
	ini.SetUnicode();
	ini.LoadFile("config.ini");
	CSimpleIni::TNamesDepend Isections;
	ini.GetAllSections(Isections);
	std::vector<CSimpleIni::Entry> sections;
	Isections.sort(CSimpleIni::Entry::LoadOrder());
	while (!Isections.empty())
	{
		sections.push_back(Isections.front());
		Isections.pop_front();
	}

	for (int i = 1; i < sections.size(); ++i)
	{
		std::cout << i << ". " << sections.at(i).pItem << '\n';
	}
	std::cout << "Choice: ";
	try
	{
		std::cin >> c;
		if (!(c < sections.size() && c > 0))
		{
			throw std::range_error("Input non valid, exiting.\n\n");
		}
	}
	catch (const std::exception &err)
	{
		std::cout << err.what();
		system("pause");
		return 1;
	}
	selection = (char*)sections.at(c).pItem;
	CSimpleIni::TNamesDepend IKey;
	ini.GetAllKeys(selection, IKey);
	std::vector<CSimpleIni::Entry> key;
	key.push_back(IKey.front());
	std::string relative = ini.GetValue("Settings", key.at(0).pItem, NULL);
	std::string x86relative = ini.GetValue("Settings", key.at(1).pItem, NULL);
	std::string value = ini.GetValue(selection, key.at(0).pItem, NULL);
	if (strcmp(key.at(0).pItem, "path") == 0)
	{
		for (int i = 0; value[i] != '\0'; ++i)
		{
			if (value[i] == '\\' || value[i] == '/')
				value[i] = '\\\\';
		}
		value.insert(0, relative);
		value = quoter(value);
		getchar();
		getchar();
		openApplication(value.c_str());
	}
	else if (strcmp(key.at(0).pItem, "cpath") == 0)
	{
		for (int i = 0; value[i] != '\0'; ++i)
		{
			if (value[i] == '\\' || value[i] == '/')
				value[i] = '\\\\';
		}
		value = quoter(value);
		std::cout << value;
		getchar();
		getchar();
		openApplication(value.c_str());
	}
	else if (strcmp(key.at(0).pItem, "x86path") == 0)
	{
		for (int i = 0; value[i] != '\0'; ++i)
		{
			if (value[i] == '\\' || value[i] == '/')
				value[i] = '\\\\';
		}
		value.insert(0, x86relative);
		value = quoter(value);
		getchar();
		getchar();
		openApplication(value.c_str());
	}
	else if (strcmp(key.at(0).pItem, "url") == 0)
	{
		if (value.compare(0, 7, "http://") > 0)
		{
			if (!(value.compare(0, 8, "https://") > 0))
			{
				value.replace(0, 8, "");
			}
			value.insert(0, "http://");
			openApplication(value.c_str());
		}
		else
		{
			openApplication(value.c_str());
		}
	}
	else
	{
		try
		{
			throw "Key in config.ini not valid, check your configuration file.\n\n";
		}
		catch (const std::exception &err)
		{
			std::cout << err.what();
			system("pause");
			return 1;
		}
	}
	return 0;
}