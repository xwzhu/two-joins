#include "include/DBOperation.h"
#include "include/Leapfrog.h"

bool prepare_minibase(const string dirName, map<string, RelationSpec*> &relSpecs) {
	string dbSpecPath = dirName + "databasefile";
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
			RelationSpec* curSpec = prepare_relation(dirName, eachRel);
			relSpecs[curSpec->relName] = curSpec;
		}
		dbSpecFile.close();
		return true;
	} else {
		cerr << "Cannot open database specification file." << endl;
		return false;
	}
}

RelationSpec* prepare_relation(const string dirName, const string RelSpecStr) {
	cerr << "Preparing relation: " << RelSpecStr << endl;
	istringstream ss(RelSpecStr);
	char temp[256];
	// get the path of relation file
	ss.getline(temp, 256, ',');
	string relFilePath(temp);
	trim(relFilePath);
	relFilePath = dirName + relFilePath;

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

void process_queries(const string queryPath, map<string, RelationSpec*> &relSpecs) {
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
		map<string, bool> joinRelMap;
		for (size_t i = 0; i != orgJoinRels.size(); i++) {
			joinRelMap[orgJoinRels.at(i)] = true;
		}

		cerr << "Showing result for leapfrog join:" << endl;
		int localCount = 0;
		Leapfrog * leap = new Leapfrog(relSpecs, orgJoinRels);
		while (!leap->at_end()) {
			cerr << ++localCount << ": ";
			leap->show();
			leap->next();
		}
		cerr << endl;

		// join the relations in the order of attributes
		while (!joinAttrOrder.empty()) {
			vector<string> candidateRels;
			string curAttr = joinAttrOrder.front();
			// find at most 2 candidate relations for this attributes
			for (map<string, bool>::iterator iter = joinRelMap.begin();
					iter != joinRelMap.end(); iter++) {
				string curRel = iter->first;
				assert(relSpecs.find(curRel) != relSpecs.end());

				RelationSpec* curRelSpec = relSpecs[curRel];
				if (curRelSpec->has_attr(curAttr)) {
					candidateRels.push_back(curRel);
					if (candidateRels.size() == 2)
						break;
				}
			}
			assert(candidateRels.size() <= 2);
			if (candidateRels.size() == 2) {
				// join the two candidateRels
				// erase the two candidates from joinrels
				joinRelMap.erase(candidateRels.at(0));
				joinRelMap.erase(candidateRels.at(1));
				// insert the joined one
				RelationSpec* rSpec = relSpecs[candidateRels.at(0)];
				RelationSpec* sSpec = relSpecs[candidateRels.at(1)];
				RelationSpec* joinedSpec = sort_merge_join(rSpec, sSpec, curAttr, joinAttrOrder);

				// delete the relation after joining
//				delete rSpec; rSpec = NULL;
//				delete sSpec; sSpec = NULL;
//				relSpecs.erase(candidateRels.at(0));
//				relSpecs.erase(candidateRels.at(1));

				joinRelMap[joinedSpec->relName] = true;
				relSpecs[joinedSpec->relName] = joinedSpec;
			} else {
				// move to next join attributes
				joinAttrOrder.erase(joinAttrOrder.begin());
			}
		}
	} else {
		cerr << "Cannot open database specification file." << endl;
	}
}
