/*
 * Leapfrog.h
 *
 *  Created on: Mar 27, 2013
 *      Author: parallels
 */

#ifndef LEAPFROG_H_
#define LEAPFROG_H_

#include "RelationSpec.h"
#include "TrieIterator.h"

class Leapfrog {
public:
	Leapfrog(const vector<TrieIterator*> &iterArrayIn, const string& attrName);

	// interface of Leapfrog,
	// coherent with the paper
	void init();
	void search();
	void next();
	void seek(const int seekKey);
	bool at_end();
	int key();
	void open_all();
	void up_all();

	// debugging functions
	void show();
	void show_depth();
private:
	vector<TrieIterator *> _iterArray;
	bool _atEnd;
	size_t _pIdx;
	size_t _size;
	int _key;
	string _attrName;
};

#endif /* LEAPFROG_H_ */
