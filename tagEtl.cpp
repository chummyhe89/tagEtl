#include "comdef.h"
#include "refresher.h"
#include "webLib.h"
#include "localLib.h"
#include "searchFileGenerator.h"
#include "logger.h"
#include <string>
#include <iostream>
#include <vector>
#include <map>

using namespace std;
class CWebTagEtl
{
public:
	CWebTagEtl(){};
	~CWebTagEtl(){};
	UINT EtlDoWork();
private:
	UINT MatchSongsAndWriteMissedToFile(vector<WebSong*>& vecpWebSongs,map<string,LocalSong*>& mapSongs,vector<SidTag*>& vecpMatched);
	UINT	InitializeLogger();
	UINT	GetLoggerPath(string& strPath);
	CRefresher	m_ref;
	CWebLib		m_webLib;
	CLocalLib	m_localLib;
	CSearchFileGenerator m_searchFile;
	Logger		m_logger;
};

UINT CWebTagEtl::EtlDoWork()
{
	UINT uRet		= 0;
	vector<WebSong*>	vecpWebSongs;
	vector<WebSong*>	vecpVarSongs;
	map<string,LocalSong*>	mapNameKeySongs;
	vector<SidTag*>		vecpMatchedSidTags;
	vector<SidTag*>		vecpVarMatchedSidTags;
	//init logger
	if((uRet = InitializeLogger()) != 0)	
	{
		cerr<<"error : initialize logger failed! Error Code:"<<uRet<<endl;
		return uRet;
	}	
	//init search file
	if((uRet = m_searchFile.SearchFileInit()) != 0)
	{
		LOGGER->error("%s[%d]:search file initialize failed! Error Code : %u",__FILE__,__LINE__,uRet);
		return uRet;
	}
	//load songs 
	if((uRet = m_webLib.ConnectToLib(WEBMUSICDB)) != 0)
	{
		LOGGER->error("%s[%d]:connect to weblib failed! Error Code : %u",__FILE__,__LINE__,uRet);
		return uRet;
	}
	if((uRet = m_localLib.ConnectToLib(LOCALMUSICDB)) != 0)
	{
		LOGGER->error("%s[%d]:connect to locallib failed! Error Code : %u",__FILE__,__LINE__,uRet);
		return uRet;
	}
	if((uRet = m_webLib.LoadWebSongs(vecpWebSongs)) != 0)
	{
		LOGGER->error("%s[%d]:load web songs failed! Error Code : %u",__FILE__,__LINE__,uRet);
		return uRet;
	}
	if((uRet = m_webLib.LoadWebVarSongs(vecpVarSongs)) != 0)
	{
		LOGGER->error("%s[%d]:load web var songs failed! Error Code : %u",__FILE__,__LINE__,uRet);
		return uRet;
	}
	if((uRet = m_localLib.LoadSongs(mapNameKeySongs)) != 0)
	{
		LOGGER->error("%s[%d]:load local songs failed! Error Code : %u",__FILE__,__LINE__,uRet);
		return uRet;
	}

	//match stable songs and write missed
	if((uRet = MatchSongsAndWriteMissedToFile(vecpWebSongs,mapNameKeySongs,vecpMatchedSidTags)) != 0)
	{
		LOGGER->error("%s[%d]:match stable songs failed! Error Code : %u",__FILE__,__LINE__,uRet);
		return uRet;
	}
	//store stable songs
	if((uRet = m_localLib.StoreStableTags(vecpMatchedSidTags)) != 0)
	{
		LOGGER->error("%s[%d]:store stable sidtags failed! Error Code : %u",__FILE__,__LINE__,uRet);
		return uRet;
	}
	//destroy stable websong and sidtags
	m_webLib.DestroyWebSongs(vecpWebSongs);
	m_localLib.DestroySidTags(vecpMatchedSidTags);
	if(m_webLib.NeedFlushVarTags())
	{
		//match variable songs
		if((uRet = MatchSongsAndWriteMissedToFile(vecpVarSongs,mapNameKeySongs,vecpVarMatchedSidTags)) != 0)
		{
			LOGGER->error("%s[%d]:match variable songs failed! Error Code : %u",__FILE__,__LINE__,uRet);
			return uRet;
		}
		//clean local old relation
		if((uRet = m_localLib.CleanLocalVarTagsRelation()) != 0)
		{
			LOGGER->error("%s[%d]:clean local lib old relation failed! Error Code : %u",__FILE__,__LINE__,uRet);
			return uRet;
		}
		//store variable songs
		if((uRet = m_localLib.StoreVarTags(vecpVarMatchedSidTags)) != 0)
		{
			LOGGER->error("%s[%d]:store variable songs failed! Error Code : %u",__FILE__,__LINE__,uRet);
			return uRet;
		}
		//destroy variable songs
		m_webLib.DestroyWebSongs(vecpVarSongs);
		m_localLib.DestroyLocalSongs(mapNameKeySongs);
		m_localLib.DestroySidTags(vecpVarMatchedSidTags);
		//clean web varialbe tags
		if((uRet = m_webLib.CleanWebOldVarTags()) !=0)
		{
			LOGGER->error("%s[%d]:clean web var songs failed! Error Code : %u",__FILE__,__LINE__,uRet);
			return uRet;
		}
	}	

	//back up stable relatino
	if((uRet = m_localLib.BackUpStableTags()) != 0)	
	{
		LOGGER->error("%s[%d]:backup stable tags failed! Error Code : %u",__FILE__,__LINE__,uRet);
		return uRet;
	}
	//refresh radio
	if((uRet = m_ref.RefreshPublicRadio()) != 0)
	{
		LOGGER->error("%s[%d]:refresh public radio failed! Error Code : %u",__FILE__,__LINE__,uRet);
		return uRet;
	}	
	m_webLib.DisConnFromLib();
	m_localLib.DisConnFromLib();
	return uRet;
}
UINT CWebTagEtl::MatchSongsAndWriteMissedToFile(vector<WebSong*>& vecpWebSongs,map<string,LocalSong*>& mapSongs,vector<SidTag*>& vecpMatched)
{
	UINT	uRet 	= 0;
	LocalSong*	pLocalSong	= NULL;
	WebSong*	pWebSong	= NULL;
	map<string,LocalSong*>::iterator	localSongIter;
	SidTag*		pSidTag		= NULL;
	for(int iPos = 0; iPos < static_cast<int>(vecpWebSongs.size()); iPos++ )
	{
		pWebSong	= vecpWebSongs[iPos];
		if(!((*pWebSong).strSingerName.empty()) && (*pWebSong).strSingerName.compare(""))
		{
			string strSongKey	=	(*pWebSong).strSongName + "_" + (*pWebSong).strSingerName;
			string strSearchKey	=	(*pWebSong).strSongName + "\t" + (*pWebSong).strSingerName;
			if((localSongIter = mapSongs.find(strSongKey)) == mapSongs.end())
			{
				if((uRet = m_searchFile.WriteSearchSongs(strSearchKey)) != 0 )
				{
					LOGGER->info("%s[%d]:Write not matched song(%s) to file failed! Error Code : %u",__FILE__,__LINE__,strSearchKey.c_str(),uRet);	
				}	
				continue;
			}
			pLocalSong = (*localSongIter).second;
		}
		else
		{
			continue;
		}
		pSidTag	= new SidTag;
		(*pSidTag).uSid = (*pLocalSong).uSid;
		(*pSidTag).uTagid = (*pWebSong).uTagid;
		vecpMatched.push_back(pSidTag);
	}
	
	return uRet;

}

UINT CWebTagEtl::GetLoggerPath(string& strPath)
{
	UINT	uRet	= 0;
	CProfile	prof;
	if((uRet = prof.LoadProfile(SYS_CONFIG_POS)) != 0)
		return uRet;
	if((uRet = prof.GetProfileStringW(SYS_LOG,LOG_PATH,strPath)) != 0)
		return uRet;
	return uRet;
}
UINT CWebTagEtl::InitializeLogger()
{
	UINT	uRet	= 0;
	string  strLogPath;
	if((uRet=GetLoggerPath(strLogPath)) !=0)
	{	
		cerr<<"error : get logger path failed! Error Code:"<<uRet<<endl;
		return uRet;
	}
	if((uRet = m_logger.init("info",strLogPath.c_str())) != 0)
	{
		return uRet;
	}

	return uRet;
}












int main(int argc, char** argv)
{
	UINT	uRet	= 0;
	CWebTagEtl	wTEtl;
	if((uRet=wTEtl.EtlDoWork()) != 0)
		return uRet;
	return uRet;
}
