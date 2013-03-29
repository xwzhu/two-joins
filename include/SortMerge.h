/*
 * sortmerge.h
 *
 *  Created on: Mar 26, 2013
 *      Author: parallels
 */

#ifndef SORTMERGE_H_
#define SORTMERGE_H_
#include "RelationSpec.h"

bool comparePtr(int const* a, int const* b, int sortIdx);
class sorter {
	int sortIdx;
public:
	sorter(int sortIdx) {
		this->sortIdx = sortIdx;
	}
	bool operator()(int const* o1, int const* o2) const {
		return comparePtr(o1, o2, sortIdx);
	}
};

vector<string> sort_union(vector<string> attr1, vector<string> attr2);

vector<string> sort_intersect(vector<string> attr1, vector<string> attr2);

void make_record(int* &rRec, int* &sRec, int* rAttrIdx, int* sAttrIdx,
		const int nAttrSize, int *nRecPtr);

bool check_other_attrs(int* &rRec, int* &sRec, int* rOffs, int* sOffs,
		const int otherAttrsSize);

RelationSpec* sortmerge_join(RelationSpec* rSpec, RelationSpec* sSpec,
		const string &mainJoinAttr, const vector<string> &joinAttrOrder);

RelationSpec* sequential_sortmege_join(const vector<string> &joinAttrOrderIn,
		map<string, RelationSpec*> &relSpecs, map<string, bool> &joinRelMapIn);

#endif /* SORTMERGE_H_ */
