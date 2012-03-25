#include "memcachepool.h"

Memcachepool::Memcachepool()
{
	iSize_ = 0;
	iPos = 0;
	pthread_mutex_init(&mutex,NULL);
}

Memcachepool::Memcachepool(int size)
{
	iSize_ = 0;
	iPos = 0;
	pthread_mutex_init(&mutex,NULL);
	SetSize(size);
}

Memcachepool::~Memcachepool()
{
	/*for(vector<memcached_st*>::iterator it = m_pstList.begin();it != m_pstList.end();++it)
	{
		MISMemcachedST::close(*it);
	}*/
	for(deque<memcached_st*>::iterator it = m_pstQueue.begin();it != m_pstQueue.end();++it)
	{
		MISMemcachedST::close(*it);
	}
	pthread_mutex_destroy(&mutex);
}

int  Memcachepool::Setpst(memcached_st* p_st)
{
	if(iPos > iSize_)
	{
		return -1;
	}
	else
	{
//		m_pstList.push_back(p_st);
		m_pstQueue.push_back(p_st);
	}
	return 0;
}

int Memcachepool::Releasepst(memcached_st* p_st)
{
	int ret = 0;
	pthread_mutex_lock(&mutex);
	/*if(iPos == 0)
	{
		ret =  -1;
	}
	else
	{
		ret = 0;
		m_pstList[--iPos] = p_st;
		AC_DEBUG("Releasepst num = %d\n",iPos);
	}*/
	m_pstQueue.push_back(p_st);
	pthread_mutex_unlock(&mutex);
	return ret;
}

memcached_st*  Memcachepool::Getpst()
{
	memcached_st* p_st;
	pthread_mutex_lock(&mutex);
	/*if(iPos >= iSize_)
	{
		
		p_st = NULL;
	}
	else
	{
		AC_DEBUG("get pst num = %d\n",iPos);
		p_st = m_pstList[iPos++];
	}*/
	if(m_pstQueue.size() == 0)
	{
		AC_DEBUG("get p_st == NULL");
		p_st = NULL;
	}
	else
	{
		p_st = m_pstQueue.front();
		m_pstQueue.pop_front();
	}
	pthread_mutex_unlock(&mutex);
	return p_st;
}

int Memcachepool::SetSize(int iSize)
{
	iSize_ = iSize;
	return 0;
}
