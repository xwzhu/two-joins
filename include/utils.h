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
#include <string.h>
#include <time.h>

#define PRINT_NUM 5000000
#define MAX_NUM_TO_PRE_BUILD_TRIE 8000
using namespace std;

const string whiteSpaces(" \f\n\r\t\v");

void trimRight(string& str, const string& trimChars = whiteSpaces);

void trimLeft(string& str, const string& trimChars = whiteSpaces);

void trim(string& str, const string& trimChars = whiteSpaces);
vector<string> split_comma(istringstream &ss);

void split_comma(const string line, int* record);

int find_offset(const vector<string> &attrs, const string &target);

string get_dir_path(string dbSpecPath);

double diff_in_ms(timespec start, timespec end);

#endif /* UTILS_H_ */
