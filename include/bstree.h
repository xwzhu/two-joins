/*
 * bstree.h
 *
 *  Created on: Mar 26, 2013
 *      Author: parallels
 */

#ifndef BSTREE_H_
#define BSTREE_H_

#include "relationspec.h"

class CompleteArrayBST {
private:
	vector<int> array;
	RelationSpec* spec;
	int attrIdx;
	int curPos;
	void make_tree(const vector<int*> &sortedArray, int curRootPos, int lowIdx,
			int highIdx);
public:
	CompleteArrayBST(RelationSpec* specIn, int attrIdxIN = 0);
	void search(int value);
	void show(int subRoot);
};

#endif /* BSTREE_H_ */
