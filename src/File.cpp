/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

#include "File.h"

#include "TypeId.h"
#include "TypeName.h"
#include "stringutils.h"

#ifdef MY_DEBUG
#include <iostream>
#include <sstream>
#endif

File* File::m_instance = nullptr;

void File::appendNewByte(Byte b)
{
	m_bytes.push_back(b);
}

void File::appendNewPage()
{
	unsigned int pageSizeInBytes = (1 << LOG2_OF_PAGE_SIZE_IN_BITS) >> 3;
	for (unsigned int i = 0; i < pageSizeInBytes; i++)
	{
		appendNewByte(0);
	}
}

void File::writeBit(Address a, bool bit)
{
	assert(a < m_bitCount());
	if (bit)
		m_bytes[a / 8] |= (1 << (7-(a % 8)));
	else
		m_bytes[a / 8] &= ~(1 << (7-(a % 8))); 
}

bool File::readBit(Address a)
{
	assert(a < m_bitCount());
	return (m_bytes[a / 8] >> (7-(a % 8))) & 1;
}

void File::seek(Address a)
{
	m_nextBitToRW = a;
}

/*
Writes the bit where the file pointer currently points. Then increments the file pointer.
*/
void File::writeNextBit(bool bit)
{
	assert(m_nextBitToRW < m_bitCount());
	writeBit(m_nextBitToRW++, bit);
}

/*
Reads the bit where the file pointer currently points. Then increments the file pointer.
*/
bool File::readNextBit()
{
	assert(m_nextBitToRW < m_bitCount());
	return readBit(m_nextBitToRW++);
}

/*
Prerequsite: The file pointer points inside item data (i.e. not page metadata).

Writes the bit where the file pointer currently points. 
Then moves the file pointer to the next bit belonging to the item currently that was just written to.
If there is no such bit (i.e. was reading the last bit of last page), a new page is claimed for that item
and the file pointer is set to its first data bit. (see definition of getNextBitInAnItem)
*/
void File::writeNextItemBit(bool bit)
{
	// Can't use writeNextBit because it would increment the file pointer!
	writeBit(m_nextBitToRW, bit);

	m_nextBitToRW = getNextBitInAnItem(m_nextBitToRW);
}

/*
Prerequsite: The file pointer points inside item data (i.e. not page metadata).

Reads the bit where the file pointer currently points.
Then moves the file pointer to the next bit belonging to the item that was just read.
If there is no such bit (i.e. was reading the last bit of last page), a new page is claimed for that item
and the file pointer is set to its first data bit. (see definition of getNextBitInAnItem)
*/
bool File::readNextItemBit()
{
	// Can't use writeNextBit because it would increment the file pointer!
	bool result = readBit(m_nextBitToRW);

	m_nextBitToRW = getNextBitInAnItem(m_nextBitToRW);
	return result;
}

/*
Returns the (unique) 'file pointer'.
*/
Address File::getNextBitToRW()
{
	return m_nextBitToRW;
}

/*
Clears the valid bit.
*/
void File::invalidatePage(PageIndex pageIndex)
{
	Address pageStart = pageIndex << LOG2_OF_PAGE_SIZE_IN_BITS;
	writeBit(pageStart, false);
}

/*
Sets the valid bit and clears all other bits of the page.
*/
void File::initializePage(PageIndex pageIndex)
{
	if (pageIndex > getLastUsedPageIndex() + 1)
	{
		assert(false); // why would you skip a page?
	}
	else if (pageIndex == getLastUsedPageIndex() + 1) // we have to append a new page
	{
		appendNewPage();
		// write the valid bit:
		Address pageStart = pageIndex << LOG2_OF_PAGE_SIZE_IN_BITS;
		writeBit(pageStart, true);
	}
	else
	{
		// write the valid bit:
		Address pageStart = pageIndex << LOG2_OF_PAGE_SIZE_IN_BITS;
		writeBit(pageStart, true);
		// clear other bits:
		for (unsigned int i = 1; i < (1 << LOG2_OF_PAGE_SIZE_IN_BITS); i++)
		{
			writeBit(pageStart + i, false);
		}
	}
}

