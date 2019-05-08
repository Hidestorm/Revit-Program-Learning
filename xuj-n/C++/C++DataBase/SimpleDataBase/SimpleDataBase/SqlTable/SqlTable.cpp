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
	uint32_t numCells = *Node::GetNodeNumCells(node);
	if (numCells >= LEAF_NODE_MAX_CELLS)
	{
		return EXECUTE_TABLE_FULL;
	}

	//Cursor *pCursor = end();
	Row * row2Insert = &(statement->rowToInsert);
	uint32_t key2Insert = row2Insert->id;
	Cursor *pCursor = find(key2Insert);
	
	if (pCursor->m_cellNum < numCells)
	{
		uint32_t key_at_index = *Node::GetNodeKey(node, pCursor->m_cellNum);
		if (key_at_index == key2Insert)
		{
			return  EXECUTE_DUPLICATE_KEY;
		}
	}

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
		Node::InitializeNode(root_nude);
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
	uint32_t num_cells = *Node::GetNodeNumCells(rootNode);
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
	uint32_t num_cells = *Node::GetNodeNumCells(root_node);
	pCursor->m_cellNum = num_cells;
	pCursor->m_endTable = true;

	return pCursor;
}

Cursor * SqlTable::find(uint32_t key)
{
	char * rootNode = getPage(m_RootPageNum);
	if (Node::GetNodeType(rootNode) == NODE_LEAF)
	{
		return FindLeafNode(m_RootPageNum, key);
	}
	else
	{
		// 未实现
		return nullptr;
	}
	return nullptr;
}

/*
Return the position of the given key.
If the key is not present, return the position
where it should be inserted
*/

Cursor * SqlTable::FindLeafNode(uint32_t page_num, uint32_t key)
{
	char * node = getPage(page_num);
	uint32_t numCells = *Node::GetNodeNumCells(node);
	
	Cursor *pCursor = begin();

	// Binary Search
	uint32_t minIndex = 0;
	uint32_t one_past_max_index = numCells;
	while (one_past_max_index != minIndex)
	{
		uint32_t index = (minIndex + one_past_max_index) / 2;
		uint32_t key_at_index = *Node::GetNodeKey(node, index);
		if (key == key_at_index)
		{
			pCursor->m_cellNum = index;
			return pCursor;
		}

		if (key < key_at_index)
		{
			one_past_max_index = index;
		}
		else
		{
			minIndex = index + 1;
		}
	}

	pCursor->m_cellNum = minIndex;
	return pCursor;
}

void SqlTable::printfLeafNode()
{
	std::cout << "Tree:" << std::endl;
	char* node = getPage(0);
	uint32_t numCells = *Node::GetNodeNumCells(node);
	std::cout << "Leaf Size:" << numCells << std::endl;
	for (uint32_t i = 0; i < numCells; i++)
	{
		uint32_t key = *Node::GetNodeKey(node, i);
		std::cout << " -" << i << ":" << key << std::endl;
	}
}

void SqlTable::PrintTree(uint32_t pageNum, uint32_t indentationLevel)
{
	char* node = getPage(pageNum);
	uint32_t numKeys, child;

	switch (Node::GetNodeType(node))
	{
	case NODE_LEAF:
	{
		numKeys = *Node::GetNodeNumCells(node);
		indent(numKeys);
		std::cout << "- leaf size: " << numKeys << std::endl;
		for (uint32_t i =0; i < numKeys; i++)
		{
			indent(indentationLevel + 1);
			std::cout << "- " << uint32_t(*Node::GetNodeKey(node, i)) << std::endl;
		}
		break;
	}
	case NODE_INTERNAL:
	{
		numKeys = *Node::GetInternalNodeNumKeys(node);
		indent(indentationLevel);
		std::cout << "- internal size: " << numKeys << std::endl;
		for (uint32_t i = 0; i < numKeys; i++)
		{
			child = *Node::GetInternalNodeChild(node, i);
			PrintTree(child, indentationLevel + 1);

			indent(indentationLevel);
			std::cout << "- key " << uint32_t(*Node::GetInternalNodeKey(node, i)) << std::endl;
		}
		child = *Node::GetInternalNodeRightChild(node);
		PrintTree(child, indentationLevel + 1);
		break;
	}
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
	uint32_t num_cells = *Node::GetNodeNumCells(node);
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
			memcpy(Node::GetNodeCell(node, i), Node::GetNodeCell(node, i - 1), LEAF_NODE_CELL_SIZE);
		}
	}
	char * cellsNum = Node::GetNodeNumCells(node);
	*cellsNum += 1;
	char * pKey = (Node::GetNodeKey(node, pCursor->m_cellNum));
	*pKey = key;
	serializeRow(row, Node::GetNodeValue(node, pCursor->m_cellNum));
}

