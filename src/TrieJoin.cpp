/*
 * TrieJoin.cpp
 *
 *  Created on: Mar 28, 2013
 *      Author: parallels
 */

#include "../include/TrieJoin.h"

void get_triejoin_record(TrieJoin* triejoin, deque<int> &seq,
		vector<int*> &memDB, size_t &recordCount) {
	while (!triejoin->at_end()) {
		seq.push_back(triejoin->key());
		if (triejoin->get_depth() != triejoin->get_attr_num() - 1) {
			triejoin->open(); //go to next depth
			get_triejoin_record(triejoin, seq, memDB, recordCount);
			triejoin->up();
		} else {
			assert(seq.size() == triejoin->get_attr_num());
			recordCount ++;
//			int tmpCount = 0;
//			int *record = new int[triejoin->get_attr_num()];
//			while (tmpCount != triejoin->get_attr_num()) {
//				seq.push_back(seq.front());
//				record[tmpCount] = seq.front();
//				seq.pop_front();
//				tmpCount++;
//			}
//			memDB.push_back(record);
			if (recordCount % PRINT_NUM == 1)
				cerr << '.';
			if (recordCount % (PRINT_NUM*20) == 0)
				cerr << endl;
		}
		seq.pop_back();
		triejoin->next();
	}
}

RelationSpec* leapfrog_triejoin(TrieJoin* triejoin, size_t &recordCount) {
	assert(triejoin->get_depth() == -1);
	deque<int> seq;
	vector<int*> memDB;

	RelationSpec* retSpec = NULL;
	retSpec = new RelationSpec("Joined", "", triejoin->get_attr_order());

	triejoin->open(); // go to depth 0
	get_triejoin_record(triejoin, seq, retSpec->memDB, recordCount);
	cerr << endl;
	triejoin->up(); // go back to depth -1

//	cerr << retSpec->relName << " " << retSpec->memDB.size() << endl;
	return retSpec;
}

TrieJoin::TrieJoin(const vector<string> &orgJoinRels,
		const vector<string> &joinAttrOrder,
		map<string, RelationSpec*> &relSpecs,
		const vector<TrieIterator*> &tries) {
	this->_trieJoinAttrOrder = joinAttrOrder;

	for (size_t i = 0; i != orgJoinRels.size(); i++) {
		RelationSpec* curSpec = relSpecs[orgJoinRels[i]];
		for (int j = 0; j != curSpec->numOfAttr; j++) {
			string curAttr = curSpec->attrNames[j];
			if (find_offset(joinAttrOrder, curAttr) == -1) {
				_trieJoinAttrOrder.push_back(curAttr);
			}
		}
	}
	_numOfAttrs = _trieJoinAttrOrder.size();

	// Construct an array of leapfrog join instnaces
	// one for each attribute
	for (int i = 0; i != _numOfAttrs; i++) {
		string curAttr = _trieJoinAttrOrder[i];

		//The leapfrog join for a variable x is given an array of pointers to
		//trie-iterators, one for each atom in whose argument list
		//the variable appears.
		vector<TrieIterator*> trieArray;
		for (size_t j = 0; j != orgJoinRels.size(); j++) {
			RelationSpec* curSpec = relSpecs[orgJoinRels[j]];
			if (curSpec->has_attr(curAttr)) {
				trieArray.push_back(tries[j]);
			}
		}
		Leapfrog* leapfrog = new Leapfrog(trieArray, curAttr);
		_leapArray.push_back(leapfrog);
	}

	// The triejoin uses a variable depth to track the current
	// attribute for which a binding is being sought; initially
	// depth = -1
	_depth = -1;
	_curLeap = NULL;
}

TrieJoin::~TrieJoin() {
	for (int i = 0; i != _numOfAttrs; i++) {
		delete _leapArray[i];
	}
}

void TrieJoin::open() {
	_depth++;
	assert(_depth >= 0);
	assert(_depth < _numOfAttrs);
	// for each iter in leapfrog join at current depth, call open()
	_curLeap = _leapArray[_depth];
	_curLeap->open_all();
	// call leapfrog-init() for leapfrog join at current depth
	_curLeap->init();
}

void TrieJoin::up() {
	assert(_depth >= 0);
	assert(_depth < _numOfAttrs);

	// for each iter in leapfrog join at current depth, call up()
	_curLeap->up_all();

	_depth--;
	_curLeap = _leapArray[_depth];
}

void TrieJoin::next() {
	_curLeap->next();
}

bool TrieJoin::at_end() {
	return _curLeap->at_end();
}

int TrieJoin::key() {
	return _curLeap->key();
}

int TrieJoin::get_depth() {
	return _depth;
}

int TrieJoin::get_attr_num() {
	return _numOfAttrs;
}

vector<string> TrieJoin::get_attr_order() {
	return _trieJoinAttrOrder;
}

void TrieJoin::show() {
	_curLeap->show();
}

void TrieJoin::show_trie_depth() {
	_curLeap->show_depth();
}

void TrieJoin::show_cur_attr() {
	assert(_depth >= 0);
	assert(_depth < _numOfAttrs);
	cerr << _trieJoinAttrOrder[_depth] << endl;
}
