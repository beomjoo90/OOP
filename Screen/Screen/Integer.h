#pragma once
class Integer
{
public:
	Integer(int inp);
	Integer(const Integer& inp);
	~Integer();

	friend Integer operator+(const Integer& a, const Integer& inp);

	Integer operator+(const Integer& inp);
	Integer& operator=(const Integer& inp);

	static int nConstructorsCalled;
	static int nCopyConstructorsCalled;
	static int nGlobal;

	static void printStat();

	void print();
private:
	int internal;
};