/*
Sets the valid bit and the owner id, clears all other bits of the page.
*/
void File::initializePage(PageIndex pageIndex, TypeId id)
{
	initializePage(pageIndex);

	//set the firstPageBit:
	Address firstPageIndicatorBit = (pageIndex << LOG2_OF_PAGE_SIZE_IN_BITS) + 1;
	writeBit(firstPageIndicatorBit, true);

	Address lastBitOfId;
	getFirstDataBitOfPage(lastBitOfId, pageIndex);
	lastBitOfId--; // after this line, variable's content matches the name lastBitOfId

	Id idVal = id.getValue();
	for (unsigned int i = 0; i < sizeof(Id) * 8; i++)
	{
		writeBit(lastBitOfId--, (idVal & 1));
		idVal >>= 1;
	}
}

/*
Returns a nonnegative number as described by function name.
*/
PageIndex File::getLastUsedPageIndex()
{
	return (m_bitCount() / (1 << LOG2_OF_PAGE_SIZE_IN_BITS)) - 1;
}

/*
Returns false iff the page is free, otherwise sets result to address of first data bit of the page.
*/
bool File::getFirstDataBitOfPage(Address& result, PageIndex pageIndex)
{
	if (isPageFree(pageIndex))
		return false;
	
	Address pageStart = pageIndex << LOG2_OF_PAGE_SIZE_IN_BITS;

	if (isPageAFirstPage(pageIndex))
	{
		result = pageStart + 2 + sizeof(Id) * 8;
		return true;
	}
	else
	{
		result = pageStart + 2;
		return true;
	}
}

/*
Returns 0 in case of fail.
*/
TypeId File::findAnyItemWithName(String name, unsigned int maxStringDist)
{
	PageIndex pageIndex = 0;
	while (true)
	{
		pageIndex++;
		if (!isPageAFirstPage(pageIndex))
			continue;
		
		TypeId itemId = readPageItemId(pageIndex);
		ItemType itemType = itemId.getItemType();

		// Only these 4 types are named.
		if (itemType != IT_ALBUM &&
			itemType != IT_ARTIST &&
			itemType != IT_SONG &&
			itemType != IT_USER)
		{
			continue;
		}
		
		TypeName itemName = readPageItemName(pageIndex);
		Name itemName_ = itemName.getValue();

		if (StringDistance(itemName_, name) <= maxStringDist)
		{
			return itemId;
		}
	}
	return TypeId(0);
}

/*
Returns 0 in case of fail.
*/
TypeId File::findItemWithNameAndItemType(String name, ItemType itemType, unsigned int maxStringDist)
{
	// Only these 4 types are named.
	if (itemType != IT_ALBUM &&
		itemType != IT_ARTIST &&
		itemType != IT_SONG &&
		itemType != IT_USER)
	{
		return TypeId(0);
	}
	PageIndex pageIndex = 0;
	while (true)
	{
		pageIndex++;
		if (pageIndex > File::i()->getLastUsedPageIndex())
			break;

		if (!isPageAFirstPage(pageIndex))
			continue;

		TypeId itemId = readPageItemId(pageIndex);
		ItemType itemType = itemId.getItemType();
		if (itemType != itemType)
		{
			continue;
		}

		TypeName itemName = readPageItemName(pageIndex);
		Name itemName_ = itemName.getValue();

		if (StringDistance(itemName_, name) <= maxStringDist)
		{
			return itemId;
		}
	}
	return TypeId(0);
}

#ifdef MY_DEBUG // <--------------------------------------------------------------- DEBUGGING START ---|

/*
This function is only available if MY_DEBUG is defined.
*/
String File::debug_binaryContents()
{
	String result = String{};
	for (Byte byte : m_bytes)
	{
		String buffToReverseWith = String{};
		for (unsigned int i = 0; i < 8; i++)
		{
			buffToReverseWith.append((byte & 1) ? L"1" : L"0");
			byte >>= 1;
		}
		for (unsigned int i = 0; i < 8; i++)
		{
			result.push_back(buffToReverseWith.at(7 - i));
		}
	}
	return result;
}

