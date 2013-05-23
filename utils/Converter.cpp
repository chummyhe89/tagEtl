#include "Converter.h"

#include <stdio.h>

CConverter:: CConverter()
{
}
CConverter:: ~CConverter()
{
}

string CConverter:: Uint2String(unsigned int uSou)
{
	char cDis[UINTDECLENGTH] 	= {'\0'};
	
	if((int) uSou < 0)
		sprintf(cDis, "0x%x", uSou);
	else
		sprintf(cDis, "%u", uSou);
	return string(cDis);
}
