#include "Profile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


CProfile::CProfile(void)
	:pProfile(NULL)
{
}


CProfile::~CProfile(void)
{
	ReleaseProfile();
}

UINT CProfile::LoadProfile(string path)
{
	UINT  uRet					= S_OK;
	FILE* fpProf				= NULL;
	unsigned long ulfileSize	= 0;
	char* pBuffProf				= NULL;

	// parameter checking
	if(path.empty())
		return ERROR_INVALID_PARAMETER;

	// open file
	if(NULL == (fpProf = fopen(path.c_str(), "r")))
		return ERROR_CANNOT_OPEN_PROFILE;

	// get file size
	fseek(fpProf,0,SEEK_END);
	if(0 == (ulfileSize = ftell(fpProf)))
	{
		uRet = ERROR_FILE_INVALID;
		goto e_exit;
	}

	// allocate memery
	if(NULL == (pBuffProf = new char[ulfileSize + 1]))
	{
		uRet = ERROR_NOT_ENOUGH_MEMORY;
		goto e_exit;
	}
	memset(pBuffProf, '\0',ulfileSize);

	// read profile contents
	fseek(fpProf, 0, SEEK_SET);
	if(fread(pBuffProf,1,ulfileSize,fpProf) <= 0)
	{
		uRet = ERROR_READ_FAULT;
		goto e_exit;
	}
	pBuffProf[ulfileSize] = '\0';

	// build profile queue
	uRet = BuildProfileQueue(pBuffProf);

e_exit:
	if(NULL != fpProf)
		fclose(fpProf);
	if(NULL != pBuffProf)
		delete[] pBuffProf;

	return uRet;
}

UINT CProfile::GetProfileIntW(string strSec, string strItm, int& iValue)
{
	UINT uRet	= S_OK;
	string strValue;

	// get value
	uRet = GetProfValue(strSec, strItm, strValue);

	// convert to int
	if(S_OK == uRet)
		iValue = atoi(strValue.c_str());

	return uRet;
}

UINT CProfile::GetProfileDoubleW(string strSec, string strItm, double& dValue)
{
	UINT uRet	= S_OK;
	string strValue;

	// get value
	uRet = GetProfValue(strSec, strItm, strValue);

	// convert to float
	if(S_OK == uRet)
		dValue = atof(strValue.c_str());

	return uRet;
}

UINT CProfile::GetProfileStringW(string strSec, string strItm, string& strValue)
{
	return GetProfValue(strSec, strItm, strValue);
}

void CProfile::ReleaseProfile()
{
	Section* pSecNode		= NULL;
	Section* pPreSecNode		= NULL;
	Item* pItmNode			= NULL;
	Item* pPreItmNode		= NULL;

	// delete section
	pSecNode = pProfile;
	while(NULL != pSecNode)
	{
		// delete items
		pItmNode = (*pSecNode).pItemList;
		while(NULL != pItmNode)
		{
			pPreItmNode = pItmNode;
			pItmNode = (*pItmNode).next;

			delete pPreItmNode;
		}
		
		pPreSecNode = pSecNode;
		pSecNode = (*pSecNode).next;

		delete pPreSecNode;
	}

	pProfile = NULL;
}

