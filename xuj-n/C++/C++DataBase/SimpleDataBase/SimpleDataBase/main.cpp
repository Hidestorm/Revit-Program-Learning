#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
	string inputStr;
	while (true)
	{
		cout << "db >";
		getline(cin, inputStr);

		if (inputStr.compare(".exit") == 0)
		{
			exit(EXIT_SUCCESS);
		}
		else
		{
			cout << "Unrecognized command : " << inputStr << endl;
		}
	}
}