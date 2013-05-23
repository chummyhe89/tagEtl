#include "lib.h"
#include "comdef.h"
#include <string>
#include <map>
#include <vector>

#define  ERROR_NO_MATCHED_SONG	0x90000013

using namespace std;

typedef struct  _localSong
{
	UINT uSid;
	string strSongName;
	string strSingerName;
}LocalSong;
typedef struct _sidTag
{	
	UINT uSid;
	UINT uTagid;
}SidTag;

class CLocalLib:public CLib
{
public:
	CLocalLib();
	~CLocalLib();
	UINT LoadSongs(map<string,LocalSong*>& mapNameKeySongs);
	void DestroyLocalSongs(map<string,LocalSong*>& mapNameKeySongs);
	UINT CleanLocalVarTagsRelation();
	UINT StoreStableTags(vector<SidTag*>& vecpSidTags);
	UINT StoreVarTags(vector<SidTag*>& vecpVarSidTags);
	void DestroySidTags(vector<SidTag*>& vecpSidTags);
	UINT BackUpStableTags();
private:
	UINT StoreTags(vector<SidTag*>& vecpSidTags,bool bStable);
};
