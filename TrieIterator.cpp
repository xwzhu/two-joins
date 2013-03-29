/*
 * TrieIterator.cpp
 *
 *  Created on: Mar 27, 2013
 *      Author: parallels
 */

#include "include/TrieIterator.h"

TrieIterator::TrieIterator(RelationSpec* specIn) {
	spec = specIn;
	_arity = spec->numOfAttr;
	_depth = -1;
	_atEnd = false;
	_state = new int[_arity];
	_iter = new LinearIterator(specIn, _depth, _state);
}

int TrieIterator::key() {
	assert(!_atEnd);
	assert(_depth >= 0);
	return _state[_depth];
}

bool TrieIterator::at_end() {
	return _atEnd;
}

void TrieIterator::open() {
	assert(!_atEnd);
	assert(_depth < _arity - 1);
///	assert(!_iter->at_end());
	++_depth;
	_stack.push(_iter);
	//open new iter
	_iter = new LinearIterator(spec, _iter->get_idx_map(), _depth, _state);
	update_state();
}

void TrieIterator::up() {
	assert(_depth >= 0);
	--_depth;
	_atEnd = false;
	// delete old _iter from lower layer, pop _iter from stack
	delete _iter;
	_iter = _stack.top();
	_stack.pop();
}

void TrieIterator::next() {
	assert(_depth >= 0);
	assert(!_atEnd);
	_iter->next(); //++_iter
	update_state();
}

int TrieIterator::get_depth() {
	return _depth;
}

void TrieIterator::seek(int seekKey) {
	assert(_depth >= 0);
	assert(!_atEnd);
//	int* seekTuple = _state;
//	seekTuple[_depth] = seekKey;
//	for (int i = _depth + 1; i < _arity; ++i)
//		seekTuple[_depth] = numeric_limits<int>::min(); //e.g.numeric_limits<int>::min();
//	_iter.seek_lub(seekTuple);
	_iter->seek(seekKey);
	update_state();
}

void TrieIterator::update_state() {
	if (_iter->at_end() || match_depth(_iter->tuple(), _state) < _depth - 1) {
		_atEnd = true;
	}
	else {
		_state[_depth] = _iter->tuple()[_depth];
	}
}

int TrieIterator::match_depth(int* x, int* y) {
	for (int i = 0; i < _arity; ++i)
		if (x[i] != y[i])
			return i-1;
	return _arity - 1;
}
