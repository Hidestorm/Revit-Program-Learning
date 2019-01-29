#include "SqlTable.h"
#include <fstream>
#include <iostream>
#include "Cursor.h"
#include "TreeStruct.h"

using namespace std;

SqlTable::~SqlTable()
{
	db_Close();
}

ExecuteResult SqlTable::ExcuteInsert(Statement * statement)
{
	char * node = getPage(m_RootPageNum);
	if ((*LEAF_NODE_NUM_CELLS(node)) >= LEAF_NODE_MAX_CELLS)
	{
		return EXECUTE_TABLE_FULL;
	}

	Cursor *pCursor = end();
	Row * row2Insert = &(statement->rowToInsert);
	
	insertLeafNode(pCursor, row2Insert->id, row2Insert);
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
}

void SqlTable::db_Open(const char * fileName)
{
	m_pager = pager_open(fileName);
	m_pager->num_pages = m_pager->flie_length / ROW_SIZE;

	m_RootPageNum = 0;

	if (m_pager->num_pages == 0)
	{
		char *root_nude = getPage(0);
		INITIALIZE_LEAF_NODE(root_nude);
	}
}

size_t SqlTable::GetNumRows()
{
	return  0;// m_numRows;
}

Cursor * SqlTable::begin()
{
	Cursor *pCursor = new Cursor;
	pCursor->m_table = this;
	pCursor->m_pageNum = m_RootPageNum;
	pCursor->m_cellNum = 0;

	char * rootNode = getPage(m_RootPageNum);
	uint32_t num_cells = *LEAF_NODE_NUM_CELLS(rootNode);
	pCursor->m_endTable = (num_cells == 0);
	return pCursor;
}

Cursor * SqlTable::end()
{
	Cursor *pCursor = new Cursor;
	pCursor->m_table = this;
	//pCursor->m_RowNum = m_numRows;
	pCursor->m_pageNum = m_RootPageNum;
	char* root_node = getPage(m_RootPageNum);
	uint32_t num_cells = *LEAF_NODE_NUM_CELLS(root_node);
	pCursor->m_cellNum = num_cells;
	pCursor->m_endTable = true;

	return pCursor;
}

void SqlTable::printfLeafNode()
{
	std::cout << "Tree:" << std::endl;
	char* node = getPage(0);
	uint32_t numCells = *LEAF_NODE_NUM_CELLS(node);
	std::cout << "Leaf Size:" << numCells;
	for (uint32_t i = 0; i < numCells; i++)
	{
		uint32_t key = *LEAF_NODE_KEY(node, i);
		std::cout << " -" << i << ":" << key << std::endl;
	}
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

void SqlTable::insertLeafNode(Cursor * pCursor, uint32_t key, Row * row)
{
	char *node = getPage(pCursor->m_pageNum);
	uint32_t num_cells = *LEAF_NODE_NUM_CELLS(node);
	if (num_cells >= LEAF_NODE_MAX_CELLS)
	{
		// Node Full
		cout << "Need to implement splitting a leaf node";
		return;
	}
	if (pCursor->m_cellNum < num_cells)
	{
		// make room for new cell
		for (uint32_t i = num_cells; i > pCursor->m_cellNum; i--)
		{
			// 后移,空出位置
			memcpy(LEAF_NODE_CELL(node, i), LEAF_NODE_CELL(node, i - 1), LEAF_NODE_CELL_SIZE);
		}
	}
	char * cellsNum = LEAF_NODE_NUM_CELLS(node);
	*cellsNum += 1;
	char * pKey = (LEAF_NODE_KEY(node, pCursor->m_cellNum));
	*pKey = key;
	serializeRow(row, LEAF_NODE_VALUE(node, pCursor->m_cellNum));
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
	return Pager::openPager(name);
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
		}

		m_pager->pages[pageNum] = page;
		if (pageNum >= m_pager->num_pages)
		{
			m_pager->num_pages = pageNum + 1;
		}

	}
	return m_pager->pages[pageNum];
}

void SqlTable::db_Close()
{
	//uint32_t numFullPages = m_numRows / ROWS_PER_RAGE;

	for (uint32_t i = 0; i < m_pager->num_pages; i++)
	{
		if(m_pager->pages[i] == nullptr)
			continue;

		m_pager->pager_Flush(i/*, PAGE_SIZE*/);
		free(m_pager->pages[i]);
		m_pager->pages[i] = nullptr;
	}

	// Rows 不够一个页
	//uint32_t numAdditionalRows = m_numRows % ROWS_PER_RAGE;
	//if (numAdditionalRows > 0)
	//{
	//	uint32_t pageNum = numFullPages;
	//	if (m_pager->pages[pageNum] != nullptr)
	//	{
	//		m_pager->pager_Flush(pageNum, numAdditionalRows * ROW_SIZE);
	//		free(m_pager->pages[pageNum]);
	//		m_pager->pages[pageNum] = nullptr;
	//	}
	//}

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

Pager * Pager::openPager(const char * filename)
{	
	std::fstream * readFile = new std::fstream;

	readFile->open(filename, ios::in | ios::out | ios::ate);
	if (!readFile->is_open())
	{
		readFile->close();

		ofstream creatFile(filename);
		if (creatFile)
		{
			creatFile.close();
		}
		readFile->open(filename, ios::in | ios::out | ios::ate);
	}

	if (readFile->is_open())
	{
		off_t file_length = readFile->tellg();
		Pager *pager = new Pager;

		pager->flie_length = file_length;
		pager->file_descriptor = readFile;
		pager->num_pages = (file_length / PAGE_SIZE);

		if (file_length % PAGE_SIZE != 0)
		{
			cout << "Db File is not a whole number of pages";
			exit(EXIT_FAILURE);
		}

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
	return nullptr;
}

void Pager::pager_Flush(uint32_t page_num/*, uint32_t size*/)
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

	file_descriptor->write(pages[page_num], PAGE_SIZE);
	file_descriptor->flush();
}

void printfConstants()
{
	std::cout << "ROW_SIZE:" << ROW_SIZE << std::endl;
	std::cout << "COMMON_NODE_HEADER_SIZE: " << COMMOND_NODE_HEADER_SIZE << std::endl;
	std::cout << "LEAF_NODE_HEADER_SIZE: " << LEAF_NODE_HEADER_SIZE << std::endl;
	std::cout << "LEAF_NODE_CELL_SIZE: " << LEAF_NODE_CELL_SIZE << std::endl;
	std::cout << "LEAF_NODE_SPACE_FOR_CELLS: " << LEAF_NODE_SPACE_FOR_CELLS << std::endl;
	std::cout << "LEAF_NODE_MAX_CELLS: " << LEAF_NODE_MAX_CELLS << std::endl;
}
