#include "mongo.h"

int MongoClient::insert(BSONObj obj,string & dbName)
{
	int ret = 0;
	pthread_mutex_lock(&mutex);
	try{
		conn->insert(dbName,obj);
	}
	catch( DBException &e){
		this->errmsg = e.what();
		ret = -1;
	}
	pthread_mutex_unlock(&mutex);
	 return ret;
}


int MongoClient::remove(BSONObj obj,string & dbName)
{
	int ret = 0;
	pthread_mutex_lock(&mutex);
	try{
		conn->remove(dbName,obj);
	}
	catch( DBException &e ) {
	   	 this->errmsg = e.what();
		ret= -1;
  	}
	pthread_mutex_unlock(&mutex);
	return ret;
}

int MongoClient::removeAll(string & dbName)
{
	int ret = 0;
	pthread_mutex_lock(&mutex);
	try
	{
		conn->remove(dbName,BSONObj());
	}
	catch( DBException &e ) {	
		this->errmsg = e.what();
		ret = -1;
  	}
	pthread_mutex_unlock(&mutex);
	return ret;
}


int MongoClient::findOne(BSONObj obj,string & dbName,BSONObj * result)
{
	int ret = 0;
	pthread_mutex_lock(&mutex);
	try{
		*result = conn->findOne(dbName,obj);
		if(result->isEmpty() == true)
		{
				this->errmsg = "no data found";
				ret = 0;//no data
		}
		else
		{
				ret = 1;//has data
		}
	}
	catch( DBException &e ) {
	    	this->errmsg = e.what();
		ret = -1;//error
	 }
	 pthread_mutex_unlock(&mutex);
	 return ret;
}

int MongoClient::query(BSONObj obj,BSONVector& result,string & dbName,char * orderBy)
{
	int ret = 0;
	pthread_mutex_lock(&mutex);
	try
	{	
		Query query(obj);
		if(orderBy != NULL){
			query = query.sort(orderBy);
		}
		auto_ptr<DBClientCursor>cursor = conn->query(dbName,query);
		while(cursor->more())
		{
			result.push_back(cursor->next());
		}
	}
	catch( DBException &e ) {
		this->errmsg = e.what();
		ret = -1;
	}
	pthread_mutex_unlock(&mutex);
	return ret;
}

int MongoClient::update(BSONObj  oldObj,BSONObj  newObj,string & dbName,bool isUpsert)
{
	int ret = 0;
	pthread_mutex_lock(&mutex);
	try
	{
		BSONObj old = conn->findOne(dbName,oldObj);
		if(old.isEmpty()==true)
		{
			if(isUpsert == true)
			{
				conn->insert(dbName,newObj);	
			}
		}
		else
		{
			conn->update(dbName,old,newObj,isUpsert);			
		}
	}
	catch( DBException &e ) {
		this->errmsg = e.what();
		ret = -1;
  	}
	 pthread_mutex_unlock(&mutex);
	 return ret;
}

