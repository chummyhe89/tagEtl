#pragma	once
#include "comdef.h"
#include "Profile.h"
#include "PacSocket.h"
#include <string>

#define 	ERROR_REF_FAILED	0x90000015
#define		ERROR_REF_INVALID_MSG	0x90000016
using namespace std;
typedef struct	_refreshParam
{
	int	iPort;
	string	strIp;
	string	strMsg;
}RefreshParam;

class	CRefresher
{
public:
	CRefresher();
	~CRefresher(){};
	UINT	RefreshPublicRadio();
	UINT	GetRefreshSetting(RefreshParam& rp);
private:
	RefreshParam	m_rfParam;	
};
