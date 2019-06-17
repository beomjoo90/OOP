#define _CRT_SECURE_NO_WARNINGS
#include "MyString.h"


MyString::MyString(int size)
	:size(size), data(new char[size])
{
	memset(data, '\0', size);
}
MyString::MyString(const char *inp)
	:size(strlen(inp)+1), data(new char[strlen(inp)+1])
{
	strcpy(this->data, inp);
}
MyString::MyString(const MyString& other)
	: size(other.size), data(new char[other.size])
{
	strcpy(this->data, other.data);
}
MyString::~MyString()
{
	if (data) {
		delete[] data;
	}
	data = nullptr;
	size = 0;
}

MyString& MyString::operator=(const MyString& other)
{
	if (data) {
		delete[] data;
	}
	data = new char[other.size];
	size = other.size;
	strcpy(data, other.data);
	return *this;
}
MyString MyString::operator+(const MyString& other)
{
	MyString temporary(this->size + other.size - 1);
	strcpy(temporary.data, this->data);
	strcat(temporary.data, other.data);
	return temporary;
}

ostream& operator<<(ostream& os, const MyString& mine)
{
	os << mine.data;
	return os;
}