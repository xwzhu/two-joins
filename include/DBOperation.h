#ifndef DBOPERATION_H
#define DBOPERATION_H

#include "RelationSpec.h"

bool prepare_minibase(const string dirName, map<string, RelationSpec*> &relSpecs);
RelationSpec* prepare_relation(const string dirName, const string RelSpecStr);
void process_queries(const string queryPath, map<string, RelationSpec*> &relSpecs);
RelationSpec* sort_merge_join(RelationSpec* rSpec, RelationSpec* sSpec,
		const string &mainJoinAttr, const vector<string> &joinAttrOrder);

#endif
