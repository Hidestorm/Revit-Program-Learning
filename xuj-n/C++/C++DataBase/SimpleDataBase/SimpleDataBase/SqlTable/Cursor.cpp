#include "Cursor.h"
#include "SqlTable.h"
#include "TreeStruct.h"

void Cursor::Next()
{
	char * pNode = m_table->getPage(m_pageNum);
	m_cellNum++;
	if (m_cellNum >= (*Node::GetNodeNumCells(pNode)))
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
	return Node::GetNodeValue(page, m_cellNum);
}
