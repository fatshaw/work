#ifndef MONGO_H
#define MONGO_H
#include <dbclient.h>
#include <stdio.h>
#include <pthread.h>
using namespace mongo;

class MongoClient
{
	public:
		typedef vector<BSONObj> BSONVector;
		MongoClient(){conn = NULL;pthread_mutex_init(&mutex,NULL);}
		int connect(const char * hostname){
			this->hostname = hostname;
			if(conn != NULL){
				delete conn;
				conn = NULL;
			}
			conn = new DBClientConnection;
			if(conn == NULL){
				return -1;
			}
			if(conn->connect(this->hostname,this->errmsg) == false)
			{
				return -1;
			}
			return 0;
		}

		~MongoClient(){if(conn) delete conn;pthread_mutex_destroy(&mutex);}
		
		int removeAll(string & dbName);
		int insert(BSONObj obj,string & dbName);
		int update(BSONObj oldObj, BSONObj  newObj,string & dbName,bool isUpsert = false );
		int remove(BSONObj obj, string & dbName);
		int query(BSONObj obj, BSONVector&resul,string & dbNamet,char * orderBy=NULL);
		int findOne(BSONObj obj,string & dbName,BSONObj * result = NULL);
		string & getErrorMsg(){return errmsg;}
	private:
		string hostname;
		DBClientConnection * conn;
		string errmsg;
		pthread_mutex_t mutex;
};
#endif

