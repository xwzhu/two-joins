/*
 * TrieJoin.cpp
 *
 *  Created on: Mar 28, 2013
 *      Author: parallels
 */

#include "include/TrieJoin.h"

void get_triejoin_record(TrieJoin* triejoin, deque<int> &seq,
		vector<int*> &memDB) {
	while (!triejoin->at_end()) {
		seq.push_back(triejoin->key());
		if (triejoin->get_depth() != triejoin->get_attr_num() - 1) {
			triejoin->open(); //go to next depth
			get_triejoin_record(triejoin, seq, memDB);
			triejoin->up();
		} else {
			assert(seq.size() == triejoin->get_attr_num());
			int tmpCount = 0;
			int *record = new int[triejoin->get_attr_num()];
			while (tmpCount != triejoin->get_attr_num()) {
				seq.push_back(seq.front());
				record[tmpCount] = seq.front();
				seq.pop_front();
				tmpCount++;
			}
			memDB.push_back(record);
		}
		seq.pop_back();
		triejoin->next();
	}
}

RelationSpec* leapfrog_triejoin(TrieJoin* triejoin) {
	assert(triejoin->get_depth() == -1);
	deque<int> seq;
	vector<int*> memDB;
	triejoin->open(); // go to depth 0
	get_triejoin_record(triejoin, seq, memDB);
	triejoin->up(); // go back to depth -1
	cerr << memDB.size() << endl;
}

TrieJoin::TrieJoin(const vector<string> &orgJoinRels,
		const vector<string> &joinAttrOrder,
		map<string, RelationSpec*> &relSpecs,
		const vector<TrieIterator*> &tries) {
	numOfAttrs = joinAttrOrder.size();
	this->joinAttrOrder = joinAttrOrder;

	cerr << "Constructing: TrieJoin ... ";

	// Construct an array of leapfrog join instnaces
	// one for each attribute
	for (int i = 0; i != numOfAttrs; i++) {
		string curAttr = joinAttrOrder[i];

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
		leapArray.push_back(leapfrog);
	}

	// The triejoin uses a variable depth to track the current
	// attribute for which a binding is being sought; initially
	// depth = -1
	depth = -1;
	curLeap = NULL;
	cerr << "done." << endl;
}

TrieJoin::~TrieJoin() {
	for (int i = 0; i != numOfAttrs; i++) {
		delete leapArray[i];
	}
}

void TrieJoin::open() {
	depth++;
	assert(depth >= 0);
	assert(depth < numOfAttrs);
	// for each iter in leapfrog join at current depth, call open()
	curLeap = leapArray[depth];
	curLeap->open_all();
	// call leapfrog-init() for leapfrog join at current depth
	curLeap->init();
}

void TrieJoin::up() {
	assert(depth >= 0);
	assert(depth < numOfAttrs);

	// for each iter in leapfrog join at current depth, call up()
	curLeap->up_all();

	depth--;
	curLeap = leapArray[depth];
}

void TrieJoin::next() {
	curLeap->next();
}

bool TrieJoin::at_end() {
	return curLeap->at_end();
}

int TrieJoin::key() {
	return curLeap->key();
}

int TrieJoin::get_depth() {
	return depth;
}

int TrieJoin::get_attr_num() {
	return numOfAttrs;
}

void TrieJoin::show() {
	curLeap->show();
}

void TrieJoin::show_trie_depth() {
	curLeap->show_depth();
}

void TrieJoin::show_cur_attr() {
	assert(depth >= 0);
	assert(depth < numOfAttrs);
	cerr << joinAttrOrder[depth] << endl;
}
