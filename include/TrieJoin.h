/*
 * TrieJoin.h
 *
 *  Created on: Mar 28, 2013
 *      Author: parallels
 */

#ifndef TRIEJOIN_H_
#define TRIEJOIN_H_

#include "Leapfrog.h"

class TrieJoin {
private:
	vector<Leapfrog *> _leapArray;
	vector<string> _trieJoinAttrOrder;
	int _depth;
	int _numOfAttrs;
	Leapfrog* _curLeap;
public:
	TrieJoin(const vector<string> &orgJoinRels,
			const vector<string> &joinAttrOrder,
			map<string, RelationSpec*> &relSpecs,
			const vector<TrieIterator*> &tries);
	~TrieJoin();
	
	// interfaces coherent with paper
	void open();
	void up();
	void next();
	bool at_end();
	int key();

	// debugging functions
	void show();
	void show_cur_attr();
	void show_trie_depth();
	int get_depth();
	int get_attr_num();
	vector<string> get_attr_order();
};

RelationSpec* leapfrog_triejoin(TrieJoin* triejoin, size_t &recordCount,
		bool saveResult = false);
void get_triejoin_record(TrieJoin* triejoin, deque<int> &seq,
		vector<int*> &memDB, size_t &recordCount, bool &saveResult);

#endif /* TRIEJOIN_H_ */
