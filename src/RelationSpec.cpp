/*
 * relationspec.cpp
 *
 *  Created on: Mar 10, 2013
 *      Author: parallels
 */

#include "../include/RelationSpec.h"

RelationSpec::RelationSpec(string relNameIn, string relPathIn,
		vector<string> attrNamesIn, bool addLineNo) {
	// TODO Auto-generated constructor stub
	_addLineNo = addLineNo;
	this->attrNames = attrNamesIn;
	if (_addLineNo)
		this->attrNames.push_back(relNameIn+"LineNo");
	this->numOfAttr = attrNames.size();

	for (int i = 0; i != this->numOfAttr; i++)
		this->_attrNamesMap.insert(make_pair(attrNames.at(i), i));

	this->relName = relNameIn;
	this->relFilePath = relPathIn;
}

RelationSpec::~RelationSpec() {
	// TODO Auto-generated destructor stub
//	cerr << "calling RelationSpec destructor for: " << relName << endl;
	for (size_t i = 0; i != memDB.size(); i++) {
		delete[] memDB[i];
	}
}

bool RelationSpec::build_relation() {
//	cerr << "Building relation for: " << relName << endl;
	map<string, int> duplicate;
	// open relation file
	ifstream relFile(relFilePath.c_str());
	if (relFile.is_open()) {
		string line;
		// read records from relation file
		int lineNo = 0;
		while (relFile.good()) {
			int* curRec = new int[numOfAttr];
			getline(relFile, line);
//			if (duplicate.find(line) != duplicate.end()) {
//				duplicate[line] += 1;
//				continue;
//			} else {
//				duplicate[line] = 0;
//			}
			if (line.empty())
				continue;
			split_comma(line, curRec);
			if (_addLineNo)
				curRec[numOfAttr-1] = lineNo;
			// insert records into memory db
			memDB.push_back(curRec);

			lineNo ++;
		}
		relFile.close();
		return true;
	} else {
		cerr << "Cannot open relation file: " << relName << endl;
		return false;
	}
}

bool RelationSpec::has_attr(string attrName) {
	return _attrNamesMap.find(attrName) != _attrNamesMap.end();
}

int RelationSpec::get_attr_idx(string attrName) {
	if (has_attr(attrName)) {
		return _attrNamesMap[attrName];
	} else {
		return -1;
	}
}

void RelationSpec::print_relation(size_t limit) {
	if (limit < memDB.size()) {
		cerr << "Warning: partial results." << endl;
	}
	for (size_t i = 0; i != min(memDB.size(), limit); i++) {
		for (int j = 0; j != numOfAttr; j++) {
			cerr << memDB[i][j] << " ";
		}
		cerr << endl;
	}
}
