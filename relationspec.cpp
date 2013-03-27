/*
 * relationspec.cpp
 *
 *  Created on: Mar 10, 2013
 *      Author: parallels
 */

#include "include/relationspec.h"

RelationSpec::RelationSpec(string relName, string relPath,
		vector<string> attrNames) {
	// TODO Auto-generated constructor stub
	this->attrNames = attrNames;
	this->numOfAttr = attrNames.size();

	for (int i=0; i!=this->numOfAttr; i++)
		this->attrNamesMap.insert(make_pair(attrNames.at(i), i));

	this->recLen = attrNames.size() * sizeof(int);
	this->relName = relName;
	this->relFilePath = relPath;
}

RelationSpec::~RelationSpec() {
	// TODO Auto-generated destructor stub
//	cerr << "calling RelationSpec destructor for: " << relName << endl;
	for (int i=0; i!=memDB.size(); i++) {
		delete[] memDB[i];
	}
}

bool RelationSpec::build_relation() {
	cerr << "Building relation for: " << relName << endl;

	// open relation file
	ifstream relFile(relFilePath.c_str());
	if (relFile.is_open()) {
		string line;
		// read records from relation file
		while (relFile.good()) {
			int* curRec = new int[numOfAttr];
			getline(relFile, line);
			if (line.empty())
				continue;
			split_comma(line, curRec);

			// insert records into memory db
			memDB.push_back(curRec);
		}
		relFile.close();
		return true;
	}
	else {
		cerr << "Cannot open relation file: " << relName << endl;
		return false;
	}
}

bool RelationSpec::has_attr(string attrName) {
	return attrNamesMap.find(attrName) != attrNamesMap.end();
}

int RelationSpec::get_attr_idx(string attrName) {
	if (has_attr(attrName)) {
		return attrNamesMap[attrName];
	}
	else {
		return -1;
	}
}
