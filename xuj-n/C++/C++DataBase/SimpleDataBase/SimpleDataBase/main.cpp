#include <iostream>
#include <string>
#include "SqlCore.h"
#include "Parser.h"

using namespace std;


int main(int argc, char* argv[])
{
	mySql::SqlTable *table = new mySql::SqlTable;
	Parser *parser = new Parser(table);

	string inputStr;
	while (true)
	{
		inputStr.clear();
		cout << "db >";
	
		getline(cin, inputStr);

		cout << parser->ExcuteCmd(inputStr) << endl;
		inputStr.clear();

		//if (!inputStr.empty() && inputStr[0] == '.')
		//{
		//	switch (doMetaCommand(inputStr))
		//	{
		//	case(META_COMMAND_SUCESS):
		//		continue;
		//	case(META_COMMAND_UNRECOGNIZED_COMMAND):
		//		cout << "Unrecognize command " + inputStr << endl;
		//		continue;
		//	}
		//}

		//Statement statement;

		//switch(prepareStatement(inputStr, statement))
		//{
		//case(PREPARE_SUCCESS):
		//	break;
		//case(PREPARE_SYNTAX_ERROR):
		//	cout << "Syntax error. Could not parse statement." << endl;
		//	continue;
		//case(PREPARE_UNRECOGNIZEED_STATEMENT):
		//	cout << "Unrecognize keyword at start of " + inputStr << endl;
		//	continue;
		//}

		//switch (ExcuteStatement(&statement, table))
		//{
		//case(EXECUTE_SUCCESS):
		//	cout << "Executed." << endl;
		//	break;
		//case(EXECUTE_TABLE_FULL):
		//	cout << "Error: Table full." << endl;
		//	break;
		//}

	}
	delete table;
}