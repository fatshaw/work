/*
 * CommonUtil.h
 *
 *  Created on: 2010-1-22
 *      Author: root
 */

#ifndef COMMONUTIL_H_
#define COMMONUTIL_H_

#include <string>
#include <vector>
#include <cstdlib>
#include <sys/time.h>
using namespace std;

class CommonUtil {
public:
	//static void getNeighbour(string & key, vector<string>&neighbor);not used
	static void getNeighbour(vector<int> & data,vector<vector<int> > & neighbour);
	static void getInteger(const vector<double> & data , vector<int> & intData);
	//static void getKeyByIndex(int i, string & plus, string & minus);not used
	static void getKeyByIndex(int i, vector<int> & plus, vector<int>&minus);
	static string getString(const vector<int>&data);
	//static bool isNeighbor(const string & key1, const string & key2);not used
	static bool isNeighbor(const vector<int> & coor1,const vector<int> & coor2);

	static double getMax(const vector<double> & d, int lines);
	static double getMin(const vector<double> & d, int lines);

	static void standardize(vector<vector<double> > & d, int dimension/*dimension*/,
			int readline/*READLINE*/);

	static	void getMaxMin(const vector<double> & d, int lines, double & max, double & min);
	static void split(const string & s, char delim, vector<
				string>&items);

	static void genGuassRandom(double means , double stdDev,double & ran1,double & ran2);

	static void findType(const string & key , int & dos, int & normal, int & r2l,int & u2l, int &probing);
	static int findType(const string & key);
	static int findBorder(const vector<int>&coordinate);
	static double getMicroSecond();
private:
	static void  normalize(vector<double> & d,int lines,double max,double min);
	static double ranf(){
		return double(rand())/RAND_MAX;
	}
	static double getStandardDeviation(const vector<double> & d, int lines);
	static double getAverage(const vector<double> & d, int lines);
	static	void standardize(vector<double> & d, int lines);

};
#endif /* COMMONUTIL_H_ */
