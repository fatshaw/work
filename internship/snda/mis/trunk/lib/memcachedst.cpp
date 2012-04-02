#include "memcachedst.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

/*
#ifndef __DEBUG
#define __DEBUG
#endif
*/

memcached_st* MISMemcachedST::m_pst = NULL; 

bool MISMemcachedST::set(const string& key, const string& value, time_t timeout) {
	if (!m_pst) return false;
	
#ifdef __DEBUG
	bool ret = MEMCACHED_SUCCESS == memcached_set(m_pst, key.c_str(), key.length(), value.c_str(), value.length(), timeout, (uint32_t)0);
	printf("set value:[%s=%s] result:%s\n", key.c_str(), value.c_str(), ret ? "ok" : "failed");
	return ret;
#else
	return MEMCACHED_SUCCESS == memcached_set(m_pst, key.c_str(), key.length(), value.c_str(), value.length(), timeout, (uint32_t)0);
#endif
}

bool MISMemcachedST::check(const string& key) {
	if (!m_pst) return false;
	size_t len;
	uint32_t flag;
	memcached_return rc;
	char* pvalue = memcached_get(m_pst, key.c_str(), key.length(), &len, &flag, &rc);
	bool ret = rc == MEMCACHED_SUCCESS && pvalue;
	if (pvalue) free(pvalue);
#ifdef __DEBUG
	printf("check value:[%s] result:%s\n", key.c_str(), ret ? "existed" : "not existed");
#endif
	return ret;
}

int MISMemcachedST::getTimes(const string& key, time_t timeout) {
	if (!m_pst) return false;

	time_t now = time(NULL);

	size_t len;
	uint32_t flag;
	memcached_return rc;
	int times = 0;

	char* pvalue = memcached_get(m_pst, key.c_str(), key.length(), &len, &flag, &rc);
	if (rc == MEMCACHED_SUCCESS && pvalue) {
		size_t sz = sizeof(time_t);
		for (size_t i = 0; i < len; i += sz) {
			time_t t = *(time_t*)(pvalue + i);
#ifdef __DEBUG
			printf("time:%d \n", (int)t);
#endif
			if (t + timeout >= now) ++times;
		}
	}
#ifdef __DEBUG
	printf("check value:[%s] times:%d  data len:%d\n", key.c_str(), times, len);
#endif
	if (pvalue) free(pvalue);
	return times;
}

bool MISMemcachedST::addTime(const string& key, int max_times, time_t timeout) {
	if (!m_pst) return false;
	time_t now = time(NULL);

	size_t len;
	uint32_t flag;
	memcached_return rc;
	bool ret;

	char* pvalue = memcached_get(m_pst, key.c_str(), key.length(), &len, &flag, &rc);
	if (rc == MEMCACHED_SUCCESS && pvalue) {
		size_t sz = sizeof(time_t);
		char* data = new char[len + sz];
		char* pos = data;

		size_t start = 0;

		if (len > (max_times - 1) * sz) {
			start = len - (max_times - 1) * sz;
		}

		for (size_t i = start; i < len; i += sz) {
			time_t t = *(time_t*)(pvalue + i);
			if (t + timeout >= now) {
				memcpy(pos, pvalue + i, len - i);
				pos += len - i;
				break;
			}
		}
		memcpy((char*)pos, (char*)&now, sz);
		pos += sz;
		ret = MEMCACHED_SUCCESS == memcached_set(m_pst, key.c_str(), key.length(), data, pos - data, timeout, (uint32_t)0);
		delete data;
	} else {
		ret = MEMCACHED_SUCCESS == memcached_set(m_pst, key.c_str(), key.length(), (char*)&now, sizeof(time_t), timeout, (uint32_t)0);
	}
#ifdef __DEBUG
	printf("add time for:[%s] \n", key.c_str());
#endif
	if (pvalue) free(pvalue);
	return ret;
}

bool MISMemcachedST::init(const char* servers) {
	if (servers == NULL)  return false;

	if (m_pst) {
		memcached_free(m_pst);
		m_pst = NULL;
	}

	m_pst = memcached_create(NULL);
	memcached_return rc;

	memcached_server_st *pservers = memcached_servers_parse(servers);
	if (!pservers) return false;

	//connect server
	rc = memcached_server_push(m_pst, pservers);
	memcached_server_free(pservers);
	return rc == MEMCACHED_SUCCESS;
}

void MISMemcachedST::close() {
	if (m_pst) {
		memcached_quit(m_pst);
		memcached_free(m_pst);
		m_pst = NULL;
	}
}
