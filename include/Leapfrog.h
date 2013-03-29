/*
 * Leapfrog.h
 *
 *  Created on: Mar 27, 2013
 *      Author: parallels
 */

#ifndef LEAPFROG_H_
#define LEAPFROG_H_

#include "RelationSpec.h"
#include "TrieIterator.h"

class Leapfrog {
public:
//	Leapfrog(map<string, RelationSpec *> &relSpecs, vector<string> &orgJoinRels);
	Leapfrog(const vector<TrieIterator*> &iterArrayIn, const string& attrName);
	void init();
	void search();
	void next();
	void seek(const int seekKey);
	bool at_end();
	void show();
	void open_all();
	void up_all();
	void show_depth();
	int key();
private:
	vector<TrieIterator *> iterArray;
	bool atEnd;
	size_t pIdx;
	size_t size;
	int _key;
	string attrName;
};

#endif /* LEAPFROG_H_ */
