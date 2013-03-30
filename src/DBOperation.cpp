#include "../include/DBOperation.h"
#include "../include/SortMerge.h"
#include "../include/TrieJoin.h"

bool prepare_minibase(const string dbSpecPath,
		map<string, RelationSpec*> &relSpecs, const bool addLineNo) {
	string dirPath = get_dir_path(dbSpecPath);
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
			RelationSpec* curSpec = prepare_relation(dirPath, eachRel,
					addLineNo);
			relSpecs[curSpec->relName] = curSpec;
		}
		dbSpecFile.close();
		return true;
	} else {
		cerr << "Cannot open database specification file." << endl;
		return false;
	}
}

RelationSpec* prepare_relation(const string dbSpecPath, const string RelSpecStr,
		const bool addLineNo) {
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
	RelationSpec* relSpec = new RelationSpec(relName, relFilePath, attrNames,
			addLineNo);

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
		bool useTrieJoin, bool saveResults) {
//	cerr << "Processing queries..." << endl;

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
		map<string, bool> joinRelMap;
		for (size_t i = 0; i != orgJoinRels.size(); i++) {
			joinRelMap[orgJoinRels.at(i)] = true;
		}

		timespec time1, time2;
		double timeDiff;
		RelationSpec* joinedSpec = NULL;
		size_t recordCount;

		if (useTrieJoin) {
			vector<TrieIterator*> tries;
			for (size_t i = 0; i != orgJoinRels.size(); i++) {
				RelationSpec* curSpec = relSpecs[orgJoinRels.at(i)];
				TrieIterator* trie;
				if (curSpec->memDB.size() > MAX_NUM_TO_PRE_BUILD_TRIE) {
					cerr << "trie structure is built on the fly" << endl;
					trie = new TrieIterator(curSpec, true);
				} else {
					trie = new TrieIterator(curSpec, false);
				}
				tries.push_back(trie);
			}
			recordCount = 0;
			TrieJoin *triejoin = new TrieJoin(orgJoinRels, joinAttrOrder,
					relSpecs, tries);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
			joinedSpec = leapfrog_triejoin(triejoin, recordCount, saveResults);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
			timeDiff = diff_in_ms(time1, time2);
			cerr << endl << "leapfrog : " << recordCount << ". used: "
					<< timeDiff << "ms." << endl;
			delete triejoin;
		}

		if (useSortmerge) {
			recordCount = 0;
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
			joinedSpec = sequential_sortmege_join(joinAttrOrder, relSpecs,
					joinRelMap, recordCount, saveResults);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
			timeDiff = diff_in_ms(time1, time2);
			cerr << endl << "sortmerge: " << recordCount << ". used: "
					<< timeDiff << "ms." << endl;
		}
	} else {
		cerr << "Cannot open database specification file." << endl;
	}
}
