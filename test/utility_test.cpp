#include "gtest/gtest.h"
#include "Utility.h"

TEST(CUtilityTest,TrimTest)
{
	EXPECT_EQ("abcde	",CUtility::ltrim("  	abcde	"));
	EXPECT_EQ("  abcde",CUtility::rtrim("  abcde	"));
	EXPECT_EQ("abcde",CUtility::trim("  	abcde	"));
	EXPECT_EQ("abc",CUtility::trim(" 	\r \n	abc	 	\r \n	 "));
	EXPECT_EQ("\\rabc",CUtility::trim(" 	\r \n	\\rabc	 	\r \n	 "));

}
