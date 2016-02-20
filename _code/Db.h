#pragma once
/*

Db.h
  we create some apis to package the IO stream and BTree

*/

/*

1. Return value of all the functions    ok!
2. Reference problem    ok!
3. Vector model problem    ok£¡
4. The machanism of catch and throw   ok!
5. Friendly Design and mistakes reminding  ok!

*/
#include<iostream>
#include<fstream>
#include<string>
#include"Tool.h"
#include"BTree.h"
#include"Reference.h"
using namespace std;
/*

   Space Left Coefficient
   vector model

*/
#define Coefficient 0.8  
/*

DATABASE CLASS

1. createOutputFile(...)
2. createInputFile(...)
3. deleteData(...) 
4. pushData(...)
5. setData(...)
6. openDataBase(...)

*/
template<typename T>
class DATABASE
{

public:
	
	friend class BTREE<Refer>;
	/*

	DATABASE()
	we store all the data into one file its name like 1.data etc
	we also use _size and _emptysize 

	*/
	DATABASE()
	{
		filenumber = 1;
		_path = Tools::toString(filenumber) + ".data";
		createOutputFile(_path);
		createInputFile(_path);
		_size = 0;
		_emptysize = 0;
	}
	/*

	DATABASE(string ReferName,string DbName)
	we open the reference file and database file
	to initialize our database
	
	*/
	DATABASE(string ReferName,string DbName)
	{
		openDataBase(ReferName);
		ofs.open(DbName, ios::out | ios::_Nocreate);
		ifs.open(DbName, ios::out | ios::_Nocreate);
		_emptysize = 0;
	}
	/*

	~DATABASE()
	close all the file and reserve the space
	
	*/
	~DATABASE()
	{
		ofs.close();
		ifs.close();
		ReferenceReturn();
		repertory.clear();
	}

	/*
	
	void createOutputFile(string file)
	open one file

	*/
	void createOutputFile(string file)
	{
		ofs.open(file,ios::out);
	}
	/*

	void createInputFile(string file)
	open one file

	*/
	void createInputFile(string file)
	{
		ifs.open(file,ios::in);
	}

	/*

	bool pushData(int  key, string  value)
	push the key and value into the database
	
	*/

	bool pushData(int  key, string  value)
	{
		bool check=repertory.insert(Refer(key, _size, value.length()));
		if (check)
		{
			dataPush(value); _size += value.length();
			return true;
		}
		else 
		{
			throw(exception("Data pushing fail!"));
			return false;
		}
	
	}

	/*
	
	bool deleteData(int key)
	first we find the data with the key,next we delete the reference and data together

	*/

	bool deleteData(int key)
	{
		Result<Refer> reference = repertory.find(key);
		if (reference.Pnode == NULL)
		{
			return false;
		}
		else
		{
			int len = reference.Pnode->keyValue[reference.sequence].length;
			int loc = reference.Pnode->keyValue[reference.sequence].location;
			if (reference.tag)
			{

				bool check = repertory.remove(Refer(key, loc, len));
				dataDelete(loc, len);
				if (check)
				{
					return true;
				}
				else
				{
					
					throw(exception("Data deleting fail!"));
					return false;
				}
			}
			else
			{
				throw(exception("Data deleting fail!"));
				return false;
			}
		}
	}

	/*
	
	string getData(int  key)
	find the data according to the key 
	
	*/

	string getData(int  key)
	{
		ofs.flush();
		
		Result<Refer> reference = repertory.find(key);
		
		if (reference.Pnode==NULL)
		{
			return "";
		}

		else 
		{
			int len = reference.Pnode->keyValue[reference.sequence].length;
			int loc = reference.Pnode->keyValue[reference.sequence].location;
			if (reference.tag)
			{

				return dataGet(loc, len);
			}
			else
			{
				throw(exception("Data getting fail!"));
				return "";
			}
		}
	}
	
	/*

	bool setData(int  key, string  value)
	first we find the value according to the key
	second we subscribe the value to the original string
	
	*/

