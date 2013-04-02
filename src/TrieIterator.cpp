/*
 * TrieIterator.cpp
 *
 *  Created on: Mar 27, 2013
 *      Author: parallels
 */

#include "../include/TrieIterator.h"

// We can use buildOnTheFly to decide whether to pre-build the TrieIterator
// That is whether we want to pre-build the map between trie path and LinearIterators
TrieIterator::TrieIterator(RelationSpec* specIn, bool buildOnTheFly) {
	spec = specIn;
	_arity = spec->numOfAttr;
	_depth = -1;
	_atEnd = false;
	_state = new int[_arity];
	_iter = new LinearIterator(specIn, _depth, _state);
	_rootIter = NULL;
	_vState.resize(_arity);
	_buildOnTheFly = buildOnTheFly;
	if (!_buildOnTheFly)
		build_map(_iter);
}

TrieIterator::~TrieIterator() {
	delete[] _state;
	delete _iter;
}

// This function is used to build the map between trie path and LinearIterators
// It is recursively called to iterate over the entire trie, and build the map
// during the iteration
void TrieIterator::build_map(LinearIterator* upperIter) {
	_depth++;
	if (_depth == 0) {
		_rootIter = new LinearIterator(spec, upperIter->get_idx_map(), _depth,
				_state);
		build_map(_rootIter);
	} else if (_depth != _arity) {
		assert(_depth >= 1);
		while (!upperIter->at_end()) {
			_state[_depth - 1] = upperIter->key();
			LinearIterator* thisIter = new LinearIterator(spec,
					upperIter->get_idx_map(), _depth, _state);
			update_vState();
			_linearIterMap[_vState] = thisIter;
			// recursively called to build next layer
			build_map(thisIter);
			upperIter->next();
		}
		upperIter->reset();
	}
	_depth--;
}

// The result part are implemented based on the pseudo code
// provided by the creator of Leapfrog Triejoin, they are not complicated
// and the key idea is using a stack to store the state of current layer
// before moving on the the deeper one

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
	++_depth;
	_stack.push(_iter);
	//open new iter
	if (!_buildOnTheFly) {
		if (_depth == 0) {
			_iter = _rootIter;
			_iter->reset();
		} else {
			update_vState();
			_iter = _linearIterMap[_vState];
			_iter->reset();
		}
	}
	else {
		_iter = new LinearIterator(spec, _iter->get_idx_map(), _depth, _state);
	}
	update_state();
}

void TrieIterator::up() {
	assert(_depth >= 0);
	--_depth;
	_atEnd = false;
	// delete old _iter from lower layer, pop _iter from stack
	if (_buildOnTheFly)
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
	_iter->seek(seekKey);
	update_state();
}

void TrieIterator::update_state() {
	if (_iter->at_end() || match_depth(_iter->tuple(), _state) < _depth - 1) {
		_atEnd = true;
	} else {
		_state[_depth] = _iter->tuple()[_depth];
	}
}

void TrieIterator::update_vState() {
	assert (_depth >= 1);
	for (int i = 0; i != _arity; i++) {
		if (i <= _depth - 1)
			_vState[i] = _state[i];
		else
			_vState[i] = -1;
	}
}

int TrieIterator::match_depth(int* x, int* y) {
	for (int i = 0; i < _arity; ++i)
		if (x[i] != y[i])
			return i - 1;
	return _arity - 1;
}
