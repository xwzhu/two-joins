/*
 * LinearIterator.h
 *
 *  Created on: Mar 27, 2013
 *      Author: parallels
 */

#ifndef LINEARITERATOR_H_
#define LINEARITERATOR_H_

#include "RelationSpec.h"
#include "CompleteArrayBST.h"

typedef map<int, pair<size_t, size_t> > idx_t;

class LinearIterator {
private:
	int* _state;
	int _depth;
	idx_t _thisIdxMap; // the memDB array index map for the current layer of trie
	idx_t _lastIdxMap; // the memDB array index map for the upper layer of trie
	CompleteArrayBST* _bst;
public:
	LinearIterator(RelationSpec* specIn, int depthIn = -1,
			int* stateIn = NULL);
	LinearIterator(RelationSpec* specIn, const idx_t &lastIdxMapIn, int depthIn,
				int* stateIn);
	~LinearIterator();

	// interfaces to be called by TrieIterator
	// coherent with the paper
	int key();
	void next();
	bool at_end();
	void seek(const int seekKey);
	int* tuple();
	idx_t get_idx_map();
	void reset();

	// debugging fuction
	void display_record();
private:
	void bst_vector(vector<int>& thisKeys);
public:
	RelationSpec *spec;
};

#endif /* LINEARITERATOR_H_ */
