/*
 * utils.h
 *
 *  Created on: Mar 26, 2013
 *      Author: parallels
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <assert.h>
#include <algorithm>
#include <stack>
#include <limits>
#include <string.h>
using namespace std;

const string whiteSpaces(" \f\n\r\t\v");

void trimRight(string& str, const string& trimChars = whiteSpaces);

void trimLeft(string& str, const string& trimChars = whiteSpaces);

void trim(string& str, const string& trimChars = whiteSpaces);
vector<string> split_comma(istringstream &ss);

void split_comma(const string line, int* record);

#endif /* UTILS_H_ */
