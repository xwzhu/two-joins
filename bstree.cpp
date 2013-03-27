/*
 * bstree.cpp
 *
 *  Created on: Mar 26, 2013
 *      Author: parallels
 */

#include "include/bstree.h"

CompleteArrayBST::CompleteArrayBST(RelationSpec* specIn, int attrIdxIN) {
	cerr << "Building BST: " << specIn->relName << endl;
	spec = specIn;
	array.resize(spec->memDB.size());
	attrIdx = attrIdxIN;
	curPos = 0;
	make_tree(spec->memDB, 0, 0, spec->memDB.size()-1);
}

// Build an array-based complete binary search tree
void CompleteArrayBST::make_tree(const vector<int*> &sortedArray, int curRootPos,
		int lowIdx, int highIdx) {
	int subTreeSize = (highIdx - lowIdx + 1);
	assert(subTreeSize >= 1);

	if (subTreeSize == 1) {
		array[curRootPos] = *(sortedArray[lowIdx] + attrIdx);
	} else {
		// get the height of the subtree
		int height = 0;
		int tmp = subTreeSize;
		while (tmp != 0) {
			tmp /= 2;
			height++;
		}

		// get the bottom count
		int bottomCapacity = 1 << (height - 1);
		int bottomCount = subTreeSize - (bottomCapacity - 1);

		int leftCount, rightCount;
		if (bottomCount <= bottomCapacity / 2) {
			leftCount = (1 << (height - 2)) - 1 + bottomCount;
			rightCount = subTreeSize - 1 - leftCount;
		} else {
			leftCount = (1 << (height - 1)) - 1;
			rightCount = subTreeSize - 1 - leftCount;
		}
		array[curRootPos] = *(sortedArray[lowIdx + leftCount] + attrIdx);

		make_tree(sortedArray, curRootPos * 2 + 1, lowIdx, lowIdx + leftCount - 1);
		if (rightCount > 0)
			make_tree(sortedArray, curRootPos * 2 + 2, highIdx - rightCount + 1,
					highIdx);
	}
}

void CompleteArrayBST::search(int value) {
	curPos = 0;
	if (value > array[curPos]) {
		curPos = curPos * 2 + 2;
		if (curPos > array.size())
			curPos = array.size();
	}
	else if (value < array[curPos]) {
		curPos = curPos * 2 + 1;
		if (curPos >= array.size())
			curPos = (curPos - 1) / 2;
	}
}

void CompleteArrayBST::show(int subRoot) {
	if (subRoot*2+1 < array.size()) {
		show(subRoot*2+1);
	}
	cerr << array[subRoot] << " ";
	if (subRoot*2+2 < array.size()) {
		show(subRoot*2+2);
	}
}
