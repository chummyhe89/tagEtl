#pragma once
#include <string>
/*
#define		ASCII_SPACE	32
#define		ASCII_ENTER	13
#define		ASCII_TAB	9
#define		ASCII_WRAP	10

#define		ASCII_DQUOT	34
#define		ASCII_QUOT	39
*/
#ifndef ASCII_CHAR_MAP
#define ASCII_CHAR_MAP
#define ASCII_SPACE			32
#define ASCII_ENTER			13
#define ASCII_TAB			9
#define ASCII_WRAP			10

#define ASCII_DQUOT 			34
#define ASCII_QUOT			39
#endif
using namespace std;

class CUtility
{
public:
	CUtility();
	~CUtility();

	static string ltrim(string str);
	static string rtrim(string str);
	static string trim(string str);
};
