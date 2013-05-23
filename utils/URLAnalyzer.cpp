#include "URLAnalyzer.h"
#include <string.h>
#include <stdlib.h>


CURLAnalyzer::CURLAnalyzer(void)
{
}


CURLAnalyzer::~CURLAnalyzer(void)
{
	ReleaseUrl();
}



UINT CURLAnalyzer::LoadUrl(const string strUrl)
{
	if(strUrl.empty())
		return ERROR_INVALID_PARAMETER;

	// analyze url
	return AnalyzeUrl(strUrl);
}

void CURLAnalyzer::ReleaseUrl()
{
	if(!m_strProtocol.empty())
		m_strProtocol.clear();	
	if(!m_strDomain.empty())		
		m_strDomain.clear();	
	if(!m_strPort.empty())			
		m_strPort.clear();
	if(!m_strPath.empty())				
		m_strPath.clear();	
	if(!m_strParam.empty())			
		m_strParam.clear();
}

string CURLAnalyzer::GetProtocol()
{
	return m_strProtocol;
}

string CURLAnalyzer::GetDomain()
{
	return m_strDomain;
}

string CURLAnalyzer::GetPort()
{
	return m_strPort;
}

string CURLAnalyzer::GetPath()
{
	return m_strPath;
}

UINT CURLAnalyzer::SelectParam(const string strName, string& strValue)
{
	// parameter check
	if(strName.empty() || m_strParam.empty())
		return ERROR_INVALID_PARAMETER;

	// select parameter
	return do_SelectParam(strName, strValue);
}

UINT CURLAnalyzer::AnalyzeUrl(const string strUrl)
{
	UINT 	status		= S_OK;				// error code
	size_t 	pos		= 0;				// url index
	size_t 	sPos		= 0;				// position of the string to be searched
	bool 	bDefPort	= true;				// have there a port
	string strtUrl;						// url have no parameter
	string strtValue;					// temporary string value

	// parameter check
	if(strUrl.empty())
		return ERROR_INVALID_PARAMETER;

	// divide url
	if((sPos = strUrl.find("?")) != string::npos)
	{
		strtUrl = strUrl.substr(pos,sPos - pos);
		m_strParam = strUrl.substr(sPos, strUrl.length() - sPos);
	}
	else
		strtUrl = strUrl;

	// filter invalid character
	FilterInvalidChar(strUrl, pos, DIREC_PRE);

	// analyze protocal
	if((sPos = strtUrl.find("://")) != string::npos)
	{
		m_strProtocol = strtUrl.substr(pos,sPos - pos);
		pos = sPos + strlen("://");
	}

	// filter invalid character
	FilterInvalidChar(strUrl, pos, DIREC_PRE);

	// analyze domain
	if((sPos = strtUrl.find(":",pos)) != string::npos)
		bDefPort = false;
	else
		sPos = strtUrl.find("/", pos);
	// is home page?
	if(sPos == string::npos)
	{
		if(strtUrl.length() == pos)
		{
			status = ERR_INVALID_URL;
			goto e_exit;
		}

		m_strDomain = strtUrl.substr(pos, strtUrl.length() - pos);
		goto e_exit;
	}
	// is sub page
	m_strDomain = strtUrl.substr(pos, sPos - pos);
	pos = sPos + 1;

	// analyze port
	if(!bDefPort)
	{
		// filter invalid character
		FilterInvalidChar(strUrl, pos, DIREC_PRE);

		// is home page?
		if((sPos = strtUrl.find("/",pos)) == string::npos)
		{
			if(strtUrl.length() == pos)
			{
				status = ERR_INVALID_URL;
				goto e_exit;
			}

			m_strPort = strtUrl.substr(pos, strtUrl.length() - pos);
			goto e_exit;
		}
		
		// is sub page
		m_strPort = strtUrl.substr(pos, sPos - pos);
		pos = sPos + 1;
	}

	// filter invalid character
	FilterInvalidChar(strUrl, pos, DIREC_PRE);

	// analyze path
	m_strPath = strtUrl.substr(pos, strtUrl.length() - pos);

e_exit:
	return status;
}

