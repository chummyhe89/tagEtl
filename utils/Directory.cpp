#include "Directory.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

CDirectory:: CDirectory()
{

}

CDirectory:: ~CDirectory()
{

}


UINT CDirectory:: MakeDir(string strPath)
{
	if(strPath.empty())
		return ERROR_INVALID_PARAMETER;

	if(IsDirExist(strPath))
		return ERROR_DIR_AREADLY_EXIST;

	AddDirEndSymb(strPath);

	if(mkdir(strPath.c_str(), S_IRWXU) != 0)
		return ERROR_DIR_CREATE;

	return S_OK;
}

UINT CDirectory:: RemoveDir(string strPath)
{
	if(strPath.empty())
		return ERROR_INVALID_PARAMETER;

	AddDirEndSymb(strPath);

	if(!IsDirExist(strPath))
		return ERROR_DIR_NOT_EXIST;

	if(rmdir(strPath.c_str()) != 0)
		return ERROR_DIR_REMOVE;

	return S_OK;
}


bool CDirectory:: IsEmptyDir(string strPath)
{
	DIR*	dp		= NULL;
	struct dirent* dirp	= NULL;

	if(strPath.empty() || !IsDirExist(strPath))
		return ERROR_INVALID_PARAMETER;

	AddDirEndSymb(strPath);
	
	if((dp = opendir(strPath.c_str())) == NULL)
		return ERROR_DIR_CANNOT_OPEN;

	while((dirp = readdir(dp)) != NULL)
	{
		if(strcmp((*dirp).d_name, ".") == 0 || strcmp((*dirp).d_name, "..") == 0)
			continue;
		else
			return false;
	}

	return true;
}

bool CDirectory:: IsDirExist(string strPath)
{
	if(strPath.empty())
		return false;

	AddDirEndSymb(strPath);

	if(opendir(strPath.c_str()) == NULL)
		return false;

	return true;
}

void CDirectory:: AddDirEndSymb(string& strPath)
{
	if(strPath[strPath.length() - 1] != '/')
		strPath = strPath + "/";
}
