/*
 * main.cpp
 *
 *  Created on: 2010-1-25
 *      Author: root
 */

#include "StreamOperation.h"
#include <cstdlib>
#include "DataPoint.h"
#include "DStream.h"
#include <vector>
#include <ctime>
#include <bitset>
#include "Dbscan.h"
using namespace std;
int roundtime = 1;
typedef hash_map<string, int, str_hash> hashMap;
typedef hash_map<string, int, str_hash>::iterator hashMapIt;
void resultdata(vector<int> &index, vector<DataPoint>&dp, int time, double & ssq, double & purity);

void output(vector<DataPoint>&dp) {
	ofstream out("cluster");
	for (vector<DataPoint>::iterator it = dp.begin(); it != dp.end(); ++it) {
		out << (*it).getLabel() << ", " << (*it).clusterid << endl;
	}
	out.flush();
	out.close();
}

void experimentResult(vector<DataPoint>&dp, int clusterid) {
	ofstream out("experimentdata");
	vector<vector<int> > index;
	for (int i = 0; i < clusterid; ++i) {
		index.push_back(vector<int> ());
	}

	//cout << "clusterid" << clusterid << endl;
	int noise = 0;
	int unclassified = 0;
	for (std::size_t i = 0; i != dp.size(); ++i) {
		if (dp.at(i).clusterid == NOISE)
			++noise;
		else if (dp.at(i).clusterid == UNCLASSIFIED)
			++unclassified;
		else
			index.at(dp.at(i).clusterid).push_back(i);
	}

	double ssq = 0;
	double purity = 0;

	out << "cluster num = " << clusterid << endl;
	out << "noise num = " << noise << endl;
	out << "unclassified num = " << unclassified << endl;
	for (int i = 0; i != clusterid; ++i) {
		resultdata(index.at(i), dp, 0, ssq, purity);
		out << "clusterid = " << i << endl;
		out << "ssq = " << ssq << endl;
		out << "purity" << purity << endl;
	}
	out.flush();
	out.close();
	out.clear();
}

typedef hash_map<string, int, str_hash> pureHash;
typedef hash_map<string, int, str_hash>::iterator pureHashIt;

void resultdata(vector<int> &index, vector<DataPoint>&dp, int time, double & ssq, double & purity) {
	pureHash pure;
	vector<double> sum;
	vector<double> data;
	string label("");
	for (std::size_t i = 0; i < index.size(); ++i) {
		if (dp.at(index.at(i)).getTimeArrive() < time)
			continue;
		label = dp.at(index.at(i)).getLabel();
		if (pure.find(label) == pure.end()) {
			pure[label] = 1;
		} else {
			pure[label]++;
		}
		data = dp.at(index.at(i)).getData();
		if (sum.size() == 0) {
			for (vector<double>::iterator it = data.begin(); it != data.end(); ++it) {
				sum.push_back(*it);
			}
		} else {
			for (std::size_t it = 0; it != data.size(); ++it) {
				sum[it] += data.at(it);
			}
		}
	}

	for (std::size_t i = 0; i != sum.size(); ++i) {
		sum[i] /= index.size();
	}

	ssq = 0;
	double temp = 0;
	for (std::size_t i = 0; i != index.size(); ++i) {
		temp = 0;
		data = dp.at(index.at(i)).getData();
		for (std::size_t j = 0; j != data.size(); ++j) {
			temp += pow(data.at(j) - sum.at(j), 2);
		}
		ssq += temp;
	}

	int max = 0;
	for (pureHashIt it = pure.begin(); it != pure.end(); ++it) {
		if (it->second > max) {
			max = it->second;
		}
	}

	purity = double(max) / index.size() * 100;

}

