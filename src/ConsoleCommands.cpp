#include "ConsoleCommands.h"

#include "ProgramState.h"
#include "stringutils.h"

#include <iostream>

PrimaryCommand GetPrimaryCommand(String approximation)
{
	for (auto kvp : PCToString)
	{
		if (StringDistance(approximation, kvp.second) == 0)
			return kvp.first;
	}
	return PC_INVALIDCOMMAND;
}

SecondaryCommand GetSecondaryCommand(String approximation)
{
	for (auto kvp : SCToString)
	{
		if (StringDistance(approximation, kvp.second) == 0)
			return kvp.first;
	}
	return SC_INVALIDCOMMAND;
}

/*
Only valid item types this function may return are Album, Artist, Song, User.
 */
ItemType GetItemType(String approximation)
{
	for (auto kvp : ITToString)
	{
		if (StringDistance(approximation, kvp.second) == 0)
			return kvp.first;
	}
	return IT_INVALID;
}

void checkNumOfParams(PrimaryCommand pc, std::vector<String> userInput)
{
	if (PCToNumParams.find(pc) == PCToNumParams.end())
		return; // No constraints were put, allow progress.

	unsigned int fromUser = (unsigned int) userInput.size() - 1;
	for (unsigned int acceptedSize : PCToNumParams.at(pc))
	{
		if (fromUser == acceptedSize)
		{
			return;
		}
	}

	// Display user-error message.
	std::wcout << "\nIncorrect number of parameters (" << fromUser << ")for command \'" << PCToString.at(pc) << "\', should be one of the following: ";
	for (unsigned int acceptedSize : PCToNumParams.at(pc))
	{
		std::wcout << acceptedSize << ", ";
	}
	std::wcout << ".";
}

void execute(std::vector<String> userInput)
{
	PrimaryCommand pc = GetPrimaryCommand(userInput.at(0));
	checkNumOfParams(pc, userInput);

	switch (pc)
	{
	case PC_INVALIDCOMMAND:
		std::wcout << "\nThere's no command matching \'" << userInput.at(0) << "\'. Try again or type in \'help\' for a list of available commands.";
		break;
	case PC_HELP:
		std::wcout << "\nHelp hasn't been implemented yet. Help yourself.";
		break;
	case PC_LOGIN:
	{
		String userName = userInput.at(1);
		TypeId userId = File::i()->findItemWithName(userName, 0);
		if (userId.getValue() == 0)
		{
			std::wcout << "\nThere's no user \'" << userName << "\'. Try again or type in \'help\' for a list of available commands.";
		}
		else
		{
			ProgramState::i()->logUserIn(userId);
			std::wcout << "\nLogin successful.";
		}
	}
		break;
	case PC_CREATE:
	{
		String itemTypeStr = userInput.at(1);
		Name itemNameStr = userInput.at(2);

		ItemType itemType = GetItemType(itemTypeStr);
		if (itemType == IT_INVALID)
		{
			std::wcout << "\n\'" << itemTypeStr << "\' can not be used in this context. You can only create one of the following: Album, Artist, Song, User.";
		}
	}
		break;
	case PC_DELETE:
		std::wcout << "\nHelp hasn't been implemented yet. Help yourself.";
		break;
	case PC_ADD:
		std::wcout << "\nHelp hasn't been implemented yet. Help yourself.";
		break;
	case PC_REMOVE:
		std::wcout << "\nHelp hasn't been implemented yet. Help yourself.";
		break;
	case PC_RATE:
	{
		if (!ProgramState::i()->isAnyUserLoggedIn)
		{
			std::wcout << "\nSome user must be logged in to rate an item.";
			return;
		}
	}
		break;
	default:

		break;
	}
}
