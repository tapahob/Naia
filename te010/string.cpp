#include "precompiled.h"

typedef std::string string;

string ToStr(int num, int base)
{
	char str[MAX_DIGITS_IN_INT];
	memset(str,0,MAX_DIGITS_IN_INT);
	_itoa_s(num,str,MAX_DIGITS_IN_INT, base);
	return (string(str));
}

string ToStr(unsigned int num, int base)
{
	char str[MAX_DIGITS_IN_INT];
	memset(str, 0, MAX_DIGITS_IN_INT);
	_ultoa_s((unsigned long)num, str, MAX_DIGITS_IN_INT, base);
	return (string(str));
}

string ToStr(unsigned long num, int base)
{
	char str[MAX_DIGITS_IN_INT];
	memset(str,0,MAX_DIGITS_IN_INT);
	_ultoa_s(num,str,MAX_DIGITS_IN_INT,base);
	return (string(str));
}

string ToStr(float num)
{
	char str[64];
	memset(str,0,64);
	_sprintf_p(str,64,"%f",num);
	return (string(str));
}

string ToStr(double num)
{
	char str[64];
	memset(str,0,64);
	_sprintf_p(str,64,"%fL",num);
	return (string(str));
}

string ToStr(bool val)
{
	return (string( (val == true ? "true" : "false") ));
}