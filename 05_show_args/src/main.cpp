#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{

	if (argc == 1)
	{
		cout << "No arguments provided"; // true if no arguments provided
	}
	else
	{
		for (int index = 1; index < argc; index++)
		{
			cout << argv[index] << endl; //print all the arguments provided
		}
	}
	

	return 0;
}