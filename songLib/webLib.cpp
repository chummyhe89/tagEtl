#include "webLib.h"
#include <cstdlib>
#include <cstring>
#include "Profile.h"
#include "logger.h"

CWebLib::CWebLib()
{
}
CWebLib::~CWebLib()
{
}
UINT CWebLib::LoadSongs(vector<WebSong*>& vecpSongs,bool bVolatile)
{
	UINT	uRet = 0;
	MYSQL_RES*	pQueryRes	= NULL;
	WebSong*	pWebSong	= NULL;
	char**		ppWebSongs	= NULL;
	string		strSql;

	if(bVolatile)
		strSql = "select tag_id, sid, title, artist from re_songs_vartags";
	else
		strSql = "select tag_id, sid, title, artist from re_songs_tags";
	
	if((uRet = m_mySqlHelper.ExecuteQuery(strSql,&pQueryRes)) != 0)
		return uRet;
	while(m_mySqlHelper.FetchOneRow(pQueryRes,&ppWebSongs) == 0)
	{
		pWebSong = new WebSong;
		if(NULL != ppWebSongs[1])
			(*pWebSong).uSid	= atoi(ppWebSongs[1]);
		if(NULL != ppWebSongs[2])
			(*pWebSong).strSongName = ppWebSongs[2];
		if(NULL != ppWebSongs[3])
			(*pWebSong).strSingerName = ppWebSongs[3];
		(*pWebSong).uTagid = atoi(ppWebSongs[0]);
		
		vecpSongs.push_back(pWebSong);
	}
	
	return  uRet;
}
UINT CWebLib::CleanWebOldVarTags()
{
	UINT 	uRet = 0;
	string  strSql;
	
	strSql = "delete from re_songs_vartags";
	if((uRet = m_mySqlHelper.ExecuteNonQuery(strSql)) != 0)
		return uRet;
	return uRet;
}

UINT CWebLib::LoadWebSongs(vector<WebSong*>& vecpWebSongs)
{
	return LoadSongs(vecpWebSongs,false);
}

UINT CWebLib::LoadWebVarSongs(vector<WebSong*>& vecpWebVarSongs)
{
	return LoadSongs(vecpWebVarSongs,true);
}

void CWebLib::DestroyWebSongs(vector<WebSong*>& vecpWebSongs)
{
	for(size_t iPos = 0; iPos < vecpWebSongs.size(); iPos++)
	{	delete	vecpWebSongs[iPos];
		vecpWebSongs[iPos] = NULL;
	}
}
bool CWebLib::NeedFlushVarTags()
{
	bool		bRet	= true;
	bool 		bHaveTagsData = false;
	UINT		uRet	= 0;
	int  		iMinFlushThreshold = 0;
	MYSQL_RES*	pQueryRes	= NULL;
	char** 		ppWebTagNum		= NULL;
	string 		strSql;
	CProfile	prof;	

	if(prof.LoadProfile(SYS_CONFIG_POS) != 0)
		iMinFlushThreshold = DEFAULTFLUSHTHRESHOLD;
	if(prof.GetProfileIntW(FLUSH_SETTING,VAR_TAGS_FLUSH_THRESHOLD,iMinFlushThreshold) != 0)
		iMinFlushThreshold = DEFAULTFLUSHTHRESHOLD;
	strSql = "select tag_id,count(1) from  re_songs_vartags group by tag_id";
	if((uRet = m_mySqlHelper.ExecuteQuery(strSql,&pQueryRes)) != 0)
	{
		LOGGER->error("%s[%d] : NeedflushVarTags ExecuteQuery Failed! Error code:%u",__FILE__,__LINE__,uRet);
		bRet = false;
		return bRet;
	}
	while(m_mySqlHelper.FetchOneRow(pQueryRes,&ppWebTagNum) == 0)
	{	
		bHaveTagsData	= true;
		if(atoi(ppWebTagNum[1]) < iMinFlushThreshold)
		{
			LOGGER->info("Tag  %s  lower than min flush threshold(%u),so do not flush varTags this time!",ppWebTagNum[0],iMinFlushThreshold);
			bRet = false;
			return bRet;
		}
	}
	if(!bHaveTagsData)
	{	
		LOGGER->info("No varTags exist,so do not flush varTags this time!");
		return false;
	}
	return   bRet && bHaveTagsData;
}
