#include "../include/SortMerge.h"

RelationSpec* sequential_sortmege_join(const vector<string> &joinAttrOrderIn,
		map<string, RelationSpec*> &relSpecs, map<string, bool> &joinRelMapIn, size_t &recordCount) {
	vector<string> joinAttrOrder = joinAttrOrderIn;
	map<string, bool> joinRelMap = joinRelMapIn;

	RelationSpec* retSpec = NULL;
	// join the relations in the order of attributes
	while (!joinAttrOrder.empty()) {
		vector<string> candidateRels;
		string curAttr = joinAttrOrder.front();
		// find at most 2 candidate relations for this attributes
		for (map<string, bool>::iterator iter = joinRelMap.begin();
				iter != joinRelMap.end(); iter++) {
			string curRel = iter->first;
			assert(relSpecs.find(curRel) != relSpecs.end());

			RelationSpec* curRelSpec = relSpecs[curRel];
			if (curRelSpec->has_attr(curAttr)) {
				candidateRels.push_back(curRel);
				if (candidateRels.size() == 2)
					break;
			}
		}
		assert(candidateRels.size() <= 2);
		if (candidateRels.size() == 2) {
			// join the two candidateRels
			// erase the two candidates from joinrels
			joinRelMap.erase(candidateRels.at(0));
			joinRelMap.erase(candidateRels.at(1));
			// insert the joined one
			RelationSpec* rSpec = relSpecs[candidateRels.at(0)];
			RelationSpec* sSpec = relSpecs[candidateRels.at(1)];

			RelationSpec* joinedSpec;
			if (!joinRelMap.empty()) {
				recordCount = 0;
				joinedSpec = sortmerge_join(rSpec, sSpec, curAttr,
						joinAttrOrder, recordCount, true);
			} else { // we do not save result for the last joined result
				recordCount = 0;
				joinedSpec = sortmerge_join(rSpec, sSpec, curAttr,
						joinAttrOrder, recordCount, false);
			}

			// delete the relation after joining
//				delete rSpec; rSpec = NULL;
//				delete sSpec; sSpec = NULL;
//				relSpecs.erase(candidateRels.at(0));
//				relSpecs.erase(candidateRels.at(1));

			joinRelMap[joinedSpec->relName] = true;
			relSpecs[joinedSpec->relName] = joinedSpec;
			retSpec = joinedSpec;
//			joinedSpec->print_relation();
		} else {
			// move to next join attributes
			joinAttrOrder.erase(joinAttrOrder.begin());
		}
	}
	return retSpec;
}

