/*
 * Leapfrog.cpp
 *
 *  Created on: Mar 27, 2013
 *      Author: parallels
 */

#include "include/Leapfrog.h"

bool iterSorter(LinearIterator* iter1, LinearIterator* iter2) {
	return iter1->key() < iter2->key();
}

Leapfrog::Leapfrog(map<string, RelationSpec *> &relSpecs,
		vector<string> &orgJoinRels) {
	// TODO Auto-generated constructor stub
	atEnd = false;
	for (size_t i = 0; i != orgJoinRels.size(); i++) {
		LinearIterator* linearPtr = new LinearIterator(
				relSpecs[orgJoinRels[i]]);
		iterArray.push_back(linearPtr);
		if (linearPtr->at_end())
			atEnd = true;
	}
	size = iterArray.size();

	// sort the array iterArray by keys at which the iterators are positioned
	sort(iterArray.begin(), iterArray.end(), iterSorter);

	pIdx = 0;
	search();
}

void Leapfrog::search() {
	int maxKey = iterArray[(pIdx - 1) % size]->key();
	while (true) {
		int leastKey = iterArray[pIdx]->key();
		if (leastKey == maxKey) {
			key = leastKey;
			return;
		} else {
			iterArray[pIdx]->seek(maxKey);
			if (iterArray[pIdx]->at_end()) {
				atEnd = true;
				return;
			} else {
				maxKey = iterArray[pIdx]->key();
				pIdx = (pIdx + 1) % size;
			}
		}
	}
}

void Leapfrog::next() {
	iterArray[pIdx]->next();
	if (iterArray[pIdx]->at_end()) {
		atEnd = true;
	} else {
		pIdx = (pIdx + 1) % size;
		search();
	}
}

void Leapfrog::seek(const int seekKey) {
	iterArray[pIdx]->seek(seekKey);
	if (iterArray[pIdx]->at_end()) {
		atEnd = true;
	} else {
		pIdx = (pIdx + 1) % size;
		search();
	}
}