	bool setData(int  key, string  value)
	{
		bool checkDelete=deleteData(key);
		if (checkDelete)
		{
			ofs.seekp(_size, ios::beg);
		    bool checkPush=pushData(key, value);
			if (checkPush)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else 
		{
			return false;
		}
    }

	/*

	void openDataBase(string dbName)
    first we input all the number ,and we push them into Btree,
	at the same time, we update the size and other things at the same time
	
	*/

	void openDataBase(string dbName)
	{
		ifstream opendb(dbName,ios::_Nocreate);
		int nodeIndex,nodeLoction,nodeLength;
		while (!opendb.eof())
		{
			opendb >> nodeIndex >> nodeLoction >> nodeLength;
			Refer re;
			re.index = nodeIndex;
			re.location = nodeLoction;
			re.length = nodeLength;
			repertory.insert(re);
			_size += nodeLength;
		}
	}



private:
	
	/*

	void dataPush(string value)
	push all the value into one file
	just use ofstream to input value

	*/
	
	void dataPush(string value)
	{
		ofs << value;

	}

	/*

	string dataGet(int index, int lens)
	use ifstream to get data

	*/

	string dataGet(int index, int lens)
	{
		ifs.seekg(ios::beg);
		int i = lens;
		string ret = "";
		ifs.seekg(index, ios::beg);
		while (i-- > 0)ret += ifs.get();
		return ret;
	}

	/*
	
	void dataDelete(int index, int lens)
	we use whitespace to mark the data to be deleted
	
	*/

	void dataDelete(int index, int lens)
	{
		ofs.seekp(ios::beg);
		char ch = ' ';
		int i = lens;
		ofs.seekp(index, ios::beg);

		while (i-- > 0)ofs.write(&ch, 1);
		_emptysize += lens;
		if ((double(_emptysize) /double(_size)) >= Coefficient)
		{
			return spaceClear();
		}

		else return;
	}

	/*

	void spaceClear()
	this is a vector model which we use Coefficient to judge whether we need spaceClear()
	we clear the origin space and we put all the data with the  tense format into a new file 
	
	*/

	void spaceClear()
	{
		filenumber++;
		_size = 0;
		_emptysize = 0;
		string file = Tools::toString(filenumber) + ".data";
		
		ofstream writeof;
		writeof.open(file);
		convertInAnotherFile(repertory.m_pRoot, writeof);
		writeof.close();
		
		string origin= Tools::toString(filenumber-1) + ".data";
		ofs.close();
		ifs.close();
		remove(origin.c_str());
		ofs.open(file,ios::_Nocreate);
		ifs.open(file,ios::_Nocreate);
	}
	
	/*

	void convertInAnotherFile(Node<Refer>*p, ofstream & os)
	this function is a helper of spaceClear()
	
	*/

	void convertInAnotherFile(Node<Refer>*p, ofstream & os)
	{
		if (p == NULL)return;
		for (int i = 0; i < p->keyNum; i++)
		{
			Refer & re = p->keyValue[i];
			string result = getData(re.index);
			os << result;
			re.location = _size;
			_size += result.length();
		}
		for (int i = 0; i <= p->keyNum; i++)
		{
			convertInAnotherFile(p->pChild[i], os);
		}
	}
	
	/*

    void ReferenceReturn()
	when we close the database, we output the reference into a file
	so, next time when we want to open the same database,we can read the reference file and the data file 
	
	*/

	void ReferenceReturn()
	{
		ofstream os("reference.data");
		referenceReturnTraverse(repertory.m_pRoot, os);
		os.close();
	}

    /*
	
	void referenceReturnTraverse(Node<Refer>* p, ofstream & os)
	this is a function of referenceReturn helper 

	*/

	void referenceReturnTraverse(Node<Refer>* p, ofstream & os)
	{
		if (p == NULL)return;
		for (int i = 0; i < p->keyNum; i++)
		{
			os << p->keyValue[i].index << " " << p->keyValue[i].location << " " << p->keyValue[i].length << " ";
		}
		for (int i = 0; i <= p->keyNum; i++)
		{
			referenceReturnTraverse(p->pChild[i], os);
		}
	}
private:
	long filenumber;
	long long _size;
	long long _emptysize;
	string _path;
	ofstream ofs;
	ifstream ifs;
	BTREE<Refer> repertory;
};