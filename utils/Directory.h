#pragma once

#include "comdef.h"

#include <string>
using namespace std;

#define ERROR_DIR_AREADLY_EXIST		0x94000001
#define ERROR_DIR_NOT_EXIST		0x94000101
#define ERROR_DIR_CREATE		0x94000100
#define ERROR_DIR_REMOVE		0x94000101
#define ERROR_DIR_CANNOT_OPEN		0x94000201

class CDirectory
{
public:
	CDirectory();
	~CDirectory();

	UINT MakeDir(string strPath);
	UINT RemoveDir(string strPath);

	bool IsEmptyDir(string strPath);
	bool IsDirExist(string strPath);

private:
	void AddDirEndSymb(string& strPath);
};
