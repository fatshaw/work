/*
 * Dbscan.h
 *
 *  Created on: 2010-1-29
 *      Author: root
 */

#ifndef DBSCAN_H_
#define DBSCAN_H_

#include "DataPoint.h"
#include <fstream>
#include <list>
class Dbscan{
public:
	ofstream out;
	typedef std::size_t pos;
	int clusterID;
	void build(vector<DataPoint>&dp);
	bool expand(DataPoint & data,vector<DataPoint>&dp);
	void rangeQuery(DataPoint & data,vector<DataPoint>&dp,list<pos>&seedlist,int type);
	Dbscan(){
		clusterID = 0;
	 out.open("distance");
	}
};

#endif /* DBSCAN_H_ */
