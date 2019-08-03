#include <cstdio>
#include <string>
#include <vector>
#define PI 3.1415926

#define SWAP(a,b,t) {t=a;a=b;b=t;}

using namespace std;
#pragma once

using namespace std;
float fscanFloat(FILE* fp);
int fscanInt(FILE* fp);
int charToNumber(char c);
int strToNumber(const string& numStr);
string fscanStr(FILE* fp, const string& expectedStr = "");
string fscanOneQuotation(FILE* fp);
vector<string> divideStr(const string& str, const string& dividerChars);

float FastInverseSqrt(float x);
