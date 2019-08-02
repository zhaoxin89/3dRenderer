#include <cstdio>
#include <string>
#include <vector>
#define PI 3.1415926
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

