#include <iostream>
#include <vector>
#include <cstring>
#include <Windows.h> // Questa e' la cosa piu' brutta che vedrai nella tua vita.
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
	// sections.pop_front();
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
	std::cout << "Inserisci un numero: ";
	std::cin >> c;
	selection = (char*) sections.at(c).pItem;
	CSimpleIni::TNamesDepend IKey;
	ini.GetAllKeys(selection, IKey);
	std::vector<CSimpleIni::Entry> key;
	key.push_back(IKey.front());
	const char* value = ini.GetValue(selection, key.at(0).pItem, NULL);
	const char* browser = ini.GetValue("Settings", "browser", NULL);
	if (strcmp(key.at(0).pItem, "path") == 0)
		openApplication(value);
	else if (strcmp(key.at(0).pItem, "url") == 0)
		openApplication(value);
	return 0;
}