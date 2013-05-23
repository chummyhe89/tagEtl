#pragma once
#include "comdef.h"
#include "Profile.h"
#include <string>
#include <fstream>
#include <iostream>

#define		ERROR_WRITE_SEARCH_SONG	0x90000014

using namespace std;
class CSearchFileGenerator
{
public:
	CSearchFileGenerator();
	~CSearchFileGenerator();
	UINT SearchFileInit();
	UINT WriteSearchSongs(const string& strSong);
private:
	UINT GetSearchFilePath(string& strFilePath);
	ofstream	m_searchFile;
	string		m_path;
};
