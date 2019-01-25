#include "SqlTable.h"
#include <fstream>
#include <iostream>
#include "Cursor.h"

using namespace std;

SqlTable::~SqlTable()
{
	db_Close();
}

ExecuteResult SqlTable::ExcuteInsert(Statement * statement)
{
	Cursor *pCursor = end();
	if (m_numRows >= TABLE_MAX_ROWS)
		return EXECUTE_TABLE_FULL;

	Row * row2Insert = &(statement->rowToInsert);
	serializeRow(row2Insert, pCursor->Value());
	m_numRows++;
	delete pCursor;
	return EXECUTE_SUCCESS;
}

ExecuteResult SqlTable::ExcuteSelect(Statement * statement)
{
	Cursor * pCursor = begin();

	while (!pCursor->isEnd())
	{
		Row row;
		deserializeRow(pCursor->Value(), &row);
		pCursor->Next();
		printRow(row);
	}
	delete end();
	return EXECUTE_SUCCESS;

	//Row row;
	//for (int i = 0; i <m_numRows; i++)
	//{
	//	deserializeRow(RowSlot(i), &row);
	//	printRow(row);
	//}
	//return EXECUTE_SUCCESS;
}

void SqlTable::db_Open(const char * fileName)
{
	m_pager = pager_open(fileName);
	m_numRows = m_pager->flie_length / ROW_SIZE;
}

size_t SqlTable::GetNumRows()
{
	return m_numRows;
}

Cursor * SqlTable::begin()
{
	Cursor *pCursor = new Cursor;
	pCursor->m_table = this;
	pCursor->m_RowNum = 0;
	pCursor->m_endTable = (m_numRows == 0);
	return pCursor;
}

Cursor * SqlTable::end()
{
	Cursor *pCursor = new Cursor;
	pCursor->m_table = this;
	pCursor->m_RowNum = m_numRows;
	pCursor->m_endTable = true;

	return pCursor;
}

void SqlTable::serializeRow(Row * source, char * destination)
{
	std::memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
	std::memcpy(destination + USERNAME_OFFSET, &(source->userName), USERNAME_SIZE);
	std::memcpy(destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}

void SqlTable::deserializeRow(char * source, Row * destination)
{
	std::memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
	std::memcpy(&(destination->userName), source + USERNAME_OFFSET, USERNAME_SIZE);
	std::memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);
}

char * SqlTable::RowSlot(size_t rowNum)
{
	size_t pageNum = rowNum / ROWS_PER_RAGE;
	char *page = getPage(pageNum);
	size_t rowOffset = rowNum % ROWS_PER_RAGE;
	size_t byteOffset = rowOffset * ROW_SIZE;

	return page + byteOffset;
}

void SqlTable::printRow(const Row & row)
{
	std::cout << row.id << " ";
	std::cout << row.userName << " ";
	std::cout << row.email << std::endl;
}

Pager * SqlTable::pager_open(const char * name)
{
	//ofstream creatFile(name);
	//if (creatFile)
	//{
	//	creatFile.close();
	//}

	std::fstream * readFile = new std::fstream;;
	readFile->open(name, ios::in | ios::out | ios::ate);
	if (readFile->is_open())
	{
		off_t file_length = readFile->tellg();
		Pager *pager = new Pager;

		pager->flie_length = file_length;
		pager->file_descriptor = readFile;

		for (int i = 0; i < TABLE_MAX_PAGES; i++)
		{
			pager->pages[i] = nullptr;
		}
		return pager;
	}
	else
	{
		cout << "Unable to open file" << endl;
		exit(EXIT_FAILURE);
	}
	//int fd = open(name, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
	return nullptr;
}

char * SqlTable::getPage(size_t pageNum)
{
	if (pageNum > TABLE_MAX_PAGES)
	{
		cout << "Tried to fetch page number out of bounds." << pageNum << ">" << TABLE_MAX_PAGES;
		exit(EXIT_FAILURE);
	}
	if (m_pager->pages[pageNum] == nullptr)
	{
		// 缺页，分配内存，从文件中读取数据
		char * page = new char[PAGE_SIZE];
		uint32_t numPages = m_pager->flie_length / PAGE_SIZE;

		// We might save a partial page at the end of the file
		uint32_t partialPageSize = m_pager->flie_length % PAGE_SIZE;
		//if (partialPageSize)
		//{
		//	numPages += 1;
		//}

		if (pageNum <= numPages)
		{
			m_pager->file_descriptor->seekg(pageNum * PAGE_SIZE);
			try
			{
				if (pageNum == numPages)
				{
					m_pager->file_descriptor->read(page, partialPageSize);
				}
				else
				{
					m_pager->file_descriptor->read(page, PAGE_SIZE);
				}
			}
			catch (...)
			{
				cout << "Error reading file: " << errno << endl;
				exit(EXIT_FAILURE);
			}

			m_pager->pages[pageNum] = page;
		}
	}
	return m_pager->pages[pageNum];
}

void SqlTable::db_Close()
{
	uint32_t numFullPages = m_numRows / ROWS_PER_RAGE;

	for (uint32_t i = 0; i < numFullPages; i++)
	{
		if(m_pager->pages[i] == nullptr)
			continue;

		m_pager->pager_Flush(i, PAGE_SIZE);
		free(m_pager->pages[i]);
		m_pager->pages[i] = nullptr;
	}

	// Rows 不够一个页
	uint32_t numAdditionalRows = m_numRows % ROWS_PER_RAGE;
	if (numAdditionalRows > 0)
	{
		uint32_t pageNum = numFullPages;
		if (m_pager->pages[pageNum] != nullptr)
		{
			m_pager->pager_Flush(pageNum, numAdditionalRows * ROW_SIZE);
			free(m_pager->pages[pageNum]);
			m_pager->pages[pageNum] = nullptr;
		}
	}

	m_pager->file_descriptor->close();

	for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++)
	{
		char * page = m_pager->pages[i];
		if (page)
		{
			delete page;
			m_pager->pages[i] = nullptr;
		}
	}
	delete m_pager;
}

void Pager::pager_Flush(uint32_t page_num, uint32_t size)
{
	if (pages[page_num] == nullptr)
	{
		cout << "Tried to flush null page" << endl;
	}
	try
	{
		file_descriptor->seekp(page_num * PAGE_SIZE);
	}
	catch (...)
	{
		cout << "Seekp failed";
	}

	file_descriptor->write(pages[page_num], size);
	file_descriptor->flush();
}
