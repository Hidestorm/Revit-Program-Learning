#include "Cursor.h"
#include "SqlTable.h"
#include "TreeStruct.h"

void Cursor::Next()
{
	char * pNode = m_table->getPage(m_pageNum);
	m_cellNum++;
	if (m_cellNum >= (*LEAF_NODE_NUM_CELLS(pNode)))
	{
		m_endTable = true;
	}
}

bool Cursor::isEnd()
{
	return m_endTable;
}

char * Cursor::Value()
{
	char* page = m_table->getPage(m_pageNum);
	return LEAF_NODE_VALUE(page, m_cellNum);
}
