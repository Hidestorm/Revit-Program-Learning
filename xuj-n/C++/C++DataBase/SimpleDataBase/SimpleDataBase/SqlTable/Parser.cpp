#include "Parser.h"

Parser::Parser(mySql::SqlTable * table)
{
	m_table = table;
}

Parser::~Parser()
{
}

std::string Parser::ExcuteCmd(const std::string& cmd)
{
	m_Cmd = cmd;
	if (!m_Cmd.empty() && m_Cmd[0] == '.')
	{
		switch (ExcuteMetaCmd())
		{
		case(META_COMMAND_SUCESS):
			return "";
		case(META_COMMAND_UNRECOGNIZED_COMMAND):
			return  "Unrecognize command " + cmd;
		}
	}
	
	switch (PrapareStateMent())
	{
	case(PREPARE_SUCCESS):
		break;
	case(PREPARE_SYNTAX_ERROR):
		return "Syntax error. Could not parse statement.";
	case(PREPARE_UNRECOGNIZEED_STATEMENT):
		return "Unrecognize keyword at start of " + m_Cmd;
	}

	switch (ExcuteStateMent())
	{
	case(EXECUTE_SUCCESS):
		return "Executed.";
	case(EXECUTE_TABLE_FULL):
		return "Error: Table full.";
	}


}

MetaCommondResult Parser::ExcuteMetaCmd()
{
	if (m_Cmd.compare(".exit") == 0)
	{
		m_table->~SqlTable();
		//delete m_table;
		exit(EXIT_SUCCESS);
	}
	else
	{
		return META_COMMAND_UNRECOGNIZED_COMMAND;
	}
}

void Parser::ExcuteCmd()
{
}

PrepareResult Parser::PrapareStateMent()
{
	if (m_Cmd.compare(0, 6, "insert") == 0)
	{
		m_Statement.type = STATEMENT_INSERT;
		//
		int idInt;
		int argsAssigned = std::sscanf(m_Cmd.c_str(), "insert %d %s %s", &idInt,
			m_Statement.rowToInsert.userName, m_Statement.rowToInsert.email);

		m_Statement.rowToInsert.id = size_t(idInt);
		if (argsAssigned < 3)
		{
			return PREPARE_SYNTAX_ERROR;
		}
		return PREPARE_SUCCESS;
	}

	if (m_Cmd.compare("select") == 0)
	{
		m_Statement.type = STATEMENT_SELECT;
		return PREPARE_SUCCESS;
	}
	return PREPARE_UNRECOGNIZEED_STATEMENT;
}

ExecuteResult Parser::ExcuteStateMent()
{
	switch (m_Statement.type)
	{
	case(STATEMENT_INSERT):
		return m_table->ExcuteInsert(&m_Statement);
	case(STATEMENT_SELECT):
		return m_table->ExcuteSelect(&m_Statement);
	}
}

void Parser::PrePareResutParser(PrepareResult result)
{
}

void Parser::ExecuteResutParser(ExecuteResult result)
{
}

void Parser::MetaResutParser(MetaCommondResult result)
{
}
