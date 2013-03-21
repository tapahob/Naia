#pragma once
#define MAX_DIGITS_IN_INT 12

std::string ToStr(int num, int base = 10);
std::string ToStr(unsigned int num, int base = 10);
std::string ToStr(unsigned long num, int base = 10);
std::string ToStr(float num);
std::string ToStr(double num);
std::string ToStr(bool val);