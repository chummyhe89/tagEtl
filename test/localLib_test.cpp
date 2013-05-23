#include "localLib.h"
#include "comdef.h"
#include "gtest/gtest.h"

class LocalLibTest:public testing::Test
{
protected:
	static void SetUpTestCase()
	{
		m_pLocalLib = new CLocalLib;
	}
	static void TearDownTestCase()
	{
		delete m_pLocalLib;
		m_pLocalLib = NULL;
	}
	
	static CLocalLib*	m_pLocalLib;
};
CLocalLib* LocalLibTest::m_pLocalLib = NULL;

TEST_F(LocalLibTest,BackUpTest)
{
	ASSERT_EQ(0u,m_pLocalLib->ConnectToLib(LOCALMUSICDB));
	EXPECT_EQ(0u,m_pLocalLib->BackUpStableTags());
	m_pLocalLib->DisConnFromLib(LOCALMUSICDB);
}