/*
This function is only available if MY_DEBUG is defined.
*/
String File::debug_prettyBinaryContents()
{
	PageIndex lastUsedPageIndex = getLastUsedPageIndex();
	std::wstringstream stream{};
	String raw = debug_binaryContents();

	stream << raw << "\n\n";

	for (PageIndex pi = 0; pi <= lastUsedPageIndex; pi++)
	{
		Address firstBitOfTheNextPagePointer = getFirstBitOfTheNextPagePointer(pi);
		Address pageOffsetIntoRawString = (pi << LOG2_OF_PAGE_SIZE_IN_BITS);
		
		stream << "\n---Page #" << pi << ":";
		stream << "\nfree=" << (raw.at(pageOffsetIntoRawString + 0) == L'1') ? "0" : "1";
		stream << ", first=" << (raw.at(pageOffsetIntoRawString + 1) == L'1') ? "1" : "0";

		unsigned int pos = pageOffsetIntoRawString + 2;

		if (raw.at(pageOffsetIntoRawString + 1) == L'1') // i.e. the page is a first page, so it has an owner id
		{
			stream << ", ownerId=" << raw.substr(pageOffsetIntoRawString + 2, sizeof(Id) * 8);
			pos += sizeof(Id) * 8;
		}
		stream << ",\ndata=";
		for (unsigned int counter = 0; pos < firstBitOfTheNextPagePointer; pos++)
		{
			stream << raw.at(pos);
			counter++;
			if (counter == 8 && pos != firstBitOfTheNextPagePointer-1) // 2nd condition only so that we don't finish with an underscore.
			{
				counter = 0;
				stream << L'_'; // separate bytes of data with underscores
			}
		}

		Address firstBitOfTheNextPage = (pi+1) << LOG2_OF_PAGE_SIZE_IN_BITS;
		unsigned int pageSizeInBits = 1 << LOG2_OF_PAGE_SIZE_IN_BITS;
		unsigned int bitsOfPagePointer = firstBitOfTheNextPage - firstBitOfTheNextPagePointer;
		stream << ",\nnextPageIndex=" << raw.substr(pageOffsetIntoRawString + (pageSizeInBits - bitsOfPagePointer), bitsOfPagePointer);

		stream << "\n";
	}
	return stream.str();
}

/*
This function is only available if MY_DEBUG is defined.
*/
void File::debug_pageInformation()
{
	std::wcout << "\n\n=========> BEGIN DEBUG - Page Information: <=========\n";
	Address pageSizeInBits = 1 << LOG2_OF_PAGE_SIZE_IN_BITS;

	// Check if page 0 is marked not free, but otherwise empty:
	bool pageZeroIsNotFree = readBit(0);
	std::wcout << (pageZeroIsNotFree ? "\nOk, page #0 is marked non free." : "Problem, page #0 is marked free.");
	bool pageZeroIsEmpty = true;
	for (Address a = 1; a < pageSizeInBits; a++)
		pageZeroIsEmpty = pageZeroIsEmpty && !readBit(a);
	std::wcout << (pageZeroIsEmpty ? "\n\nOk, page #0 is empty." : "Problem, page #0 is NOT empty.");

	// What is the last page index in use?
	PageIndex lastUsedPageIndex = getLastUsedPageIndex();
	std::wcout << "\n\nPage indices in use: #0 - #" << lastUsedPageIndex << " (inclusive).";

	for (PageIndex i = 1; i <= lastUsedPageIndex; i++)
	{
		std::wcout << "\n\n---Page #" << i << ":";

		bool pageIsFree = isPageFree(i);
		std::wcout << (pageIsFree ? "\nPage is free." : "\nPage is NOT free.");

		bool pageIsFirst = isPageAFirstPage(i);
		std::wcout << (pageIsFirst ? "\nPage is the first page of an item." : "\nPage is NOT the first page of an item.");

		if (pageIsFirst)
		{
			TypeId ownerId = readPageItemId(i);
			std::wcout << "\nPage is owned by id " << ownerId.getValue() << ".";

			ItemType ownerType = ownerId.getItemType();
			std::wcout << "\nThe owner's type is " << itemTypeToString(ownerType) << ".";

			FlagType flagType = ownerId.getFlags();
			std::wcout << "\nThe owner's flags are " << flagTypeToString(flagType) << ".";
		}
		// TODO: Think what things are unexpected and list them at the end.
	}

	std::wcout << "\n\n=========> END OF DEBUG - Page Information: <=========\n";
}
#endif // <-------------------------------------------------------------------------- DEBUGGING END ---|

