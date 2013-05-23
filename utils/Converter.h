#pragma once 

#include <string>
using namespace std;

#define	UINTDECLENGTH			32

class CConverter
{
public:
	CConverter();
	~CConverter();

	static string Uint2String(unsigned int uSou);
};
