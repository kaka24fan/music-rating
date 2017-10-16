/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

#include "ConsoleCommands.h"

#include "ProgramState.h"
#include "stringutils.h"
#include "userio.h"

#include "User.h"
#include "File.h"

#include <iostream>



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

bool checkNumOfParams(PrimaryCommand pc, std::vector<String> userInput)
{
	if (PCToNumParams.find(pc) == PCToNumParams.end())
		return true; // No constraints were put, allow progress.

	unsigned int fromUser = (unsigned int) userInput.size() - 1; // -1 for the primary command that must have been input too
	for (unsigned int acceptedSize : PCToNumParams.at(pc))
	{
		if (fromUser == acceptedSize)
		{
			return true;
		}
	}

	// Display user-error message.
	std::wcout << "\nIncorrect number of parameters (" << fromUser << ") for command \'" << PCToString.at(pc) << "\', should be one of the following: ";
	
	bool thisIsFirstItem = true; // for pretty printing purposes
	for (unsigned int acceptedSize : PCToNumParams.at(pc))
	{
		if (thisIsFirstItem)
		{
			thisIsFirstItem = false;
			std::wcout << acceptedSize;
		}
		else
		{ 
			std::wcout << ", " << acceptedSize;
		}
	}
	std::wcout << ".";

	return false;
}

void execute(std::vector<String> userInput)
{
	PrimaryCommand pc = GetPrimaryCommandFromStringApprox(userInput.at(0));
	if (!checkNumOfParams(pc, userInput))
		return;

	switch (pc)
	{
	case PC_INVALIDCOMMAND:
		std::wcout << "\nThere's no command matching \'" << userInput.at(0) << "\'. Try again or type in \'help\' for a list of available commands.";
		break;
	case PC_HELP:
	{
		if (userInput.size() == 1)
		{   // just list the commands
			std::wcout << "\nExample commands:";
			std::wcout << "\n";
			std::wcout << "\nHelp" << "                            <--- " << "Displays a list of example commands.";
			std::wcout << "\nHelp Create" << "                     <--- " << "Helps you with a specific command, e.g. Create.";
			std::wcout << "\n";
			std::wcout << "\nLogin \"Jasper\"" << "                  <--- " << "Logs you into a specific user's account.";
			std::wcout << "\n";
			std::wcout << "\nCreate Album \"Recovery\"" << "         <--- " << "Creates a new album with the given name.";
			std::wcout << "\nCreate Artist \"Mac Miller\"" << "      <--- " << "Creates a new artist with the given name.";
			std::wcout << "\nCreate Song \"Forgot About Dre\"" << "  <--- " << "Creates a new song with the given name.";
			std::wcout << "\nCreate User \"Jasper\"" << "            <--- " << "Creates a new user with the given name.";
			std::wcout << "\n";
			std::wcout << "\nAddTo Album \"Recovery\"" << "          <--- " << "Adds a new song to an album. Will request extra input during execution.";
			std::wcout << "\nAddTo Song \"No Love\"" << "            <--- " << "Adds a new artist to a song. Will request extra input during execution.";
			std::wcout << "\n";
			std::wcout << "\nView Album \"Recovery\"" << "           <--- " << "Shows information about the album with the given name.";
			std::wcout << "\nView Artist \"Mac Miller\"" << "        <--- " << "Shows information about the artist with the given name.";
			std::wcout << "\nView Song \"Forgot About Dre\"" << "    <--- " << "Shows information about the song with the given name.";
			std::wcout << "\nView User \"Jasper\"" << "              <--- " << "Shows information about the user with the given name.";
		}
		else if (userInput.size() == 2)
		{
			String help_pc_str = userInput.at(1);
			PrimaryCommand help_pc = GetPrimaryCommandFromStringApprox(help_pc_str);
			switch (help_pc)
			{
			case PC_INVALIDCOMMAND:
				std::wcout << "\nCould not recognize command \'" << help_pc_str << "\'. Try again.";
				break;
			case PC_HELP:
				std::wcout << "Type \'help\' to list commands or \'help <command-name>\' to get help on a specific command";
				break;
			default:
				std::wcout << "Help for the command \'" << help_pc_str << "\' has not been implemented yet. Try again later.";
			}
		}
	}
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
	/*
	AddTo Song "Kamienie i mury" Producer "Zeus"
	AddTo Album "Zeus. Jest super." "Orinoko flow"
	*/
	case PC_ADDTO: 
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

		if (itemType == IT_ALBUM) // e.g. AddTo Song "Kamienie i mury" Producer "Zeus"
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
		else if (itemType == IT_SONG) // e.g. AddTo Song "Kamienie i mury" Producer "Zeus"
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
		// TODO ...
	}
		break;
	case PC_VIEW: // View Song "Gwiazdy"
	{
		String itemTypeStr = userInput.at(1);
		Name itemNameStr = userInput.at(2);

		ItemType itemType = GetItemTypeFromStringApprox(itemTypeStr);

		if (itemType != IT_ALBUM
			&& itemType != IT_ARTIST
			&& itemType != IT_SONG
			&& itemType != IT_USER)
		{
			std::wcout << "\n\'" << itemTypeStr << "\' can not be used in this context. You can only view one of the following: Album, Artist, Song, User.";
			break;
		}

		TypeId itemId = File::i()->findItemWithNameAndItemType(itemNameStr, itemType, 0);
		if (itemId.getValue() == 0)
		{
			std::wcout << "\nNo " << itemTypeStr << " with name \'" << itemNameStr << "\', try again.";
			break;
		}

		switch (itemType)
		{
		case IT_ALBUM:
		{
			std::wcout << "\nViewing albums hasn't been implemented yet. Sorry.";
		}
		break;
		case IT_ARTIST:
		{
			std::wcout << "\nViewing artists hasn't been implemented yet. Sorry.";
		}
		break;
		case IT_SONG:
		{
			std::wcout << "\nViewing songs hasn't been implemented yet. Sorry.";
		}
		break;
		case IT_USER:
		{
			std::wcout << "\nUser id:         " << itemId.getValue();
			std::wcout << "\nUser name:       " << INamedItem::readName(itemId); // we already have this name from user input but use this as a double check for consistency
			std::wcout << "\nFavourite songs: " << "_to be implemented_";
		}
		break;
		} // end switch (itemType)
	}
		break;
	default:

		break;
	}
}
