/*
 * MyTime.cpp
 * 访问控制类的一些工具方法
 * Created on: 2010-8-25
 * Author: leo
 */

#include "AccessControlerUtils.h"

//去除字符串左边空字符
string lTrim(const string& str) {
	return str.substr(str.find_first_not_of(" \n\r\t"));
}

//去除字符串右边空字符
string rTrim(const string& str) {
	return str.substr(0, str.find_last_not_of(" \n\r\t") + 1);
}

//去除字符串左右两边空字符
string trim(const string& str) {
	return lTrim(rTrim(str));
}

//按照分隔符分隔字符串，并返回一个string型的vector
vector<string> splitStrByDelim(string str, string delim) {
	int last = 0;
	vector<string> ret;
	for (int i = 0; i + delim.size() <= str.size(); i++) {

		bool ok = true;
		for (int j = 0; j < (int)delim.size() && ok; j++)
			ok = str[i + j] == delim[j];
		if (ok) {
			if (i - last) {
				ret.push_back(trim(str.substr(last, i - last)));
			}
			last = i + delim.size();
		}
	}
	if (last < (int)str.size())
		ret.push_back(trim(str.substr(last)));
	return ret;
}

//合并两个vector，并返回合并结果
vector<string> mergeVectors(vector<string> a, vector<string> b) {
	vector<string> ret = a;
	vector<string>::iterator iterB = b.begin();
	for (; iterB != b.end(); iterB++) {
		vector<string>::iterator iterRet = ret.begin();
		while (*iterRet != *iterB && iterRet != ret.end()) {
			iterRet++;
		}
		if (iterRet == ret.end()) {
			ret.push_back(*iterB);
		}
	}

	return ret;
}

//打印时间数组
void showTimeArray(std::list<time_t> lst) {
	cout << "TimeArray:  ";
	std::list<time_t>::iterator iter;
	for (iter = lst.begin(); iter != lst.end(); iter++) {
		cout << *iter << "  ";
	}
	cout << endl;
}

//插入一个时间值
void insertTimeArray(std::list<time_t> &lst, time_t t) {
	lst.push_back(t);
}

//重置时间数组，将一个周期前的访问记录删除
void resetTimeArray(std::list<time_t> &lst, time_t now, long period4Freq) {
	if (0 == lst.size()) {
		return;
	}
	std::list<time_t>::iterator iter;
	for (iter = lst.begin(); iter != lst.end();) {
		if ((now - *iter) > period4Freq) {
			std::list<time_t>::iterator iterTmp = iter;
			iter++;
			lst.erase(iterTmp);
		} else {
			break;
		}
	}
}

//找出vector<time_t>中一个周期之内的访问频率
long getFreqPerPeriod(std::list<time_t> &lst, time_t now, long period4Freq) {
	if (0 == lst.size()) {
		return 0L;
	}
	resetTimeArray(lst, now, period4Freq);
//	long count = 0L;
//	std::list<time_t>::iterator iter;
//	for (iter = lst.begin(); iter != lst.end(); iter++) {
//		if ((now - *iter) <= period4Freq) {
//			count++;
//		}
//	}
//	return count;

	return lst.size();
}

