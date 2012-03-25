/*
 * CommonUtil.cpp
 *
 *  Created on: 2010-1-22
 *      Author: root
 */
#include "CommonUtil.h"
#include "ConstVar.h"
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
#include <sstream>
using namespace std;
void CommonUtil::getNeighbour(vector<int> & data,
		vector<vector<int> > & neighbour) {
	vector<int> plus;
	vector<int> minus;
	for (int i = 0; i != VECTORDIMENSION; ++i) {
		plus = data;
		minus = data;
		getKeyByIndex(i, plus, minus);
		neighbour.push_back(plus);
		neighbour.push_back(minus);
	}
}

void CommonUtil::getKeyByIndex(int i, vector<int> & plus, vector<int>&minus) {
	int temp = plus.at(i);
	if(temp < globalWarp - 1){
		plus[i] = temp + 1;
	}
	if(temp > 0)
		minus[i] = temp - 1;
}

bool CommonUtil::isNeighbor(const vector<int> & coor1,
		const vector<int> & coor2) {
	bool flag = false;
	if (coor1.size() != coor2.size())
		return false;
	vector<int>::size_type size = coor1.size();
	for (vector<int>::size_type index = 0; index != size; ++index) {
		if (coor1.at(index) == coor2.at(index))
			continue;
		else {
			if (abs(coor1.at(index) - coor2.at(index)) == 1) {
				if (flag == true) {
					return false;
				} else {
					flag = true;
				}
			} else
				return false;
		}
	}
	return true;
}
/*
bool CommonUtil::isNeighbor(const string & key1, const string & key2) {
	bool flag = false;
	if (key1.size() != key2.size())
		return false;
	if (key1.compare(key2) == 0)
		return false;
	for (string::size_type i = 0; i != key1.size(); ++i) {
		if (key1.at(i) == key2.at(i))
			continue;
		else {
			if (abs(key1.at(i) - key2.at(i)) == 1) {
				if (flag == true) {
					return false;
				} else {
					flag = true;
				}
			} else
				return false;
		}
	}
	return true;
}*/

double CommonUtil::getMax(const vector<double> & d, int lines) {
	double max = d[0];
	for (int i = 0; i < lines; ++i) {
		if (d[i] > max) {
			max = d[i];
		}
	}
	return max;
}

double CommonUtil::getMin(const vector<double> & d, int lines) {
	double min = d[0];
	for (int i = 0; i < lines; ++i) {
		if (d[i] < min) {
			min = d[i];
		}
	}
	return min;
}

void CommonUtil::getMaxMin(const vector<double> & d, int lines, double & max,
		double & min) {
	max = d[0];
	min = d[0];
	for (int i = 0; i != lines; ++i) {
		if (d[i] > max)
			max = d[i];
		if (d[i] < min)
			min = d[i];
	}
}

double CommonUtil::getStandardDeviation(const vector<double> & d, int lines) {
	double sum = 0;
	double average = 0;
	average = getAverage(d,lines);
	for (int i = 0; i != lines; ++i) {
		sum += pow(d[i] - average, 2);
		//sum +=abs(d[i] - average);
	}
	sum = sqrt(sum / (lines));
	return sum;
}

double CommonUtil::getAverage(const vector<double> & d, int lines) {
	double sum = 0;
	for (int i = 0; i != lines; ++i) {
		sum += d[i];
	}
	return sum / lines;
}

void CommonUtil::standardize(vector<vector<double> > & d,
		int dimension/*dimension*/, int readline/*READLINE*/) {
	double max = 0;
	double min = 0;
	for (int i = 0; i != dimension; ++i) {
//		if(i == PROTOCOL_TYPE || i == SERVICE || i == FLAG){
//			continue;
//		}
//		if(i == DURATION || i == DST_BYTES || SRC_BYTES /*||  i == DST_HOST_COUNT || i == DST_HOST_SRV_COUNT*/){
//			standardize(d.at(i), readline);
//		}
		//standardize(d.at(i), readline);
		//getMaxMin(d.at(i), readline, max, min);
		//cout<<"dimension "<<i<<" max = "<<max<<endl;
		//cout<<"dimension "<<i<<" min = "<<min<<endl;
//		if(max > totalmax.at(i)){
//			totalmax[i] = max;
//			cout<<"i = "<<i<<"max = "<<max<<endl;
//		}
//		if(min < totalmin.at(i)){
//			totalmin[i] = min;
//			cout<<"i = "<<i<<"min = "<<min<<endl;
//		}
		max = totalmax.at(i);
		min = totalmin.at(i);
		normalize(d.at(i), readline, max, min);
	}
}

