#include "UDPSocket.h"
#include <string>
#include "CooCaaLog.h"

#ifdef ANDROID
#include <errno.h>
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

sockaddr_in convertAddress(const SocketAddress& address)
{
	sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family      = AF_INET;
	if (address.getHostAddress() == "")
	{
		sin.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr(address.getHostAddress());
	}
	else
	{
		sin.sin_addr.s_addr = inet_addr(address.getHostAddress().c_str());
	}
	sin.sin_port = htons(address.getPort());       // set the port
	return sin;
}

std::string NetUtil::getHostByName(const std::string &host)
{

#ifdef _WIN32
	WSADATA wsadata; 
	if(WSAStartup(MAKEWORD(1,1),&wsadata)!=0) 
	{ 
		return "";
	} 
#endif
	hostent *phostent; 
	char * addr ;
	if (host.length() != 0)
	{  
		phostent=gethostbyname(host.c_str());
		//phostent=gethostbyaddr(host.charContent(), 4, AF_INET);
		if (phostent == NULL)
		{
			return "";
		}
		if(phostent->h_addr_list[0] == NULL || phostent->h_addr_list[0] == std::string(""))
		{
			return "";
		}
		addr = inet_ntoa(*((in_addr*)phostent->h_addr_list[0]));  
		return addr;		
	}
	return "";
}


UDPSocket::UDPSocket():_socket(INVALID_SOCKET)
{
}

UDPSocket::~UDPSocket()
{
	close();
}

int UDPSocket::open(bool isStream)
{
	if (_socket != INVALID_SOCKET)
	{
		return -1;
	}
	//create UDP socket
#ifdef _WIN32
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested=MAKEWORD(1,1);
	int err=WSAStartup(wVersionRequested,&wsaData);
#endif

	_socket=::socket(AF_INET,SOCK_DGRAM,0);
	if(_socket<0)
	{
		COOCAALOGD("error occurs on IFWOSUDPSocketImp::open, error:%s\n", strerror(errno));
		return -1;
	}
	return 0;
}

int UDPSocket::bind(const SocketAddress& address)
{

	if (_socket == INVALID_SOCKET)
	{
		return -1;
	}

	unsigned int yes = 1;
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&yes, sizeof(yes)) < 0 )
	{
		COOCAALOGD("error occurs on IFWOSUDPSocketImp::setsockopt SO_REUSEADDR, error:%s\n", strerror(errno));
#ifdef _WIN32
		::closesocket(_socket);
#elif ANDROID
		::close(_socket);
#endif
		return -1;
	}

	sockaddr_in sin = convertAddress(address);
	if(::bind(_socket,(struct sockaddr*)&sin,sizeof(sin))<0)		
	{
		COOCAALOGD("error occurs on IFWOSUDPSocketImp::bind, error:%s\n", strerror(errno));
		return -1;
	}
	return  0;

}

int UDPSocket::close()
{
	if (_socket != INVALID_SOCKET)
	{
#ifdef _WIN32
		int result = ::closesocket(_socket);
#elif ANDROID
		int result = ::close(_socket);
#endif
		if(result==SOCKET_ERROR)
		{ 
			COOCAALOGD("error occurs on IFWOSUDPSocketImp::close, error:%s\n", strerror(errno));
			return -1;
		}
		_socket = INVALID_SOCKET;
	}
	return 0;

}

void UDPSocket::setTimeOut(unsigned int time)
{
	setsockopt( _socket, SOL_SOCKET, SO_RCVTIMEO, ( char * )&time, sizeof( unsigned int));
}

int UDPSocket::setSendBufferSize(const unsigned int bufferSize)
{
	int result = setsockopt(_socket,SOL_SOCKET,SO_SNDBUF,(const char *)&bufferSize,sizeof(unsigned int));
	if (result < 0)
	{
		COOCAALOGD("error occurs on IFWOSUDPSocketImp::setSendBufferSize, error:%s\n", strerror(errno));
		return -1;
	}
	return 0;
}

