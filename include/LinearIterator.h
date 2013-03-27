/*
 * LinearIterator.h
 *
 *  Created on: Mar 27, 2013
 *      Author: parallels
 */

#ifndef LINEARITERATOR_H_
#define LINEARITERATOR_H_

#include "CompleteArrayBST.h"

class LinearIterator {
public:
	LinearIterator(RelationSpec* specIn, int attrIdxIN = 0);
	int key();
	void next();
	bool at_end();
	void seek(const int seekKey);
private:
	CompleteArrayBST* bst;
};

#endif /* LINEARITERATOR_H_ */
