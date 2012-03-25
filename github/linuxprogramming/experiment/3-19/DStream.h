/*
 * Dstream.h
 *
 *  Created on: 2010-1-25
 *      Author: root
 */

#ifndef DSTREAM_H_
#define DSTREAM_H_

#include "Grid.h"
#include "Cluster.h"
#include "StreamOperation.h"
#include "CommonUtil.h"
#include "cudaRelated.h"
#include <ctime>

class Dstream {
	typedef hash_map<string, GlobalGrid, str_hash_test> hashMap;
	typedef hashMap::iterator hashMapIt;
	typedef hash_map<string, LocalGrid, str_hash_test> localHashMap;
	typedef localHashMap::iterator localHashMapIt;
	typedef hash_map<string, string> borderHashMap;
	typedef borderHashMap::iterator borderHashMapIt;
	typedef hash_map<string, list<string> > borderHash;
	typedef borderHash::iterator borderHashIt;
	//	typedef map<string,Grid>hashMap;
	//	typedef hashMap::iterator hashMapIt;
	hashMap globalGridHash;
	localHashMap localGridHash;
	borderHashMap borderGridHash;
	borderHash borderGrid;
	int clusterID;
	vector<Cluster> cluster;
	hashMapIt hasGlobalGrid(const string key) {
		return globalGridHash.find(key);
	}
	localHashMapIt hasLocalGrid(const string key) {
		return localGridHash.find(key);
	}

	borderHashIt hasBorderGrid(const string key) {
		return borderGrid.find(key);
	}

public:
	void cudaLocalBuildCluster();
private:
	void deleteGlobalGrid(GlobalGrid & globalGrid);
	void GlobalMerge(GlobalGrid & g1, GlobalGrid & g2);
	void GlobalIncrementalAdjust(double timeArrive);
	bool GlobalIncrementalRangeQuery(GlobalGrid & g, list<string>&globalKeyVector, list<string>&seedlist, int type);
	bool GlobalIncrementalExpand(GlobalGrid & g, list<string>&globalKeyVector);
	int GlobalIncrementalMergeCluster(GlobalGrid & g1, GlobalGrid & g2, list<string>&globalKeyVector);
	void searchInBorder(GlobalGrid & globalGrid, list<string>&keyList, list<string>&neighbor, int type);
	void borderCluster(LocalGrid & localGrid, int dimension, set<pair<string, string> >&localGridPair, hashSet & globalGridKeySet);
	void borderCluster(bool isUpdating = false);
	void borderIncrementalAdjust();
	bool borderExpand(GlobalGrid & globalGrid, list<string>&seedlist, list<string>&keyList);
	string localFindNeighbor(LocalGrid & localGrid, int dimension);
	void globalCluster(LocalGrid & localGrid, int dimension, set<pair<string, string> >&localGridPair);
	void localEplisonRangeQuery(GlobalGrid & globalGrid, list<string>&globalKey, list<string>&seedlist, int type);
	bool localExpandCluster(GlobalGrid & globalGrid, list<string>&seedlist, LocalGrid & localGrid, int type = EXPAND);
	void localBuildCluster();
	bool incrementalExpand(GlobalGrid & g);
	bool incrementalCreate(GlobalGrid & g, int oldClusterid);
	bool incrementalFindNeighbor(GlobalGrid & g, set<string>&neighbor, set<string>&denseGrid);
	bool createGlobalGrid(const string & key, const string & localKey, const string & borderKey, DataPoint & dp);
	bool createBorderGrid(const string & borderKey, const string & globalKey);
	bool createLocalGrid(const string & localKey, const string & globalKey, vector<int>&coordinate);
	bool incrementalIsConnected(set<string>&neighbor);
	int resultdata(vector<string> &index, double & ssq, double & weightedSSQ, double & purity, double & correctedRate, double & weightedPurity,
			string &type);
	int incrementalMergeCluster(GlobalGrid & g1, GlobalGrid & g2);
	void incrementalAdjust(double timeArrive);
	bool incrementalRangeQuery(GlobalGrid & g, list<string>&seedlist, int type);
	void updateDensity(double timeArrive) {
		for (hashMapIt it = globalGridHash.begin(); it != globalGridHash.end(); ++it) {
			it->second.updateDensity(timeArrive);
		}
	}
	bool expandCluster(GlobalGrid & g, list<string>&seedList, int type = EXPAND);
	void epsilonRangeQuery(GlobalGrid & g, list<string> & seedList, int type);
	void clearCluster();
	int getGridNum(const string key);
	int getGridNum(int clusterid);
	void buildCluster();
	void addToCluster(hashMapIt it, Cluster & c);
	void addToCluster(vector<vector<int> >& neighbor, Cluster & c);
	void mergeCluster(int clusterid1, int clusterid2);
	void initCluster(double timeArrive);
	void adjustCluster(double timeArrive);
	void joinToCluster(Cluster & c1, Cluster & c2);
	void reClustering(vector<int>&key);
	int createCluster(hashMapIt it);
	int removeFromCluster(hashMapIt it);
	bool changeClusterLabel(GlobalGrid & globalGrid);
	vector<int> findLargestNeighbor(vector<int> & coordinate);
	StreamOperation os;
	int gridnum;
	int hitnum;
	ofstream out1;
	vector<double> guassRandom;
public:
	Dstream() {
		out1.open("./file/cf");
		clusterID = 0;
		gridnum = 0;
		hitnum = 0;
		double ran1 = 0;
		double ran2 = 0;
		double means = 0;
		double stdDev = 1;
		int times = DIMENSION + PROTOCOL_TYPE_LENGTH + SERVICE_LENGTH + FLAG_LENGTH;
		bool flag = false;
		if (times % 2 != 0) {
			flag = true;
			--times;
		}
		times /= 2;
		for (int i = 0; i != times; ++i) {
			CommonUtil::genGuassRandom(means, stdDev, ran1, ran2);
			guassRandom.push_back(ran1);
			guassRandom.push_back(ran2);
		}
		if (flag) {
			CommonUtil::genGuassRandom(means, stdDev, ran1, ran2);
			guassRandom.push_back(ran1);
		}
	}

	void output();
	void insert(const string & key);
	bool addPoint(DataPoint & dp);
	void experimentResult(clock_t time);
};

#endif /* DSTREAM_H_ */
