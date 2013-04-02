#ifndef DBOPERATION_H
#define DBOPERATION_H

#include "RelationSpec.h"

// The names of the function are their use

bool prepare_minibase(const string dbSpecPath,
		map<string, RelationSpec*> &relSpecs, const bool addLineNo = true);

RelationSpec* prepare_relation(const string dirName, const string RelSpecStr,
		const bool addLineNo = true);

void process_queries(const string queryPath,
		map<string, RelationSpec*> &relSpecs, bool useSortmerge = true,
		bool useTrieJoin = true, bool saveResults = false);

#endif
