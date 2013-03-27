/*
 * LinearIterator.cpp
 *
 *  Created on: Mar 27, 2013
 *      Author: parallels
 */

#include "include/LinearIterator.h"
#include "include/SortMerge.h"

LinearIterator::LinearIterator(RelationSpec* specIn, int attrIdxIN) {
	// TODO Auto-generated constructor stub

	// sort the memDB based on attrIdxIN
	sort(specIn->memDB.begin(), specIn->memDB.end(), sorter(attrIdxIN));
	bst = new CompleteArrayBST(specIn, attrIdxIN);
//	cerr << endl;
//	bst->show_seek();
//	cerr << endl;
}

int LinearIterator::key() {
	return bst->key();
}

void LinearIterator::next() {
	bst->next();
}

bool LinearIterator::at_end(){
	return bst->at_end();
}

void LinearIterator::seek(const int seekKey){
	bst->seek(seekKey);
}
