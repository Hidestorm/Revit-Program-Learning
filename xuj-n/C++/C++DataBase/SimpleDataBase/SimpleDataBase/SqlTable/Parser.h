#pragma once
#include "SqlTable.h"
#include <string>

/*
* 解析指令，向Table传递指令
*/


class Parser
{
public:
	Parser(mySql::SqlTable *table);
	~Parser();

	std::string ExcuteCmd(const std::string& cmd);

private:
	MetaCommondResult ExcuteMetaCmd();
	void ExcuteCmd();
	
	PrepareResult PrapareStateMent();
	ExecuteResult ExcuteStateMent();

	void MetaResutParser(MetaCommondResult result);
	void PrePareResutParser(PrepareResult result);
	void ExecuteResutParser(ExecuteResult result);
private:
	mySql::SqlTable * m_table;
	Statement m_Statement;
	std::string m_Cmd;
	std::string m_ExcuteResult;
};