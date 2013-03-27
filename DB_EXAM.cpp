#include "include/DBOperation.h"

int main(int argc, char *argv[]) {
	string dirName = "/home/parallels/Joins/Joins-bin/dataset2-zipf/scale1/";
	string queryPath = "/home/parallels/Joins/Joins-bin/query1";

	pair<vector<string>, vector<string> > joinSpecs;
	map<string, RelationSpec*> relSpecs;
	prepare_minibase(dirName, relSpecs);
	process_queries(queryPath, relSpecs);

	return 0;
}