int UDPSocket::setReceiveBufferSize(const unsigned int bufferSize)
{
	int result = setsockopt(_socket,SOL_SOCKET,SO_RCVBUF,(const char *)&bufferSize,sizeof(unsigned int));
	if (result < 0)
	{
		COOCAALOGD("error occurs on IFWOSUDPSocketImp::setReceiveBufferSize, error:%s\n", strerror(errno));
		return -1;
	}
	return 0;
}

bool UDPSocket::getSendBufferSize(unsigned int& bufferSize)
{
#ifdef _WIN32
	int len = sizeof(unsigned int);
#elif ANDROID
	socklen_t len = sizeof(unsigned int);
#endif // _WIN32

	
	int result = getsockopt(_socket,SOL_SOCKET,SO_SNDBUF,(char *)&bufferSize,&len);
	if (result < 0)
	{
		COOCAALOGD("error occurs on IFWOSUDPSocketImp::getSendBufferSize, error:%s\n", strerror(errno));
		return -1;
	}
	return 0;
}

bool UDPSocket::getReceiveBufferSize(unsigned int& bufferSize)
{
#ifdef _WIN32
	int len = sizeof(unsigned int);
#elif ANDROID
	socklen_t len = sizeof(unsigned int);
#endif // _WIN32
	int result = getsockopt(_socket,SOL_SOCKET,SO_RCVBUF,(char *)&bufferSize,&len);
	if (result < 0)
	{
		COOCAALOGD("error occurs on IFWOSUDPSocketImp::getReceiveBufferSize, error:%s\n", strerror(errno));
		return false;
	}
	return true;
}

unsigned int UDPSocket::send(const DatagramPacket& packet, const bool nodelay)
{
	sockaddr_in sin = convertAddress(packet.getAddress());
	return ::sendto(_socket,reinterpret_cast<const char*>(packet.getData()),packet.getSize(),0, (struct sockaddr*)&sin,sizeof(sockaddr_in));
}

SocketAddress UDPSocket::getSocketAddress()
{
	sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
#ifdef _WIN32
	int len = sizeof(sin);
#elif ANDROID
	socklen_t len = sizeof(sin);
#endif // _WIN32
	int result = ::getsockname(_socket, (sockaddr*)&sin, &len); 
	if (result < 0)
	{        
		COOCAALOGD("error occurs on IFWOSUDPSocketImp::getLocalAddress, error:%s\n", strerror(errno));
		return SocketAddress();
	}
	return SocketAddress(inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));
}

unsigned int UDPSocket::receive(DatagramPacket& packet, const unsigned int timeout)
{
	struct timeval newTimeOut;
	newTimeOut.tv_sec = timeout/1000;      //seconds
	newTimeOut.tv_usec = 0; //microseconds

	struct timeval oldTimeOut = {0,0};
	if (timeout != NetUtil::TIMEOUT_INFINITE)
	{
#ifdef _WIN32
	int len = sizeof(unsigned int);
#elif ANDROID
	socklen_t len = sizeof(unsigned int);
#endif // _WIN32
		getsockopt(_socket, SOL_SOCKET, SO_RCVTIMEO, ( char * )&oldTimeOut, &len);
		setsockopt( _socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&newTimeOut, sizeof(timeval) );
	}

	sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));

#ifdef _WIN32
	int size = sizeof(sin);
#elif ANDROID
	socklen_t size = sizeof(sin);
#endif // _WIN32
	
	int byteReceived=::recvfrom(_socket,(char*)packet.getBuffer(),packet.getSize(),0,(struct sockaddr*)&sin,&size);
	packet.setAddress(SocketAddress(inet_ntoa(sin.sin_addr), ntohs(sin.sin_port)));
	if (oldTimeOut.tv_sec > 0 || oldTimeOut.tv_usec > 0)
	{
		if (timeout != NetUtil::TIMEOUT_INFINITE)
		{
			setsockopt( _socket, SOL_SOCKET, SO_RCVTIMEO, ( char * )&oldTimeOut, sizeof( timeval ) );
		}
	}
	if (byteReceived == -1 || byteReceived < 0)
	{
		COOCAALOGD("error occurs on IFWOSUDPSocketImp::receive, error:%s\n", strerror(errno));
		return 0;
	}
	packet.setLength(byteReceived);
	return byteReceived;
}

