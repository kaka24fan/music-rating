#include "File.h"

#include "TypeId.h"
#include "TypeName.h"
#include "stringutils.h"

File* File::m_instance = nullptr;

void File::writeBit(Address a, bool bit)
{
	assert(a < m_bitCount);
	if (bit)
		m_bytes[a / 8] |= (1 << (a % 8));
	else
		m_bytes[a / 8] &= ~(1 << (a % 8)); 
}

bool File::readBit(Address a)
{
	assert(a < m_bitCount);
	return (m_bytes[a / 8] >> (a % 8)) & 1;
}

void File::seek(Address a)
{
	m_nextBitToRW = a;
}

void File::writeNextBit(bool bit)
{
	writeBit(m_nextBitToRW++, bit);

	if (!isAddressInsidePageData(m_nextBitToRW))
	{
		PageIndex currPage = m_nextBitToRW >> LOG2_OF_PAGE_SIZE_IN_BITS;
		PageIndex nextPage = 0;
		if (!getContinuationPage(nextPage, currPage))
		{
			// There's no continuation page, create new one.
			nextPage = getNextFreePage();

			setContinuationPagePointer(currPage, nextPage);
			initializePage(nextPage);
		}
		if (!getFirstDataBitOfPage(nextPage, m_nextBitToRW))
		{
			// That's supposed to be impossible - "nextPage" should not be free if we picked it to be the next page.
			assert(false);
		}
	}
}

bool File::readNextBit(bool& result)
{
	// To be returned.
	if (!isAddressInsidePageData(m_nextBitToRW))
	{
		return false;
	}
	
	result = readBit(m_nextBitToRW++);
	
	if (!isAddressInsidePageData(m_nextBitToRW))
	{
		PageIndex currPage = m_nextBitToRW >> LOG2_OF_PAGE_SIZE_IN_BITS;
		PageIndex nextPage = 0;

		if (getContinuationPage(nextPage, currPage))
		{
			bool firstDataBitOfNextPage = false;
			if (!getFirstDataBitOfPage(nextPage, firstDataBitOfNextPage))
			{
				assert(false);
			}
			m_nextBitToRW = firstDataBitOfNextPage;
		}
		// otherwise leave the m_nextBitToRW outside of data bits and the next read will fail
	}
	return true;
}

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
	Address pageStart = pageIndex << LOG2_OF_PAGE_SIZE_IN_BITS;
	writeBit(pageStart, true);
	for (unsigned int i = 1; i < (1 << LOG2_OF_PAGE_SIZE_IN_BITS); i++)
		writeBit(pageStart + i, false);
}

/*
Sets the valid bit and the owner id, clears all other bits of the page.
*/
void File::initializePage(PageIndex pageIndex, Id id)
{
	initializePage(pageIndex);
	Address lastBitOfId;
	getFirstDataBitOfPage(lastBitOfId, pageIndex);
	lastBitOfId--; // after this line, variable's content matches the name
	for (unsigned int i = 0; i < sizeof(Id) * 8; i++)
	{
		writeBit(lastBitOfId--, (id & 1));
		id >>= 1;
	}
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

TypeId File::findItemWithName(String name, unsigned int maxStringDist)
{
	PageIndex pageIndex = 0;
	while (true)
	{
		pageIndex++;
		if (!isPageAFirstPage(pageIndex))
			continue;
		
		TypeId itemId = readPageItemId(pageIndex);
		ItemType itemType = itemId.getItemType();
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
Returns true iff the address given points to a bit of actual data, not page metadata.
*/
bool File::isAddressInsidePageData(Address a)
{
	PageIndex thatPageIndex = a >> LOG2_OF_PAGE_SIZE_IN_BITS;
	Address firstDataBit = 0;
	if (!getFirstDataBitOfPage(firstDataBit, thatPageIndex))
	{
		// we're in a free page
		return false;
	}
	if (firstDataBit > a)
	{
		// we're too early
		return false;
	}
	if (a >= getFirstBitOfTheNextPagePointer(thatPageIndex))
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
		result = (result >> 1) + (readBit(a) ? 1 : 0);
	}

	return result != 0; // if result is 0, that means there's no continuation page, so return failure. Otherwise, result's ready.
}

/*
Sets the continuation page pointer of the origin page to point to the continuation page.
*/
void File::setContinuationPagePointer(PageIndex origin, PageIndex continuation)
{
	Address lastBitOfTheNextPagePointer = (origin + 1) << LOG2_OF_PAGE_SIZE_IN_BITS;
	for (Address a = lastBitOfTheNextPagePointer; a >= getFirstBitOfTheNextPagePointer(origin); a--)
	{
		writeBit(a, continuation & 1);
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
Writes a byte of data to the file, given that the address given is divisible by 8 (aligned).
*/
void File::writeAlignedByte(Address a, Byte data)
{
	assert(a % 8 == 0);
	for (unsigned int i = sizeof(Byte) * 8 - 1; i >= 0; i--)
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
	for (unsigned int i = sizeof(Byte) * 8 - 1; i >= 0; i--)
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
		bool nextBit;
		readNextBit(nextBit);
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
	for (PageIndex trial = 1; (trial + 1)*pageSizeInBits <= m_bitCount; trial++)
	{
		if (isPageFree(trial))
		{
			return trial;
		}
	}
	// We ran out of memory, pack up and leave.
	assert(false);
	return 0; // assertion is gonna kill the program anyway, do this return to suppress a warning
}

/*
Returns failure iff there's no page owned by the id given. Otherwise returns that page's index.
*/
bool File::getPageContainingId(PageIndex & result, TypeId id)
{
	unsigned int pageSizeInBits = 1 << LOG2_OF_PAGE_SIZE_IN_BITS;
	for (PageIndex trial = 1; (trial + 1)*pageSizeInBits <= m_bitCount; trial++)
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
	, m_bytes(nullptr)
	, m_bitCount(0)
{

}

bool File::isPageFree(PageIndex index)
{
	return readBit(index >> LOG2_OF_PAGE_SIZE_IN_BITS);
}

bool File::isPageAFirstPage(PageIndex index)
{
	Address pageStart = index >> LOG2_OF_PAGE_SIZE_IN_BITS;
	return readBit(pageStart) && readBit(pageStart + 1);
}

TypeId File::readPageItemId(PageIndex index)
{
	assert(isPageAFirstPage(index));
	Address pageStart = index >> LOG2_OF_PAGE_SIZE_IN_BITS;
	TypeId result = TypeId::constructFromAddress(pageStart + 2);
	return result;
}

TypeName File::readPageItemName(PageIndex index)
{
	assert(isPageAFirstPage(index));
	Address pageStart = index >> LOG2_OF_PAGE_SIZE_IN_BITS;
	TypeName result = TypeName::constructFromAddress(pageStart + 2 + sizeof(Id)*8);
	return result;
}
