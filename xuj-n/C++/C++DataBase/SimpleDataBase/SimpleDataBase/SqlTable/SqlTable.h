#pragma once
#include "stdint.h"
#include "string"
#include "SqlCore.h"
//
//const size_t COLUMN_USERNAME_SIZE = 32;
//const size_t COLUMN_EMAIL_SIZE = 255;

#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)
const size_t COLUMN_USERNAME_SIZE = 32;
const size_t COLUMN_EMAIL_SIZE = 255;

struct Row
{
	size_t id;
	char userName[COLUMN_USERNAME_SIZE];
	char email[COLUMN_EMAIL_SIZE];
};

struct Statement
{
	StatementType type;
	Row rowToInsert;
};

const size_t ID_SIZE = size_of_attribute(Row, id);
const size_t USERNAME_SIZE = size_of_attribute(Row, userName);
const size_t EMAIL_SIZE = size_of_attribute(Row, email);
const size_t ID_OFFSET = 0;
const size_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const size_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const size_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;


const size_t PAGE_SIZE = 4096;
const size_t TABLE_MAX_PAGES = 100;
const size_t ROWS_PER_RAGE = PAGE_SIZE / ROW_SIZE;
const size_t TABLE_MAX_ROWS = ROWS_PER_RAGE * TABLE_MAX_PAGES;


struct Pager
{
	static Pager* openPager(const char * filename);

	std::fstream *file_descriptor;
	uint32_t flie_length;
	uint32_t num_pages;
	char * pages[TABLE_MAX_PAGES];

	void pager_Flush(uint32_t page_num/*, uint32_t size*/);
};

class Cursor;

class SqlTable
{
	friend Cursor;
public:
	SqlTable() {
		//m_numRows = 0;
		std::memset(pages, NULL, TABLE_MAX_PAGES);
	};
	~SqlTable();

public:
	ExecuteResult ExcuteInsert(Statement * statement);
	ExecuteResult ExcuteSelect(Statement * statement);

	void db_Open(const char* fileName);

	size_t GetNumRows();

	Cursor * begin();
	Cursor * end();
	Cursor * find(uint32_t key);

	Cursor * FindLeafNode(uint32_t page_num, uint32_t key);

	// For debug
	void printfLeafNode();

private:
	void serializeRow(Row * source, char * destination);
	void deserializeRow(char * source, Row * destination);

	void insertLeafNode(Cursor *pCursor, uint32_t key, Row *row);
	
	char * RowSlot(size_t rowNum);

	void printRow(const Row & row);

	Pager* pager_open(const char* name);
	char * getPage(size_t pageNum);

	void db_Close();

private:	
	char *pages[TABLE_MAX_PAGES];
	//size_t m_numRows;
	uint32_t m_RootPageNum;
	Pager *m_pager;

};

void printfConstants();



	//void serializeRow(Row * source, char * destination);
	////{
	////	std::memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
	////	std::memcpy(destination + USERNAME_OFFSET, &(source->userName), USERNAME_SIZE);
	////	std::memcpy(destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
	////}

	//void deserializeRow(char * source, Row * destination);
	////{
	////	std::memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
	////	std::memcpy(&(destination->userName), source + USERNAME_OFFSET, USERNAME_SIZE);
	////	std::memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);
	////}


	//char * RowSlot(SqlTable* table, size_t rowNum);


	//void printRow(const Row & row);


