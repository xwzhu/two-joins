/*
 * Leapfrog.cpp
 *
 *  Created on: Mar 27, 2013
 *      Author: parallels
 */

#include "include/Leapfrog.h"

bool iterSorter(TrieIterator* iter1, TrieIterator* iter2) {
	return iter1->key() < iter2->key();
}

//Leapfrog::Leapfrog(map<string, RelationSpec *> &relSpecs,
//		vector<string> &orgJoinRels) {
//	// TODO Auto-generated constructor stub
//	atEnd = false;
//	size = orgJoinRels.size();
//	for (size_t i = 0; i != size; i++) {
//		LinearIterator* linearPtr = new LinearIterator(
//				relSpecs[orgJoinRels[i]]);
//		iterArray.push_back(linearPtr);
////		linearPtr->display_record();
//		if (linearPtr->at_end())
//			atEnd = true;
//	}
//
//	// sort the array iterArray by keys at which the iterators are positioned
//	sort(iterArray.begin(), iterArray.end(), iterSorter);
//
//	pIdx = 0;
//	search();
//}

Leapfrog::Leapfrog(const vector<TrieIterator*> &iterArrayIn,
		const string& attrNameIn) {
//	cerr << "Building Leapfrog: " << attrNameIn << endl;
	iterArray = iterArrayIn;
	attrName = attrNameIn;
	atEnd = false;
	size = iterArray.size();

	_key = -1;
	pIdx = -1;
}

void Leapfrog::init() {
	atEnd = false;

	for (size_t i = 0; i != size; i++) {
		if (iterArray[i]->at_end()) {
			atEnd = true;
			break;
		}
	}
	if (!atEnd) {
		// sort the array iterArray by keys at which the iterators are positioned
		sort(iterArray.begin(), iterArray.end(), iterSorter);
		pIdx = 0;
		search();
	}
}

void Leapfrog::search() {
	int maxKey = iterArray[(pIdx + size - 1) % size]->key();
	while (true) {
		int leastKey = iterArray[pIdx]->key();
		if (leastKey == maxKey) {
			_key = leastKey;
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

bool Leapfrog::at_end() {
	return atEnd;
}

void Leapfrog::show() {
	cerr << "pIdx " << pIdx << "    ";
	for (size_t i = 0; i != size; i++) {
		cerr << iterArray[i]->spec->relName << " " << iterArray[i]->key()
				<< " ";
	}
	cerr << endl;
}

void Leapfrog::show_depth() {
	for (size_t i = 0; i != size; i++) {
		cerr << iterArray[i]->spec->relName << ": " << iterArray[i]->get_depth()
				<< " ";
	}
	cerr << endl;
}

void Leapfrog::open_all() {
	for (size_t i = 0; i != size; i++) {
		iterArray[i]->open();
	}
}

void Leapfrog::up_all() {
	for (size_t i = 0; i != size; i++) {
		iterArray[i]->up();
	}
}

int Leapfrog::key() {
	assert(!atEnd);
	return _key;
}
