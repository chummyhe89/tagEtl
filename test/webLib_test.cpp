#include "webLib.h"
#include "comdef.h"
#include "gtest/gtest.h"
#include "logger.h"
class CWebLibTest:public testing::Test
{
protected:
	static void SetUpTestCase()
	{
		m_webLib = new CWebLib;
	}

	static void TearDownTestCase()
	{
		delete m_webLib;
		m_webLib = NULL;
	}
	
	static CWebLib* m_webLib;
};

CWebLib* CWebLibTest::m_webLib = NULL;

TEST_F(CWebLibTest,LoadWebVarSongsTest)
{	
	vector<WebSong*> vecpWebVarSongs;
	ASSERT_EQ(0u,m_webLib->ConnectToLib(WEBMUSICDB));
	EXPECT_EQ(0u,m_webLib->LoadWebVarSongs(vecpWebVarSongs));
	EXPECT_EQ(1u,vecpWebVarSongs.size());
	EXPECT_EQ("u r beautiful",(*(vecpWebVarSongs[0])).strSongName);
	EXPECT_EQ("me",(*(vecpWebVarSongs[0])).strSingerName);
	EXPECT_EQ(11u,(*(vecpWebVarSongs[0])).uTagid);
	m_webLib->DestroyWebSongs(vecpWebVarSongs);
	EXPECT_EQ(1u,vecpWebVarSongs.size());
	EXPECT_EQ(NULL,vecpWebVarSongs[0]);
	m_webLib->DisConnFromLib(WEBMUSICDB);
	
}
TEST_F(CWebLibTest,NeedFlushVarTagsTest)
{
	Logger ttlogger;
	ttlogger.init("info","./log.txt");
	ASSERT_EQ(0u,m_webLib->ConnectToLib(WEBMUSICDB));
	EXPECT_FALSE(m_webLib->NeedFlushVarTags());
	m_webLib->DisConnFromLib(WEBMUSICDB);
}
/*
TEST_F(CWebLibTest,CleanOldVarTagsTest)
{
	ASSERT_EQ(0u,m_webLib->ConnectToLib(WEBMUSICDB));
	EXPECT_EQ(0u,m_webLib->CleanWebOldVarTags);
	m_webLib->DisConnFromLib(WEBMUSICDB);
}
*/
