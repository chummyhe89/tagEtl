#include "lib.h"
#include "Profile.h"

CLib::CLib()
{
	int ret	= 0;
	if((ret = pthread_mutex_init(&m_conn_mutex,NULL)))
		throw ret;
}

CLib::~CLib()
{
	DisConnFromLib();
	pthread_mutex_destroy(&m_conn_mutex);
}

UINT CLib::ConnectToLib(string strDB,bool lock)
{
	UINT uRet 	= S_OK;
	ConnParam conpa;
	if((uRet = GetConnParam(conpa,strDB)) != S_OK)
		return uRet;
	if(lock)
		pthread_mutex_lock(&m_conn_mutex);
	uRet = m_mySqlHelper.SqlConn(conpa.strHost,conpa.port,conpa.strDBName,conpa.strUserName,conpa.strPasswd);
	return uRet;
}

void CLib::DisConnFromLib(bool lock)
{
	if(m_mySqlHelper.IsConnected())
		m_mySqlHelper.SqlClose();
	if(lock)
		pthread_mutex_unlock(&m_conn_mutex);
}

UINT CLib::GetConnParam(ConnParam& cp,string strDB)
{
	UINT uRet = S_OK;
	CProfile prof;
	
	if((uRet = prof.LoadProfile(DB_CONFIG_POS)) != S_OK)
		goto e_exist;
	if((uRet = prof.GetProfileStringW(strDB,HOST,cp.strHost)) != S_OK)
		goto e_exist;
	if((uRet = prof.GetProfileIntW(strDB,PORT,cp.port)) != S_OK)
		goto e_exist;
	if((uRet = prof.GetProfileStringW(strDB,DB_NAME,cp.strDBName)) != S_OK)
		goto e_exist;
	if((uRet = prof.GetProfileStringW(strDB,USER_NAME,cp.strUserName)) != S_OK)
		goto e_exist;
	if((uRet = prof.GetProfileStringW(strDB,PASSWORD,cp.strPasswd)) != S_OK)
		goto e_exist;
e_exist:
	prof.ReleaseProfile();
		return uRet;
}
