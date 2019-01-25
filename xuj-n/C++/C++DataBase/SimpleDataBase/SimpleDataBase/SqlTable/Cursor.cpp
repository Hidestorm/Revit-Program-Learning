#include "Cursor.h"
#include "SqlTable.h"

//Cursor * Cursor::CreateTableStart(SqlTable * table)
//{
//	Cursor *pCursor = new Cursor;
//	pCursor->m_table = table;
//	pCursor->m_RowNum = 0;
//	pCursor->m_endTable = (table->GetNumRows() == 0);
//
//	return pCursor;
//}
//
//Cursor * Cursor::CreateTableEnd(SqlTable * table)
//{
//	Cursor *pCursor = new Cursor;
//	pCursor->m_table = table;
//	pCursor->m_RowNum = table->GetNumRows();
//	pCursor->m_endTable = true;
//
//	return pCursor;
//}

void Cursor::Next()
{
	m_RowNum++;
	if (m_RowNum >= m_table->m_numRows)
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
	uint32_t page_num = m_RowNum / ROWS_PER_RAGE;
	char* page = m_table->getPage(page_num);
	uint32_t row_offset = m_RowNum % ROWS_PER_RAGE;
	uint32_t byte_offset = row_offset * ROW_SIZE;
	return page + byte_offset;
}
