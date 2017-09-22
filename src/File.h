#pragma once

#include "stdafx.h"

class TypeId;
class TypeName;

class File
{
public:
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
	void initializePage(PageIndex pageIndex, Id id);

	bool getFirstDataBitOfPage(Address& result, PageIndex pageIndex);

	TypeId findItemWithName(String name, unsigned int maxStringDist);
	
	static File* i();

private:
	static File* m_instance;
	File();
	
	Byte* m_bytes = nullptr;
	Address m_bitCount = 0;
	Address m_nextBitToRW = 0;

	bool isPageFree(PageIndex index);
	bool isPageAFirstPage(PageIndex index);
	TypeId readPageItemId(PageIndex index);
	TypeName readPageItemName(PageIndex index);
	bool isAddressInsidePageData(Address a);
	bool getContinuationPage(PageIndex& result, PageIndex pageIndex);
	void setContinuationPagePointer(PageIndex origin, PageIndex continuation);
	Address getFirstBitOfTheNextPagePointer(PageIndex pageIndex);
	unsigned int getSizeOfNextPagePointer();
};