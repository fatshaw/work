#include "Grid.h"
#include "CommonUtil.h"
GlobalGrid::GlobalGrid(const string & key, vector<double> data, const string & localGrid, const string & borderGrid) :
	key(key), pointNum(0), localGridKey(localGrid), borderGridKey(borderGrid) {
	tLastDeleted = 0;
	tLastUpdated = 0;
	//cf = new double[DIMENSION];
	for (int i = 0; i != VECTORDIMENSION; ++i) {
		cf.push_back(0);
		cf2.push_back(0);
	}
	clusterID = UNCLASSIFIED;
	density = 0;
	status = UNSTATUS;
	isChangedStatus = false;
	label = NORMAL;// SPORADIC;
	CommonUtil::getInteger(data, coordinate);
}

bool GlobalGrid::addPoint(DataPoint & dp) {
	pointLabel.push_back(dp.getLabel());
	density = density * getDecayValue(dp.timeArrive) + 1;
	updateCF(dp.timeArrive, dp.data);
	label  = NORMAL;
	tLastUpdated = dp.timeArrive;
	++pointNum;
	return true;
}

void GlobalGrid::updateDensity(double timeArrive) {
	density = density * getDecayValue(timeArrive);
	if (density >= GlobalGrid::meansDensity * cm) {
		if (status != DENSE)
			isChangedStatus = true;
		else
			isChangedStatus = false;
		status = DENSE;
		label = NORMAL;
	}
	else if (density <= GlobalGrid::meansDensity * cl) {
		if (status != SPARSE)
			isChangedStatus = true;
		else
			isChangedStatus = false;
		status = SPARSE;
		if(label != SPORADIC){
		label = SPORADIC;
		tLastDeleted = timeArrive;
		}
	}
	else if (density > GlobalGrid::meansDensity * cl && density < GlobalGrid::meansDensity * cm) {
		if (status != TRANSITIONAL)
			isChangedStatus = true;
		else
			isChangedStatus = false;
		status = TRANSITIONAL;
		label = NORMAL;
	}
	tLastUpdated = timeArrive;
}

void GlobalGrid::updateCF(double timeArrive, const vector<double> & data) {
	for (int i = 0; i != VECTORDIMENSION; ++i) {
		cf[i] = (cf[i] * pointNum /** getDecayValue(timeArrive)*/+ data[i]) / (pointNum + 1);
		cf2[i] = (cf2[i] * pointNum /*getDecayValue(timeArrive)*/+ pow(data[i], 2)) / (pointNum + 1);
	}
}

double GlobalGrid::setMeanDensity() {
	double d = 1 / decayFactor;
	for (int i = 0; i != DIMENSION; ++i) {//i did not use VECTORDIMENSION here cause it really has DIMENSION space
		d /= globalWarp;//partitionNum;
	}
	return d;
}

GlobalGrid::GlobalGrid(const GlobalGrid& g) {
	this->cf = g.cf;
	this->clusterID = g.clusterID;
	this->density = g.density;
	this->isChangedStatus = g.isChangedStatus;
	this->key = g.key;
	this->pointLabel = g.pointLabel;
	this->tLastUpdated = g.tLastUpdated;
	this->tLastDeleted = g.tLastDeleted;
	this->status = g.status;
	this->coordinate = g.coordinate;
	this->label = g.label;
	this->pointNum = g.pointNum;
	this->cf2 = g.cf2;
	this->borderGridKey = g.borderGridKey;
	this->localGridKey = g.localGridKey;
}

double GlobalGrid::meansDensity = setMeanDensity();

double GlobalGrid::distance(GlobalGrid & g) {
	if (this == &g)
		return 0;
	double temp = 0;
	//		double temp1 = 0;
	//		double protocol = 0;
	//		double service = 0;
	//		double flag = 0;
	//		for(std::size_t index = 0; index != cf.size();++index){
	//			if(index >=1 && index <= 2){
	//				protocol += pow(cf[index] - g.cf[index],2);
	//			}
	//			else if(index >=3  && index <= 9){
	//				service += pow(cf[index] - g.cf[index],2);
	//			}
	//			else if(index >=10 && index <=13){
	//				flag += pow(cf[index] - g.cf[index],2);
	//			}else{
	//				temp1 += pow(cf[index] - g.cf[index],2);
	//			}
	//		}
	//		protocol = (protocol <=1?protocol:1);
	//		flag = (flag <=1?flag:1);
	//		service = (service <=1?service:1);
	//		temp1 += (protocol + flag + service);

	for (std::size_t index = 0; index != cf.size(); ++index) {
		if (index == PROTOCOL_TYPE) {
			if (abs(cf[index] - g.cf[index]) >= 0.33 * 0.9) {
				temp += 1 * protocol_factor;
			}
			else {
				temp += 0;
			}
		}
		else if (index == SERVICE) {
			if (abs(cf[index] - g.cf[index]) >= 0.015 * 0.9) {
				temp += 1 * service_factor;
			}
			else {
				temp += 0;
			}
		}
		else if (index == FLAG) {
			if (abs(cf[index] - g.cf[index]) >= 0.1 * 0.9) {
				temp += flag_factor;
			}
			else {
				temp += 0;
			}
		}
		else if (index == SRC_BYTES) {
			double t = pow(cf[index] - g.cf[index], 2) * src_bytes_factor;
			temp += t;
		}else if(index == DST_BYTES){
			temp += pow(cf[index] - g.cf[index], 2) * dst_bytes_factor;
		}
		else if(index == 6){//fagment
			temp += pow(cf[index] - g.cf[index], 2) * fagment_factor;
		}
		else if(index == 8){//hot
			temp += pow(cf[index] - g.cf[index], 2) * hot_factor;
		}
		else if (index == 17){
			temp += pow(cf[index] - g.cf[index], 2) *count_factor;
		}else if(index == 18){
			temp += pow(cf[index] - g.cf[index], 2) * srv_count_factor;
		}
		else if (index == 26) {//rerror_rate , //dst_host_diff_srv_rate
			temp += pow(cf[index] - g.cf[index], 2) * dst_factor;
		}
		else if (index == 27) {//rerror_rate , //dst_host_diff_srv_rate
			temp += pow(cf[index] - g.cf[index], 2) * dst_srv_factor;
		}else if(index == 33){
			temp += pow(cf[index] - g.cf[index], 2) * serror_factor;
		}
		else {
			temp += pow(cf[index] - g.cf[index], 2);
		}
	}
	return sqrt(temp);
}

void LocalGrid::addDenseBorderGrid(const string & denseGridKey, int dimension) {
	integerHashIt it = denseGridHash.find(dimension);
	if (it == denseGridHash.end()) {
		hashSet hs;
		hs.insert(denseGridKey);
		denseGridHash.insert(integerHash::value_type(dimension, hs));
	}
	else {
		it->second.insert(denseGridKey);
	}
}

hashSet LocalGrid::getDenseBorderGrid(int dimension) {
	integerHashIt it = denseGridHash.find(dimension);
	if (it != denseGridHash.end()) {
		return it->second;
	}
	else {
		hashSet hs;
		return hs;
	}
}
