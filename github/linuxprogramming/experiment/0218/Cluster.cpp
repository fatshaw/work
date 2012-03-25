/*
 * Cluster.cpp
 *
 *  Created on: 2010-1-22
 *      Author: root
 */

#include "Cluster.h"
#include "CommonUtil.h"
#include "ConstVar.h"

#ifdef WIN32
#include <hash_set>
using namespace stdext;
#endif

#ifdef LINUX
#include <ext/hash_set>
using namespace __gnu_cxx;
#endif

vector<vector<int> > Cluster::getOutsideGrid() {
	if ((int)gridCoordinate.size() <= VECTORDIMENSION) {
		return gridCoordinate;
	}
	vector<vector<int> > outsideGrid;
	for (vector<vector<int> >::iterator it = gridCoordinate.begin(); it != gridCoordinate.end(); ++it) {
		if (isOutsideGrid(*it) == true) {
			outsideGrid.push_back(*it);
		}
	}
	return outsideGrid;
}

bool Cluster::isOutsideGrid(const vector<int> & coordinate) {
	vector<int> plus;
	vector<int> minus;
	for (int i = 0; i != VECTORDIMENSION; ++i) {
		plus = coordinate;
		minus = coordinate;
		CommonUtil::getKeyByIndex(i,plus, minus);
		if ((find(gridCoordinate.begin(), gridCoordinate.end(), plus) == gridCoordinate.end())
				|| (find(gridCoordinate.begin(), gridCoordinate.end(), minus)
						== gridCoordinate.end()))
			return true;
	}
	return false;
}


bool Cluster::isOutsideGrid(vector<int> coordinate , vector<int> key) {
	gridCoordinate.push_back(key);
	bool result = isOutsideGrid(coordinate);
	gridCoordinate.pop_back();
	return result;
}

bool Cluster::isConnected(vector<int>&  key,vector<int>& unConnectedKey) {
	//cout<<" in isConnected"<<endl;
	vector<vector<int> >set;
	vector<vector<int> >neighbor;
	vector<int> plus;
	vector<int> minus;
	CommonUtil::getNeighbour(key,neighbor);
	//cout<<"neighbor size"<<neighbor.size()<<endl;
	set.push_back(neighbor.front());
	bool flag = false;
	for(vector<vector<int> >::iterator it = neighbor.begin() + 1; it != neighbor.end(); ++it){
		flag = false;
		for(vector<vector<int> >::reverse_iterator rit = set.rbegin() ; rit != set.rend() ; ++ rit){
			for(int i = 0 ;i < VECTORDIMENSION;++i){
				plus = *rit;
				minus=*rit;
						CommonUtil::getKeyByIndex(i,plus,minus);
						//cout<<"get key by index "<<i<<endl;
						if(isInCluster(plus) && CommonUtil::isNeighbor(plus,*it))
						{
							flag = true;
							break;
						}
						else if(isInCluster(minus) && CommonUtil::isNeighbor(minus,*it)){
							flag = true;
							break;
						}
						else
							continue;
			}
			if(flag == true){
				set.push_back(*it);
				break;
			}
		}
		if(flag == false){
			//cout<<"flag = false"<<endl;
			unConnectedKey = *it;
			//cout<<"assign"<<unConnectedKey<<endl;
			return false;
		}
	}
	return true;
}


