#include <exception>
#include "searchFileGenerator.h"

CSearchFileGenerator::CSearchFileGenerator()
{
}
CSearchFileGenerator::~CSearchFileGenerator()
{
	m_searchFile.close();
}
UINT CSearchFileGenerator::SearchFileInit()
{
	UINT	uRet	= 0;
	if((uRet = GetSearchFilePath(m_path)) != 0 )
		return uRet;
	m_searchFile.open(m_path.c_str(),fstream::out | fstream::app);
	return uRet;
}

UINT CSearchFileGenerator::WriteSearchSongs(const string& strSong)
{
	UINT	uRet	= 0;
	try
	{
		m_searchFile<<strSong<<"\n";
	}
	catch(exception& e)
	{
		cerr<<"write search songs exception catched: "<<e.what()<<endl;		
		return ERROR_WRITE_SEARCH_SONG; 
	}
	return uRet;
}

UINT CSearchFileGenerator::GetSearchFilePath(string& strFilePath)
{	
	UINT	uRet	= 0;
	CProfile	prof;
	
	if((uRet = prof.LoadProfile(SYS_CONFIG_POS)) != 0)
	{
		cout<<"get file ok"<<endl;
		goto	e_exist;
	}
	if((uRet = prof.GetProfileStringW(SEARCH_FILE,SEARCH_FILE_PATH,strFilePath)) != 0)
		goto	e_exist;
e_exist:
	prof.ReleaseProfile();
		return uRet;
}

