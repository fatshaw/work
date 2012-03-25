#include "Message.h"
#include "Folder.h"

Message::Message(string content):content(content){
	
}

void Message::save(Folder * f){
	folders.insert(f);
}

void Message::remove(Folder * f){
	folders.erase(f);
}

void Message::addToFolders()
{
	for(set<Folder*>::iterator it = folders.begin();it != folders.end();++it){
		(*it)->save(this);
	}
}

void Message::removeFromFolders()
{
	for(set<Folder*>::iterator it = folders.begin();it!=folders.end();++it){
		(*it)->remove(this);
	}
}

Message::Message(const Message &m):content(m.content),folders(m.folders){
	addToFolders();
}

Message Message::operator=(const Message & m){
	if(this != &m){
		removeFromFolders();
	}

	content = m.content;
	folders = m.folders;

	addToFolders();

	return *this;
}

Message::~Message(){
	removeFromFolders();
}