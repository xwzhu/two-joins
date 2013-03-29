#include "include/DBOperation.h"

int main(int argc, char *argv[]) {
	string dirName;
	string queryPath;

	bool useSortmerge = false;
	bool useTrieJoin = true;

	if (argc == 5) {
//		cerr << "using arguments" << endl;
		queryPath = argv[2];
		dirName = argv[4];
	} else {
		dirName = "/home/parallels/Joins/Joins-bin/dataset2-zipf/scale1/databasefile";
		queryPath = "/home/parallels/Joins/Joins-bin/query2";
	}

	pair<vector<string>, vector<string> > joinSpecs;
	map<string, RelationSpec*> relSpecs;
	prepare_minibase(dirName, relSpecs);
	process_queries(queryPath, relSpecs, useSortmerge, useTrieJoin);

	return 0;
}
