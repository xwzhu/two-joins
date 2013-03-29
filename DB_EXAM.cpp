#include "include/DBOperation.h"

int main(int argc, char *argv[]) {
	cerr << argv[0] << endl;

	string dirName;
	string queryPath;

	bool useSortmerge = true;
	bool useTrieJoin = false;

	if (argc == 5) {
		queryPath = argv[2];
		dirName = argv[4];
	} else {
		dirName = "/home/parallels/Joins/Joins-bin/dataset2-zipf/scale2/databasefile";
		queryPath = "/home/parallels/Joins/Joins-bin/query3";
	}

	pair<vector<string>, vector<string> > joinSpecs;
	map<string, RelationSpec*> relSpecs;
	prepare_minibase(dirName, relSpecs);
	process_queries(queryPath, relSpecs, useSortmerge, useTrieJoin);

	return 0;
}
