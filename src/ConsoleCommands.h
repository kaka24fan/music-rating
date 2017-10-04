/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

#pragma once

#include "typedefs.h"
#include "ProgramState.h"

#include <vector>
#include <map>

typedef enum primaryCommand 
{
	PC_INVALIDCOMMAND,
	PC_HELP,
	PC_LOGIN,
	PC_CREATE,
	PC_DELETE,
	PC_ADDTO,
	PC_REMOVE,
	PC_RATE,
} PrimaryCommand;

typedef enum secondaryCommand
{
	SC_INVALIDCOMMAND,
	SC_ALBUM,
	SC_ARTIST,
	SC_PERFORMER,
	SC_SONG,
	SC_USER,
} SecondaryCommand;

static std::map<PrimaryCommand, String> PCToString =
{
	{ PC_HELP,				String(L"help") },
	{ PC_LOGIN,				String(L"login") },
	{ PC_CREATE,			String(L"create") },
	{ PC_DELETE,			String(L"delete") },
	{ PC_ADDTO,				String(L"addto") },
	{ PC_REMOVE,			String(L"remove") },
	{ PC_RATE,				String(L"rate") },
};

static std::map<SecondaryCommand, String> SCToString =
{
	{ SC_ALBUM,				String(L"album") },
	{ SC_ARTIST,			String(L"artist") },
	{ SC_PERFORMER,			String(L"performer") },
	{ SC_SONG,				String(L"song") },
	{ SC_USER,				String(L"user") },
};

static std::map<ItemType, String> ITToString =
{
	{ IT_ALBUM,				String(L"album") },
	{ IT_ARTIST,			String(L"artist") },
	{ IT_SONG,				String(L"song") },
	{ IT_USER,				String(L"user") },
};

static std::map<PrimaryCommand, std::vector<unsigned int>> PCToNumParams =
{
	{ PC_HELP,				std::vector<unsigned int> {0, 1} },
	{ PC_LOGIN,				std::vector<unsigned int> {1} },
};


PrimaryCommand GetPrimaryCommandFromStringApprox(String approximation);
SecondaryCommand GetSecondaryCommandFromStringApprox(String approximation);
ItemType GetItemTypeFromStringApprox(String approximation);

void checkNumOfParams(PrimaryCommand pc, std::vector<String> userInput);

void execute(std::vector<String> userInput);
