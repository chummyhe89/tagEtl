#pragma once

#include <libmemcached/memcached.h>
#include <time.h>
#include <pthread.h>
#ifdef WIN32
	#include <Windows.h>
#else
	#include "winerror.h"
#endif

#include <string>
using namespace std;

#define MEM_SERV_ADDR_SIZE		32
#define MEMCACHED_INVALID_ARGUMENTS 	38

typedef memcached_st*		Memcache_Handler;
typedef memcached_return	Memcache_Res; 

class CMemcache
{
public:
	CMemcache();
	~CMemcache();

	Memcache_Res Memcac_Create(string strIp, unsigned int uPort);
	Memcache_Res Memcac_Free();

	Memcache_Res Memcac_Get(string strKey, char** ppValue, size_t& res_size);
	Memcache_Res Memcac_Set(string strKey, const char* pValue, size_t val_size, time_t expiration);

	Memcache_Res Memcac_Append(string strKey, const char* pValue, size_t val_size, time_t expiration);
	Memcache_Res Memcac_Prepend(string strKey, const char* pValue, size_t val_size, time_t expiration);

	Memcache_Res Memcac_Delete(string strKey);

private:
	Memcache_Handler m_hMc;
	pthread_mutex_t m_hmc_mutex;
};
