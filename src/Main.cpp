#include "stdafx.h"

#include <iostream>
#include <string>

#include "stringutils.h"
#include "ConsoleCommands.h"
#include <windows.h>

#include "TypeId.h"



int main(int argc, char *argv[])
{
	// SETUP ----------------------------------------------------------------------------------------------------------
	SetConsoleCP(852); // enables Polish characters
	
	// STATE ----------------------------------------------------------------------------------------------------------
	String lineOfInput;

	// MAIN LOOP ------------------------------------------------------------------------------------------------------
	while (true)
	{
		std::getline(std::wcin, lineOfInput);
		for (auto ch : lineOfInput)
		{
			std::wcout << (int)ch << std::endl;
		}

		std::vector<String> splitResult = split(lineOfInput, L' ');

		execute(splitResult);
	}
}