#include "Utility.h"

CUtility::CUtility()
{}
CUtility::~CUtility()
{}

string CUtility::ltrim(string str)
{
	size_t u_pos		= 0;

	while(u_pos < str.size() && (ASCII_SPACE == str[u_pos] || ASCII_TAB == str[u_pos] || ASCII_ENTER == str[u_pos] || ASCII_WRAP == str[u_pos]))
		u_pos ++;
	if(0 != u_pos)
		str = str.substr(u_pos, str.size() - u_pos);

	return str;
}

string CUtility::rtrim(string str)
{
	size_t u_pos		= str.size() - 1;

	while(u_pos >= 0 && (ASCII_SPACE == str[u_pos] || ASCII_TAB == str[u_pos] || ASCII_ENTER == str[u_pos] || ASCII_WRAP == str[u_pos]))
		u_pos --;
	if(u_pos < str.size() - 1)
	{
		u_pos = u_pos < 0 ? 0 : u_pos;
		str = str.substr(0,u_pos+1); 
	}

	return str;
}

string CUtility::trim(string str)
{
	str = ltrim(str);
	str = rtrim(str);

	return str;
}
