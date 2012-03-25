#ifndef MESSAGE
#define MESSAGE

#include <set>
#include <string>
using namespace std;
class Folder;

class Message
{
public:
	Message(string content);
	Message(const Message & m);
	Message operator=(const Message & m);
	void save(Folder * f);
	void remove(Folder * f);

	~Message();
private:
	set<Folder*>folders;
	string content;

	void addToFolders();
	void removeFromFolders();
};

#endif