void test() {
	totalNormal = 0;
	totalDos = 0;
	totalR2l = 0;
	totalU2l = 0;
	totalProbing = 0;
	srand((unsigned) time(0));
	double time = 1;
	clock_t time_t = clock();
	StreamOperation so;
	std::ifstream input;
	input.open(fileName, ios_base::in);
	//input.open("kddcup.data_10_percent_corrected", ios_base::in);
	vector<string> lines;
	vector<double> data;
	vector<string> items;
	vector<vector<double> > dataVector;
	hashMap protocol;
	hashMap service;
	hashMap flags;
	vector<string> labelVector;
	vector<DataPoint> dpVector;
	Dstream dstream;
	//Dbscan dbscan;
	double temp = 0;
	int lineNum = 0;
	int columnNum = 0;
	int readLineCnt = 0;
	readPointNum = 0;
	time_t = clock();
	dataVector.clear();
	int timeunit = 1;
	char t[50];
	for (int i = 0; i != DIMENSION; ++i) {
		dataVector.push_back(vector<double> (readLine));
	}

	for (int round = 0; round != readTimes; ++round) {
		//cout << "round = " << round << endl;
		lineNum = 0;
		lines.clear();
		readLineCnt = so.readStream(input, fileName, lines, readLine);
		readPointNum += readLineCnt;
		//	cout << "readLine" << readLineCnt << endl;
		for (vector<string>::iterator it = lines.begin(); it != lines.end(); ++it) {
			columnNum = 0;
			items.clear();
			CommonUtil::split(*it, ',', items);
			for (vector<string>::size_type index = 0; index != items.size(); ++index) {
				if (index == PROTOCOL_TYPE) {
					if (protocol.find(items.at(index)) == protocol.end()) {
						protocol.insert(hashMap::value_type(items.at(index), protocol.size() + 1));
					}
					temp = protocol[items.at(index)];
				} else if (index == SERVICE) {
					if (service.find(items.at(index)) == service.end()) {
						service.insert(hashMap::value_type(items.at(index), service.size() + 1));
					}
					temp = service[items.at(index)];
				} else if (index == FLAG) {
					if (flags.find(items.at(index)) == flags.end()) {
						flags.insert(hashMap::value_type(items.at(index), flags.size() + 1));
					}
					temp = flags[items.at(index)];
					//	temp = log(temp);
				}
				if (index != LAND && index != LOGGED_IN && index != IS_HOST_LOGIN && index != IS_GUEST_LOGIN && index != NUM_OUTBOUND_CMDS && index != items.size() - 1 /*&& index != PROTOCOL_TYPE && index != SERVICE && index != FLAG*/) {
					temp = atof(items[index].c_str());
				}
				//				else if(index == DURATION || index == SRC_BYTES || index == DST_BYTES || index == WRONG_FRAGMENT || index == URGENT ||
				//						index == HOT || index == NUM_FAILED_LOGINS || index == NUM_COMPROMISED || index == NUM_ROOT || index == NUM_FILE_CREATIONS
				//						|| index == NUM_SHELLS || index == NUM_ACCESS_FILES || index == COUNT || index == SRV_COUNT || index == DST_HOST_COUNT ||
				//						index == DST_HOST_SRV_COUNT){
				//					temp = atof(items[index].c_str());
				//				}
				else
					continue;
				dataVector[columnNum++][lineNum] = temp;
			}
			CommonUtil::findType(items.back(), totalDos, totalNormal, totalR2l, totalU2l, totalProbing);
			labelVector.push_back(items.back());
			++lineNum;
		}

		CommonUtil::standardize(dataVector, DIMENSION, readLineCnt);
		for (int lines = 0; lines != readLineCnt; ++lines) {
			data.clear();
			for (int dimension = 0; dimension != DIMENSION; ++dimension) {
				//				if (dimension == PROTOCOL_TYPE) {
				//					double temp = dataVector[dimension][lines];
				//					bitset<PROTOCOL_TYPE_LENGTH> bit((int) temp);
				//					for (std::size_t bitindex = 0; bitindex != bit.size(); ++bitindex) {
				//						data.push_back(bit[bitindex]);
				//					}
				//				} else if (dimension == SERVICE) {
				//					double temp = dataVector[dimension][lines];
				//					bitset<SERVICE_LENGTH> bit((int) temp);
				//					for (std::size_t bitindex = 0; bitindex != bit.size(); ++bitindex) {
				//						data.push_back(bit[bitindex]);
				//					}
				//				} else if (dimension == FLAG) {
				//					double temp = dataVector[dimension][lines];
				//					bitset<FLAG_LENGTH> bit((int) temp);
				//					for (std::size_t bitindex = 0; bitindex != bit.size(); ++bitindex) {
				//						data.push_back(bit[bitindex]);
				//					}
				//				} else {
				//					data.push_back(dataVector[dimension][lines]);
				//				}
				data.push_back(dataVector[dimension][lines]);
			}
			dpVector.push_back(DataPoint(data, time++, labelVector.at(lines)));
			if (readLineCnt < readLine)
				break;
		}
		for (vector<DataPoint>::iterator it = dpVector.begin(); it != dpVector.end(); ++it) {
			dstream.addPoint(*it);
		}
		labelVector.clear();
		dpVector.clear();
		if (readPointNum == 10000 || readPointNum == 50000 || readPointNum == 90000 || readPointNum == 130000 || readPointNum == 170000 || readPointNum == 210000 || readPointNum == 250000 || readPointNum == 290000
				|| readPointNum == 330000 || readPointNum == 370000 || readPointNum == 410000 || readPointNum == 450000) {
			//			clock_t time_tt = (clock() - time_t) / CLOCKS_PER_SEC;
			//			dstream.experimentResult(time_tt);
			//			memset(t, 50, 0);
			//			sprintf(t, "cp ./file/experimentdata /root/expdata/expdata%d_%d", timeunit,roundtime);
			//			system( t);
			//			timeunit +=4;
		}
		if (readLineCnt < readLine) {/*cout<<"readline < readLine"<<endl;*/
			break;
		}
	}

	//	ofstream out("distance2");
	//
	//	for(vector<DataPoint>::iterator it = dpVector.begin();it != dpVector.end();++it){
	//		out<<*it<<endl;
	//	}

	//dbscan.build(dpVector);
	//output(dpVector);
	//experimentResult(dpVector,dbscan.clusterID);
	//dpVector.clear();
	input.close();
	input.clear();
	time_t = (clock() - time_t) / CLOCKS_PER_SEC;
	dstream.experimentResult(time_t);
	//	memset(t, 50, 0);
	//	sprintf(t, "cp ./file/experimentdata /root/expdata/expdata%d_%d", timeunit,roundtime);
	//	system( t);
}

