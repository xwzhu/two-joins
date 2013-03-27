/*
 * Leapfrog.h
 *
 *  Created on: Mar 27, 2013
 *      Author: parallels
 */

#ifndef LEAPFROG_H_
#define LEAPFROG_H_

#include "RelationSpec.h"
#include "LinearIterator.h"

class Leapfrog {
public:
	Leapfrog(map<string, RelationSpec *> &relSpecs, vector<string> &orgJoinRels);
	void search();
	void next();
	void seek(const int seekKey);
	bool at_end();
	void show();
private:
	vector<LinearIterator *> iterArray;
	bool atEnd;
	size_t pIdx;
	size_t size;
	int key;
};

#endif /* LEAPFROG_H_ */
