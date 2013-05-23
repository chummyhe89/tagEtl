#include "Logger.h"

#include <stdio.h>
#include <time.h>

CLogger:: CLogger()
	:m_bAlreadyOpenLog(false),
	 m_pfpLogfile(NULL)
{
}

CLogger:: ~CLogger()
{
}


UINT CLogger:: OpenLog(string strLogPath)
{
	// parameter checking
	if(strLogPath.empty())
		return ERROR_INVALID_PARAMETER;

	if(m_bAlreadyOpenLog)
		return S_OK;
	
	// open file
	if((m_pfpLogfile = fopen(strLogPath.c_str(), "a+")) == NULL)
		return ERROR_OPEN_FILES;

	m_bAlreadyOpenLog = true;
	return S_OK;
}

UINT CLogger:: WriteLog(string strLogMsg)
{
	time_t tCurrTime		= 0;
	char pCurrTime[LOCAL_TIME_SIZE] = {'\0'};
	struct tm tmCurrTime;
	string strtLogMsg;

	// parameter checking
	if(strLogMsg.empty())
		return ERROR_INVALID_PARAMETER;
	if(!m_bAlreadyOpenLog || NULL == m_pfpLogfile)
		return ERROR_LOG_NOT_OPEN;

	// get local times
	tCurrTime = time(NULL);
	localtime_r(&tCurrTime, &tmCurrTime);
	
	sprintf(pCurrTime, "[%04d-%02d-%02d %02d:%02d:%02d]:", \
		(tmCurrTime.tm_year + 1900), (tmCurrTime.tm_mon + 1), tmCurrTime.tm_mday,\
		tmCurrTime.tm_hour, tmCurrTime.tm_min, tmCurrTime.tm_sec);
	strtLogMsg = string(pCurrTime) + strLogMsg + string("\n");

	// write log
	fputs(strtLogMsg.c_str(), m_pfpLogfile);
	fflush(m_pfpLogfile);

	return S_OK;
}

void CLogger:: CloseLog()
{
	if(m_bAlreadyOpenLog && NULL != m_pfpLogfile)
		fclose(m_pfpLogfile);

	m_pfpLogfile = NULL;
	m_bAlreadyOpenLog = false;
}
