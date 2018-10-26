#pragma once
#include <iostream>
#include <string>
#include "RowStruct.h"

enum MetaCommondResult
{
	META_COMMAND_SUCESS,
	META_COMMAND_UNRECOGNIZED_COMMAND
};

enum PrepareResult
{
	PREPARE_SUCCESS,
	PREPARE_SYNTAX_ERROR,
	PREPARE_UNRECOGNIZEED_STATEMENT
};

enum StatementType
{
	STATEMENT_INSERT,
	STATEMENT_SELECT
};

enum ExecuteResult
{
	EXECUTE_TABLE_FULL,
	EXECUTE_SUCCESS
};

struct Statement
{
	StatementType type;
	Row rowToInsert;
};

MetaCommondResult doMetaCommand(const std::string& str)
{
	if (str.compare(".exit") == 0)
	{
		exit(EXIT_SUCCESS);
	}
	else
	{
		return META_COMMAND_UNRECOGNIZED_COMMAND;
	}
}

PrepareResult prepareStatement(const std::string & str, Statement &statement)
{
	if (str.compare(0, 6, "insert") == 0)
	{
		statement.type = STATEMENT_INSERT;
		//
		int idInt;
		int argsAssigned = std::sscanf(str.c_str(), "insert %d %s %s", &idInt,
			statement.rowToInsert.userName, statement.rowToInsert.email);

		statement.rowToInsert.id = size_t(idInt);
		if(argsAssigned < 3)
		{
			return PREPARE_SYNTAX_ERROR;
		}
		return PREPARE_SUCCESS;
	}

	if (str.compare("select") == 0)
	{
		statement.type = STATEMENT_SELECT;
		return PREPARE_SUCCESS;
	}
	return PREPARE_UNRECOGNIZEED_STATEMENT;
}

ExecuteResult ExcuteInsert(Statement * statement, SqlTable * table)
{
	if (table->numRows >= TABLE_MAX_ROWS)
		return EXECUTE_TABLE_FULL;

	Row * row2Insert = &(statement->rowToInsert);
	serializeRow(row2Insert, RowSlot(table, table->numRows));
	table->numRows += 1;
}

ExecuteResult ExcuteSelect(Statement *statement, SqlTable * table)
{
	Row row;
	for (int i = 0; i < table->numRows; i++)
	{
		deserializeRow(RowSlot(table, i), &row);
		// printRow(row);
	}
	return EXECUTE_SUCCESS;
}

ExecuteResult ExcuteStatement(Statement * statement, SqlTable *table)
{
	switch (statement->type)
	{
	case(STATEMENT_INSERT):
		return ExcuteInsert(statement, table);
	case(STATEMENT_SELECT):
		return ExcuteSelect(statement, table);
	}
}