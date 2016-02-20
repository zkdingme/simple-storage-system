#pragma once
	/*

	Test.h
	we organize several test together
	and we also test all the implementation 
	delete set get and push

	another thing: we output all the result into one file

	*/
#include"BTree.h"
#include"Tool.h"
#include"Db.h"
#include<iostream>
#include<string>
#include<fstream>
#include"Reference.h"
#include<time.h>
#include<ctime>
#include<map>

	/*

	we define the two things:
	first is the random function in which we can produce the random number 
	second is the times of the loop

	*/

#define random(x) (rand()%x)
#define TIMES 5
using namespace std;
void correctTest()
{
	cout << "correct Test begin:" << endl;
	clock_t start, finish;
	string info = "aaaabbbcccc";
	string info2 = "fafafa";
	bool check = false;
	DATABASE<string>db;
	map<int, string>mm;
	start = clock();
	for (int i = 0; i < 1000000; i++)
	{
		mm[i] = info;
		db.pushData(i, info);
	}
	for (int i = 0; i < 5000; i++)
	{

		mm[i] = info2;
		db.setData(i, info2);
	}
	for (int i = 0; i < 1000000; i++)
	{
		if ((db.getData(i)) != (mm[i]))
		{
			cout << "Some thing error!" << endl;
			check = true;
			break;
		}
	}

	finish = clock();
	if (!check)cout << "No error!" << endl;
	cout << "correct Test pass" <<endl<< "correct Test time is: " << (finish - start) <<" ms"<< endl<<endl;

}
void speedTest()
{
	clock_t start, finish;
	ofstream of("speedtest.data");
	string info = "5140379061tyxsjtu";
	string sub = "Hello";
	for (int i = 0; i < 10000; i++)
	{
		info += '$';
		sub += '$';
	}
	DATABASE<string>db;

	of << "speed Test begin" << endl<<endl;

	of << "Test one---pushing Test" << endl << endl << endl << endl;
	cout << "Test one---pushing Test begin:" << endl;
	start = clock();
	for (int i = 0; i < 1000000; i++)
	{
		db.pushData(i, Tools::toString(i) + info);
	}
	finish = clock();
	of << endl<<"Test one pass" << endl << "Test one time:" << (finish - start) << " ms" << endl << endl << endl << endl;
	cout <<"Data insert success!"<<endl<< "Test one pass" << endl << "Test one time:" << (finish - start) <<" ms"<< endl;	
	cout << endl << endl;
	
	
	of << "Test two---getting Test" << endl<<endl<<endl<<endl;
	cout << "Test two---getting Test begin:" << endl;
	start = clock();
	for (int i = 0; i < 10000; i++)
	{
		string temp = db.getData(i);
		of << temp;
	}
	of << endl;
	finish = clock();
	of <<endl<< "Test two pass" << endl << "Test two time:" << (finish - start) << " ms" << endl << endl << endl << endl;
	cout <<"Data get success!"<<endl<< "Test two pass" << endl << "Test two time:" << (finish - start)<< " ms" << endl;
	cout << endl << endl;
	
	

	of << "Test three---Random setting and getting Test" << endl<<endl<<endl<<endl;
	cout << "Test three---Random setting and getting Test begin:" << endl;
	start = clock();
	for (int i = 0; i <TIMES; i++)
	{
		//srand((int)time(0));
		int x = random(1000);
		cout << "Random x is: " << x << endl;
		int y = random(1000);
		cout << "Random y is: " << y << endl;
		int z = random(1000);
		cout << "Random z is: " << z << endl;
		of << db.getData(x) << endl;
		db.deleteData(y);
		of << db.getData(y) << endl;
		db.setData(z, sub);
		of << db.getData(z) << endl;
	}
	finish = clock();
	of <<endl<< "Test three pass" << endl << "Test three time:" << (finish - start) << " ms" << endl << endl << endl << endl;
	cout <<"Data random success!"<<endl<< "Test three pass" << endl << "Test three time:" << (finish - start)<< " ms" << endl;
	cout << endl << endl;



	of << "Test four---deleting Test" << endl<<endl<<endl<<endl;
	cout << "Test four---deleting Test begin:" << endl;
	start = clock();
	for (int i = 1002; i < 100000; i++)
	{
		db.deleteData(i);
	}
	finish = clock();
	of <<endl<< "Test four pass" << endl << "Test four time:" << (finish - start) << " ms" << endl << endl << endl << endl;
	cout<<"Data delete success!"<<endl << "Test four pass" << endl << "Test four time:" << (finish - start)<< " ms" << endl;
	cout << endl << endl;


	of << "speed Test end" << endl<<endl;

	of.close();



}
void openTest()
{
	clock_t start, finish;
	ofstream of("opentest.data");

	DATABASE<string>db("reference.data", "1.data");

	of << "openTest begin" << endl << endl;

	of << "openTest ---open and close Test" << endl<<endl<<endl<<endl;
	cout << "openTest ---open and close Test" << endl;
	start = clock();
	for (int i = 200000; i < 300000; i++)
	{
		of << db.getData(i);
	}
	finish = clock();
	of <<endl<< "openTest pass" << endl << "openTest time:" << (finish - start) << " ms" << endl<<endl<<endl<<endl;
	cout <<"Data open success!"<<endl<< "openTest pass" << endl << "openTest time:" << (finish - start) << " ms" << endl;
	cout << endl << endl;


	of << "openTest end" << endl << endl;

	of.close();

}
void vmTest()
{
	clock_t start, finish;
	ofstream of("vmtest.data");
	DATABASE<string>db;
	string info = "5140379061tyxsjtu";

	of << "vmTest begin" << endl<<endl;

	of << "vmTest---vector model Test" << endl << endl << endl << endl;
	cout << "vmTest---vector model Test" << endl;
	start = clock();
	for (int i = 0; i < 1000000; i++)
	{
		db.pushData(i, info+Tools::toString(i));
	}
	for (int i = 0; i < 1000000; i++)
	{
		db.deleteData(i);
	}
	finish = clock();
	of << endl << "vmTest pass" << endl << "vmTest time:" << (finish - start) << " ms" << endl << endl << endl << endl;
	cout << "Vector model Test success!" << endl << "vmTest pass" << endl << "vmTest time:" << (finish - start) << " ms" << endl;
	cout << endl << endl;

	of << "vmTest end" << endl << endl;

	of.close();

}
void compareTest()
{
	clock_t start, finish;
	map<int, string>m;
	DATABASE<string>db;
	cout << "compare Test begin:" << endl;
	string info = "tangyuxin";
	string sub = "tang!!!!";


	start = clock();
	for (int i = 0; i < 10000000; i++)
	{
		m[i] = info;
	}
	finish = clock();
	cout << "Map add speed is: " << (finish - start) << " ms" << endl;
	
	cout << endl << endl;

	start = clock();
	for (int i = 0; i < 10000000; i++)
	{
		db.pushData(i, info);
	}
	finish = clock();

	cout << "DataBase add speed is: " << (finish - start) << " ms" << endl;


	cout << endl << endl;

	start = clock();
	for (int i = 0; i < 1000000; i++)
	{
		m.erase(i);
	}
	finish = clock();
	cout << "Map delete speed is: " << (finish - start) << " ms" << endl;

	cout << endl << endl;

	start = clock();
	for (int i = 0; i < 1000000; i++)
	{
		db.deleteData(i);
	}
	finish = clock();
	cout << "DataBase delete speed is: " << (finish - start) << " ms" << endl;

	cout << endl << endl;

	start = clock();
	for (int i = 1000001; i < 2000000; i++)
	{
		m[i] = sub;
	}
	finish = clock();
	cout << "Map set speed is: " << (finish - start) << " ms" << endl;

	cout << endl << endl;

	start = clock();
	for (int i = 1000001; i < 2000000; i++)
	{
		db.setData(i, sub);
	}
	finish = clock();
	cout << "DataBase set speed is: " << (finish - start) << " ms" << endl;

	cout << endl << endl;


	start = clock();
	ofstream of("compareGettest.data");
	of << "compareGettest begin" << endl << endl;

	for (int i = 1000001; i < 2000000; i++)
	{
		of << m[i];
	}
	finish = clock();
	cout << "Map get speed is: " << (finish - start) << " ms" << endl;

	cout << endl << endl;

	start = clock();
	for (int i = 1000001; i < 2000000; i++)
	{
		of<<db.getData(i);
	}
	finish = clock();
	cout << "DataBase get speed is: " << (finish - start) << " ms" << endl;

	of << "compareGettest end" << endl << endl;
	
	cout << "compare Test pass" << endl;

	of.close();


}
void UserDo()
{
	int choice;
	int key;
	string value;
	DATABASE<string>db;
	while (true)
	{
		cout << "1.Insert data" << endl;
		cout << "2.Get data" << endl;
		cout << "3.Delete data" << endl;
		cout << "4.Set data" << endl;
		cin >> choice;
		switch (choice)
		{
		case 1:cout << "Please input the key" << endl; cin >> key; cout << "Please input the value" << endl; cin >> value; db.pushData(key, value); cout << "Inserting complete!" << endl; break;
		case 2:cout << "Please input the key" << endl; cin >> key; cout << "Data is: " << db.getData(key) << endl; cout << "Getting complete!" << endl; break;
		case 3:cout << "Please input the key" << endl; cin >> key; db.deleteData(key); cout << "Deleting complete!" << endl; break;
		case 4:cout << "Please input the key" << endl; cin >> key; cout << "Please input the value" << endl; cin >> value; db.setData(key, value); break;
		}
	}

}