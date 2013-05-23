#include "Memcache.h"
#include <stdio.h>


CMemcache:: CMemcache()
	: m_hMc(NULL)
{
	int iRet	= 0;

	// initialize rwlock
	if((iRet = pthread_mutex_init(&m_hmc_mutex, NULL)))
		throw iRet;
}

CMemcache:: ~CMemcache()
{
	Memcac_Free();

	pthread_mutex_destroy(&m_hmc_mutex);
}


Memcache_Res CMemcache:: Memcac_Create(string strIp, unsigned int uPort)
{
	Memcache_Res mRes 		= MEMCACHED_SUCCESS;
	Memcache_Handler htMc		= NULL;
	memcached_server_st* pServ	= NULL;
	char pMem_Server_Addr[MEM_SERV_ADDR_SIZE]	= {0};

	if(strIp.empty() || 0 == uPort)
		return  static_cast<Memcache_Res>(MEMCACHED_INVALID_ARGUMENTS);

	// create memcache server address
	sprintf(pMem_Server_Addr, "%s:%d", strIp.c_str(), uPort);
	
	// create memcache
	htMc = memcached_create(NULL);

	pServ = memcached_servers_parse(pMem_Server_Addr);
	if((mRes = memcached_server_push(htMc, pServ)) != MEMCACHED_SUCCESS)
		return mRes;
	memcached_server_list_free(pServ);

	if((mRes = memcached_behavior_set(htMc, MEMCACHED_BEHAVIOR_BINARY_PROTOCOL, 0)) != MEMCACHED_SUCCESS)
		return mRes;

	// return
	if(NULL != m_hMc)
		Memcac_Free();

	pthread_mutex_lock(&m_hmc_mutex);
	m_hMc = htMc;
	pthread_mutex_unlock(&m_hmc_mutex);

	return MEMCACHED_SUCCESS;
}

Memcache_Res CMemcache:: Memcac_Free()
{
	Memcache_Res		mRes = MEMCACHED_SUCCESS;

	if(NULL == m_hMc)
		return static_cast<Memcache_Res>(MEMCACHED_SERVER_ERROR);

	// lock and free
	pthread_mutex_lock(&m_hmc_mutex);
	mRes = memcached_flush(m_hMc, 0);
	m_hMc = NULL;
	pthread_mutex_unlock(&m_hmc_mutex);

	return mRes;
}


Memcache_Res CMemcache:: Memcac_Get(string strKey, char** ppValue, size_t& res_size)
{
	Memcache_Res mRes 	= MEMCACHED_SUCCESS;
	size_t rest_size 	= 0;
	char* ptVal		= NULL;
	uint32_t uFlag		= 0;

	if(strKey.empty() || NULL == ppValue)
		return static_cast<Memcache_Res>(MEMCACHED_INVALID_ARGUMENTS);

	// lock and get
	pthread_mutex_lock(&m_hmc_mutex);
	if(NULL == m_hMc)
	{
		pthread_mutex_unlock(&m_hmc_mutex);
		return MEMCACHED_SERVER_ERROR;
	}
	
	ptVal = memcached_get(m_hMc, strKey.c_str(), strKey.length(), &rest_size, &uFlag, &mRes);
	pthread_mutex_unlock(&m_hmc_mutex);

	// return
	*ppValue = ptVal;
	res_size = rest_size;
	return mRes;
}

Memcache_Res CMemcache:: Memcac_Set(string strKey, const char* pValue, size_t val_size, time_t expiration)
{
	Memcache_Res mRes 	= MEMCACHED_SUCCESS;

	if(strKey.empty() || NULL == pValue)
		return static_cast<Memcache_Res>(MEMCACHED_INVALID_ARGUMENTS);
	
	// lock and set
	pthread_mutex_lock(&m_hmc_mutex);
	if(NULL == m_hMc)
	{
		pthread_mutex_unlock(&m_hmc_mutex);
		return MEMCACHED_SERVER_ERROR;	
	}

	mRes = memcached_set(m_hMc, strKey.c_str(), strKey.length(), pValue, val_size, expiration, 0);
	pthread_mutex_unlock(&m_hmc_mutex);

	return mRes;
}


Memcache_Res CMemcache:: Memcac_Append(string strKey, const char* pValue, size_t val_size, time_t expiration)
{
	Memcache_Res mRes 	= MEMCACHED_SUCCESS;

	if(strKey.empty() || NULL == pValue)
		return static_cast<Memcache_Res>(MEMCACHED_INVALID_ARGUMENTS);

	// lock and append
	pthread_mutex_lock(&m_hmc_mutex);
	if(NULL == m_hMc)
	{
		pthread_mutex_unlock(&m_hmc_mutex);
		return MEMCACHED_SERVER_ERROR;
	}
	
	mRes = memcached_append(m_hMc, strKey.c_str(), strKey.length(), pValue, val_size, expiration, 0);
	if(MEMCACHED_NOTSTORED == mRes)
		mRes = memcached_add(m_hMc, strKey.c_str(), strKey.length(), pValue, val_size, expiration, 0);
	pthread_mutex_unlock(&m_hmc_mutex);

	return mRes;
}

Memcache_Res CMemcache:: Memcac_Prepend(string strKey, const char* pValue, size_t val_size, time_t expiration)
{
	Memcache_Res mRes 	= MEMCACHED_SUCCESS;

	if(strKey.empty() || NULL == pValue)
		return static_cast<Memcache_Res>(MEMCACHED_INVALID_ARGUMENTS);

	// lock and prepend
	pthread_mutex_lock(&m_hmc_mutex);
	if(NULL == m_hMc)
		return MEMCACHED_SERVER_ERROR;
	
	mRes = memcached_prepend(m_hMc, strKey.c_str(), strKey.length(), pValue, val_size, expiration, 0);
	if(MEMCACHED_NOTSTORED == mRes)
		mRes = memcached_add(m_hMc, strKey.c_str(), strKey.length(), pValue, val_size, expiration, 0);
	pthread_mutex_unlock(&m_hmc_mutex);

	return mRes;
}


Memcache_Res CMemcache:: Memcac_Delete(string strKey)
{
	Memcache_Res mRes 	= MEMCACHED_SUCCESS;

	if(strKey.empty())
		return static_cast<Memcache_Res>(MEMCACHED_INVALID_ARGUMENTS);

	// lock and delete
	pthread_mutex_lock(&m_hmc_mutex);
	mRes = memcached_delete(m_hMc, strKey.c_str(), strKey.length(), 0);
	pthread_mutex_unlock(&m_hmc_mutex);

	return mRes;
}
