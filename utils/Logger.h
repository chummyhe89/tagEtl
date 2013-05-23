#pragma once

#ifdef WIN32
	#include <Windows.h>
#else
	#include "winerror.h"

	typedef unsigned int	UINT;
#endif

#include <string>
using namespace std;

#define ERROR_LOG_NOT_OPEN		0x90050901
#define LOCAL_TIME_SIZE			32

class CLogger
{
public:
	CLogger();
	~CLogger();

	UINT OpenLog(string strLogPath);
	UINT WriteLog(string strLogMsg);
	void CloseLog();

private:
	bool m_bAlreadyOpenLog;
	FILE* m_pfpLogfile;
};
