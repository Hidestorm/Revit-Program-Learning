#include <iostream>
#include <string>

using namespace std;

enum MetaCommondResult
{
	META_COMMAND_SUCESS,
	META_COMMAND_UNRECOGNIZED_COMMAND
};

enum PrepareResult
{
	PREPARE_SUCCESS,
	PREPARE_UNRECOGNIZEED_STATEMENT
};

enum StatementType
{
	STATEMENT_INSERT,
	STATEMENT_SELECT
};

struct Statement
{
	StatementType type;

};

MetaCommondResult doMetaCommand(const string& str)
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

PrepareResult prepareStatement(const string & str, Statement &statement)
{
	if (str.compare(0, 6, "insert") == 0)
	{
		statement.type = STATEMENT_INSERT;
		return PREPARE_SUCCESS;
	}

	if (str.compare("select") == 0)
	{
		statement.type = STATEMENT_SELECT;
		return PREPARE_SUCCESS;
	}
	return PREPARE_UNRECOGNIZEED_STATEMENT;
}

void ExecuteStatement(const Statement& statement)
{
	switch (statement.type)
	{
	case(STATEMENT_INSERT):
		cout << "This is where we would do an insert." << endl;
		break;
	case(STATEMENT_SELECT):
		cout << "This is where we would do an select." << endl;
		break;
	}
}


int main(int argc, char* argv[])
{
	string inputStr;
	while (true)
	{
		cout << "db >";
		getline(cin, inputStr);

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
		case(PREPARE_UNRECOGNIZEED_STATEMENT):
			cout << "Unrecognize keyword at start of " + inputStr << endl;
			continue;
		}

		ExecuteStatement(statement);

		cout << "Executed." << endl;

	}
}