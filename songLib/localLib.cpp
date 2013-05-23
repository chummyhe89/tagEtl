#include "localLib.h"
#include "logger.h"
#include <cstdlib>
#include "Utility.h"
#include "Converter.h"

CLocalLib::CLocalLib()
{
}

CLocalLib::~CLocalLib()
{
}

UINT CLocalLib::LoadSongs(map<string,LocalSong*>& mapNameKeySongs)
{
	UINT 		uRet		= 0;
	MYSQL_RES*	pQueryRes	= NULL;
	LocalSong*	pLocalSong	= NULL;
	char**		ppLocalSong	= NULL;
	string 		strSql;
	string		strKey;
	
	strSql = "select song_id,song_name,singer_chinese_name from v_songsinfo";
	if((uRet = m_mySqlHelper.ExecuteQuery(strSql,&pQueryRes)) != 0)
		return uRet;
	
	while(m_mySqlHelper.FetchOneRow(pQueryRes,&ppLocalSong) == 0)
	{
		pLocalSong = new LocalSong;
		if(NULL != ppLocalSong[0]) (*pLocalSong).uSid = atoi(ppLocalSong[0]);
		if(NULL != ppLocalSong[1]) (*pLocalSong).strSongName = CUtility::trim(ppLocalSong[1]);
		if(NULL != ppLocalSong[2]) (*pLocalSong).strSingerName = CUtility::trim(ppLocalSong[2]);
		
		strKey = (*pLocalSong).strSongName + "_" +(*pLocalSong).strSingerName;
		mapNameKeySongs.insert(pair<string,LocalSong*>(strKey,pLocalSong));
	}

	return uRet;
}

void CLocalLib::DestroyLocalSongs(map<string,LocalSong*>& mapNameKeySongs)
{
	map<string,LocalSong*>::iterator iterLocalSong;
	for(iterLocalSong = mapNameKeySongs.begin();iterLocalSong != mapNameKeySongs.end();iterLocalSong ++)
	{delete	 (*iterLocalSong).second;}
}

UINT CLocalLib::CleanLocalVarTagsRelation()
{	
	UINT	uRet	= 0;
	string 	strSql;
	
	strSql = "insert into re_songs_music_vartags_backup select song_id,tag_id,date_time from re_songs_music_vartags";
	if((uRet = m_mySqlHelper.ExecuteNonQuery(strSql)) != 0)
	{
		LOGGER->error("%s[%d]:backup relation between vartags and songid failed! Error Code : %u",__FILE__,__LINE__,uRet);
		return uRet;
	}
	LOGGER->info("begin cleaning old relation between vartags and songid !");
	strSql = "delete from re_songs_music_vartags";
	if((uRet = m_mySqlHelper.ExecuteNonQuery(strSql)) != 0)
	{
		LOGGER->error("%s[%d]:cleaning old relation between vartags and songid failed! Error Code : %u",__FILE__,__LINE__,uRet);
		return uRet;	
	}
	return uRet;

}

UINT CLocalLib::StoreStableTags(vector<SidTag*>& vecpSidTags)
{
	UINT	uRet	= 0;
	if((uRet = StoreTags(vecpSidTags,true)) != 0)	
	{
		if(ERROR_NO_MATCHED_SONG == uRet)
			LOGGER->error("%s[%d]:no stable tag songs matched ! Error Code : %x",__FILE__,__LINE__,uRet);
		else
			LOGGER->error("%s[%d]:store stable tag songs failed ! Error Code : %u",__FILE__,__LINE__,uRet);
		return uRet;
	}

	return uRet;
}
UINT CLocalLib::StoreVarTags(vector<SidTag*>& vecpSidTags)
{
	UINT	uRet	= 0;
	if((uRet = StoreTags(vecpSidTags,false)) != 0)	
	{
		if(ERROR_NO_MATCHED_SONG == uRet)
			LOGGER->error("%s[%d]:no var tag songs matched ! Error Code : %x",__FILE__,__LINE__,uRet);
		else
			LOGGER->error("%s[%d]:store var tag songs failed ! Error Code : %u",__FILE__,__LINE__,uRet);
		return uRet;
	}
	return uRet;


}

void CLocalLib::DestroySidTags(vector<SidTag*>& vecpSidTags)
{
	for(size_t uPos = 0; uPos < vecpSidTags.size(); uPos ++)
		{delete vecpSidTags[uPos];}
}

UINT CLocalLib::StoreTags(vector<SidTag*>& vecpSidTags,bool bStable)
{
	UINT 	uRet	= 0;
	string  strSql;
	SidTag* pSidTag = NULL;
	if ( 0 == vecpSidTags.size())
	{
		uRet = ERROR_NO_MATCHED_SONG;
		return uRet;	
	}

	for(size_t uPos = 0; uPos < vecpSidTags.size(); uPos++)
	{
		pSidTag	= vecpSidTags[uPos];
		if(bStable)
		{
			strSql = "insert into re_songs_music_tags(song_id,tag_id) \
				values("+CConverter::Uint2String((*pSidTag).uSid)+","+CConverter::Uint2String((*pSidTag).uTagid)+")";
		}
		else
		{
			strSql = "insert into re_songs_music_vartags(song_id,tag_id) \
				values("+CConverter::Uint2String((*pSidTag).uSid)+","+CConverter::Uint2String((*pSidTag).uTagid)+")";
			
		}
		if(m_mySqlHelper.ExecuteNonQuery(strSql) != 0)
			;
//			LOGGER->info("song_id : %u tag_id %u: has exists !",(*pSidTag).uSid,(*pSidTag).uTagid);
	}
	
	return uRet;
}
UINT CLocalLib::BackUpStableTags()
{
	UINT	uRet	= 0;
	string  strSql;
	
	strSql = "delete from re_songs_music_tags_tmp";
	if((uRet = m_mySqlHelper.ExecuteNonQuery(strSql)) != 0)
		return uRet;
	strSql = "insert into re_songs_music_tags_tmp(song_id,tag_id) select song_id,tag_id from v_re_songs_music_tags";
	if((uRet = m_mySqlHelper.ExecuteNonQuery(strSql)) != 0)
		return uRet;
	return uRet;
}
