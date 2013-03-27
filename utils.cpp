/*
 * utils.cpp
 *
 *  Created on: Mar 26, 2013
 *      Author: parallels
 */

#include "include/utils.h"

void trimRight(string& str, const string& trimChars) {
	string::size_type pos = str.find_last_not_of(trimChars);
	str.erase(pos + 1);
}

void trimLeft(string& str, const string& trimChars) {
	string::size_type pos = str.find_first_not_of(trimChars);
	str.erase(0, pos);
}

void trim(string& str, const string& trimChars) {
	trimRight(str, trimChars);
	trimLeft(str, trimChars);
}

vector<string> split_comma(istringstream &ss) {
	char temp[256];
	vector<string> ret;
	while (ss.getline(temp, 256, ',')) {
		string element(temp);
		trim(element);
		if (!element.empty()) {
			ret.push_back(element);
		}
	}
	return ret;
}

void split_comma(const string line, int* record) {
	char temp[256];
	int i = 0;
	istringstream ss(line);
	while (ss.getline(temp, 256, ',')) {
		string attrValue(temp);
		istringstream ssValue(attrValue);
		ssValue >> record[i];
		i++;
	}
}



