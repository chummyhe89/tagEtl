#pragma once

#include <string>
using namespace std;

// Common status
#ifdef WIN32
	#include <Windows.h>
#else
	#include "winerror.h"
#endif

// Common type
#ifdef WIN32
	#include <Windows.h>
#else
	typedef unsigned int UINT;
#endif

// error
#define ERR_INVALID_URL		0x90000100

// status
#define STA_CANT_FIND		0x90010100

// Constant define
#define ASCII_SPACE			32
#define ASCII_ENTER			13
#define ASCII_TAB			9
#define ASCII_WRAP			10

#define UTF8_SPACE			"%20"
#define UTF8_SPACE2			'+'

#define DIREC_PRE			0
#define DIREC_BAC			1

class CURLAnalyzer
{
public:
	CURLAnalyzer(void);
	~CURLAnalyzer(void);

	UINT LoadUrl(const string strUrl);
	void ReleaseUrl();

	string GetProtocol();
	string GetDomain();
	string GetPort();
	string GetPath();
	UINT SelectParam(const string strName, string& strValue);

private:
	UINT AnalyzeUrl(const string strUrl);
	UINT do_SelectParam(const string strName, string& strValue);

	UINT UrlToUtf8(const string strUrl, string& strUtf8, int size, bool bUpperToLower = true);
	void FilterInvalidChar(const string str, size_t& pos, const UINT direc);

private:
	string m_strProtocol;			// transfer protocol
	string m_strDomain;				// domain information
	string m_strPort;				// listening port
	string m_strPath;				// request path
	string m_strParam;				// request parameter
};