UINT CProfile::BuildProfileQueue(string strProf)
{
	UINT 	uRet			= S_OK;
	size_t 	pos			= 0;
	size_t 	tPos			= 0;
	Section* plstSecNode 	= NULL;
	Section* pSecNode	= NULL;
	Item* pLstItmNode	= NULL;
	Item* pItmNode		= NULL;
	string strtProf;
	string strtLine;

	// copy parameter
	strtProf = strProf;

	// parameter checking
	if(tirm(strtProf).empty())
		return ERROR_INVALID_PARAMETER;

	// analyze profile line by line
	while(pos < strtProf.length())
	{
		// get a line
		tPos = strtProf.find(ASCII_WRAP,pos) == string::npos? strtProf.length() : strtProf.find(ASCII_WRAP,pos);
		strtLine = strtProf.substr(pos, tPos - pos);
		pos = tPos + 1;

		// note
		if(tirm(strtLine).empty() || ';' == strtLine[0])
			continue;

		// section
		if('[' == strtLine[0])
		{
			// checking
			if((tPos = strtLine.find(']', 1)) == string::npos)
			{
				uRet = ERROR_BAD_FORMAT;
				goto e_exit;
			}

			// new section
			pSecNode = new Section;
			(*pSecNode).strName = strtLine.substr(1, tPos - 1);
			(*pSecNode).pItemList = NULL;
			(*pSecNode).next = NULL;

			tirm((*pSecNode).strName);

			// first node
			if(NULL == pProfile)
				pProfile = pSecNode;
			else
				(*plstSecNode).next = pSecNode;

			plstSecNode = pSecNode;
			continue;
		}

		// item
		tPos = strtLine.find(';', 0) == string::npos? strtLine.length(): strtLine.find(';', 0);
		if(strtLine.length() > tPos)
		{
			strtLine = strtLine.substr(0, tPos);
			strtLine = tirm(strtLine);
		}

		// new item
		pItmNode = new Item;
		if((tPos = strtLine.find('=', 0)) == string::npos)
		{
			uRet = ERROR_BAD_FORMAT;
			goto e_exit;
		}
		(*pItmNode).strName = strtLine.substr(0, tPos);
		(*pItmNode).strValue = strtLine.substr(tPos + 1, strtLine.length());
		(*pItmNode).next = NULL;
		
		tirm((*pItmNode).strName);
		tirm((*pItmNode).strValue);
		QuotTirm((*pItmNode).strValue);

		// first
		if(NULL == (*plstSecNode).pItemList)
			(*plstSecNode).pItemList = pItmNode;
		else
			(*pLstItmNode).next = pItmNode;

		pLstItmNode = pItmNode;
	}

e_exit:
	if(S_OK != uRet)
		ReleaseProfile();

	return uRet;
}

UINT CProfile::GetProfValue(string strSec, string strItm, string& strValue)
{
	Section* pSecNode	= NULL;
	Item* pItmNode		= NULL;

	// checking parameter
	if(strSec.empty() || strItm.empty())
		return ERROR_INVALID_PARAMETER;

	// search section
	pSecNode = pProfile;
	while((NULL != pSecNode)&&(0 != (*pSecNode).strName.compare(tirm(strSec))))
		pSecNode = (*pSecNode).next;

	if(NULL == pSecNode)
		return ERROR_DEV_NOT_EXIST;

	// search item
	pItmNode = (*pSecNode).pItemList;
	while(NULL != pItmNode && (0 != (*pItmNode).strName.compare(tirm(strItm))))
		pItmNode = (*pItmNode).next;

	if(NULL == pItmNode)
		return ERROR_DEV_NOT_EXIST;

	// get value
	strValue = (*pItmNode).strValue;
	return S_OK;
}


string tirm(string& str)
{
	size_t prePos	= 0;
	size_t reaPos 	= 0;
	string strt;

	strt = str;
	// parameter checking
	if(str.empty())
		return strt;

	// copy parameter
	strt = str;

	// left tirm
	for(prePos = 0; prePos < strt.length(); prePos ++)
	{
		if(strt[prePos] != ASCII_SPACE && strt[prePos] != ASCII_TAB && strt[prePos] != ASCII_ENTER && strt[prePos] != ASCII_WRAP)
			break;
	}

	// right tirm
	for(reaPos = strt.length() - 1; reaPos > 0; reaPos --)
	{
		if(strt[reaPos] != ASCII_SPACE && strt[reaPos] != ASCII_TAB && strt[reaPos] != ASCII_ENTER && strt[reaPos] != ASCII_WRAP)
			break;
	}

	// return
	strt = strt.substr(prePos, reaPos - prePos + 1);
	str = strt;

	return str;
}

string QuotTirm(string& str)
{
	size_t prePos	= 0;
	size_t reaPos 	= 0;
	string strt;

	strt = str;
	// parameter checking
	if(str.empty())
		return strt;

	// copy parameter
	strt = str;

	// left tirm
	for(prePos = 0; prePos < strt.length(); prePos ++)
	{
		if(strt[prePos] != ASCII_DQUOT && strt[prePos] != ASCII_QUOT)
			break;
	}

	// right tirm
	for(reaPos = strt.length() - 1; reaPos > 0; reaPos --)
	{
		if(strt[reaPos] != ASCII_DQUOT && strt[reaPos] != ASCII_QUOT)
			break;
	}

	// return
	strt = strt.substr(prePos, reaPos - prePos + 1);
	str = strt;

	return str;
}
