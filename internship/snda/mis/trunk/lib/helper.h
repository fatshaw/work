#ifndef _MIS_HELPER_H_
#define _MIS_HELPER_H_

#include <map>
#include <string>
#include <vector>

#define MAX_LINE 2048

using namespace std;

typedef map<string, string> MISConfig;

/**
 * channel
 **/
class MISHelper {
	private:
		MISHelper() {};

	public:
		static void trim(string& txt);
		static bool readConfig(const char* file, MISConfig& result);

		static void toVector(const string& value, vector<string> result);
		static void erase(string& text, const char* value);
		static void erase(string& text, char value);
};

#endif
