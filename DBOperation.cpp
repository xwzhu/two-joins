#include "include/DBOperation.h"
#include "include/SortMerge.h"
#include "include/TrieJoin.h"

bool prepare_minibase(const string dbSpecPath,
		map<string, RelationSpec*> &relSpecs) {
	string dirPath = get_dir_path(dbSpecPath);
	cerr << dirPath << endl;
	ifstream dbSpecFile(dbSpecPath.c_str());
	// open database specification file
	if (dbSpecFile.is_open()) {
		string eachRel;
		// process each relation file
		while (dbSpecFile.good()) {
			getline(dbSpecFile, eachRel);
			if (eachRel.empty())
				break;
			// build the relation
			RelationSpec* curSpec = prepare_relation(dirPath, eachRel);
			relSpecs[curSpec->relName] = curSpec;
		}
		dbSpecFile.close();
		return true;
	} else {
		cerr << "Cannot open database specification file." << endl;
		return false;
	}
}

RelationSpec* prepare_relation(const string dbSpecPath, const string RelSpecStr) {
//	cerr << "Preparing relation: " << RelSpecStr << endl;
	string dirPath = get_dir_path(dbSpecPath);
	istringstream ss(RelSpecStr);
	char temp[256];
	// get the path of relation file
	ss.getline(temp, 256, ',');
	string relFilePath(temp);
	trim(relFilePath);
	relFilePath = dirPath + relFilePath;

	// get the name of the relation
	ss.getline(temp, 256, ',');
	string relName(temp);
	trim(relName);

	// get the list of attributes
	vector<string> attrNames = split_comma(ss);

	// generate the relation specification
	RelationSpec* relSpec = new RelationSpec(relName, relFilePath, attrNames);

	// build the relation, i.e., inserting to MINIBASE
	if (!relSpec->build_relation())
		cerr << "Cannot build relation: " << relSpec->relName << endl;

//	CompleteArrayBST * bst = new CompleteArrayBST(relSpec);
//	bst->show(0);
//	cerr << endl;
//	bst->show_array();

	return relSpec;
}

void process_queries(const string queryPath,
		map<string, RelationSpec*> &relSpecs, bool useSortmerge,
		bool useTrieJoin) {
	cerr << "Processing queries..." << endl;

	pair<vector<string>, vector<string> > querySpecs;
	ifstream queryFile(queryPath.c_str());
	if (queryFile.is_open()) {
		string allRels, allAttrs;
		getline(queryFile, allRels);
		getline(queryFile, allAttrs);
		queryFile.close();

		istringstream ssRels(allRels), ssAttrs(allAttrs);
		vector<string> orgJoinRels = split_comma(ssRels);
		vector<string> joinAttrOrder = split_comma(ssAttrs);
		vector<TrieIterator*> tries;
		map<string, bool> joinRelMap;
		for (size_t i = 0; i != orgJoinRels.size(); i++) {
			joinRelMap[orgJoinRels.at(i)] = true;
			TrieIterator* trie = new TrieIterator(relSpecs[orgJoinRels.at(i)]);
			tries.push_back(trie);
		}

		clock_t t1, t2;
		double timeDiff;

		if (useTrieJoin) {
			TrieJoin *triejoin = new TrieJoin(orgJoinRels, joinAttrOrder,
					relSpecs, tries);
			t1 = clock();
			leapfrog_triejoin(triejoin);
			t2 = clock();
			timeDiff = ((double) t2 - (double) t1) / CLOCKS_PER_SEC * 1000;
			cerr << "leapfrog triejoin used: " << timeDiff << "ms." << endl;
			delete triejoin;
		}

		if (useSortmerge) {
			t1 = clock();
			sequential_sortmege_join(joinAttrOrder, relSpecs, joinRelMap);
			t2 = clock();
			timeDiff = ((double) t2 - (double) t1) / CLOCKS_PER_SEC * 1000;
			cerr << "sort merge join used: " << timeDiff << "ms." << endl;
		}
	} else {
		cerr << "Cannot open database specification file." << endl;
	}
}