void CommonUtil::standardize(vector<double> & d, int lines) {
	double average = getAverage(d, lines);
	double standardDeviation = getStandardDeviation(d, lines);
	for (int i = 0; i != lines; ++i) {
		if (standardDeviation != 0)
			d[i] = (d[i] - average) / standardDeviation;
	}
}

void CommonUtil::normalize(vector<double> & d, int lines, double max,
		double min) {
	double delta = max - min;
	for (int i = 0; i != lines; ++i) {
		if (delta > 0) {
			d[i] = (d[i] - min) / delta;
			if(d[i] < minValue && d[i] != 0){
				d[i] = minValue;
			}
		}
		else{
			d[i] = 0;
		}
		//if(d[i] > 1 )cout<<"more than 1"<<endl;
	}
}

void CommonUtil::split(const string & s, char delim, vector<string>&items) {
	stringstream ss(s);
	string item;
	while (ss.eof() == false) {
		getline(ss, item, delim);
		if(item.length()==0){
			break;
		}
		items.push_back(item);
	}
}

void CommonUtil::getInteger(const vector<double> & data, vector<int> & intData) {
	int temp = 0;
	for (vector<double>::const_iterator it = data.begin(); it != data.end(); ++it) {
		temp = (int) ((*it) * partitionNum);
		intData.push_back(temp);
	}
}

string CommonUtil::getString(const vector<int>&data) {
	stringstream ss;
	for (vector<int>::const_iterator it = data.begin(); it != data.end(); ++it) {
		ss << *it;
	}
	return ss.str();
}

void CommonUtil::genGuassRandom(double means , double stdDev,double & ran1,double & ran2){
	double x1,x2 ,w ;
	do{
		x1 = ranf();//2* ranf() - 1;
		x2 = ranf();//2* ranf() - 1;
		w = x1 * x1 + x2 * x2;
	}
	while(w >=1.0);

	w = sqrt((-2.0*log(w)) / w);
	ran1= x1 *w;
	ran2 = x2 * w;

	ran1 = means + ran1 * w;
	ran2 = means + ran2 * w;
}

int CommonUtil::findType(const string & key){
	if(DOS.find(key) != DOS.end()){
		return 0;
	}
	else if(R2L.find(key) != R2L.end()){
		return 1;
	}
	else if(U2L.find(key)!=U2L.end()){
		return 2;
	}
	else if(PROBING.find(key) != PROBING.end()){
		return 3;
	}
	else return -1;
}

void CommonUtil::findType(const string & key , int & dos, int & normal, int & r2l,int & u2l, int &probing){
	int result = findType(key);
	switch(result){
	case -1:
		++normal;
		break;
	case 0:
		++dos;
		break;
	case 1:
		++r2l;
		break;
	case 2:
		++u2l;
		break;
	case 3:
		++probing;
		break;
	}
}

/*	test coordinate is in the border of a grid
 * 	if the value of dimension i mode localWarp == 0 or 4 ,then we can say the coordinate is in the border in dimension i;
*
* 		if the value mode localWarp == 0 ,then it is in the lefe border of the grid
*
* 		if the value mode localWarp == 4 then it is in the right border of the grid
*
*
 * */
int CommonUtil::findBorder(const vector<int>&coordinate){
	int mode  = 0;
	for(std::size_t index = 0;index != coordinate.size();++index){
		mode = coordinate.at(index) % (int)localWarp;
		if(mode == 0 && coordinate.at(index) != 0)
			return (index+1) * -1; //i cant express -0 then i add 1 to index
		else if(mode == localWarp - 1 && coordinate.at(index) != globalWarp - 1){
			return (index + 1) ;
		}
	}
	return 0;
}

double CommonUtil::getMicroSecond(){
	timeval timer;
	gettimeofday(&timer,0);
	return timer.tv_sec * 1000000 + timer.tv_usec;
}
