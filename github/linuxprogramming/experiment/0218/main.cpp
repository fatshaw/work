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
int roundtime = 1;

void process() {
	typedef hash_map<string, int, str_hash> hashMap;
	typedef hash_map<string, int, str_hash>::iterator hashMapIt;
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
				if (index != LAND && index != LOGGED_IN && index != IS_HOST_LOGIN && index != IS_GUEST_LOGIN && index != NUM_OUTBOUND_CMDS
						&& index != items.size() - 1 /*&& index != PROTOCOL_TYPE && index != SERVICE && index != FLAG*/) {
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
		if (readPointNum == 10000 || readPointNum == 50000 || readPointNum == 90000 || readPointNum == 130000 || readPointNum == 170000 || readPointNum
				== 210000 || readPointNum == 250000 || readPointNum == 290000 || readPointNum == 330000 || readPointNum == 370000
				|| readPointNum == 410000 || readPointNum == 450000) {
			clock_t time_tt = (clock() - time_t) / CLOCKS_PER_SEC;
			dstream.experimentResult(time_tt);
			memset(t, 50, 0);
			sprintf(t, "cp ./file/experimentdata /root/expdata/expdata%d_%d", timeunit, roundtime);
			system(t);
			timeunit += 4;
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
	memset(t, 50, 0);
	sprintf(t, "cp ./file/experimentdata /root/expdata/expdata%d_%d", timeunit, roundtime);
	system(t);
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
	vector<int> gaps;
	gaps.push_back(10000);
	//gaps.push_back(5000);
	vector<int> len;
	//len.push_back(25);
	len.push_back(25);
	for (vector<int>::iterator git = gaps.begin(); git != gaps.end(); ++git) {
		gap = *git;
		if (gap == 5000)
			beta = 2;
		for (vector<int>::iterator it = len.begin(); it != len.end(); ++it) {
			globalWarp = *it;
			//for (vector<int>::iterator sit = speed.begin(); sit != speed.end(); ++sit) {
			// = *sit;
			//for(int lines = 10000;lines<=500000;lines+=40000){
			//readTimes = 500000 / readLine;
			//time_t = clock();
			process();
			++roundtime;
			//time_t = (clock() - time_t) / CLOCKS_PER_SEC;
			//memset(t, 50, 0);
			//sprintf(t, "cp ./file/experimentdata /root/expdata/expdata%d", i++);
			//system( t);
			//}
			//}
		}
	}

	//test();


#ifdef WIN32
	system("pause");
#endif
	return 0;
}

