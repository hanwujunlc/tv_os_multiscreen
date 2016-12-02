#include "MulticastSocket.h"
#include "CooCaaLog.h"
#include <errno.h>
#ifdef ANDROID
#include<netdb.h> 
#include<sys/types.h> 
#include<netinet/in.h> 
#include<sys/socket.h> 
#include<unistd.h> 
#include<arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#endif

USING_NS_COOCAA;

int MulticastSocket::setTTL(const unsigned int ttl)
{
	int ret = setsockopt(_socket,IPPROTO_IP,IP_MULTICAST_TTL,(char*)&ttl,sizeof(unsigned int));
	if ( SOCKET_ERROR == ret )
	{
		COOCAALOGD("error occurs on IFWOSUDPSocketImp::setTTL, error:%s\n", strerror(errno));
		return -1;
	}
	return 0;
}

int MulticastSocket::joinGroup(const SocketAddress & groupAddr, const SocketAddress & localAddr)
{
	struct ip_mreq  mreq;
#ifdef _WIN32
	int len = sizeof(mreq);
#elif ANDROID
	socklen_t len = sizeof(mreq);
#endif // _WIN32

	memset(&mreq,0x00,sizeof(mreq));
	mreq.imr_multiaddr.s_addr = inet_addr(groupAddr.getHostAddress().c_str());
	mreq.imr_interface.s_addr = inet_addr(localAddr.getHostAddress().c_str());
	if(setsockopt( _socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, len) < 0 )
	{
		//int lasterror = WSAGetLastError();
		COOCAALOGD("error occurs on IFWOSUDPSocketImp::joinGroup, error:%s\n", strerror(errno));
		return -1;
	}
	return 0;

}

int MulticastSocket::joinGroup(const SocketAddress& address)
{
	struct ip_mreq  mreq;
#ifdef _WIN32
	int len = sizeof(mreq);
#elif ANDROID
	socklen_t len = sizeof(mreq);
#endif // _WIN32
	memset(&mreq,0x00,sizeof(mreq));
	mreq.imr_multiaddr.s_addr = inet_addr(address.getHostAddress().c_str());
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	if(setsockopt( _socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, len) < 0 )
	{
		//warning:use WSAStartup(1.1) ,must link wsock32,not ws2_32.lib,or will receive 10042 error!!
		//int lasterror = WSAGetLastError();
		COOCAALOGD("error occurs on IFWOSUDPSocketImp::joinGroup, IP_ADD_MEMBERSHIP = 5 , error:%s\n", strerror(errno));
	}
	return 0;
}

int MulticastSocket::leaveGroup(const SocketAddress &address)
{
	struct ip_mreq  mreq;
#ifdef _WIN32
	int len = sizeof(mreq);
#elif ANDROID
	socklen_t len = sizeof(mreq);
#endif // _WIN32
	mreq.imr_multiaddr.s_addr = inet_addr(address.getHostAddress().c_str());
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	if(setsockopt( _socket, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&mreq, len) < 0 )
	{
		COOCAALOGD("error occurs on IFWOSUDPSocketImp::leaveGroup, error:%s\n", strerror(errno));
		return -1;
	}
	return 0;
}

unsigned int MulticastSocket::getIpAddressCount()
{
	int *addrList = nullptr;
	unsigned  cnt = getLocalIPAddressList(&addrList);
	if (addrList != nullptr)
	{
		free(addrList);
		addrList = nullptr;
	}
	return cnt;
}

std::string MulticastSocket::getIpAddressByIndex(unsigned int index)
{
	int *addrList = nullptr;
	unsigned int cnt = getLocalIPAddressList(&addrList);

	if (cnt >= 1)
	{
		if (index>=cnt)
		{
			return std::string("");
		}

		in_addr in;
		in.s_addr = addrList[index];
		char *a = inet_ntoa(in);
		std::string  ret(a);
		if (addrList != nullptr)
		{
			free(addrList);
			addrList = nullptr;
		}
		return ret;
	}
	else
	{
		return std::string("");
	}
}

