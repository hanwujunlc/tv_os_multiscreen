#include "TCPSocket.h"
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

TCPSocket::TCPSocket() :_socket(INVALID_SOCKET), remoteAddress()
{

}
TCPSocket::TCPSocket(SOCKET sck,const SocketAddress &address) : _socket(sck), remoteAddress(address)
{

}

TCPSocket::~TCPSocket()
{

}

int TCPSocket::open(bool isStream)
{
	if (_socket!= INVALID_SOCKET)
	{
		return -1;
	}
	//create tcp socket

#ifdef _WIN32
	//window 打开socket接口
	WORD wVersionRequest;
	WSADATA wsaData;
	wVersionRequest = MAKEWORD(1,1);
	int err = WSAStartup(wVersionRequest, &wsaData);
	COOCAALOGD("on window start wsa %d\n",err);
#endif
	_socket = ::socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (_socket < 0)
	{
		COOCAALOGD("socket open error:%s\n",strerror(errno));//strerror(errno)全局上一个调用错误
		return -2;
	}
	return 0;
}



int TCPSocket::close()
{
	if (_socket!=INVALID_SOCKET)
	{
#ifdef _WIN32
		int result = ::closesocket(_socket);
#elif ANDROID
		::shutdown(_socket, SHUT_RDWR);
		int result = ::close(_socket);
		
#endif // _WIN32
		COOCAALOGD("@tcpsocket,tcp close result:%d", result);
		if (result==SOCKET_ERROR)
		{
			return SOCKET_ERROR;
		}
		return result;
	}
	return 0;
}

sockaddr_in TCPSocket::convertTCPAddress(const SocketAddress& address)
{
	sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	if (address.getHostAddress() =="")
	{
		sin.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		sin.sin_addr.s_addr = inet_addr(address.getHostAddress().c_str());
	}
	sin.sin_port = htons(address.getPort());
	return sin;
}

int TCPSocket::bind(const SocketAddress& address)
{
	if (_socket==INVALID_SOCKET)
	{
		return -1;
	}
	sockaddr_in sin = convertTCPAddress(address);
	if (::bind(_socket,(struct sockaddr*)&sin,sizeof(sin))<0)
	{
		COOCAALOGD("tcp socket bind error,%s\n",strerror(errno));
		return -1;
	}
	return 0;
}

void TCPSocket::setTimeOut(unsigned int time)
{
	setsockopt(_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&time, sizeof(unsigned int));
}

int TCPSocket::setSendBufferSize(const unsigned int bufferSize)
{
	int result = setsockopt(_socket, SOL_SOCKET, SO_SNDBUF, (const char*)&bufferSize, sizeof(unsigned int));
	if (result<0)
	{
		COOCAALOGD("set send buffer error:%s\n",strerror(errno));
		return -1;
	}
	return 0;
}

bool TCPSocket::getSendBufferSize(unsigned int& bufferSize)
{
#ifdef _WIN32
	int len = sizeof(unsigned int);
#elif ANDROID
	socklen_t len = sizeof(unsigned int);
#endif
	int result = getsockopt(_socket, SOL_SOCKET, SO_SNDBUF,(char*)&bufferSize,&len);
	if (result<0)
	{
		COOCAALOGD("get send buffer error:%s",strerror(errno));
		return false;
	}
	return true;
}

int TCPSocket::setReceiveBufferSize(const unsigned int bufferSize)
{
	int result = setsockopt(_socket, SOL_SOCKET, SO_RCVBUF, (char*)&bufferSize, sizeof(unsigned int));
	if (result<0)
	{
		COOCAALOGD("set receiver buffer error:%s\n",strerror(errno));
		return -1;
	}
	return 0;
}

bool TCPSocket::getReceiveBufferSize(unsigned int& bufferSize)
{
#ifdef _WIN32
	int len = sizeof(unsigned int);
#elif ANDROID
	socklen_t len = sizeof(unsigned int);
#endif
	int result = getsockopt(_socket, SOL_SOCKET, SO_RCVBUF, (char*)&bufferSize, &len);
	if (result<0)
	{
		COOCAALOGD("get receive buffer size error:%s\n",strerror(errno));
		return false;
	}
	return true;
}

SocketAddress TCPSocket::getSocketAddress()
{
	sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
#ifdef _WIN32
	int len = sizeof(sin);
#elif ANDROID
	socklen_t len = sizeof(sin);
#endif
	int result = ::getsockname(_socket, (sockaddr*)&sin, &len);
	if (result<0)
	{
		COOCAALOGD("get socket address error:%s\n",strerror(errno));
		return SocketAddress();
	}
	return SocketAddress(inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));
}

