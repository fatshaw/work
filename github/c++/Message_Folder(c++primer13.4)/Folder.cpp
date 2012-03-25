#include "Message.h"
#include "Folder.h"

Folder::Folder(string folderName):folderName(folderName){

}

Folder::Folder(const Folder &f):folderName(f.folderName),messages(f.messages){
	for(set<Message*>::iterator it = messages.begin();it!=messages.end();++it){
		(*it)->save(this);
	}
}

Folder Folder::operator =(const Folder& f){
	if(this != &f){
		removeFromMessage();		
	}

	folderName = f.folderName;
	messages= f.messages;

	addMessage();
	return *this;
}


void Folder::removeFromMessage()
{
	for(set<Message*>::iterator it = messages.begin();it!=messages.end();++it){
		(*it)->remove(this);
	}
	messages.clear();
}

void Folder::addMessage()
{
	for(set<Message*>::iterator it = messages.begin();it!=messages.end();++it){
		(*it)->save(this);
	}
}

void Folder::save(Message * m){
	messages.insert(m);
	m->save(this);
}

void Folder::remove(Message * m){
	messages.erase(m);
	m->remove(this);
}

Folder::~Folder(){
	removeFromMessage();
}