#include "include/DBOperation.h"

int main(int argc, char *argv[]) {
	string dirName;
	string queryPath;

	bool useSortmerge = false;
	bool useTrieJoin = true;
	bool saveResults = false;

	if (argc == 5) {
		string tmp1 = argv[1];
		string tmp2 = argv[3];
		if (tmp1 == "-query" and tmp2 == "-database") {
			queryPath = argv[2];
			dirName = argv[4];
		}
		else if (tmp1 == "-database" and tmp2 == "-query") {
			dirName = argv[2];
			queryPath = argv[4];
		}
		else {
			cerr << "Wrong arguments provided" << endl;
			return 1;
		}
	} else {
		dirName =
				"/home/parallels/Joins/Joins-bin/dataset2-zipf/scale5/databasefile";
		queryPath = "/home/parallels/Joins/Joins-bin/query1";
	}

	map<string, RelationSpec*> relSpecs;
	if (useTrieJoin) {
		// add LineNo
		prepare_minibase(dirName, relSpecs, true);
	} else {
		// do not add LineNo
		prepare_minibase(dirName, relSpecs, false);
	}
	process_queries(queryPath, relSpecs, useSortmerge, useTrieJoin, saveResults);

	return 0;
}
