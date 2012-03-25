#include "DataPoint.h"
#include <sstream>
#include "CommonUtil.h"
#include <iostream>
#include <cmath>
#include "iomanip"
/*		compute coordinate of grid
 *		global grid coordinate scope  = 0 ~ globlaWarp-1
 * 	local grid coordinate scope = 0 ~ localWarp - 1
 * */
void DataPoint::getCoordinate(string & globalKey,string & localKey,string & borderKey,vector<int>&coordinate) {
	int temp = 0;
	stringstream global;
	stringstream local;
	stringstream border;
	bool isBorder = false;
	for (std::size_t i = 0 ;i != data.size();++i){
		temp = (int)(data[i] * globalWarp);
		if(temp == globalWarp)--temp;
		global<<temp;
		coordinate.push_back(temp);
		temp = (int)(data[i] * localWarp);
		if(temp == localWarp)--temp;
		local << temp;
		temp = (int)(data[i] * borderWarp);
		if(temp == borderWarp)--temp;
		border<<temp;
		if(isBorder == false){
			if(data[i]>=borderPosMin1 && data[i]<borderPosMax1){
				isBorder  = true;
			}
		}
	}
	if(isBorder  == true){
		borderKey = border.str();
	}
	else
	{
		borderKey = "";
	}
	globalKey = global.str();
	localKey = local.str();
}

vector<string> DataPoint::getKeyVector(string & key){
	stringstream ss;
	vector<int>coordinate;
	vector<vector<int> >neighbor;
	vector<string>keyVector;
	int temp = 0;
	for(int i = 0; i< VECTORDIMENSION; ++i){
		temp = (int) (data[i] * partitionNum);
		ss<<temp;
		coordinate.push_back(temp);
	}
	key = ss.str();

	CommonUtil::getNeighbour(coordinate,neighbor);
	for(vector<vector<int> >::iterator it = neighbor.begin();it != neighbor.end() ; ++it){
		keyVector.push_back(CommonUtil::getString(*it));
	}
	return keyVector;
}

void DataPoint::norm(const vector<double> & eachDimensionMax , const vector<double> & eachDimensionMin ){
	double delta = 0;
	for(int i = 0 ; i< DIMENSION;++i){
		delta = eachDimensionMax.at(i) -eachDimensionMin.at(i);
		if(delta > 0)
			data[i] = ((data[i] - eachDimensionMin.at(i)) / delta);
		else
			data[i] = 0;
	}
}

ostream& operator<<(ostream & out, const DataPoint & dp){
	for(vector<double>::const_iterator it = dp.data.begin();it != dp.data.end() ;++it){
		out<<setprecision(3)<<setw(3)<<*it<<",";
	}
	return out;
}

