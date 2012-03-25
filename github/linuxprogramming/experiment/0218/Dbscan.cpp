/*
 * Dbscan.cpp
 *
 *  Created on: 2010-1-29
 *      Author: root
 */

#include "Dbscan.h"

void Dbscan::build(vector<DataPoint>&dp) {
	for (vector<DataPoint>::iterator it = dp.begin(); it != dp.end(); it++) {
		if ((*it).clusterid == UNCLASSIFIED) {
			if(expand(*it, dp)){
				clusterID++;
			}
		}
	}
}

bool Dbscan::expand(DataPoint & data, vector<DataPoint>&dp) {
	list<pos> seedlist;
	rangeQuery(data, dp, seedlist, EXPAND);
	if ((int)seedlist.size() < minPts) {
		data.clusterid = NOISE;
		return false;
	}

	for (list<pos>::iterator it = seedlist.begin(); it != seedlist.end();) {
		dp.at(*it).clusterid = clusterID;
		if (dp.at(*it) == data) {
			it = seedlist.erase(it);
		} else {
			++it;
		}
	}
	//cout << "before neighr" << endl;
	list<pos> neighbor;
	for (list<pos>::iterator it = seedlist.begin(); it != seedlist.end();) {
		rangeQuery(dp.at(*it), dp, neighbor, FINDNEIGHBOR);
		//cout << "neighbor size=  " << seedlist.size() << endl;
		if ((int)neighbor.size() >= minPts) {
			for (list<pos>::iterator nit = neighbor.begin(); nit
					!= neighbor.end(); ++nit) {
				if (dp.at(*nit).clusterid == NOISE || dp.at(*nit).clusterid
						== UNCLASSIFIED) {
					if (dp.at(*nit).clusterid == UNCLASSIFIED) {
						seedlist.push_back(*nit);
					}
					dp.at(*nit).clusterid = clusterID;
				}
			}
		}
		it = seedlist.erase(it);
	}
	return true;
}
void Dbscan::rangeQuery(DataPoint & data, vector<DataPoint>&dp,
		list<pos>&seedlist, int type) {
	double distance = 0;
	int pointNum = 0;
	for (std::size_t index = 0; index != dp.size(); ++index) {
		if (type == FINDNEIGHBOR && dp.at(index).clusterid > UNCLASSIFIED && pointNum >= minPts)continue;
		else {
			distance = data.distance(dp.at(index));
			if (distance <= eps && distance > 0) {
				seedlist.push_back(index);
				++pointNum;
			}
		}
	}
}
