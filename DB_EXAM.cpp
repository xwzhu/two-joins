#include "include/DBOperation.h"

int main(int argc, char *argv[]) {
	string dirName;
	string queryPath;

	bool useSortmerge = true;
	bool useTrieJoin = false;

	if (argc == 5) {
		queryPath = argv[2];
		dirName = argv[4];
	} else {
		dirName = "/home/parallels/Joins/Joins-bin/dataset2-zipf/scale5/databasefile";
		queryPath = "/home/parallels/Joins/Joins-bin/query1";
	}

	pair<vector<string>, vector<string> > joinSpecs;
	map<string, RelationSpec*> relSpecs;
	if (useTrieJoin) {
		prepare_minibase(dirName, relSpecs, true);
	}
	else {
		prepare_minibase(dirName, relSpecs, false);
	}
	process_queries(queryPath, relSpecs, useSortmerge, useTrieJoin);

	return 0;
}
