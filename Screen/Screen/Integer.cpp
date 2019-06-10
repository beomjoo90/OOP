#include <iostream>
#include "Integer.h"

int Integer::nConstructorsCalled = 0;
int Integer::nCopyConstructorsCalled = 0;

void Integer::printStat()
{
	printf("# of constructors = %d, # of copy constructors = %d, # of global = %d\n",
		nConstructorsCalled,
		nCopyConstructorsCalled,
		nGlobal
	);
}

Integer::Integer(int inp)
	: internal(inp)
{
	++nConstructorsCalled;
}

int Integer::nGlobal = 0;

Integer::Integer(const Integer& inp)
	: internal(inp.internal)
{
	++nCopyConstructorsCalled;
}


Integer::~Integer()
{
}


void Integer::print()
{
	printf("%d", internal);
}

Integer Integer::operator+(const Integer& b)
{
	Integer temporary{ this->internal + b.internal };
	return temporary;
}


Integer operator+(const Integer& a, const Integer& b)
{
	Integer temporary{ a.internal + b.internal };
	++Integer::nGlobal;
	return temporary;
}


Integer& Integer::operator=(const Integer& inp)
{
	this->internal = inp.internal;
	return *this;
}
