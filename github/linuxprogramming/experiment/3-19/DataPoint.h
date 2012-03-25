#ifndef DATAPOINT
#define DATAPOINT
#include "ConstVar.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

class GlobalGrid;

class DataPoint {
	friend class GlobalGrid;
private:
	vector<double> data;//initial data with d dimension
	double timeArrive; //data arriving time
public:
	int clusterid ;
	string label; //initial type of point
public:
	DataPoint(){};
	DataPoint(vector<double> data, double time,string  label) {
		this->data = data;
		this->timeArrive = time;
		this->label = label;
		clusterid = UNCLASSIFIED;
	}

	DataPoint(const DataPoint & dp){
		this->data = dp.data;
		this->timeArrive = dp.timeArrive;
		this->label = dp.label;
		this->clusterid = dp.clusterid;
	}
	~DataPoint(){data.clear();}

	vector<double> getData() {
		return data;
	}

	double getTimeArrive() {
		return timeArrive;
	}

	const string  getLabel() {
		return label;
	}

	void setData(vector<double> data) {
		this->data = data;
	}

	void setLabel(string l) {
		this->label = l;
	}

	void setTimeArrive(double time) {
		this->timeArrive = time;
	}

	void norm(const vector<double> & eachDimensionMax , const vector<double> & eachDimensionMin);

	void getCoordinate(string & key,string & highLevelKey,string & borderKey,vector<int>&coordinate);

	double getDimension(int index){
		return data.at(index);
	}

	void setDimension(int index , double value){
		data[index] = value;
	}
	vector<string>  getKeyVector(string & key);

	double distance(DataPoint & dp){
		double temp = 0;
		for(vector<double>::size_type index = 0; index != data.size();++index){
			temp += pow(data[index] - dp.data[index] ,2);
		}
		return sqrt(temp);
	}

	bool operator==(const DataPoint & dp){
		if(data == dp.data && clusterid == dp.clusterid && label == dp.label){
			return true;
		}
		else return false;
	}

	friend ostream& operator<<(ostream & out, const DataPoint & dp);
};

#endif
