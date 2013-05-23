#include "gtest/gtest.h"
#include "searchFileGenerator.h"
#include "comdef.h"

class CSearchFileTest:public testing::Test
{
protected:
	static void SetUpTestCase()
	{
		m_searchFile = new CSearchFileGenerator;
	}

	static void TearDownTestCase()
	{
		delete m_searchFile;
		m_searchFile = NULL;
	}
	
	static CSearchFileGenerator* m_searchFile;
};

CSearchFileGenerator* CSearchFileTest::m_searchFile = NULL;

TEST_F(CSearchFileTest,WriteSongsTest)
{
//	EXPECT_EQ(0u,m_searchFile->GetSearchFilePath(strPath));
	EXPECT_EQ(0u,m_searchFile->SearchFileInit());
//	EXPECT_EQ("data/search.txt",strPath);
	EXPECT_EQ(0u,m_searchFile->WriteSearchSongs("我的歌声里 曲婉婷"));
}