/*
Returns true iff the address given points to a bit of actual data, not page metadata.
*/
bool File::isAddressInsidePageData(Address a)
{
	PageIndex thatPageIndex = a >> LOG2_OF_PAGE_SIZE_IN_BITS;
	if (isPageFree(thatPageIndex))
	{
		// we're in a free page
		return false;
	}

	Address firstDataBit = 0;
	getFirstDataBitOfPage(firstDataBit, thatPageIndex);
	if (a < firstDataBit)
	{
		// we're too early
		return false;
	}
	if (getFirstBitOfTheNextPagePointer(thatPageIndex) <= a)
	{
		// we're too late
		return false;
	}

	return true;
}

/*
Returns failure iff the page has no continuation page. Otherwise sets result to its pageIndex.
*/
bool File::getContinuationPage(PageIndex& result, PageIndex pageIndex)
{
	result = 0;

	Address firstBitOfNextPage = (pageIndex + 1) << LOG2_OF_PAGE_SIZE_IN_BITS;
	Address firstBitOfTheNextPagePointer = getFirstBitOfTheNextPagePointer(pageIndex);

	for (Address a = firstBitOfTheNextPagePointer; a < firstBitOfNextPage; a++)
	{
		result = (result << 1) + (readBit(a) ? 1 : 0);
	}

	return result != 0; // if result is 0, that means there's no continuation page, so return failure. Otherwise, result's ready.
}

/*
Sets the continuation page pointer of the origin page to point to the continuation page.
*/
void File::setContinuationPagePointer(PageIndex origin, PageIndex continuation)
{
	Address lastBitOfTheNextPagePointer = ((origin + 1) << LOG2_OF_PAGE_SIZE_IN_BITS) - 1;
	for (Address a = lastBitOfTheNextPagePointer; a >= getFirstBitOfTheNextPagePointer(origin); a--)
	{
		writeBit(a, (bool)(continuation & 1));
		continuation >>= 1;
	}
}

/*
Returns address of first bit of this page's "next page pointer".
*/
Address File::getFirstBitOfTheNextPagePointer(PageIndex pageIndex)
{
	Address firstBitOfNextPage = (pageIndex + 1) << LOG2_OF_PAGE_SIZE_IN_BITS;
	return firstBitOfNextPage - getSizeOfNextPagePointer();
}

/*
Returns the length in bits of the 'next page pointer'.
*/
unsigned int File::getSizeOfNextPagePointer()
{
	return sizeof(PageIndex)*8 - LOG2_OF_PAGE_SIZE_IN_BITS;
}

/*
Prerequsite: The 'previous' address lies inside item data (i.e. not page metadata).

Returns the next bit belonging to the item containing 'previous'.
If there is no such bit (i.e. was reading the last bit of last page), a new page is claimed for that item
and the file pointer is set to its first data bit.
*/
Address File::getNextBitInAnItem(Address previous)
{
	assert(isAddressInsidePageData(previous));

	previous++;
	if (!isAddressInsidePageData(previous))
	{
		PageIndex currPage = previous >> LOG2_OF_PAGE_SIZE_IN_BITS;
		PageIndex nextPage = 0;

		if (!getContinuationPage(nextPage, currPage))
		{
			// There's no continuation page set yet, create new one.
			nextPage = getNextFreePage();

			setContinuationPagePointer(currPage, nextPage);
			initializePage(nextPage);
		}

		// This assert should be impossible to fail - "nextPage" should not be free if we picked it to be the next page.
		// It does assign the correct value to 'previous' (passed by reference).
		assert(getFirstDataBitOfPage(previous, nextPage));
	}
	return previous;
}

