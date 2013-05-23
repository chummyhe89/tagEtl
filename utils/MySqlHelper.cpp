#include "MySqlHelper.h"

#include <stdio.h>

CMySqlHelper:: CMySqlHelper()
	: m_pSqlConn(NULL),
	  m_numRef(0)
{
	int iRet	= 0;

	// initialize rwlock
	if((iRet = pthread_mutex_init(&m_sqlConn_mutex, NULL)))
		throw iRet;
}

CMySqlHelper:: ~CMySqlHelper()
{
	pthread_mutex_destroy(&m_sqlConn_mutex);
	
	mysql_close(m_pSqlConn);
	delete m_pSqlConn;
}


UINT CMySqlHelper:: SqlConn(string strHost, UINT port, string strDBName, string strUserName, string strPasswd)
{
	UINT  uRet		= S_OK;
	char  pPort[32]		= {'\0'};
	string strSqlConn;

	// construct connection string
	sprintf(pPort, "%u",port);
	strSqlConn = strHost + string(pPort) + strDBName + strUserName + strPasswd;

	// check connection status
	pthread_mutex_lock(&m_sqlConn_mutex);
	if(NULL != m_pSqlConn)
	{
		if(!m_strSqlConn.compare(strSqlConn))
		{
			m_numRef ++;
			pthread_mutex_unlock(&m_sqlConn_mutex);

			return S_OK;
		}
		
		pthread_mutex_unlock(&m_sqlConn_mutex);
		return ERROR_DB_ALREADY_CONNECTED;			
	}

	// initialization
	m_pSqlConn = new MYSQL;
	if(!mysql_init(m_pSqlConn))
	{
		pthread_mutex_unlock(&m_sqlConn_mutex);

		uRet = ERROR_MYSQL_INIT;
		goto e_exit;
	}

	// connection and choose database
	if(!(m_pSqlConn = mysql_real_connect(m_pSqlConn, strHost.c_str(), strUserName.c_str(), strPasswd.c_str(), NULL, port, NULL, 0)))
	{
		pthread_mutex_unlock(&m_sqlConn_mutex);

		uRet = ERROR_CONNECTION_REFUSED;
		goto e_exit;
	}
	if(mysql_select_db(m_pSqlConn, strDBName.c_str()))
	{
		pthread_mutex_unlock(&m_sqlConn_mutex);

		uRet = ERROR_DATABASE_FAILURE;
		goto e_exit;
	}
	pthread_mutex_unlock(&m_sqlConn_mutex);

	// encode format
	uRet = SetEncode(DEFAULT_ENCODE);
	if(S_OK == uRet)
	{
		pthread_mutex_lock(&m_sqlConn_mutex);		
		m_strSqlConn = strSqlConn;
		m_numRef = 1;
		pthread_mutex_unlock(&m_sqlConn_mutex);
	}

e_exit:
	if(S_OK != uRet)
		SqlClose();

	return uRet;
}

void CMySqlHelper:: SqlClose()
{
	pthread_mutex_lock(&m_sqlConn_mutex);
	if(NULL == m_pSqlConn)
	{
		pthread_mutex_unlock(&m_sqlConn_mutex);
		return;
	}

	// close
	if(m_numRef > 0)
		m_numRef --;
	
	if(0 == m_numRef)
	{
		mysql_close(m_pSqlConn);
		delete m_pSqlConn;
	
		m_pSqlConn = NULL;
	}

	pthread_mutex_unlock(&m_sqlConn_mutex);
}

	
int CMySqlHelper:: ExecuteQuery(string strSql, MYSQL_RES** ppQueRes)
{
	int iRet		= S_OK;
	MYSQL_RES* ptQueRes	= NULL;

	// query
	pthread_mutex_lock(&m_sqlConn_mutex);
	if((iRet = Execute(strSql, false)) != S_OK)
	{
		pthread_mutex_unlock(&m_sqlConn_mutex);
		return iRet;
	}

	if(!(ptQueRes = mysql_store_result(m_pSqlConn)))
	{
		pthread_mutex_unlock(&m_sqlConn_mutex);
		return ERROR_MYSQL_STORE_RES;
	}
	pthread_mutex_unlock(&m_sqlConn_mutex);

	// return
	*ppQueRes = ptQueRes;
	return S_OK;
}

UINT CMySqlHelper:: ExecuteNonQuery(string strSql)
{
	return Execute(strSql);
}


UINT CMySqlHelper:: FetchOneRow(MYSQL_RES* pQueRes, char*** pppResRow)
{
	UINT uRet	= S_OK;
	MYSQL_ROW row;

	// parameter checking
	if(NULL == pQueRes)
		return ERROR_INVALID_PARAMETER;

	// fetch rows
	if(!(row = mysql_fetch_row(pQueRes)))
	{
		uRet = ERROR_MYSQL_FETCH_FAILED;
		goto e_exit;
	}
	
	// return rows
	*pppResRow = row;

e_exit:
	if(S_OK != uRet)
		FreeMySqlRes(pQueRes);
	return uRet;
}

bool CMySqlHelper:: FreeMySqlRes(MYSQL_RES* pQueRes)
{
	// parameter checking
	if(NULL == pQueRes)
		return false;

	// free
	mysql_free_result(pQueRes);

	return true;
}


UINT CMySqlHelper:: SetEncode(string strEncode)
{
	string strSqlEncode;

	// parameter checking
	if(strEncode.empty())
		return ERROR_INVALID_PARAMETER;

	// set encode
	strSqlEncode = "SET NAMES "+strEncode;
	
	pthread_mutex_lock(&m_sqlConn_mutex);
	// connection checking
	if(NULL == m_pSqlConn)
	{
		pthread_mutex_unlock(&m_sqlConn_mutex);
		return ERROR_NOT_CONNECTED;
	}

	if(mysql_query(m_pSqlConn, strSqlEncode.c_str()))
	{
		pthread_mutex_unlock(&m_sqlConn_mutex);
		return ERROR_MYSQL_SET_ENCODE;
	}
	pthread_mutex_unlock(&m_sqlConn_mutex);

	return S_OK;
}

bool CMySqlHelper:: IsConnected()
{
	return (NULL != m_pSqlConn && m_numRef > 0);
}


int CMySqlHelper:: Execute(string strSql, bool bNeedlock)
{
	int iRet	= S_OK;

	// parameter checking
	if(strSql.empty())
		return ERROR_INVALID_PARAMETER;

	if(bNeedlock) pthread_mutex_lock(&m_sqlConn_mutex);
	// connection checking
	if(NULL == m_pSqlConn)
	{
		if(bNeedlock) pthread_mutex_unlock(&m_sqlConn_mutex);
		return ERROR_NOT_CONNECTED;
	}

	// query
	if((iRet = mysql_real_query(m_pSqlConn, strSql.c_str(), strSql.length())))
	{
		if(bNeedlock) pthread_mutex_unlock(&m_sqlConn_mutex);
		return iRet;
	}
	if(bNeedlock) pthread_mutex_unlock(&m_sqlConn_mutex);

	return S_OK;
}

void CMySqlHelper:: mysql_escape_character(string& str_sql)
{
	size_t	pos_escape_charater		= 0;
	
	while((pos_escape_charater = str_sql.find("\'", pos_escape_charater)) != string::npos)
	{
		str_sql.replace(pos_escape_charater, 1, "\\\'");
		pos_escape_charater += 2;
	}

	pos_escape_charater = 0;
	while((pos_escape_charater = str_sql.find("\"", pos_escape_charater)) != string::npos)
	{
		str_sql.replace(pos_escape_charater, 1, "\\\"");
		pos_escape_charater += 2;
	}
}
