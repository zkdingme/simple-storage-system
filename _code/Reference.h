#pragma once
	/*

	reference.h 
	this is the organization of the reference
	we have three members:index, length and length
	to represent the key , the location of first char and the length of the value  

	*/
#include<iostream>
using namespace std;
struct Refer
{
	int index;
	int location;
	int length;
	Refer(int in=-1,int lo=-1,int le=-1):index(in),location(lo),length(le){}
	/*

	we overload several function to make things easy 
	well done!
	
	*/
	friend bool operator<(const Refer& father,const Refer& son)
	{
		if (father.index < son.index)return true;
		else return false;
	}
	friend bool operator<=(const Refer& father, const Refer& son)
	{
		if (father.index <= son.index)return true;
		else return false;
	}
	friend bool operator>(const Refer& father, const Refer& son)
	{
		if (father.index > son.index)return true;
		else return false;
	}
	friend bool operator>=(const Refer& father, const  Refer& son)
	{
		if (father.index >= son.index)return true;
		else return false;
	}
	friend bool operator==(const Refer&father, const Refer& son)
	{
		if (father.index == son.index)return true;
		else return false;
	}
	friend ostream& operator <<(ostream& os, const Refer & son)
	{
		os << son.index;
		return os;
	}
};