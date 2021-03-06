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


	// TESTING --------------------------------------------------------------------------------------------------------

	while (true)
		break;

	// MAIN LOOP ------------------------------------------------------------------------------------------------------
	while (true)
	{
		File::i()->debug_pageInformation();


		std::wcout << "\n\n=========> BEGIN DEBUG - Binary file contents: <=========\n";
		std::wcout << "\n" << File::i()->debug_prettyBinaryContents();
		std::wcout << "\n=========> END OF DEBUG - Binary file contents: <=========\n";

		lineOfInput = promptForInput();

		std::vector<String> splitResult = split(lineOfInput, L' ');

		execute(splitResult);
	}
}