#ifndef GRID
#define GRID

#include "DataPoint.h"
#include <fstream>
#include <list>
#include <map>
#ifdef WIN32
#include <hash_map>
#include <hash_set>
using namespace stdext;
#endif

#ifdef LINUX
#include <ext/hash_map>
#include <ext/hash_set>
using namespace __gnu_cxx;
#endif

using namespace std;

struct equst {
	bool operator()(const char * c1, const char * c2) const {
		return strcmp(c1, c2) == 0;
	}
};

namespace __gnu_cxx {
template<> struct hash<std::string> {
	size_t operator()(const std::string&s) const {
		return __stl_hash_string(s.c_str());
	}
};
}

struct str_hash {
	inline std::size_t operator()(const std::string & s) const {
		return __stl_hash_string(s.c_str());
	}

	inline size_t myHashString(const char * str){
		unsigned long h = 0;
		for(;*str;++str){
			h = h + *str;
		}
		return size_t(h);
	}
};

struct str_hash_test {
	inline std::size_t operator()(const std::string & s) const {
		size_t h  =  myHashString(s.c_str());
		//outfile<<s<<" , "<<h<<endl;
		//cout<<s<<","<<h<<endl;
		return h;
	}

	inline size_t myHashString(const char * str)const{
		unsigned long h = 0;
		for(;*str;++str){
			h = h*5 + *str;
		}
		return size_t(h);
	}
};

typedef hash_set<string>hashSet;
typedef hashSet::iterator hashSetIt;
//typedef set<string>hashSet;
//typedef hashSet::iterator hashSetIt;
typedef hash_map<int,hashSet>integerHash;
typedef integerHash::iterator integerHashIt;

class LocalGrid{
private:
	string key;
	vector<int>coordinate;
	list<string> globalKeySet;
	integerHash denseGridHash;
public:
	LocalGrid(const string & localKey,const string & globalKey,vector<int> & coordinate):
		key(localKey),coordinate(coordinate){
		this->globalKeySet.push_back(globalKey);
	}

	LocalGrid(const LocalGrid & localGrid){
		this->coordinate = localGrid.coordinate;
		this->key = localGrid.key;
		this->globalKeySet = localGrid.globalKeySet;
		this->denseGridHash = localGrid.denseGridHash;
	}

	void addGlobalGrid(const string & globalKey){
		globalKeySet.push_back(globalKey);
	}

	list<string> getGlobalKey(){
		return globalKeySet;
	}

	void deleteGlobalKey(const string & key){
		this->globalKeySet.remove(key);
	}
	void addDenseBorderGrid(const string & denseGridKey,int dimension);

	hashSet getDenseBorderGrid(int dimension);

	vector<int>getCoordinate(){
		return coordinate;
	}

	string getKey(){
		return key;
	}
};

struct ConnectedInfo {
	int dimension;
	string key;	//connected global grid key
};

class GlobalGrid {
private:
	static double meansDensity; //mean density of all grid in the space
	double tLastUpdated;
	double tLastDeleted;
	double density;
	vector<double> cf;//cf1x
	vector<double>cf2;//cf2x
	int clusterID; //begin from 0, if -1 means grid does not belong to any cluster.
	int status; //dense , sparse , transitional
	bool isChangedStatus;
	string key; //coordinate in form of string
	vector<string> pointLabel; // used in the kdd cup 99
	int pointNum;
	vector<int>coordinate;	//used when searching neighbor
	int label; // NORMAL = 0 , SPORADIC = 1 WITH initial value = SPORADIC
	bool isConnectedGrid;
	ConnectedInfo ci;	//if isConnectedGrid is true then we can get info from ci;
	string localGridKey;//belonged local grid key
	string borderGridKey;
	double getDecayValue(double timeArrive) {
		return pow(decayFactor, timeArrive - tLastUpdated);
	}
	static double setMeanDensity();
	void updateCF(double timeArrive, const vector<double> &  data);
public:
	GlobalGrid(const string & key,vector<double> dp,const string & localGrid , const string & borderGrid);
	GlobalGrid(const GlobalGrid& g);
	bool addPoint(DataPoint & dp);
	void updateDensity(double timeArrive);
	double distance(GlobalGrid & g);

	int getGridStatus() {
		return status;
	}
	string   getGridKey() {
		return key;
	}
	string getLocalGridKey(){
		return localGridKey;
	}
	string getBorderGridKey(){
		return borderGridKey;
	}
	void setClusterId(int clusterId) {
		this->clusterID = clusterId;
	}
	int getClusterId() {
		return clusterID;
	}

	bool isChanged() {
		return isChangedStatus;
	}

	vector<string> getPointLabel(){
		return pointLabel;
	}


	vector<int>  getCoordinate(){
		return coordinate;
	}

	double getLastDeletedTime(){
		return tLastDeleted;
	}

	int getLabel(){
		return label;
	}
	vector<double> getCF(){
		return cf;
	}
	vector<double> getCF2(){
		return cf2;
	}

	bool operator==(const GlobalGrid & g){
		if(key == g.key)return true;
		else return false;
	}

	int getPointNum(){
		return pointNum;
	}

	void setIsConnectedGrid(bool isConnected){
		this->isConnectedGrid = isConnected;
	}

	bool getIsConnectedGrid(){
		return this->isConnectedGrid;
	}

	void setCi(ConnectedInfo ci){
		this->ci = ci;
	}

	ConnectedInfo getCi(){
		return ci;
	}
	void setLocalGridKey(const string & localKey){
		this->localGridKey = localKey;
	}
	string getLocalKey(){
		return this->localGridKey;
	}

	void setIsChanged(bool isChanged){
		this->isChangedStatus = isChanged;
	}
};

#endif
