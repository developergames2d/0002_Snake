#include "Exception.hpp"
#include <iostream>
#include "Game.hpp"

using namespace std;

int main()
{
	try
	{
		Game().Run();
	}
	catch (const std::string & e)
	{
		cout << e << endl;
		system("pause");
	}

	return 0;
}