#include "PacSocket.h"

#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

CPacSocket:: CPacSocket()
{

}

CPacSocket:: ~CPacSocket()
{

}

UINT CPacSocket:: Socket_Listen(UINT uServPort, UINT uConnNum, int& iSockfd)
{
	int 	itSockfd	= 0;
	struct 	sockaddr_in socin;
		
	// create socket
	if((itSockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return ERROR_SOCKET_CREATE_FAILED;

	// initialize socin
	bzero(&socin, sizeof(socin));
	socin.sin_family	= AF_INET;
	socin.sin_addr.s_addr 	= INADDR_ANY;
	socin.sin_port 		= htons(uServPort);

	// bind socket
	if(bind(itSockfd, (struct sockaddr* ) &socin, sizeof(socin)) == -1)
		return ERROR_SOCKET_BIND_FAILED;

	// listen socket 
	if(listen(itSockfd, uConnNum) == -1)
		return ERROR_SOCKET_LISTEN_FAILED;

	// return
	iSockfd = itSockfd;
	return S_OK;
}

UINT CPacSocket:: Socket_Accept(int iSockfd, int& iClientSockfd)
{
	socklen_t	socadd_size	= 0;
	int		itSockfd	= 0;
	struct  	sockaddr_in socin;

	// accept
	if((itSockfd = accept(iSockfd, (struct sockaddr* ) &socin, &socadd_size)) == -1)
		return ERROR_SOCKET_ACCEPT_FAILED;

	// return
	iClientSockfd = itSockfd;
	return S_OK;
}

	
UINT CPacSocket:: Socket_Connect(string strHostIpAddr, UINT uServPort, int& iSockfd)
{
	int 	itSockfd		= 0;
	struct 	sockaddr_in socin;

	// initialize socin
	bzero(&socin, sizeof(socin));
	socin.sin_family = AF_INET;
	inet_pton(AF_INET, strHostIpAddr.c_str(), &socin.sin_addr);
	socin.sin_port = htons(uServPort);

	// create socket
	if((itSockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return ERROR_SOCKET_CREATE_FAILED;

	// connect socket 
	if(connect(itSockfd, (struct sockaddr* ) &socin, sizeof(socin)) == -1)
		return ERROR_SOCKET_CONNECT_FAILED;

	// return
	iSockfd = itSockfd;
	return S_OK;
}


UINT CPacSocket:: Socket_Send(int iSockfd, void* data, size_t size)
{
	if(send(iSockfd, data, size, 0) == -1)
		return ERROR_SOCKET_SEND_FAILED;

	return S_OK;
}

UINT CPacSocket:: Socket_Recv(int iSockfd, void* buff, size_t size)
{
	if(recv(iSockfd, buff, size, 0) == -1)
		return ERROR_SOCKET_RECV_FAILED;

	return S_OK;
}


void CPacSocket:: Socket_Close(int iSockfd)
{
	close(iSockfd);
}
