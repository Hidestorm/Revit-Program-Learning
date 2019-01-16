#pragma once
#include "stdint.h"
#include "string"
#include "SqlCore.h"
//
//const size_t COLUMN_USERNAME_SIZE = 32;
//const size_t COLUMN_EMAIL_SIZE = 255;

#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

namespace mySql
{

	//struct Row
	//{
	//	size_t id;
	//	char userName[COLUMN_USERNAME_SIZE];
	//	char email[COLUMN_EMAIL_SIZE];
	//};

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


	class SqlTable
	{
	public:
		SqlTable() {
			m_numRows = 0;
			std::memset(pages, NULL, TABLE_MAX_PAGES);
		};

	public:
		ExecuteResult ExcuteInsert(Statement * statement);
		ExecuteResult ExcuteSelect(Statement * statement);

	private:

		void serializeRow(Row * source, char * destination);
		void deserializeRow(char * source, Row * destination);

		char * RowSlot(size_t rowNum);
		void printRow(const Row & row);

	private:	
		char *pages[TABLE_MAX_PAGES];
		size_t m_numRows;

	};




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
}

