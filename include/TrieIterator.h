/*
 * TrieIterator.h
 *
 *  Created on: Mar 27, 2013
 *      Author: parallels
 */

#ifndef TRIEITERATORS_H_
#define TRIEITERATORS_H_

#include "RelationSpec.h"
#include "LinearIterator.h"

class TrieIterator {
	int _arity;
	int _depth; /*ranges from -1 to _arity-1*/
	LinearIterator* _iter; /*Current position in BS tree*/
	stack<LinearIterator*> _stack; /*Iterator state saved when open() called*/
	int* _state; /*Elements 0.._depth contain current trie path*/
	bool _atEnd;
public:
	RelationSpec* spec;
	TrieIterator(RelationSpec* specIn);
	~TrieIterator();
	int key();
	bool at_end();
	void open();
	void up();
	void next();
	void seek(int seek_key);
	int get_depth();
private:
	void update_state();
	int match_depth(int* x, int* y);
};

#endif /* TRIEITERATORS_H_ */
