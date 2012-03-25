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

void svmTest() {
	StreamOperation so;
	std::ifstream input;
	input.open(fileName, ios_base::in);
	vector<string> lines;
	vector<string> items;
	hashMap protocol;
	hashMap service;
	hashMap flags;
	vector<double>itemvalue;
	std::ofstream output;
	output.open("/home/fatshaw/svmtest2");
	int readline = 0;
	int linenum =0;
	int column=0;
	set<string>label;
	vector<vector<double> >dp;
	if (input) {
		readline = so.readStream(input, fileName, lines,1000000);
		for(int i = 0; i< 34 ;++i){
			dp.push_back(vector<double>(readline));
		}
		cout<<"line"<<lines.size()<<endl;
		for (vector<string>::iterator it = lines.begin(); it != lines.end(); ++it) {
			items.clear();
			itemvalue.clear();
			CommonUtil::split(*it, ',', items);
			column = 0;
			for (vector<string>::size_type index = 0; index != items.size(); ++index) {
//				if (index == PROTOCOL_TYPE) {
//					if (protocol.find(items.at(index)) == protocol.end()) {
//						protocol.insert(hashMap::value_type(items.at(index), protocol.size() + 1));
//					}
//					temp = protocol[items.at(index)];
//				} else if (index == SERVICE) {
//					if (service.find(items.at(index)) == service.end()) {
//						service.insert(hashMap::value_type(items.at(index), service.size() + 1));
//					}
//					temp = service[items.at(index)];
//				} else if (index == FLAG) {
//					if (flags.find(items.at(index)) == flags.end()) {
//						flags.insert(hashMap::value_type(items.at(index), flags.size() + 1));
//					}
//					temp = flags[items.at(index)];
//				}
				if(index == PROTOCOL_TYPE || index == SERVICE || index == FLAG)continue;
				if (index != LAND && index != LOGGED_IN && index != IS_HOST_LOGIN && index != IS_GUEST_LOGIN && index != NUM_OUTBOUND_CMDS
						&& index != items.size() - 1 /*&& index != PROTOCOL_TYPE && index != SERVICE && index != FLAG*/) {
					//itemvalue.push_back(atof(items[index].c_str()));
					dp[column++][linenum]=atof(items[index].c_str());
				}
				//				else if(index == DURATION || index == SRC_BYTES || index == DST_BYTES || index == WRONG_FRAGMENT || index == URGENT ||
				//						index == HOT || index == NUM_FAILED_LOGINS || index == NUM_COMPROMISED || index == NUM_ROOT || index == NUM_FILE_CREATIONS
				//						|| index == NUM_SHELLS || index == NUM_ACCESS_FILES || index == COUNT || index == SRV_COUNT || index == DST_HOST_COUNT ||
				//						index == DST_HOST_SRV_COUNT){
				//					temp = atof(items[index].c_str());
				//				}
				else if(index == items.size() - 1){
					if(label.find(items.back())==label.end()){
						label.insert(items.back());
					}
					int result = CommonUtil::findType(items.back());
					//cout<<result<<endl;
					//result = (result ==3?-1: 1);
					dp[column++][linenum]=result;
//					output<<result<<" ";
//					for(vector<string>::size_type si = 0 ;si!=itemvalue.size();++si){
//						output<<si+1<<":"<<double((int)(itemvalue.at(si) * 1000)+0.5)/1000<<" ";
//					}
//					output<<endl;
				}
			}
			linenum++;
		}
	}
	cout<<readline<<endl;
	cout<<label.size()<<endl;
	CommonUtil::standardize(dp, 33, readline);
	for(int i = 0;i<readline;++i){
		output<<dp[33][i]<< " ";
		for(int j = 0;j<33;j++){
			output<<j+1<<":"<<dp[j][i]<<" ";
		}
		output<<endl;
	}
	input.close();
	output.flush();
	output.close();
}

void test() {
	totalNormal = 0;
	totalDos = 0;
	totalR2l = 0;
	totalU2l = 0;
	totalProbing = 0;
	srand((unsigned) time(0));
	double time = 1;
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
	double temp = 0;
	int lineNum = 0;
	int columnNum = 0;
	int readLineCnt = 0;
	readPointNum = 0;
	dataVector.clear();
	int timeunit = 2;
	char t[50];
	for (int i = 0; i != DIMENSION; ++i) {
		dataVector.push_back(vector<double> (readLine));
	}

	for (int round = 0; round != readTimes; ++round) {
		lineNum = 0;
		lines.clear();
		readLineCnt = so.readStream(input, fileName, lines, readLine);
		readPointNum += readLineCnt;
		for (vector<string>::iterator it = lines.begin(); it != lines.end(); ++it) {
			columnNum = 0;
			items.clear();
			CommonUtil::split(*it, ',', items);
			if(items.size()==0)break;
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
				}
				if (index != LAND && index != LOGGED_IN && index != IS_HOST_LOGIN && index != IS_GUEST_LOGIN && index != NUM_OUTBOUND_CMDS
						&& index != items.size() - 1 /*&& index != PROTOCOL_TYPE && index != SERVICE && index != FLAG*/) {
					temp = atof(items[index].c_str());
				}
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
				data.push_back(dataVector[dimension][lines]);
			}
			dpVector.push_back(DataPoint(data, time++, labelVector.at(lines)));
			if (readLineCnt < readLine)
				break;
		}
		clock_t temp = clock();
		for (vector<DataPoint>::iterator it = dpVector.begin(); it != dpVector.end(); ++it) {
			dstream.addPoint(*it);
		}
		total_time += (clock() - temp) / CLOCKS_PER_SEC;
		labelVector.clear();
		dpVector.clear();
		if (readPointNum == 20000 || readPointNum == 60000 || readPointNum == 100000 || readPointNum == 140000 || readPointNum == 180000
				|| readPointNum == 220000 || readPointNum == 260000 || readPointNum == 300000 || readPointNum == 340000 || readPointNum
				== 380000 || readPointNum == 420000 || readPointNum == 460000) {
			dstream.experimentResult(total_time);
			memset(t, 50, 0);
			sprintf(t, "cp ./file/experimentdata /home/fatshaw/expdata/expdata%d_%d", timeunit, roundtime);
			system(t);
			timeunit += 4;
		}

		if (readLineCnt < readLine) {/*cout<<"readline < readLine"<<endl;*/
			break;
		}
	}

	input.close();
	input.clear();
	dstream.experimentResult(total_time);
	memset(t, 50, 0);
	sprintf(t, "cp ./file/experimentdata /home/fatshaw/expdata/expdata%d_%d", timeunit, roundtime);
	system(t);
}


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
	totalmax.push_back(693376000);
	totalmin.push_back(0);
	totalmax.push_back(5155470);
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

int main(int argc,char ** argv) {

	init();
	test();
	cout << "end" << endl;
	return 0;
}