UINT CURLAnalyzer::do_SelectParam(const string strName, string& strValue)
{
	size_t pos		= 0;				// url index
	size_t sPos		= 0;				// end position of the string to be searched
	size_t prePos		= 0;				// pre position of the string
	string strtUrl;

	// parameter check
	if(strName.empty() || m_strParam.empty())
		return ERROR_INVALID_PARAMETER;

	// filter invalid character
	FilterInvalidChar(m_strParam, pos, DIREC_PRE);

	// find strName
	while((pos = m_strParam.find(strName, pos)) != string::npos)
	{
		// make sure is the parameter
		prePos = pos - 1;
		// move to the end of the string
		pos += strName.length();

		// filter invalid character
		FilterInvalidChar(m_strParam, pos, DIREC_PRE);
		FilterInvalidChar(m_strParam, prePos, DIREC_BAC);

		if('=' == m_strParam[pos] && ('?' == m_strParam[prePos] || '&' == m_strParam[prePos]))
			break;
	}
	if(string::npos == pos || '=' != m_strParam[pos])
		return STA_CANT_FIND;
	pos ++;

	// get strName
	sPos = m_strParam.find("&",pos) != string::npos? m_strParam.find("&",pos) : m_strParam.length();
	strtUrl = m_strParam.substr(pos, sPos - pos);

	// convert it to utf8
	return UrlToUtf8(strtUrl, strValue, strtUrl.length());
}

UINT CURLAnalyzer::UrlToUtf8(const string strUrl, string& strUtf8, int size, bool bUpperToLower)
{
	size_t len = strUrl.length() <= static_cast<size_t>(size)? strUrl.length() : size; 	/*min(strUrl.length(), size);*/
	string strtUtf8(size,'\0');

	// parameter check
	if (strUrl.empty())
		return ERROR_INVALID_PARAMETER;
	
	// decode
	int i =0, j =0;
	for (; i < static_cast<int>(len);)                     
	{
		if (strUrl[i] != '%')
		{
			if ('+' == strUrl[i]) strtUtf8[j] = ' ';
			else if (bUpperToLower && strUrl[i] >= 65 && strUrl[i] <= 90)
			{
				strtUtf8[j] = strUrl[i] + 32;
			}
			else
				strtUtf8[j] = strUrl[i];
			i++;
			j++;
		}
		else
		{
			char temp[4] = {0};
			strncpy(temp, &strUrl[++i], 2);
			strtUtf8[j++] = strtol(temp, NULL, 16);
			i+=2;
		}
	}
	strtUtf8.resize(j);

	strUtf8 = strtUtf8;
	return S_OK;
}

void CURLAnalyzer::FilterInvalidChar(const string str, size_t& pos, const UINT direc)
{
	size_t tPos	= 0;

	// parameter check
	if(str.empty())
		return;

	if(DIREC_PRE == direc)
	{
		while(ASCII_SPACE == str[pos] || ASCII_ENTER == str[pos] || ASCII_TAB == str[pos] || ASCII_WRAP == str[pos] || UTF8_SPACE2 == str[pos])
			pos ++;

		while((tPos = str.find(UTF8_SPACE,pos))!= string::npos  && tPos == pos)
			pos += strlen(UTF8_SPACE);
	}
	else
	{
		while(ASCII_SPACE == str[pos] || ASCII_ENTER == str[pos] || ASCII_TAB == str[pos] || ASCII_WRAP == str[pos] || UTF8_SPACE2 == str[pos])
			pos --;
		
		while((tPos = str.find(UTF8_SPACE,pos - strlen(UTF8_SPACE)))!= string::npos  && tPos == pos - strlen(UTF8_SPACE) + 1)
			pos -= strlen(UTF8_SPACE);
	}
}
