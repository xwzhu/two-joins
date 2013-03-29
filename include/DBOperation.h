#ifndef DBOPERATION_H
#define DBOPERATION_H

#include "RelationSpec.h"

bool prepare_minibase(const string dbSpecPath,
		map<string, RelationSpec*> &relSpecs);

RelationSpec* prepare_relation(const string dirName, const string RelSpecStr);

void process_queries(const string queryPath,
		map<string, RelationSpec*> &relSpecs, bool useSortmerge = true,
		bool useTrieJoin = true);

#endif
