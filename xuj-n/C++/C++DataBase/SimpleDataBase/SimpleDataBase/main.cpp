#include <iostream>
#include <string>
#include "SqlCore.h"

using namespace std;


int main(int argc, char* argv[])
{
	SqlTable *table = new SqlTable;

	string inputStr;
	while (true)
	{
		inputStr.clear();
		cout << "db >";
		try
		{
			getline(cin, inputStr);
		}
		catch (...)
		{
			continue;
		}

		if (inputStr[0] == '.')
		{
			switch (doMetaCommand(inputStr))
			{
			case(META_COMMAND_SUCESS):
				continue;
			case(META_COMMAND_UNRECOGNIZED_COMMAND):
				cout << "Unrecognize command " + inputStr << endl;
				continue;
			}
		}

		Statement statement;

		switch(prepareStatement(inputStr, statement))
		{
		case(PREPARE_SUCCESS):
			break;
		case(PREPARE_SYNTAX_ERROR):
			cout << "Syntax error. Could not parse statement." << endl;
			continue;
		case(PREPARE_UNRECOGNIZEED_STATEMENT):
			cout << "Unrecognize keyword at start of " + inputStr << endl;
			continue;
		}

		switch (ExcuteStatement(&statement, table))
		{
		case(EXECUTE_SUCCESS):
			cout << "Executed." << endl;
			break;
		case(EXECUTE_TABLE_FULL):
			cout << "Error: Table full." << endl;
			break;
		}

	}
}