SocketAddress TCPSocket::getRemoteAddress()
{
	return remoteAddress;
}

unsigned int TCPSocket::send(const DatagramPacket& datapacket)
{
	unsigned int result = ::send(_socket, reinterpret_cast<const char*>(datapacket.getData()),datapacket.getSize(),0);
	COOCAALOGD("send data result:%d\n",result);
	return result;
}

int TCPSocket::receive(DatagramPacket& data, const unsigned timeout /* = 0xFFFFFFFFu */)
{
	struct timeval timeoutStruct;
	timeoutStruct.tv_sec = timeout / 1000;
	timeoutStruct.tv_usec = 0;
	struct timeval oldTimeOut = {0,0};
	if (timeout!=NetUtil::TIMEOUT_INFINITE)
	{
#ifdef _WIN32
		int nNetTimeout = timeout;
		setsockopt(_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));
#elif ANDROID
		socklen_t len = sizeof(unsigned int);
		setsockopt(_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeoutStruct, sizeof(timeval));
#endif
	}
	
	
    int byteReceived = ::recv(_socket, reinterpret_cast<char*>(data.getBuffer()), data.getSize(), 0);
	if (oldTimeOut.tv_sec>0 || oldTimeOut.tv_usec>0)
	{
		setsockopt(_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&oldTimeOut, sizeof(timeval));
	}
	if (byteReceived == -1 || byteReceived < 0)
	{
		COOCAALOGD("error occurs on TCPSocketImp::receive, error:%s\n", strerror(errno));
		COOCAALOGD("error occurs on TCPSocketImp::receive, error:%d\n", (errno));
		COOCAALOGD("error occurs on TCPSocketImp::receive, byteReceived:%d\n", (byteReceived));
		return byteReceived;
	}
	data.setLength(byteReceived);
	return byteReceived;
}

int TCPSocket::connect(const SocketAddress& address)
{
	sockaddr_in sin = convertTCPAddress(address);
	remoteAddress = address;

	if (::connect(_socket,(struct sockaddr*)&sin,sizeof(sin))<0)
	{
		COOCAALOGD("tcp connect error:%s\n",strerror(errno));
		return -1;
	}
	return 0;
}
int TCPSocket::disconnect()
{
	int result = close();
	if (result==0)
	{
		result = open(false);
	}
	return result;
}

TCPServerSocket::TCPServerSocket()
{

}
TCPServerSocket::~TCPServerSocket()
{

}
int TCPServerSocket::bind(const SocketAddress& address)
{
	if (_socket == INVALID_SOCKET)
	{
		return -1;
	}
	sockaddr_in sin = converServerTCPAddress(address);
	if (::bind(_socket, (struct sockaddr*)&sin, sizeof(sin)) < 0)
	{
		COOCAALOGD("tcp socket bind error,%s\n", strerror(errno));
		return -1;
	}
	return 0;
}

int TCPServerSocket::listen()
{
	int result = ::listen(_socket, _MAX_SOCKET_STREAM_LISTEN);
	if (result<0)
	{
		COOCAALOGD("listen error:%s\n",strerror(errno));
		return -1;
	}
	return 0;
}
ITCPSocket* TCPServerSocket::accept()
{
	sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
#ifdef _WIN32
	int len = sizeof(sin);
#elif ANDROID
	socklen_t len = sizeof(sin);
#endif 
	SOCKET socket_client = ::accept(_socket, (struct sockaddr*)&sin, &len);
	ITCPSocket* clientSocket = new TCPSocket(socket_client, SocketAddress(inet_ntoa(sin.sin_addr), ntohs(sin.sin_port)));
	return clientSocket;
}
int TCPServerSocket::setReuseAddr()
{
	int opt = 1;
	int len = sizeof(opt);
	int result = ::setsockopt(_socket, SOL_SOCKET,SO_REUSEADDR,(char*)&opt,len);
	if (result<0)
	{
		COOCAALOGD("set send buffer error:%s\n",strerror(errno));
		return -1;
	}
	return 0;
}

sockaddr_in TCPServerSocket::converServerTCPAddress(const SocketAddress& address)
{
	sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	if (address.getHostAddress() == "")
	{
		sin.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		sin.sin_addr.s_addr = inet_addr(address.getHostAddress().c_str());
	}
	sin.sin_port = ntohs(address.getPort());
	return sin;
}

