#pragma once
#include <stdint.h>

class SqlTable;
class Cursor
{
	friend SqlTable;
public:
	Cursor() {};
	~Cursor() {};

	void Next();
	bool isEnd();
	char * Value();

protected:
	SqlTable * m_table;
	uint32_t m_RowNum;
	bool m_endTable;
};