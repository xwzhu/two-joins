#include "include/DBOperation.h"

int main(int argc, char *argv[]) {
	string dirName = "/home/parallels/Joins/Joins-bin/dataset1-uniform/scale6/";
	string queryPath = "/home/parallels/Joins/Joins-bin/query3";

	pair<vector<string>, vector<string> > joinSpecs;
	map<string, RelationSpec*> relSpecs;
	prepare_minibase(dirName, relSpecs);
	process_queries(queryPath, relSpecs);

	return 0;
}
