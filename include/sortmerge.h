/*
 * sortmerge.h
 *
 *  Created on: Mar 26, 2013
 *      Author: parallels
 */

#ifndef SORTMERGE_H_
#define SORTMERGE_H_
#include "relationspec.h"

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

int find_offset(const vector<string> &attrs, const string &target);

vector<string> sort_union(vector<string> attr1, vector<string> attr2);

vector<string> sort_intersect(vector<string> attr1, vector<string> attr2);

void make_record(int* &rRec, int* &sRec, int* rAttrIdx, int* sAttrIdx,
		const int nAttrSize, int *nRecPtr);

bool check_other_attrs(int* &rRec, int* &sRec, int* rOffs, int* sOffs,
		const int otherAttrsSize);

RelationSpec* sort_merge_join(RelationSpec* rSpec, RelationSpec* sSpec,
		const string &mainJoinAttr, const vector<string> &joinAttrOrder);

#endif /* SORTMERGE_H_ */
