#pragma once
#include "stdint.h"
const size_t COLUMN_USERNAME_SIZE = 32;
const size_t COLUMN_EMAIL_SIZE = 255;

struct Row
{
	size_t id;
	char userName[COLUMN_USERNAME_SIZE];
	char email[COLUMN_EMAIL_SIZE];
};



#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

const size_t ID_SIZE = size_of_attribute(Row, id);
const size_t USERNAME_SIZE = size_of_attribute(Row, userName);
const size_t EMAIL_SIZE = size_of_attribute(Row, email);
const size_t ID_OFFSET = 0;
const size_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const size_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const size_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

// compact representation
void serializeRow(Row * source, char * destination)
{
	std::memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
	std::memcpy(destination + USERNAME_OFFSET, &(source->userName), USERNAME_SIZE);
	std::memcpy(destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}

void deserializeRow(char * source, Row * destination)
{
	std::memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
	std::memcpy(&(destination->userName), source + USERNAME_OFFSET, USERNAME_SIZE);
	std::memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);
}

const size_t PAGE_SIZE = 4096;
const size_t TABLE_MAX_PAGES = 100;
const size_t ROWS_PER_RAGE = PAGE_SIZE / ROW_SIZE;
const size_t TABLE_MAX_ROWS = ROWS_PER_RAGE * TABLE_MAX_PAGES;

struct SqlTable
{
	char *pages[TABLE_MAX_PAGES];
	size_t numRows;

	SqlTable()
	{
		numRows = 0;
		std::memset(pages, NULL, TABLE_MAX_PAGES);
		//pages = new char[TABLE_MAX_PAGES];
	}
};

char * RowSlot(SqlTable* table, size_t rowNum)
{
	size_t pageNum = rowNum / ROWS_PER_RAGE;
	char *page = table->pages[pageNum];
	if (!page)
	{
		page = table->pages[pageNum] = new char(PAGE_SIZE);
	}

	size_t rowOffset = rowNum % ROWS_PER_RAGE;
	size_t byteOffset = rowOffset * ROW_SIZE;

	return page + byteOffset;
}

void printRow(const Row & row)
{
	std::cout << row.id;
	std::cout << row.userName;
	std::cout << row.email << std::endl;
}