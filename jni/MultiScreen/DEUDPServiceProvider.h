#ifndef __DEUDPSERVICE_PROVIDER_H_
#define __DEUDPSERVICE_PROVIDER_H_
#include "CoocaaDef.h"
#include "IDEServiceProvider.h"
#include "MulticastSocket.h"

#include <thread>
#include <condition_variable>


NS_COOCAA_BEGIN
class DEUDPServiceProvider:public IDEServiceProvider
{
public:
	DEUDPServiceProvider(const std::string& providerName, const  std::string& version, const  std::string& devicetype);
	virtual ~DEUDPServiceProvider();
	
private:
	virtual void startProvider();
	virtual void endProvider();

	void joinGroupAndBroadcast();
	//void startRouteListen();

private:
	std::string _deviceType;
	MulticastSocket *_multicastsocket;
	UDPSocket* _broadcastsocket;

	static const int ROUTE_LISTEN_PORT = 2015;
	SocketAddress _multisocketaddress;

	//SocketAddress _broadcastlistenddress;

	std::condition_variable _syncRoute;
	std::condition_variable _syncBoradCast;

	bool _endBroadCast;
	bool _endRouteListen;
	
	void handleMessage(DatagramPacket packet);
};

NS_COOCAA_END

#endif //__DEUDPSERVICE_PROVIDER_H_