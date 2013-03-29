/*
 * relationspec.h
 *
 *  Created on: Mar 10, 2013
 *      Author: parallels
 */

#ifndef RELATIONSPEC_H_
#define RELATIONSPEC_H_

#include "utils.h"

class RelationSpec {
public:
	RelationSpec(string relName, string relPath, vector<string> attrNames);
	~RelationSpec();
	bool build_relation();
	bool has_attr(string attrName);
	int  get_attr_idx(string attrName);
	void print_relation(size_t limit = 2000);

	string relName;
	string relFilePath;
	vector<int*> memDB;
	vector<string> attrNames; // the name of attributes
	int numOfAttr; // # of attributes

private:
	map<string, int> _attrNamesMap; // the name of attributes
};

#endif /* RELATIONSPEC_H_ */
