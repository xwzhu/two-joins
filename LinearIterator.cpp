/*
 * LinearIterator.cpp
 *
 *  Created on: Mar 27, 2013
 *      Author: parallels
 */

#include "include/LinearIterator.h"
#include "include/SortMerge.h"

LinearIterator::LinearIterator(RelationSpec* specIn, int depthIn,
		int* stateIn) {
//	cerr << "Depth: " << depthIn << endl;
// TODO Auto-generated constructor stub
	assert(depthIn >= -1);
	assert(stateIn!=NULL);

// sort the memDB based on attrIdxIN
	spec = specIn;
	depth = depthIn;
	state = new int[specIn->numOfAttr];
	memcpy(state, stateIn, sizeof(int) * specIn->numOfAttr);

	if (depth == -1) {
		bst = NULL;
	} else {
		vector<int> bv = bst_vector();
		cerr << bv.size() << endl;
		bst = new CompleteArrayBST(bv);
		state[depthIn] = bst->key();
	}
}

LinearIterator::LinearIterator(RelationSpec* specIn, const idx_t lastIdxMapIn,
		int depthIn, int* stateIn) {
// TODO Auto-generated constructor stub
//	cerr << "Depth: " << depthIn << endl;
	assert(depthIn >= 0);
	assert(stateIn!=NULL);

	spec = specIn;
	depth = depthIn;
	state = new int[specIn->numOfAttr];
	memcpy(state, stateIn, sizeof(int) * specIn->numOfAttr);

	lastIdxMap = lastIdxMapIn;
	vector<int> bv = bst_vector();
//	cerr << lastIdxMap.size() << " " << thisIdxMap.size() << " " << bv.size() << endl;
	bst = new CompleteArrayBST(bv);
	state[depthIn] = bst->key();
}

LinearIterator::~LinearIterator() {
	delete state;
}

vector<int> LinearIterator::bst_vector() {
	pair<size_t, size_t> idx;
	if (depth == 0) {
		idx = make_pair(0, spec->memDB.size());
	} else {
		int lastKey = state[depth - 1];
//		cerr << "last key:" << lastKey << endl;
		assert(lastIdxMap.find(lastKey) != lastIdxMap.end());
		idx = lastIdxMap[lastKey];
		assert(idx.first <= idx.second);
	}

	vector<int> thisKeys;
	int lastKey;
	bool firstRun = true;
	size_t curStart = idx.first;
	size_t curEnd;
	for (size_t i = idx.first; i != idx.second; i++) {
		int curKey = spec->memDB[i][depth];

		if (firstRun || curKey != lastKey) {
			thisKeys.push_back(curKey);
			if (!firstRun) {
				curEnd = i;
				thisIdxMap[lastKey] = make_pair(curStart, curEnd);
				curStart = i;
			}
		}

		lastKey = curKey;
		firstRun = false;
	}
	thisIdxMap[lastKey] = make_pair(curStart, idx.second);

	return thisKeys;
}


int LinearIterator::key() {
	return bst->key();
}

int* LinearIterator::tuple() {
	return state;
}

void LinearIterator::display_record() {
	int * curRec = tuple();
	for (int i = 0; i != spec->numOfAttr; i++) {
		cerr << curRec[i] << " ";
	}
	cerr << endl;
}

idx_t LinearIterator::get_idx_map() {
	return thisIdxMap;
}

void LinearIterator::next() {
	bst->next();
	state[depth] = bst->key();
}

bool LinearIterator::at_end() {
	if (bst != NULL)
		return bst->at_end();
	else
		return true;
}

void LinearIterator::seek(const int seekKey) {
	bst->seek(seekKey);
	if (!bst->at_end()) {
		state[depth] = bst->key();
	}
}
