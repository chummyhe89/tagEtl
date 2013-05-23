#include "refresher.h"
#include <cstring>

CRefresher::CRefresher()
{
	GetRefreshSetting(m_rfParam);
}
UINT CRefresher::RefreshPublicRadio()
{
	UINT	uRet	= 0;
	int	iSockfd	= 0;
	char*	pResponse	=NULL;
	CPacSocket	pSocket;

	if((uRet = pSocket.Socket_Connect(m_rfParam.strIp,m_rfParam.iPort,iSockfd)) != 0)
		return uRet;
	if((uRet = pSocket.Socket_Send(iSockfd,(void*)(m_rfParam.strMsg.c_str()),m_rfParam.strMsg.size()+1)) != 0)
		return uRet;
	
	pResponse = new char[REF_RESP_SIZE];
	
	if((uRet = pSocket.Socket_Recv(iSockfd,pResponse,REF_RESP_SIZE)) != 0)
	{
		pSocket.Socket_Close(iSockfd);
		return uRet;
	}
	pSocket.Socket_Close(iSockfd);
	if(!strcmp(pResponse,REF_SONGQUEUE_OK))
		uRet = 0;
	else if(!strcmp(pResponse,REF_SONGQUEUE_FAILED))
		uRet = ERROR_REF_FAILED;
	else
		uRet = ERROR_REF_INVALID_MSG;
	return uRet;
}

UINT CRefresher::GetRefreshSetting(RefreshParam& rp)
{
	UINT	uRet	= 0;
	CProfile	prof;
	if((uRet = prof.LoadProfile(SYS_CONFIG_POS)) != 0)
		return uRet;
	if((uRet = prof.GetProfileStringW(REFRESH_SETTING,SERVER_IP,rp.strIp) != 0))
		return uRet;
	if((uRet = prof.GetProfileIntW(REFRESH_SETTING,SERVER_PORT,rp.iPort) != 0))
		return uRet;
	if((uRet = prof.GetProfileStringW(REFRESH_SETTING,SEND_MSG,rp.strMsg) != 0))
		return uRet;
	return uRet;
}
