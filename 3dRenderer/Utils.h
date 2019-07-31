#include <cstdio>
#include <string>
#pragma once

using namespace std;
float fscanFloat(FILE* fp);
int fscanInt(FILE* fp);
string fscanStr(FILE* fp, const string& expectedStr = "");
string fscanOneQuotation(FILE* fp);