class Test {
public:
	int a;
	int b;
	Test() {
		a = 10;
		b = 10;
	}
	~Test() {
		//cout << "destroy" << endl;
		//	cout << "a = " << a << "b = " << b << endl;
	}
	Test(const Test & t) {
		cout << "copy" << endl;
		a = t.a;
		b = t.b;
		cout << "a = " << a << "b = " << b << endl;
	}
};

void init() {
	DOS.insert(string("neptune."));
	DOS.insert(string("pod."));
	DOS.insert(string("smurf."));
	DOS.insert(string("land."));
	DOS.insert(string("back."));
	DOS.insert(string("teardrop."));
	DOS.insert(string("mailbomb."));
	DOS.insert(string("apache2."));
	DOS.insert(string("udpstorm."));

	R2L.insert(string("ftp_write."));
	R2L.insert(string("guess_passwd."));
	R2L.insert(string("imap."));
	R2L.insert(string("multihop."));
	R2L.insert(string("phf."));
	R2L.insert(string("spy"));
	R2L.insert(string("xsnoop."));
	R2L.insert(string("xlock."));
	R2L.insert(string("netbus."));
	R2L.insert(string("sendmail."));
	R2L.insert(string("warezclient."));
	R2L.insert(string("warezmaster."));

	U2L.insert(string("perl."));
	U2L.insert(string("rootkit."));
	U2L.insert(string("buffer_overflow."));
	U2L.insert(string("loadmodule."));
	U2L.insert(string("eject"));
	U2L.insert(string("ps."));
	U2L.insert(string("sqlattack."));
	U2L.insert(string("xterm."));
	U2L.insert(string("fdformat."));

	PROBING.insert(string("portsweep."));
	PROBING.insert(string("ipsweep."));
	PROBING.insert(string("satan."));
	PROBING.insert(string("nmap."));
	PROBING.insert(string("mscan."));
	PROBING.insert(string("queso"));
	PROBING.insert(string("ntinfoscan."));
	PROBING.insert(string("illegal_sniffer."));
	/*
	 totalmax.push_back(82.3475);
	 totalmin.push_back(-0.0677917);
	 totalmax.push_back(0.843823);
	 totalmin.push_back(-2.27888);
	 totalmax.push_back(11.3812);
	 totalmin.push_back(-1.88837);
	 totalmax.push_back(5.80293);
	 totalmin.push_back(-1.16258);
	 totalmax.push_back(701.641);
	 totalmin.push_back(0.00306168);
	 totalmax.push_back(156.011);
	 totalmin.push_back(-0.0262873);
	 totalmax.push_back(22.2067);
	 totalmin.push_back(-0.0477201);
	 totalmax.push_back(544.438);
	 totalmin.push_back(-0.00257147);
	 totalmax.push_back(38.3141);
	 totalmin.push_back(-0.0441359);
	 totalmax.push_back(322.164);
	 totalmin.push_back(-0.00978217);
	 totalmax.push_back(491.564);
	 totalmin.push_back(-0.00567868);
	 totalmax.push_back(94.7693);
	 totalmin.push_back(-0.0105519);
	 totalmax.push_back(256.649);
	 totalmin.push_back(-0.00467567);
	 totalmax.push_back(493.358);
	 totalmin.push_back(-0.00564001);
	 totalmax.push_back(290.398);
	 totalmin.push_back(-0.0112321);
	 totalmax.push_back(181.478);
	 totalmin.push_back(-0.00991896);
	 totalmax.push_back(219.261);
	 totalmin.push_back(-0.0276318);
	 totalmax.push_back(0);
	 totalmin.push_back(0);
	 totalmax.push_back(0.838457);
	 totalmin.push_back(-1.55894);
	 totalmax.push_back(0.8854);
	 totalmin.push_back(-1.18911);
	 totalmax.push_back(2.16254);
	 totalmin.push_back(-0.464089);
	 totalmax.push_back(2.16104);
	 totalmin.push_back(-0.46352);
	 totalmax.push_back(4.0694);
	 totalmin.push_back(-0.24796);
	 totalmax.push_back(4.05899);
	 totalmin.push_back(-0.248631);
	 totalmax.push_back(0.53699);
	 totalmin.push_back(-2.03907);
	 totalmax.push_back(11.9094);
	 totalmin.push_back(-0.255243);
	 totalmax.push_back(6.81898);
	 totalmin.push_back(-0.203633);
	 totalmax.push_back(0.34797);
	 totalmin.push_back(-3.59049);
	 totalmax.push_back(0.62556);
	 totalmin.push_back(-1.77918);
	 totalmax.push_back(0.599398);
	 totalmin.push_back(-1.83499);
	 totalmax.push_back(8.86971);
	 totalmin.push_back(-0.282866);
	 totalmax.push_back(0.82705);
	 totalmin.push_back(-1.25062);
	 totalmax.push_back(23.5759);
	 totalmin.push_back(-0.158629);
	 totalmax.push_back(2.16307);
	 totalmin.push_back(-0.464417);
	 totalmax.push_back(2.16203);
	 totalmin.push_back(-0.463202);

	 totalmax.push_back(4.08468);
	 totalmin.push_back(-0.252039);
	 totalmax.push_back(4.09572);
	 totalmin.push_back(-0.249464);
	 */

	totalmax.push_back(58329);
	totalmin.push_back(0);
	totalmax.push_back(3);
	totalmin.push_back(0);
	totalmax.push_back(66);
	totalmin.push_back(0);
	totalmax.push_back(11);
	totalmin.push_back(0);
	totalmax.push_back(69337600);
	totalmin.push_back(0);
	totalmax.push_back(515547);
	totalmin.push_back(0);
	totalmax.push_back(3);
	totalmin.push_back(0);
	totalmax.push_back(3);
	totalmin.push_back(0);
	totalmax.push_back(30);
	totalmin.push_back(0);
	totalmax.push_back(5);
	totalmin.push_back(0);
	totalmax.push_back(884);
	totalmin.push_back(0);
	totalmax.push_back(1);
	totalmin.push_back(0);
	totalmax.push_back(2);
	totalmin.push_back(0);
	totalmax.push_back(993);
	totalmin.push_back(0);
	totalmax.push_back(28);
	totalmin.push_back(0);
	totalmax.push_back(2);
	totalmin.push_back(0);
	totalmax.push_back(8);
	totalmin.push_back(0);
	totalmax.push_back(511);
	totalmin.push_back(0);
	totalmax.push_back(511);
	totalmin.push_back(0);
	totalmax.push_back(1);
	totalmin.push_back(0);
	totalmax.push_back(1);
	totalmin.push_back(0);
	totalmax.push_back(1);
	totalmin.push_back(0);
	totalmax.push_back(1);
	totalmin.push_back(0);
	totalmax.push_back(1);
	totalmin.push_back(0);
	totalmax.push_back(1);
	totalmin.push_back(0);
	totalmax.push_back(1);
	totalmin.push_back(0);
	totalmax.push_back(255);
	totalmin.push_back(0);
	totalmax.push_back(255);
	totalmin.push_back(0);
	totalmax.push_back(1);
	totalmin.push_back(0);
	totalmax.push_back(1);
	totalmin.push_back(0);
	totalmax.push_back(1);
	totalmin.push_back(0);
	totalmax.push_back(1);
	totalmin.push_back(0);
	totalmax.push_back(1);
	totalmin.push_back(0);
	totalmax.push_back(1);
	totalmin.push_back(0);
	totalmax.push_back(1);
	totalmin.push_back(0);
	totalmax.push_back(1);
	totalmin.push_back(0);
	totalmax.push_back(1);
	totalmin.push_back(0);

}

int main() {

	init();
	/*
	 vector<int> gaps;
	 gaps.push_back(10000);
	 gaps.push_back(5000);
	 vector<int> len;
	 len.push_back(25);
	 len.push_back(20);
	 for (vector<int>::iterator git = gaps.begin(); git != gaps.end(); ++git) {
	 gap = *git;
	 if(gap == 5000)beta = 2;
	 for (vector<int>::iterator it = len.begin(); it != len.end(); ++it) {
	 globalWarp = *it;
	 //for (vector<int>::iterator sit = speed.begin(); sit != speed.end(); ++sit) {
	 // = *sit;
	 //for(int lines = 10000;lines<=500000;lines+=40000){
	 //readTimes = 500000 / readLine;
	 //time_t = clock();
	 test();
	 ++roundtime;
	 //time_t = (clock() - time_t) / CLOCKS_PER_SEC;
	 //memset(t, 50, 0);
	 //sprintf(t, "cp ./file/experimentdata /root/expdata/expdata%d", i++);
	 //system( t);
	 //}
	 //}
	 }
	 }
	 */

	test();

	//	Dstream ds;
	//	ds.cudaLocalBuildCluster();

#ifdef WIN32
	system("pause");
#endif
	return 0;
}

