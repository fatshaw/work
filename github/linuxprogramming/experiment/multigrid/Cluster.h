#ifndef CLUSTER
#define CLUSTER
#include <vector>
#include <algorithm>
#include "CommonUtil.h"
#include <iostream>
#include "ConstVar.h"
using namespace std;

class Cluster {
private:
	int clusterID; //begin from 0
	bool isDeleted;
	vector<string> gridKey;
	vector<vector<int> >gridCoordinate;

	bool isOutsideGrid(const vector<int> & coordinate);
	bool isInCluster(const vector<int> & coor){
		return !(find(gridCoordinate.begin(),gridCoordinate.end(),coor) == gridCoordinate.end());
	}
public:
	Cluster(int clusterID) :
		clusterID(clusterID),isDeleted(false) {
	}
	Cluster(const Cluster & c){
		clusterID = c.clusterID;
		isDeleted = c.isDeleted;
		gridKey = c.gridKey;
		gridCoordinate = c.gridCoordinate;
	}

	void addGridKey(const string & key) {
		gridKey.push_back(key);
	}
	bool isOutsideGrid(vector<int> coordinate, vector<int> key);
	void addGridCoordinate(const vector<int> coordinate){
		gridCoordinate.push_back(coordinate);
	}

	void removeGridKey(const string key) {
		vector<string>::iterator it = find(gridKey.begin(), gridKey.end(), key);
		if(it != gridKey.end())
			gridKey.erase(it);
	}

	void removeGridCoordinate(vector<int> coordinate){

		vector<vector<int> >::iterator it = find(gridCoordinate.begin() , gridCoordinate.end() , coordinate);
		if(it != gridCoordinate.end()){
			gridCoordinate.erase(it);
		}
	}
	void setClusterID(int id) {
		clusterID = id;
	}

	int getClusterID() {
		return clusterID;
	}

	int getGridNum() {
		return (int) gridKey.size();
	}

	vector<vector<int> > getOutsideGrid();

	vector<string>  getGridKey() {
		return gridKey;
	}

	bool operator==(const Cluster & c) {
		if (clusterID == c.clusterID && gridKey == c.gridKey)
			return true;
		else
			return false;
	}

	void remove(){
		//cout<<"remove"<<endl;
		setDeleted(true);
		clusterID = UNCLASSIFIED;
		gridKey.clear();
		gridCoordinate.clear();
	}
	void setDeleted(bool isDeleted){
		this->isDeleted = isDeleted;
	}

	bool getIsDeleted(){
		return isDeleted;
	}

	bool isOutsideGrid(string & key, string & ifAddKey);

	bool isConnected(vector<int> & key,vector<int>& unConnectedKey);

	vector<vector<int> > getGridCoordinate(){
		return gridCoordinate;
	}


};

#endif
