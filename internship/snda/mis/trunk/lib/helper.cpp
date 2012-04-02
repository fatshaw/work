#include "helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void MISHelper::trim(string& str) {
  string::size_type pos = str.find_last_not_of(" \t\r\n");
  if (pos != string::npos) {
    str.erase(pos + 1);
    pos = str.find_first_not_of(" \t\r\n");
    if (pos != string::npos) str.erase(0, pos);
  }
  else str.erase(str.begin(), str.end());
}

bool MISHelper::readConfig(const char* file, MISConfig& result) {
	FILE *fp;
	char line[MAX_LINE];
	fp = fopen(file, "r");
	if (!fp) return false;
	while (fgets(line, MAX_LINE - 1, fp)) {
		if (line[0] == '#' || line[0] == '\0') continue;
		char* pos = strchr(line, '=');
		if (pos >= line) {
			*pos = '\0';
			string name = line;
			trim(name);
			string value = pos + 1;
			trim(value);
			result[name] = value;
		}
	}
	fclose(fp);
	return true;
}

void MISHelper::toVector(const string& value, vector<string> result) {
	size_t start = 0;
	size_t end = value.find(',');
	while (end != string::npos) {
		if (end > start) {
			string sub(value, start, end-start);
			trim(sub);
			if (!sub.empty()) {
				result.push_back(sub);
			}
		}
		start = end + 1;
		if (start == value.length()) break;
		end = value.find(',', start);
	}
	if (start < value.length() - 1) {
		string sub(value, start);
		trim(sub);
		if (!sub.empty()) {
			result.push_back(sub);
		}
	}
}


void MISHelper::erase(string& text, const char* value) {
	int size = strlen(value);
	if (size == 0) return;

	size_t pos = text.find(value);
	while (pos != string::npos) {
		text.erase(pos, size);
		pos = text.find(value, pos);
	}
}


void MISHelper::erase(string& text, char value) {
	size_t pos = text.find(value);
	while (pos != string::npos) {
		text.erase(pos, sizeof(char));
		pos = text.find(value, pos);
	}
}
