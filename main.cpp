#include <iostream>
#include <vector>
#include <stdexcept>
#include <Windows.h>
#include "SimpleIni.h"

void openApplication(const char* path)
{
	ShellExecute(NULL, "open", path, NULL, NULL, SW_SHOW);
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
	std::string value = ini.GetValue(selection, key.at(0).pItem, NULL);
	if (strcmp(key.at(0).pItem, "path") == 0)
	{
		for (int i = 0; value[i] != '\0'; ++i)
		{
			if (value[i] == '\\' || value[i] == '/')
				value[i] = '\\\\';
		}
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
	return 0;
}
