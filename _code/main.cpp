/*

for  testing... 

*/
#include<iostream>
#include"Test.h"
#include<bitset>
using namespace std;
int main()
{
	try
	{
		//correctTest();
		//speedTest();
		//openTest();
		//vmTest();
		compareTest();
		//UserDo();//This is used for user to test!
	}
	catch (exception& e)
	{
		cerr << e.what() << endl;
	}

	system("pause");
	return 0;
}