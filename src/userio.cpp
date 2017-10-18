#include "userio.h"

#include <iostream>
#include <Windows.h>

#include "TypeId.h"
#include "INamedItem.h"
#include "ProgramState.h"

String getPromptString()
{
	TypeId activeUserId = ProgramState::i()->getActiveUser();
	Name activeUserName = (activeUserId.getValue() == 0) ? L"guest" : INamedItem::readName(activeUserId);
	return activeUserName;
}

String promptForInput()
{
	String lineOfInput;
	std::wcout << "\n\n";

	// Set text color to light green:
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	
	std::wcout << getPromptString();
	std::wcout << "> ";

	// Set text color back to standard white:
	SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

	std::getline(std::wcin, lineOfInput);
	// no need to trim the "guest> " off?
	return lineOfInput;
}

String outputThenGetline(String output)
{
	std::wcout << output << "\n";
	return promptForInput();
}