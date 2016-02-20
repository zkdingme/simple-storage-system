#pragma once

	/*

	Tool.h
	we just put some useful tools here
	and these tools are easy for us to use

	*/

#include<iostream>
#include<sstream>
using namespace std;

	/*

	class Tools
	static string toString(const T& t)
	we just use it to convert any type to string

	*/

class Tools
{
public:
	template<typename T>
	static string toString(const T& t) 
	{
		ostringstream oss;
		oss << t;
		return oss.str();
	}
};