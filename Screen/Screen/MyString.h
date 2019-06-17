#pragma once
#include <iostream>
using namespace std;
class MyString
{
	int size;
	char* data;
public:
	MyString(int size);
	MyString(const char *inp);
	MyString(const MyString& other);
	~MyString();

	MyString& operator=(const MyString& other);
	MyString operator+(const MyString& other);

	friend ostream& operator<<(ostream& os, const MyString& mine);
};

