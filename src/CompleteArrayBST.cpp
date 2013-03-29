/*
 * bstree.cpp
 *
 *  Created on: Mar 26, 2013
 *      Author: parallels
 */

#include "../include/CompleteArrayBST.h"

CompleteArrayBST::CompleteArrayBST(const vector<int> &sortedArray) {
	size = sortedArray.size();
	assert(size > 0);
	idxArray.resize(size);
	curPos = 0;
	atEnd = false;
	make_tree(sortedArray, 0, 0, size-1);
	move_front();
}

// Build an array-based complete binary search tree
void CompleteArrayBST::make_tree(const vector<int> &sortedArray, size_t curRootPos,
		size_t lowIdx, size_t highIdx) {
	size_t subTreeSize = (highIdx - lowIdx + 1);
	assert(subTreeSize >= 1);

	if (subTreeSize == 1) {
//		cerr << sortedArray[lowIdx] << endl;
		idxArray[curRootPos] = sortedArray[lowIdx];
	} else {
		// get the height of the subtree
		int height = 0;
		size_t tmp = subTreeSize;
		while (tmp != 0) {
			tmp /= 2;
			height++;
		}

		// get the bottom count
		size_t bottomCapacity = 1 << (height - 1);
		size_t bottomCount = subTreeSize - (bottomCapacity - 1);

		size_t leftCount, rightCount;
		if (bottomCount <= bottomCapacity / 2) {
			leftCount = (1 << (height - 2)) - 1 + bottomCount;
			rightCount = subTreeSize - 1 - leftCount;
		} else {
			leftCount = (1 << (height - 1)) - 1;
			rightCount = subTreeSize - 1 - leftCount;
		}
		idxArray[curRootPos] = sortedArray[lowIdx + leftCount];

		make_tree(sortedArray, curRootPos * 2 + 1, lowIdx, lowIdx + leftCount - 1);
		if (rightCount > 0)
			make_tree(sortedArray, curRootPos * 2 + 2, highIdx - rightCount + 1,
					highIdx);
	}
}

void CompleteArrayBST::move_to(size_t pos) {
	assert(pos < size);
	curPos = pos;
}

size_t CompleteArrayBST::get_pos() {
	return curPos;
}

void CompleteArrayBST::seek(const int seekKey) {
//	assert(seekKey>=key());

//	cerr << at_end() << " " << seekKey << " " << key() << endl;
	if (at_end() || seekKey <= key()) {
		return;
	}

	if (has_parent()) {
		int pValue, side;
		pValue = peek_parent(side);

		if (side == -1 && pValue <= seekKey) { // go to the parent nod and recursive call seek
//			cerr << "move_parent" << " " << curPos << " " << key() << " " << pValue << " " << seekKey << " ";
			move_parent(side);
//			cerr << curPos << " " << side << endl;
			seek(seekKey);
		}
		else { // pValue >= value
//			cerr << "move_next" << " " << curPos << " " << key() << " " << pValue << " " << seekKey << " ";
			next();
//			cerr << curPos << endl;
			seek(seekKey);
		}
	}
	else { // no parent, we are at root, go next
		next();
		seek(seekKey);
	}
}

void CompleteArrayBST::show(size_t subRoot) {
	if (subRoot*2+1 < size) {
		show(subRoot*2+1);
	}
	cerr << key_at_pos(subRoot) << " ";
	if (subRoot*2+2 < size) {
		show(subRoot*2+2);
	}
}

void CompleteArrayBST::show(bool fromStart) {
	if (fromStart)
		move_front();
	while(!at_end()) {
		cerr << key() << " ";
		next();
	}
	cerr << endl;
}

void CompleteArrayBST::show_array() {
	for (size_t i=0; i!=size; i++) {
		cerr << idxArray[i] << " ";
	}
	cerr << endl;
}

int CompleteArrayBST::key() {
	assert ( curPos < size );
	return idxArray[curPos];
}

int CompleteArrayBST::key_at_pos(size_t pos) {
	assert ( pos < size );
	return idxArray[pos];
}

void CompleteArrayBST::next() {
//	cerr << "next" << " "<< key() << " ";
	if (at_end())
		return;

	// The leftmost child of the right child,
	// if your current node has a right child.
	// If the right child has no left child,
	// the right child is your inorder successor.
	bool found = false;
	if (has_right()) {
		move_right();
		while (has_left())
			move_left();
		found = true;
	}
	// Navigate up the parent ancestor nodes,
	// and when you find a parent whose left child is the node
	// you're currently at, the parent is
	// the inorder successor of your original node.
	else {
		while (has_parent()) {
			int parentSide = 0;
			move_parent(parentSide);
			if (parentSide == -1) {
				found = true;
				break;
			}
		}
	}
	if (!found)
		atEnd = true;
//	cerr << key() << endl;
}

size_t CompleteArrayBST::get_size() {
	return size;
}

bool CompleteArrayBST::at_end() {
	return atEnd;
}

bool CompleteArrayBST::has_left() {
	return curPos*2+1 < size;
}

bool CompleteArrayBST::has_right() {
	return curPos*2+2 < size;
}

bool CompleteArrayBST::has_parent() {
	return curPos >= 1;
}

void CompleteArrayBST::move_left() {
	curPos = curPos*2+1;
}

void CompleteArrayBST::move_right() {
	curPos = curPos*2+2;
}

void CompleteArrayBST::move_parent(int& side) {
	if (curPos % 2 == 1) {
		side = -1;
		curPos = (curPos - 1) / 2;
	}
	else {
		side = 1;
		curPos = (curPos - 2) / 2;
	}
}

void CompleteArrayBST::move_front() {
	curPos = 0;
	while (has_left())
		move_left();
	atEnd = false;
//	cerr << curPos << " " << key() << endl;
}

int CompleteArrayBST::peek_parent(int& side) {
	if (curPos % 2 == 1) {
		side = -1;
		return key_at_pos( (curPos - 1) / 2 );
	}
	else {
		side = 1;
		return key_at_pos( (curPos - 2) / 2 );
	}
}
