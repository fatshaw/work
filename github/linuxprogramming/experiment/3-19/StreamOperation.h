#ifndef STREAM_OPERATION
#define STREAM_OPERATION

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <string>
using namespace std;

class StreamOperation
{
	typedef string::size_type pos;
public:
		StreamOperation(){}
		~StreamOperation(){}

		int readStream(ifstream& in,const string & fileName, vector<string> & lines,pos lineCnt);

		void writeStream(ofstream & out, const string & fileName, const vector<string> & lines, pos lineCnt);

		void writeStream(ofstream & out,const vector<double> & data){
		for(vector<double>::const_iterator it = data.begin() ;it != data.end() ; ++it){
				out<<setprecision(2)<<setw(7)<<*it<<",";
			}
			out<<endl;
			out.flush();
		}

		void writeStream(ofstream & out ,const double data){
			out<<data<<endl;
			out.flush();
		}

		void writeStream(ofstream & out, const string & s){
			out<<s;
			out<<endl;
			out.flush();
		}
};

#endif

