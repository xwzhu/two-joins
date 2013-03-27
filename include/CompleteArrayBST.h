/*
 * bstree.h
 *
 *  Created on: Mar 26, 2013
 *      Author: parallels
 */

#ifndef BSTREE_H_
#define BSTREE_H_

#include "RelationSpec.h"

class CompleteArrayBST {
private:
	vector<int> array;
	RelationSpec* spec;
	int attrIdx;
	size_t curPos;
	size_t size;
	bool atEnd;
	void make_tree(const vector<int*> &sortedArray, size_t curRootPos, size_t lowIdx,
			size_t highIdx);
	bool has_left();
	bool has_right();
	bool has_parent();
	void move_left();
	void move_right();
	void move_parent(int& side);
	int peek_left();
	int peek_right();
	int peek_parent(int& side);
	void move_front();
public:
	CompleteArrayBST(RelationSpec* specIn, int attrIdxIN = 0);
	void search(int value);
	void show(size_t subRoot);
	void show(bool fromStart);
	void show_seek();
	void show_array();
	int key();
	int key_at_pos(size_t pos);
	size_t get_size();
	bool at_end();
	void next();
	void seek(const int seekKey);
};

#endif /* BSTREE_H_ */
