#include "userio.h"

#include <iostream>

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
	std::wcout << getPromptString() << "> ";
	std::getline(std::wcin, lineOfInput);
	// trim the "guest> " off?
	return lineOfInput;
}

String outputThenGetline(String output)
{
	std::wcout << output << "\n";
	return promptForInput();
}