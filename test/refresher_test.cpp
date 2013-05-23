#include "refresher.h"
#include "gtest/gtest.h"

TEST(CRefresherTest,RFTEST)
{
	CRefresher	refresher;
	EXPECT_EQ(0u,refresher.RefreshPublicRadio());
}
