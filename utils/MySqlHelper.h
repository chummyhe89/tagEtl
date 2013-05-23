#pragma once

#include <pthread.h>

#include <string>
using namespace std;

#ifdef WIN32
	#include <Windows.h>
	#include "mysql/include/mysql.h"

	#pragma comment(lib, "libmysql.lib")
#else
	#include <mysql.h>
	#include "winerror.h"
	typedef unsigned int	UINT;
#endif

// error info
#define ERROR_MYSQL_INIT		0x91000100
#define ERROR_MYSQL_STORE_RES		0x91000201
#define ERROR_MYSQL_FETCH_FAILED	0x91000301
#define ERROR_MYSQL_SET_ENCODE		0x91000401
#define	ERROR_DB_ALREADY_CONNECTED	0x91010001

#define DEFAULT_ENCODE		"utf8"

class CMySqlHelper
{
public:
	CMySqlHelper();
	~CMySqlHelper();

	UINT SqlConn(string strHost, UINT port, string strDBName, string strUserName, string strPasswd);
	void SqlClose();
	
	int ExecuteQuery(string strSql, MYSQL_RES** ppQueRes);
	UINT ExecuteNonQuery(string strSql);

	UINT FetchOneRow(MYSQL_RES* pQueRes, char*** pppResRow);
	bool FreeMySqlRes(MYSQL_RES* pQueRes);

	UINT SetEncode(string strEncode);
	bool IsConnected();

public:	
	void mysql_escape_character(string& str_sql);

private:
	int Execute(string strSql, bool bNeedlock = true);

private:
	MYSQL* 		m_pSqlConn;
	int 		m_numRef;
	pthread_mutex_t m_sqlConn_mutex;

	string 		m_strSqlConn;
};
