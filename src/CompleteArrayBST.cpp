/*
 * bstree.cpp
 *
 *  Created on: Mar 26, 2013
 *      Author: parallels
 */

#include "../include/CompleteArrayBST.h"

// Contructor for an array implemetation of a complete binary search tree
CompleteArrayBST::CompleteArrayBST(const vector<int> &sortedArray) {
	size = sortedArray.size();
	assert(size > 0);
	idxArray.resize(size);
	curPos = 0;
	atEnd = false;
	make_tree(sortedArray, 0, 0, size-1);
	move_front();
}

// Build an array-based complete binary search tree for a sorted array
// Recursively build left, build root, Recursively build right
// Time complexity: O(n)
void CompleteArrayBST::make_tree(const vector<int> &sortedArray, size_t curRootPos,
		size_t lowIdx, size_t highIdx) {
	size_t subTreeSize = (highIdx - lowIdx + 1);
	assert(subTreeSize >= 1);

	if (subTreeSize == 1) {
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

		// build left and right node
		make_tree(sortedArray, curRootPos * 2 + 1, lowIdx, lowIdx + leftCount - 1);
		if (rightCount > 0)
			make_tree(sortedArray, curRootPos * 2 + 2, highIdx - rightCount + 1,
					highIdx);
	}
}

// When do seek(), start from current location
// if the value is bigger than parent, go to parent node directly
// otherwise, go to the next node and seek() again
// complexity O(logN), amorised complexity O(1+log(N/m))
void CompleteArrayBST::seek(const int seekKey, const bool checkParent) {
	if (at_end() || seekKey == key()) {
		return;	
	}
	
	// if no less than right hand side parent
	//  go to the parent node directly and recursive call seek
	if (checkParent && has_parent()) {
		int pValue, side;
		pValue = peek_parent(side);

		if (side == -1 && pValue <= seekKey) {
			move_parent(side);
			seek(seekKey, true);
			return;
		}
	}
	
	if (seekKey < key()) {
		if (has_left()) {
			move_left();
			seek(seekKey, false);
		}
		else {
			// if there is no smaller left child
			// then here is the node we want to find
			return;
		}
	}
	else { // seekKey > key()
		if (has_right()) {
			move_right();
			seek(seekKey, false);
		}
		else{
			// move to next in case of the highest parent
			// or the end of tree being the node we want to find
			next();
			return;
		}
	}
}

// Move to the next position
void CompleteArrayBST::next() {
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
}

int CompleteArrayBST::key() {
	assert ( curPos < size );
	return idxArray[curPos];
}

int CompleteArrayBST::key_at_pos(size_t pos) {
	assert ( pos < size );
	return idxArray[pos];
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

// move to parent, return which side of parent 
// that the current node is located
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

// Move to the smallest value, i.e., the front
void CompleteArrayBST::move_front() {
	curPos = 0;
	while (has_left())
		move_left();
	atEnd = false;
}

// peek the value of parent, but not move to it
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
