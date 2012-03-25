/*
 * MyTime.h
 * 访问控制类的一些工具方法
 * Created on: 2010-8-25
 * Author: leo
 */

#ifndef ACCESSCONTROLERUTILS_H_
#define ACCESSCONTROLERUTILS_H_

#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>

using namespace std;

//去除字符串左边空字符
string lTrim(const string& str);

//去除字符串右边空字符
string rTrim(const string& str);

//去除字符串左右两边空字符
string trim(const string& str);

//按照分隔符分隔字符串
vector<string> splitStrByDelim(string s, string delim);

//合并两个vector，并返回合并结果
vector<string> mergeVectors(vector<string> a, vector<string> b);

//插入一个时间值
void insertTimeArray(std::list<time_t> &lst, time_t t);

//重置时间数组，将一个周期前的访问记录删除
void resetTimeArray(std::list<time_t> &lst, time_t now);

//找出vector<time_t>中一个周期内的访问频率
long getFreqPerPeriod(std::list<time_t> &lst, time_t now, long period4Freq);

//打印时间数组
void showTimeArray(std::list<time_t> lst);

#endif /* MYTIME_H_ */
