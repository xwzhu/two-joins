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
	idx_t _thisIdxMap;
	idx_t _lastIdxMap; // the memDB array index map for the upper layer of trie
	CompleteArrayBST* _bst;
public:
	LinearIterator(RelationSpec* specIn, int depthIn = -1,
			int* stateIn = NULL);
	LinearIterator(RelationSpec* specIn, const idx_t &lastIdxMapIn, int depthIn,
				int* stateIn);
	~LinearIterator();
	int key();
	void next();
	bool at_end();
	void seek(const int seekKey);
	int* tuple();
	void display_record();
	idx_t get_idx_map();
	void reset();
	RelationSpec *spec;
private:
	void bst_vector(vector<int>& thisKeys);
};

#endif /* LINEARITERATOR_H_ */
