/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

#include "ConsoleCommands.h"

#include "ProgramState.h"
#include "stringutils.h"

#include "User.h"
#include "File.h"

#include <iostream>

String outputThenGetline(String output)
{
	std::wcout << output << "\n";
	String lineOfInput;
	std::getline(std::wcin, lineOfInput);
	return lineOfInput;
}

PrimaryCommand GetPrimaryCommandFromStringApprox(String approximation)
{
	for (auto kvp : PCToString)
	{
		if (StringDistance(approximation, kvp.second) == 0)
			return kvp.first;
	}
	return PC_INVALIDCOMMAND;
}

SecondaryCommand GetSecondaryCommandFromStringApprox(String approximation)
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
ItemType GetItemTypeFromStringApprox(String approximation)
{
	for (auto kvp : ITToString)
	{
		if (StringDistance(approximation, kvp.second) == 0)
			return kvp.first;
	}
	return IT_INVALID;
}

FlagType GetFlagTypeFromStringApprox(String approximation)
{
	for (auto kvp : FTToString)
	{
		if (StringDistance(approximation, kvp.second) == 0)
			return kvp.first;
	}
	return FT_INVALID;
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
	PrimaryCommand pc = GetPrimaryCommandFromStringApprox(userInput.at(0));
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
		TypeId userId = File::i()->findItemWithNameAndItemType(userName, ItemType::IT_USER, 0);
		if (userId.getValue() == 0)
		{
			std::wcout << "\nThere's no user \'" << userName << "\'. Try again or type in \'help\' for a list of available commands.";
		}
		else
		{
			ProgramState::i()->logUserIn(userId);
			std::wcout << "\nLogin successful.";
		}
		// DONE.
	}
		break;
	case PC_CREATE:
	{
		String itemTypeStr = userInput.at(1);
		Name itemNameStr = userInput.at(2);

		ItemType itemType = GetItemTypeFromStringApprox(itemTypeStr);

		if (itemType != IT_ALBUM
			&& itemType != IT_ARTIST
			&& itemType != IT_SONG
			&& itemType != IT_USER)
		{
			std::wcout << "\n\'" << itemTypeStr << "\' can not be used in this context. You can only create one of the following: Album, Artist, Song, User.";
		}
		else
		{
			if (File::i()->findItemWithNameAndItemType(itemNameStr, itemType, 0).getValue() != 0)
			{
				String itemTypeStr = itemTypeToString(itemType);
				std::wcout << "\nThere already exists a " << itemTypeStr << "with name " << itemNameStr << ". Pick a different name";
				break;
			}
			else
			{
				TypeId newId = ProgramState::i()->generateId(itemType);
				IItem::assignAddress(newId);
				INamedItem::writeName(newId, itemNameStr);
			}
		}
		// DONE.
	}
		break;
	case PC_DELETE:
		std::wcout << "\nDelete hasn't been implemented yet. Help yourself.";
		break;
	case PC_ADDTO: // AddTo Song "Kamienie i mury" Producer "Zeus"
	{
		String itemTypeStr = userInput.at(1);
		Name itemNameStr = userInput.at(2);

		ItemType itemType = GetItemTypeFromStringApprox(itemTypeStr); 
		
		if (itemType != IT_ALBUM
			&& itemType != IT_SONG)
		{
			std::wcout << "\n\'" << itemTypeStr << "\' can not be used in this context. You can only add to one of the following: Album, Song.";
			break;
		}

		TypeId majorItemId = File::i()->findItemWithNameAndItemType(itemNameStr, itemType, 0);
		if (majorItemId.getValue() == 0)
		{
			std::wcout << "\nNo " << (itemType == IT_ALBUM) ? "album" : "song";
			std::wcout << " titled " << itemNameStr << " exists. Create it first.";
			break;
		}

		if (itemType == IT_ALBUM)
		{
			String output = L"\nWhich song would you like to add?";
			String songNameStr = outputThenGetline(output);

			TypeId songId = File::i()->findItemWithNameAndItemType(songNameStr, ItemType::IT_SONG, 0);
			if (songId.getValue() == 0)
			{
				std::wcout << "\nNo such song, try again.";
				break;
			}

			if (INamedItem::containsMemberId_WithFlagComparison(majorItemId, songId))
			{
				std::wcout << "\nThis song is already present. Try again.";
				break;
			}

			//Successful user I/O.

			INamedItem::writeMemberId(majorItemId, songId);

			std::wcout << "\nSuccess!";
		}
		else if (itemType == IT_SONG)
		{
			String output = L"\nWho would you want to add? Possibilities: Lead, Featuring, Producer.";
			String flagTypeStr = outputThenGetline(output);
			FlagType flagType = GetFlagTypeFromStringApprox(flagTypeStr);
			if (flagType == FT_INVALID)
			{
				std::wcout << "\nInvalid flag type. Try again and use Lead, Featuring or Producer.";
				break;
			}

			// Have flag type, now get name.
			output = L"\nWhat is the name of the artist?";
			String artistNameStr = outputThenGetline(output);

			TypeId artistId = File::i()->findItemWithNameAndItemType(artistNameStr, ItemType::IT_ARTIST, 0);
			if (artistId.getValue() == 0)
			{
				std::wcout << "\nNo such artist, try again.";
				break;
			}

			//Successful user I/O.
			artistId.setFlags(flagType);

			if (INamedItem::containsMemberId_WithFlagComparison(majorItemId, artistId))
			{
				std::wcout << "\nThis artist at this role is already present. Try again.";
				break;
			}
			INamedItem::writeMemberId(majorItemId, artistId);

			std::wcout << "\nSuccess!";
		}

	}
		break;
	case PC_REMOVE:
		std::wcout << "\nRemove hasn't been implemented yet. Help yourself.";
		break;
	case PC_RATE:
	{
		if (!ProgramState::i()->isAnyUserLoggedIn())
		{
			std::wcout << "\nSome user must be logged in to rate an item.";
			return;
		}
	}
		break;
	case PC_VIEW:
		std::wcout << "\nView hasn't been implemented yet. Help yourself.";
		break;
	default:

		break;
	}
}
