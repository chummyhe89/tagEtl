#include "Profile.h"
#include <gtest/gtest.h>
#include <string>

using namespace std;
class CProfileTest:public testing::Test
{
public:
	virtual void SetUp()
	{
		m_Pro.LoadProfile("../config/sysconf.ini");
	}
	virtual void TearDown()
	{
		m_Pro.ReleaseProfile();
	}
	
	CProfile m_Pro;
};

/*TEST_F(ProfileTest,GetProfileStringW)
{
	string str_dbName;
	CProfile myP;
	myP.LoadProfile("../config/sys.ini");
	myP.GetProfileStringW("LocalMusic","dbName",str_dbName);
	EXPECT_EQ("music",str_dbName);
}
*/

TEST_F(CProfileTest,GetProfileStringW)
{
	string str_dbName;
	m_Pro.GetProfileStringW("LocalMusic","dbName",str_dbName);
	EXPECT_EQ("music",str_dbName);
}
