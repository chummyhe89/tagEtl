#pragma once
#include "comdef.h"
#include "MySqlHelper.h"
#include <pthread.h>
#include <string>

using namespace std;
typedef struct _connParam
{
	string	strHost;
	int 	port;
	string  strDBName;
	string 	strUserName;
	string 	strPasswd;
}ConnParam;

class CLib
{
public:
	CLib();
	~CLib();
	UINT ConnectToLib(string strDB,bool lock  = false);
	void DisConnFromLib(bool lock = false);

private:
	UINT GetConnParam(ConnParam& cp,string strDB);


protected:
	CMySqlHelper	m_mySqlHelper;
	pthread_mutex_t m_conn_mutex;
};
