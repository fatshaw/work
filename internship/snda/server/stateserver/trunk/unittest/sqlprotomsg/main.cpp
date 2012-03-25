#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>
#include "sqlsinglequery.pb.h"
#include "sqlsingleresult.pb.h"

using namespace std;
using namespace ::google::protobuf;


void SqlSingleQueryTest()
{
	int i = 10;
	char sz[1024];
	while(--i)
	{
		//pack
		ProtoMessage::SqlSingleQuery *pSSQ = new ProtoMessage::SqlSingleQuery;
		pSSQ->set_m_isn((uint32)i);
		char szTemp[124];
		sprintf(szTemp,"select * from tablename = %d", i);
		pSSQ->set_m_strsql(szTemp,strlen(szTemp));	
		int iSize = pSSQ->ByteSize();
		pSSQ->SerializeToArray(sz,iSize);
		delete pSSQ;

		//unpack
		ProtoMessage::SqlSingleQuery *pSSQDest = new ProtoMessage::SqlSingleQuery;
		pSSQDest->ParsePartialFromArray(sz,iSize);
		cout <<"m_isn = " <<  pSSQDest->m_isn() << endl;
		cout <<"m_strSql = " << pSSQDest->m_strsql() << endl << endl;
		delete pSSQDest;
	}
}

void SqlSingleResultTest()
{
	int i = 10;
	char sz[1024];
	while(--i)
	{
		//pack
		ProtoMessage::SqlSingleResult *pSSR = new ProtoMessage::SqlSingleResult;
		pSSR->set_m_isn((uint32)i);
		pSSR->set_m_istatus(i%5);
		int j = i;
		while(j--)
		{
			char szTemp[124];
			sprintf(szTemp,"sn = %d, status=%d, count = %d", i, i%5, j);
			pSSR->add_m_strresult(szTemp, strlen(szTemp));	
		}

		int iSize = pSSR->ByteSize();
		pSSR->SerializeToArray(sz,iSize);
		delete pSSR;

		//unpack
		ProtoMessage::SqlSingleResult *pSSRDest = new ProtoMessage::SqlSingleResult;
		pSSRDest->ParsePartialFromArray(sz,iSize);
		cout <<"m_isn = " <<  pSSRDest->m_isn() << endl;
		cout <<"m_istatus = " << pSSRDest->m_istatus() << endl ;
		for(int k = 0; k < pSSRDest->m_strresult_size(); ++k)
		{
			cout << pSSRDest->m_strresult(k) << endl;
		}
		cout << endl;
		delete pSSRDest;
	}
}

int main()
{
	cout << "test SqlSingleQuery start" << endl;
	SqlSingleQueryTest();
	cout << "test SqlSingleQuery end" << endl;

	cout << "test SqlSingleResult start" << endl;
	SqlSingleResultTest();
	cout << "test SqlSingleResult end" << endl;
	return 0;
}
