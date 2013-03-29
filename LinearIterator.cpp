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
	assert(depthIn >= -1);
	assert(stateIn!=NULL);

	spec = specIn;
	_depth = depthIn;
	_state = new int[specIn->numOfAttr];
	memcpy(_state, stateIn, sizeof(int) * specIn->numOfAttr);

	if (_depth == -1) {
		_bst = NULL;
	} else {
		vector<int> bv;
		bst_vector(bv);
		cerr << bv.size() << endl;
		_bst = new CompleteArrayBST(bv);
		_state[depthIn] = _bst->key();
	}
}

LinearIterator::LinearIterator(RelationSpec* specIn, const idx_t &lastIdxMapIn,
		int depthIn, int* stateIn) {
	assert(depthIn >= 0);
	assert(stateIn!=NULL);

	spec = specIn;
	_depth = depthIn;
	_state = new int[specIn->numOfAttr];
	memcpy(_state, stateIn, sizeof(int) * specIn->numOfAttr);

	_lastIdxMap = lastIdxMapIn;
	vector<int> bv;
	bst_vector(bv);
	_bst = new CompleteArrayBST(bv);
	_state[depthIn] = _bst->key();
}

LinearIterator::~LinearIterator() {
	delete _state;
}

void LinearIterator::bst_vector(vector<int>& thisKeys) {
	pair<size_t, size_t> idx;
	if (_depth == 0) {
		idx = make_pair(0, spec->memDB.size());
	} else {
		int lastState = _state[_depth - 1];
		assert(_lastIdxMap.find(lastState) != _lastIdxMap.end());
		idx = _lastIdxMap[lastState];
		assert(idx.first <= idx.second);
	}

	int lastKey;
	bool firstRun = true;
	size_t curStart = idx.first;
	size_t curEnd;
	for (size_t i = idx.first; i != idx.second; i++) {
		int curKey = spec->memDB[i][_depth];

		if (firstRun || curKey != lastKey) {
			thisKeys.push_back(curKey);
			if (!firstRun) {
				curEnd = i;
				_thisIdxMap[lastKey] = make_pair(curStart, curEnd);
				curStart = i;
			}
		}

		lastKey = curKey;
		firstRun = false;
	}
	_thisIdxMap[lastKey] = make_pair(curStart, idx.second);
}


int LinearIterator::key() {
	return _bst->key();
}

int* LinearIterator::tuple() {
	return _state;
}

void LinearIterator::display_record() {
	int * curRec = tuple();
	for (int i = 0; i != spec->numOfAttr; i++) {
		cerr << curRec[i] << " ";
	}
	cerr << endl;
}

idx_t LinearIterator::get_idx_map() {
	return _thisIdxMap;
}

void LinearIterator::next() {
	_bst->next();
	_state[_depth] = _bst->key();
}

bool LinearIterator::at_end() {
	if (_bst != NULL)
		return _bst->at_end();
	else
		return true;
}

void LinearIterator::seek(const int seekKey) {
	_bst->seek(seekKey);
	if (!_bst->at_end()) {
		_state[_depth] = _bst->key();
	}
}