void SqlTable::splitLeafNodeAndInsert(Cursor * pCursor, uint32_t key, Row * row)
{
	/*
	Create a new node and move half the cells over.
	Insert the new value in one of the two nodes.
	Update parent or create a new parent.
	*/

	char *pOldNode = getPage(pCursor->m_pageNum);
	uint32_t newPageNum = m_pager->GetUnsedPageNum();
	char *pNewNode = getPage(newPageNum);
	Node::InitializeNode(pNewNode);

	/*
	All existing keys plus new key should be divided
	evenly between old(left) and new(right) nodes.
	Starting form the right, move each key to correct position.
	*/
	for (int i = LEAF_NODE_MAX_CELLS; i >=  0; i--)
	{
		char * destinationNode;
		if (i >= LEAF_NODE_LEFT_SPLIT_COUNT)
		{
			destinationNode = pNewNode;
		}
		else
		{
			destinationNode = pOldNode;
		}

		uint32_t indexWithinNode = i % LEAF_NODE_LEFT_SPLIT_COUNT;
		char *destination = Node::GetNodeCell(destinationNode, indexWithinNode);

		if (i == pCursor->m_cellNum)
		{
			serializeRow(row, destination);
		}
		else if (i > pCursor->m_cellNum)
		{
			memcpy(destination, Node::GetNodeCell(pOldNode, i - 1), LEAF_NODE_CELL_SIZE);
		}
		else
		{
			memcpy(destination, Node::GetNodeCell(pOldNode, i), LEAF_NODE_CELL_SIZE);
		}

		/* Update cell count on both leaf nodes */
		*(Node::GetNodeNumCells(pOldNode)) = LEAF_NODE_LEFT_SPLIT_COUNT;
		*(Node::GetNodeNumCells(pNewNode)) = LEAF_NODE_RIGHT_SPLIT_COUNT;

		if (Node::isNodeRoot(pOldNode))
		{
			return CreateNewRoot(newPageNum);
		}
		else
		{
			// Need to implement updating parent after split
			exit(EXIT_FAILURE);
		}

	}
}
/*
Handle splitting the root.
Old root copied to new page, becomes left child.
Addrss of right child passed in.
Re-initialize root page to contain the new root nude.
New root node points to two children
*/
void SqlTable::CreateNewRoot(uint32_t rightChildPageNum)
{
	char* root = getPage(m_RootPageNum);
	char* rightChild = getPage(rightChildPageNum);
	uint32_t leftChildPageNum = m_pager->GetUnsedPageNum();
	char* leftChild = getPage(leftChildPageNum);

	/* Left child has data copied from old root */
	memcpy(leftChild, root, PAGE_SIZE);
	// set No Root
	Node::SetNodeRoot(leftChild, false);

	/* Root node is a new internal node with one key and two children */
	Node::InitializeNode(root);
	Node::SetNodeRoot(root, true);
	*Node::GetInternalNodeNumKeys(root) = 1;
	*Node::GetInternalNodeChild(root, 0) = leftChildPageNum;
	uint32_t left_child_max_key = Node::GetNodeMaxKey(leftChild);
	*Node::GetInternalNodeKey(root, 0) = left_child_max_key;
	*Node::GetInternalNodeRightChild(root) = rightChildPageNum;
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
	for (uint32_t i = 0; i < m_pager->num_pages; i++)
	{
		if(m_pager->pages[i] == nullptr)
			continue;

		m_pager->pager_Flush(i/*, PAGE_SIZE*/);
		free(m_pager->pages[i]);
		m_pager->pages[i] = nullptr;
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

Pager * Pager::openPager(const char * filename)
{	
	std::fstream * readFile = new std::fstream;

	readFile->open(filename, ios::in | ios::out | ios::ate);
	if (!readFile->is_open())
	{
		// 没有打开，重新创建文件
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

uint32_t Pager::GetUnsedPageNum()
{	
	/*
	* Until we start recycling free pages, new pages will always
	* go onto the end of the database file
	*/
	return num_pages;
}

void printfConstants()
{
	std::cout << "ROW_SIZE:" << ROW_SIZE << std::endl;
	std::cout << "COMMON_NODE_HEADER_SIZE: " << COMMON_NODE_HEADER_SIZE << std::endl;
	std::cout << "LEAF_NODE_HEADER_SIZE: " << LEAF_NODE_HEADER_SIZE << std::endl;
	std::cout << "LEAF_NODE_CELL_SIZE: " << LEAF_NODE_CELL_SIZE << std::endl;
	std::cout << "LEAF_NODE_SPACE_FOR_CELLS: " << LEAF_NODE_SPACE_FOR_CELLS << std::endl;
	std::cout << "LEAF_NODE_MAX_CELLS: " << LEAF_NODE_MAX_CELLS << std::endl;
}

void indent(uint32_t level)
{
	for (uint32_t i = 0; i < level; i++)
	{
		std::cout << " ";
	}
}