/*
Writes a byte of data to the file, given that the address given is divisible by 8 (aligned).
*/
void File::writeAlignedByte(Address a, Byte data)
{
	assert(a % 8 == 0);
	for (int i = sizeof(Byte) * 8 - 1; i >= 0; i--)
	{
		writeBit(a + i, data & 1);
		data >>= 1;
	}
}

/*
Reads a byte of data from the file, given that the address given is divisible by 8 (aligned).
*/
Byte File::readAlignedByte(Address a)
{
	assert(a % 8 == 0);
	Byte result = 0;
	for (unsigned int i = 0; i < sizeof(Byte) * 8; i++)
	{
		result = (result >> 1) + (readBit(a + i) ? 1 : 0);
	}
	return result;
}

/*
Writes a byte of data to the file, starting wherever the file pointer currently is, given that its address is divisible by 8 (aligned).
*/
void File::writeNextAlignedByte(Byte data)
{
	assert(m_nextBitToRW % 8 == 0);
	for (int i = sizeof(Byte) * 8 - 1; i >= 0; i--)
	{
		writeNextBit(data & 1);
		data >>= 1;
	}
}

/*
Reads a byte of data from the file, starting wherever the file pointer currently is, given that its address is divisible by 8 (aligned).
*/
Byte File::readNextAlignedByte()
{
	assert(m_nextBitToRW % 8 == 0);
	Byte result = 0;
	for (unsigned int i = 0; i < sizeof(Byte) * 8; i++)
	{
		bool nextBit = readNextBit();
		result = (result >> 1) + (nextBit ? 1 : 0);
	}
	return result;
}

/*
Returns the page index of the leftmost free page in the entire file.
*/
PageIndex File::getNextFreePage()
{
	unsigned int pageSizeInBits = 1 << LOG2_OF_PAGE_SIZE_IN_BITS;
	PageIndex trial = 1;
	for ( ; (trial + 1)*pageSizeInBits <= m_bitCount(); trial++)
	{
		if (isPageFree(trial))
		{
			return trial;
		}
	}
	// We ran out of free pages, create a new one:
	appendNewPage();
	return trial;
}

/*
Returns failure iff there's no page owned by the id given. Otherwise returns that page's index.
*/
bool File::getPageContainingId(PageIndex & result, TypeId id)
{
	unsigned int pageSizeInBits = 1 << LOG2_OF_PAGE_SIZE_IN_BITS;
	for (PageIndex trial = 1; (trial + 1)*pageSizeInBits <= m_bitCount(); trial++)
	{
		if (TypeId::equalityCheck(readPageItemId(trial), id))
		{
			result = trial;
			return true;
		}
	}
	return false;
}

/*
Returns the instance of the singleton File class.
*/
File* File::i()
{
	if (!m_instance)
		m_instance = new File();
	
	return m_instance;
}

File::File()
	: m_nextBitToRW(0)
{
	// Initialize the 0th page:
	appendNewPage();
	writeBit(0, true);
}

Address File::m_bitCount()
{
	return (Address)(m_bytes.size() * 8);
}

bool File::isPageFree(PageIndex index)
{
	return !readBit(index << LOG2_OF_PAGE_SIZE_IN_BITS);
}

bool File::isPageAFirstPage(PageIndex index)
{
	Address pageStart = index << LOG2_OF_PAGE_SIZE_IN_BITS;
	return readBit(pageStart) && readBit(pageStart + 1);
}

TypeId File::readPageItemId(PageIndex index)
{
	assert(isPageAFirstPage(index));
	Address pageStart = index << LOG2_OF_PAGE_SIZE_IN_BITS;
	TypeId result = TypeId::constructFromAddressInPageMetadata(pageStart + 2);
	return result;
}

TypeName File::readPageItemName(PageIndex index)
{
	assert(isPageAFirstPage(index));
	Address pageStart = index << LOG2_OF_PAGE_SIZE_IN_BITS;
	TypeName result = TypeName::constructFromAddress(pageStart + 2 + sizeof(Id)*8);
	return result;
}