unsigned int MulticastSocket::getLocalIPAddressList(int **pp_int)
{
#ifdef ANDROID
	char szBuffer[16*sizeof(struct ifreq)];
	struct ifconf ifConf;
	struct ifreq ifReq;
	int nResult;
	int LocalSock;
	struct sockaddr_in LocalAddr;
	int tempresults[16];
	unsigned int ctr=0;
	int i;
	/* Create an unbound datagram socket to do the SIOCGIFADDR ioctl on. */
	if ((LocalSock = ::socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		COOCAALOGD("error occurs on IFWOSUDPSocketImp::getLocalIPAddressList, error:%s\n", strerror(errno));
		return 0u;
	}
	/* Get the interface configuration information... */
	ifConf.ifc_len = sizeof szBuffer;
	ifConf.ifc_ifcu.ifcu_buf = (caddr_t)szBuffer;
	nResult = ioctl(LocalSock, SIOCGIFCONF, &ifConf);
	if (nResult < 0)
	{
		COOCAALOGD("error occurs on IFWOSUDPSocketImp::getLocalIPAddressList, error:%s\n", strerror(errno));
		return ctr;
	}
	/* Cycle through the list of interfaces looking for IP addresses. */
	for (i = 0;(i < ifConf.ifc_len);)
	{
		struct ifreq *pifReq = (struct ifreq *)((caddr_t)ifConf.ifc_req + i);
		i += sizeof *pifReq;
		/* See if this is the sort of interface we want to deal with. */
		strcpy (ifReq.ifr_name, pifReq -> ifr_name);
		if (ioctl (LocalSock, SIOCGIFFLAGS, &ifReq) < 0)
		{
			COOCAALOGD("error occurs on IFWOSUDPSocketImp::getLocalIPAddressList, error:%s\n", strerror(errno));
			return ctr;
		}
		/* Skip loopback, point-to-point and down interfaces, */
		/* except don't skip down interfaces */
		/* if we're trying to get a list of configurable interfaces. */
		if ((ifReq.ifr_flags & IFF_LOOPBACK) || (!(ifReq.ifr_flags & IFF_UP)))
		{
			continue;
		}	
		if (pifReq -> ifr_addr.sa_family == AF_INET)
		{
			/* Get a pointer to the address... */
			memcpy (&LocalAddr, &pifReq -> ifr_addr, sizeof pifReq -> ifr_addr);
			if (LocalAddr.sin_addr.s_addr != htonl (INADDR_LOOPBACK))
			{
				tempresults[ctr] = LocalAddr.sin_addr.s_addr;
				++ctr;
			}
		}
	}
	::close(LocalSock);
	*pp_int = (int*)malloc(sizeof(int)*(ctr));
	memcpy(*pp_int,tempresults,sizeof(int)*ctr);
	return ctr;

#elif _WIN32
	//no implement
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2,2);
	err = WSAStartup(wVersionRequested,&wsaData);

	unsigned int ctr=0;
	if(err!=0)
	{
		COOCAALOGD("error occurs on IFWOSMulticastSocketImp::getLocalIPAddressList, WSAStartup failed !");
		return 0u;
	}
	char szhn[256];
	int nStatus = gethostname(szhn, sizeof(szhn));
	if(nStatus == SOCKET_ERROR)
	{
		COOCAALOGD("error occurs on IFWOSMulticastSocketImp::getLocalIPAddressList, code:%d\n", ::WSAGetLastError());
		return 0u;
	}

	HOSTENT *host = gethostbyname(szhn); 

	int tempresults[16]; // most ip address is 16
	if(host!=nullptr)
	{
		for (int i = 0; host->h_addr_list[i] != 0; ++i) {
			struct in_addr addr;
			memcpy(&addr, host->h_addr_list[i], sizeof(struct in_addr));
			tempresults[ctr] =addr.S_un.S_addr;
			++ctr;
		}
		//ipaddress = inet_ntoa( *(IN_ADDR*)host->h_addr_list[0]); 
	}
	*pp_int = (int*)malloc(sizeof(int)*(ctr));
	memcpy(*pp_int,tempresults,sizeof(int)*ctr);

	WSACleanup();

	return ctr;
#endif
}