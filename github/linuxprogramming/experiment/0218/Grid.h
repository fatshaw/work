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

	inline size_t myHashString(const char * str) {
		unsigned long h = 0;
		for (; *str; ++str) {
			h = h + *str;
		}
		return size_t(h);
	}
};

typedef hash_set<string> hashSet;
typedef hashSet::iterator hashSetIt;
//typedef set<string>hashSet;
//typedef hashSet::iterator hashSetIt;
typedef hash_map<int, hashSet> integerHash;
typedef integerHash::iterator integerHashIt;

class Grid {
protected:
	string key;
	vector<int> coordinate;
public:

	string getKey() {
		return key;
	}

	vector<int> getCoordinate() {
		return coordinate;
	}
};

class LocalGrid: public Grid {
private:
	list<string> globalKeySet;
	integerHash denseGridHash;
public:
	LocalGrid(const string & localKey, const string & globalKey, vector<int> & coordinate) {
		this->key = localKey;
		this->coordinate = coordinate;
		this->globalKeySet.push_back(globalKey);
	}

	LocalGrid(const LocalGrid & localGrid) {
		this->coordinate = localGrid.coordinate;
		this->key = localGrid.key;
		this->globalKeySet = localGrid.globalKeySet;
		this->denseGridHash = localGrid.denseGridHash;
	}

	void addGlobalGrid(const string & globalKey) {
		globalKeySet.push_back(globalKey);
	}

	list<string> getGlobalKey() {
		return globalKeySet;
	}

	void deleteGlobalKey(const string & key) {
		this->globalKeySet.remove(key);
	}
	void addDenseBorderGrid(const string & denseGridKey, int dimension);

	hashSet getDenseBorderGrid(int dimension);
};

struct ConnectedInfo {
	int dimension;
	string key; //connected global grid key
};

class GlobalGrid: public Grid {
private:
	static double meansDensity; //mean density of all grid in the space
	double tLastUpdated;
	double tLastDeleted;
	double density;
	vector<double> cf;//cf1x
	vector<double> cf2;//cf2x
	int clusterID; //begin from 0, if -1 means grid does not belong to any cluster.
	int status; //dense , sparse , transitional
	bool isChangedStatus;
	vector<string> pointLabel; // used in the kdd cup 99
	int pointNum;
	int label; // NORMAL = 0 , SPORADIC = 1 WITH initial value = SPORADIC
	bool isConnectedGrid;
	ConnectedInfo ci; //if isConnectedGrid is true then we can get info from ci;
	string localGridKey;//belonged local grid key
	string borderGridKey;
	double getDecayValue(double timeArrive) {
		return pow(decayFactor, timeArrive - tLastUpdated);
	}
	static double setMeanDensity();
	void updateCF(double timeArrive, const vector<double> & data);
public:
	GlobalGrid(const string & key, vector<double> dp, const string & localGrid, const string & borderGrid);
	GlobalGrid(const GlobalGrid& g);
	bool addPoint(DataPoint & dp);
	void updateDensity(double timeArrive);
	double distance(GlobalGrid & g);

	int getGridStatus() {
		return status;
	}

	string getLocalGridKey() {
		return localGridKey;
	}

	string getBorderGridKey() {
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

	void setIsChanged(bool isChanged) {
		this->isChangedStatus = isChanged;
	}

	vector<string> getPointLabel() {
		return pointLabel;
	}

	double getLastDeletedTime() {
		return tLastDeleted;
	}

	int getLabel() {
		return label;
	}

	vector<double> getCF() {
		return cf;
	}

	vector<double> getCF2() {
		return cf2;
	}

	bool operator==(const GlobalGrid & g) {
		if (key == g.key)
			return true;
		else
			return false;
	}

	void setIsConnectedGrid(bool isConnected) {
		this->isConnectedGrid = isConnected;
	}

	bool getIsConnectedGrid() {
		return this->isConnectedGrid;
	}

	void setCi(ConnectedInfo ci) {
		this->ci = ci;
	}

	ConnectedInfo getCi() {
		return ci;
	}

	void setLocalGridKey(const string & localKey) {
		this->localGridKey = localKey;
	}

	string getLocalKey() {
		return this->localGridKey;
	}

	int getPointNum() {
		return pointNum;
	}
};

#endif
