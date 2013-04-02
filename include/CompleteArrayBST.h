/*
 * bstree.h
 *
 *  Created on: Mar 26, 2013
 *      Author: parallels
 */

#ifndef BSTREE_H_
#define BSTREE_H_

#include "utils.h"

class CompleteArrayBST {
private:
	vector<int> idxArray;
	size_t curPos;
	size_t size;
	bool atEnd;
	void make_tree(const vector<int> &sortedArray, size_t curRootPos, size_t lowIdx,
			size_t highIdx);
	bool has_left();
	bool has_right();
	bool has_parent();
	void move_left();
	void move_right();
	void move_parent(int& side);
	int peek_parent(int& side);
	int key_at_pos(size_t pos);
public:
	CompleteArrayBST(const vector<int> &sortedArray);

	// interface of array based BST,
	// coherent with the paper
	int key();
	bool at_end();
	void next();
	void seek(const int seekKey, const bool checkParent=true);
	void move_front();
};

#endif /* BSTREE_H_ */
