#include <iostream>
#include <vector>
#include <stdexcept>
#include <Windows.h>
#include "SimpleIni.h"

// I hate WinApi
void openApplication(const char* command)
{
	ShellExecute(NULL, "open", command, NULL, NULL, SW_SHOW);
}

// Quotes paths so you can put spaces
void quoter(std::string& keyvalue)
{
	keyvalue.insert(0, "\"");
	keyvalue += "\"";
}

// Doubles backslashes to get actual backslashes, or converts *nix paths to Windows-style paths (with double backslashes of course)
void doubleslasher(std::string& keyvalue)
{
	for (int i = 0; keyvalue[i] != '\0'; ++i)
	{
		if (keyvalue[i] == '\\' || keyvalue[i] == '/')
			keyvalue[i] = '\\\\';
	}
}

void matcher(std::string& keyvalue, const std::string& relativepath)
{
	keyvalue.insert(0, relativepath);
	doubleslasher(keyvalue);
	quoter(keyvalue);
	openApplication(keyvalue.c_str());
}

void matcher(std::string& keyvalue)
{
	doubleslasher(keyvalue);
	quoter(keyvalue);
	openApplication(keyvalue.c_str());
}

int main()
{
	// Loads config file
	char* selection;
	CSimpleIniA ini;
	ini.SetUnicode();
	ini.LoadFile("config.ini");

	// Gets .ini sections
	CSimpleIni::TNamesDepend Isections;
	ini.GetAllSections(Isections);

	// Sorts them in load order
	Isections.sort(CSimpleIni::Entry::LoadOrder());
	std::vector<CSimpleIni::Entry> sections;
	while (!Isections.empty())
	{
		sections.push_back(Isections.front());
		Isections.pop_front();
	}

	// Prints sections
	for (int i = 1; i < sections.size(); ++i)
	{
		std::cout << i << ". " << sections.at(i).pItem << '\n';
	}
	std::cout << "\n0. Exit\n\n";

	// Makes you choose what you wanna launch
	std::cout << "Choice: ";
	int choice;

	// Throws exception if input is not valid
	try
	{
		std::cin >> choice;

		// Exits the program if the user wrote 0
		if (choice == 0)
			return 0;

		// Check if the choosen number is bigger than the maximum
		else if (!(choice < sections.size()) || std::cin.fail())
		{
			throw std::range_error("Input non valid, exiting.\n\n");
		}
	}
	catch (const std::exception &err)
	{
		std::cout << err.what();
		system("pause");	// "Press a key to continue . . . "
		return 1;
	}

	// Puts chosen section in selection
	selection = (char*)sections.at(choice).pItem;

	// Gets the key of the section contained in selection and puts it in key
	CSimpleIni::TNamesDepend IKey;
	ini.GetAllKeys(selection, IKey);
	std::vector<CSimpleIni::Entry> key;
	key.push_back(IKey.front());

	// Gets relatives
	const std::string relative = ini.GetValue("Settings", "relativepath", NULL);
	const std::string x86relative = ini.GetValue("Settings", "relativepathX86", NULL);

	// Gets the value of the key
	std::string keyvalue = ini.GetValue(selection, key.at(0).pItem, NULL);

	// Matching

	// Launches program from "Program Files"
	if (strcmp(key.at(0).pItem, "path") == 0)
	{
		matcher(keyvalue, relative);
	}

	// Launches program from "Program Files (x86)"
	else if (strcmp(key.at(0).pItem, "x86path") == 0)
	{
		matcher(keyvalue, x86relative);
	}

	// Launches program from custom path
	else if (strcmp(key.at(0).pItem, "cpath") == 0)
	{
		matcher(keyvalue);
	}

	// Launches url in your default browser
	else if (strcmp(key.at(0).pItem, "url") == 0)
	{
		// If the url contains "http://" or "https://" it just opens it
		if (keyvalue.compare(0, 7, "http://") == 0 || keyvalue.compare(0, 8, "https://") == 0)
		{
			openApplication(keyvalue.c_str());
		}

		else
		{
			keyvalue.insert(0, "http://");
			openApplication(keyvalue.c_str());
		}
	}

	// You put shit instead of path, x86path, cpath, or url. Shame on you
	else
	{
		std::cout << "Key in config.ini not valid, check your configuration file.\n\n";
		system("pause");
		return 1;
	}

	return 0;	 // Goodbye
}