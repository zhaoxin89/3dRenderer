#include "Utils.h"
#include <cassert>

float fscanFloat(FILE* fp) {
	assert(fp);
	float temp = 0;
	fscanf(fp, "%f", &temp);
	return temp;

}
int fscanInt(FILE* fp) {
	assert(fp);
	int temp = 0;
	fscanf(fp, "%i", &temp);
	return temp;
}

static char buffer[1024] = { 0 };//used for fscan string

string fscanStr(FILE* fp, const string& expectedStr) {
	assert(fp);
	fscanf(fp, "%s", buffer);
	string str = buffer;
	if (expectedStr.empty() == false) {
		assert(str == expectedStr);
	}
	return str;
}
string fscanOneQuotation(FILE* fp) {
	assert(fp);
	string quoatation = "";
	fscanf(fp, "%s", buffer);
	string beginStr = buffer;
	assert(beginStr[0] == '\"');
	quoatation += beginStr;
	while (1) {
		char endCh = quoatation[(int)quoatation.size() - 1];
		if (endCh == '\"') {
			break;
		}
		fscanf(fp, "%s", buffer);
		string str = buffer;
		quoatation += str;

	}//got quoatation
	return quoatation;

}