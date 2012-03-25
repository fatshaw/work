#ifndef FOLDER
#define FOLDER

#include <set>
#include <string>

using namespace std;

class Message;

class Folder{
private:
	set<Message * >messages;
	string folderName;

	void removeFromMessage();
	void addMessage();
public:
	Folder(string folderName);
	Folder(const Folder & f);
	Folder operator=(const Folder & f);
	~Folder();
	void save(Message * m);

	void remove(Message * m);

};

#endif