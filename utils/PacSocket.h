#pragma once

#include "comdef.h"

#include <string>
using namespace std;

#define ERROR_SOCKET_CREATE_FAILED	0x93000100
#define ERROR_SOCKET_BIND_FAILED	0x93000200
#define ERROR_SOCKET_LISTEN_FAILED	0x93000300
#define ERROR_SOCKET_ACCEPT_FAILED	0x93000400
#define ERROR_SOCKET_CONNECT_FAILED	0x93000500
#define ERROR_SOCKET_SEND_FAILED	0x93000600
#define ERROR_SOCKET_RECV_FAILED	0x93000700


class CPacSocket
{
public:
	CPacSocket();
	~CPacSocket();

	UINT Socket_Listen(UINT uServPort, UINT uConnNum, int& iSockfd);
	UINT Socket_Accept(int iSockfd, int& iClientSockfd);
	
	UINT Socket_Connect(string strHostIpAddr, UINT uServPort, int& iSockfd);

	UINT Socket_Send(int iSockfd, void* data, size_t size);
	UINT Socket_Recv(int iSockfd, void* buff, size_t size);

	void Socket_Close(int iSockfd);
};
