#include "logger.h"
#include "gtest/gtest.h"
#include <string>

using namespace std;

class LoggerTest:public testing::Test
{
protected:
	static void SetUpTestCase()
	{
		m_shLogger = new Logger;
		m_shLogger->init("info","./log.txt");
	}
	static void TearDownTestCase()
	{
		delete m_shLogger;
		m_shLogger = NULL;
	}
	
	static Logger* m_shLogger;
};

Logger* LoggerTest::m_shLogger = NULL;

TEST_F(LoggerTest,InfoTest)
{
	string strInfo = string("info test!");
	EXPECT_EQ(0,m_shLogger->info("msg:%s",strInfo.c_str()));
}
