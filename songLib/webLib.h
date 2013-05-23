#pragma once

#include "comdef.h"
#include "lib.h"
#include <pthread.h>
#include <string>
#include <vector>

#define DEFAULTFLUSHTHRESHOLD	500
using namespace std;

typedef struct _webSong
{	
	UINT   uTagid;
	UINT   uSid;
	string strSongName;
	string strSingerName;
}WebSong;

class CWebLib : public CLib
{
public:
	CWebLib();
	~CWebLib();

	UINT LoadWebSongs(vector<WebSong*>& vecpWebSongs);
	UINT LoadWebVarSongs(vector<WebSong*>& vecpWebVarSongs);
	UINT CleanWebOldVarTags();
	void DestroyWebSongs(vector<WebSong*>& vecpWebSongs);
	bool NeedFlushVarTags();
private:
	UINT LoadSongs(vector<WebSong*>& vecpSongs,bool bVolatile);
};