RelationSpec* sortmerge_join(RelationSpec* rSpec, RelationSpec* sSpec,
		const string &mainJoinAttr, const vector<string> &joinAttrOrder,
		size_t& recordCount, bool saveResult) {
//	cerr << "Join on " << mainJoinAttr << " using " << rSpec->relName << " & "
//			<< sSpec->relName << " ... ";

	// sort relations based on join attributes
	int roffset = rSpec->get_attr_idx(mainJoinAttr);
	int soffset = sSpec->get_attr_idx(mainJoinAttr);
	assert(roffset != -1);
	assert(soffset != -1);
	sort(rSpec->memDB.begin(), rSpec->memDB.end(), sorter(roffset));
	sort(sSpec->memDB.begin(), sSpec->memDB.end(), sorter(soffset));
//	cerr << "sorting done." << " ";

	// Check whether there are other attributes (beside the mainJoinAttr) need to be joined at the same time
	vector<string> commonAttrs = sort_intersect(rSpec->attrNames,
			sSpec->attrNames);
	assert(commonAttrs.size() > 0);
	vector<string> otherAttrsToJoin;
	if (commonAttrs.size() == 1) {
		assert(commonAttrs.at(0) == mainJoinAttr);
	} else {
//		cerr << "Additional attributes: ";
		// make sure the mainJoinAttr is in the commonAttrs
		int mainIdx = find_offset(commonAttrs, mainJoinAttr);
		assert(mainIdx != -1);
		// get other attributes need to be joined
		for (int i = 0; i != commonAttrs.size(); i++) {
			string curCommonAttr = commonAttrs.at(i);
			if (i == mainIdx)
				continue;
			if (find_offset(joinAttrOrder, curCommonAttr) != -1) {
//				cerr << curCommonAttr << " ";
				otherAttrsToJoin.push_back(curCommonAttr);
			}
		}
//		cerr << ". ";
	}

	int otherSize = otherAttrsToJoin.size();
	int *rOffs = new int[otherSize];
	int *sOffs = new int[otherSize];
	for (int i = 0; i != otherAttrsToJoin.size(); i++) {
		string curAttr = otherAttrsToJoin.at(i);
		rOffs[i] = rSpec->get_attr_idx(curAttr);
		sOffs[i] = sSpec->get_attr_idx(curAttr);
	}

	// Create the joint relation specification file
	// The attributes of the joined relation is a distinct union of the attributes of the two input relations
	vector<string> nAttrNames = sort_union(rSpec->attrNames, sSpec->attrNames);
	int nAttrSize = nAttrNames.size();
	string nRelName = rSpec->relName + sSpec->relName;
	RelationSpec* nSpec = new RelationSpec(nRelName, "", nAttrNames, false);
	int* rAttrIdx = new int[nAttrSize];
	int* sAttrIdx = new int[nAttrSize];
	for (int i = 0; i != nAttrSize; i++) {
		string curAttr = nAttrNames.at(i);
		rAttrIdx[i] = find_offset(rSpec->attrNames, curAttr);
		sAttrIdx[i] = find_offset(sSpec->attrNames, curAttr);
	}

//	cerr << "Joined schema: ";
//	for (int i = 0; i != nAttrNames.size(); i++)
//		cerr << nAttrNames.at(i) << " ";
//	cerr << endl;

	// Here start the core part of the sort merge join algorithm
	bool firstRun = true;
	int rKey, sKey, rLastKey = -1, sLastIdx = 0;
	size_t rSize = rSpec->memDB.size();
	size_t sSize = sSpec->memDB.size();
	size_t rIdx = -1, sIdx = -1;
	int *rRecPtr, *sRecPtr, *nRecPtr;
	for (rIdx = 0; rIdx != rSize; rIdx++) {
		rRecPtr = rSpec->memDB[rIdx];
		rKey = *(rRecPtr + roffset);

		if (!firstRun && rKey != rLastKey) {
			sLastIdx = sIdx;
		}

		for (sIdx = sLastIdx; sIdx != sSize; sIdx++) {
			sRecPtr = sSpec->memDB[sIdx];
			sKey = *(sRecPtr + soffset);
			if (rKey == sKey) {
				// Before we move on, we need to check other common attributes are the same, too.
				if (!check_other_attrs(rRecPtr, sRecPtr, rOffs, sOffs,
						otherSize)) {
					continue;
				} else {
					// Making new records.
					recordCount++;
					if (saveResult) {
						nRecPtr = new int[nAttrSize];
						make_record(rRecPtr, sRecPtr, rAttrIdx, sAttrIdx,
								nAttrSize, nRecPtr);
						nSpec->memDB.push_back(nRecPtr);
					}
					if (recordCount % PRINT_NUM == 1)
						cerr << ".";
					if (recordCount % (PRINT_NUM * 20) == 1)
						cerr << endl;
				}
			} else {
				if (rKey > sKey) {
					continue;
				} else { // rKey < sKey
					break;
				}
			}
		}
		rLastKey = rKey;
		firstRun = false;
	}
//	cerr << endl;
//	cerr << "Joining done. Records: " << recordCount << endl;

	//clean up
	delete[] rOffs;
	delete[] sOffs;
	delete[] rAttrIdx;
	delete[] sAttrIdx;

	return nSpec;
}

vector<string> sort_union(vector<string> attr1, vector<string> attr2) {
	// sort two attributes vector, then return their intersection
	std::vector<string> ret(attr1.size() + attr2.size());
	std::vector<string>::iterator it;

	sort(attr1.begin(), attr1.end());
	sort(attr2.begin(), attr2.end());

	it = set_union(attr1.begin(), attr1.end(), attr2.begin(), attr2.end(),
			ret.begin());
	ret.resize(it - ret.begin());
	return ret;
}

vector<string> sort_intersect(vector<string> attr1, vector<string> attr2) {
	// sort two attributes vector, then return their intersection
	std::vector<string> ret(attr1.size() + attr2.size());
	std::vector<string>::iterator it;

	sort(attr1.begin(), attr1.end());
	sort(attr2.begin(), attr2.end());

	it = set_intersection(attr1.begin(), attr1.end(), attr2.begin(),
			attr2.end(), ret.begin());
	ret.resize(it - ret.begin());
	return ret;
}

bool comparePtr(int const* a, int const* b, int sortIdx) {
	return (*(a + sortIdx) < *(b + sortIdx));
}

void make_record(int* &rRec, int* &sRec, int* rAttrIdx, int* sAttrIdx,
		const int nAttrSize, int *nRecPtr) {
	for (int i = 0; i != nAttrSize; i++) {
		if (rAttrIdx[i] != -1) {
			nRecPtr[i] = *(rRec + rAttrIdx[i]);
		} else if (sAttrIdx[i] != -1) {
			nRecPtr[i] = *(sRec + sAttrIdx[i]);
		}
	}
}

bool check_other_attrs(int* &rRec, int* &sRec, int* rOffs, int* sOffs,
		const int otherAttrsSize) {
	bool flag = true;
	for (int i = 0; i != otherAttrsSize; i++) {
		int rKey = *(rRec + rOffs[i]);
		int sKey = *(sRec + sOffs[i]);

		if (rKey != sKey) {
			flag = false;
			break;
		}
	}
	return flag;
}
