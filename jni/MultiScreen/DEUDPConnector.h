#ifndef __DEUDPCONNECTOR_H_
#define __DEUDPCONNECTOR_H_
#include "CoocaaDef.h"
#include "IDEConnector.h"
#include "UDPSocket.h"
#include <condition_variable>
#include <thread>

NS_COOCAA_BEGIN

class DEUDPConnector : public IDEConnector
{
public:
	DEUDPConnector();
	virtual ~DEUDPConnector();

	int connect(coocaa::SocketAddress &target);
	int disconnect(coocaa::SocketAddress &target);
	bool isConnected();
	unsigned int send(const coocaa::SocketAddress & target, const char* data, const unsigned int dataLen);
	void cleanup();
	void init();

	UDPSocket* getSocket();
private:
	std::string _targetnickname;
	SocketAddress _targetAddress;

	bool _endthread;
	UDPSocket* _udpSocket;

	std::condition_variable _sync;
};

NS_COOCAA_END
#endif//__DEUDPCONNECTOR_H_
