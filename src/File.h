/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

#pragma once

#include "stdafx.h"
#include "IType.h"

class TypeId;
class TypeName;

class File
{
public:

	void appendNewPage();

	void writeBit(Address a, bool bit);
	bool readBit(Address a);

	void seek(Address a);
	void writeNextBit(bool bit);
	bool readNextBit(bool& result);
	Address getNextBitToRW();

	void loadFromDisk(std::string path);
	void SaveToDisk(std::string path);


	void writeAlignedByte(Address a, Byte data);
	Byte readAlignedByte(Address a);
	void writeNextAlignedByte(Byte data);
	Byte readNextAlignedByte();

	PageIndex getNextFreePage();
	bool getPageContainingId(PageIndex& result, TypeId id);
	
	void invalidatePage(PageIndex pageIndex);
	void initializePage(PageIndex pageIndex);
	void initializePage(PageIndex pageIndex, TypeId id);

	PageIndex getLastUsedPageIndex();

	bool getFirstDataBitOfPage(Address& result, PageIndex pageIndex);
	bool isPageAFirstPage(PageIndex index);
	TypeId readPageItemId(PageIndex index);

	TypeId findAnyItemWithName(String name, unsigned int maxStringDist);
	TypeId findItemWithNameAndItemType(String name, ItemType itemType, unsigned int maxStringDist);
	
	static File* i();

private:
	static File* m_instance;
	File();
	
	std::vector<Byte> m_bytes = std::vector<Byte>{};
	Address m_bitCount();
	Address m_nextBitToRW = 0;

	void appendNewByte(Byte b);

	bool isPageFree(PageIndex index);
	TypeName readPageItemName(PageIndex index);
	bool isAddressInsidePageData(Address a);
	bool getContinuationPage(PageIndex& result, PageIndex pageIndex);
	void setContinuationPagePointer(PageIndex origin, PageIndex continuation);
	Address getFirstBitOfTheNextPagePointer(PageIndex pageIndex);
	unsigned int getSizeOfNextPagePointer();
};