/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

#include "stdafx.h"

#include <iostream>
#include <string>

#include "stringutils.h"
#include "userio.h"
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
		lineOfInput = promptForInput();

		std::vector<String> splitResult = split(lineOfInput, L' ');

		execute(splitResult);
	}
}