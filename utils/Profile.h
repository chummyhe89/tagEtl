#pragma once

#ifdef WIN32
	#include <Windows.h>
#else
	#include "winerror.h"

	typedef unsigned int	UINT;
#endif

#include <string>
using namespace std;

// Constant define
#ifndef ASCII_CHAR_MAP
#define ASCII_CHAR_MAP
#define ASCII_SPACE			32
#define ASCII_ENTER			13
#define ASCII_TAB			9
#define ASCII_WRAP			10

#define ASCII_DQUOT 			34
#define ASCII_QUOT			39
#endif

typedef struct _item
{
	string strName;
	string strValue;

	struct _item* next;
}Item;

typedef struct _section
{
	string strName;
	Item* pItemList;

	struct _section* next;
}Section;

string tirm(string& str);
string QuotTirm(string& str);

class CProfile
{
public:
	CProfile(void);
	~CProfile(void);

	UINT LoadProfile(string path);
	UINT GetProfileIntW(string strSec, string strItm, int& iValue);
	UINT GetProfileDoubleW(string strSec, string strItm, double& dValue);
	UINT GetProfileStringW(string strSec, string strItm, string& strValue);

	void ReleaseProfile();
private:
	UINT BuildProfileQueue(string strProf);
	UINT GetProfValue(string strSec, string strItm, string& strValue);

private:
	Section* pProfile;
};

