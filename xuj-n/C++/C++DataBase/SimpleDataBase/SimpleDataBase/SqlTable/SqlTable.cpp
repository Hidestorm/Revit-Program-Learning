#include "SqlTable.h"

ExecuteResult mySql::SqlTable::ExcuteInsert(Statement * statement)
{
	if (m_numRows >= TABLE_MAX_ROWS)
		return EXECUTE_TABLE_FULL;

	Row * row2Insert = &(statement->rowToInsert);
	serializeRow(row2Insert, RowSlot(m_numRows));
	m_numRows++;
	return EXECUTE_SUCCESS;
}

ExecuteResult mySql::SqlTable::ExcuteSelect(Statement * statement)
{
	Row row;
	for (int i = 0; i <m_numRows; i++)
	{
		deserializeRow(RowSlot(i), &row);
		printRow(row);
	}
	return EXECUTE_SUCCESS;
}

void mySql::SqlTable::serializeRow(Row * source, char * destination)
{
	std::memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
	std::memcpy(destination + USERNAME_OFFSET, &(source->userName), USERNAME_SIZE);
	std::memcpy(destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}

void mySql::SqlTable::deserializeRow(char * source, Row * destination)
{
	std::memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
	std::memcpy(&(destination->userName), source + USERNAME_OFFSET, USERNAME_SIZE);
	std::memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);
}

char * mySql::SqlTable::RowSlot(size_t rowNum)
{
	size_t pageNum = rowNum / ROWS_PER_RAGE;
	char *page = this->pages[pageNum];
	if (!page)
	{
		page = this->pages[pageNum] = new char[PAGE_SIZE];
	}

	size_t rowOffset = rowNum % ROWS_PER_RAGE;
	size_t byteOffset = rowOffset * ROW_SIZE;

	return page + byteOffset;
}

void mySql::SqlTable::printRow(const Row & row)
{
	std::cout << row.id << " ";
	std::cout << row.userName << " ";
	std::cout << row.email << std::endl;
}
