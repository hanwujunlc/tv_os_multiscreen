#ifndef __DEUDPSERVICE_CLIENT_H_
#define __DEUDPSERVICE_CLIENT_H_
#include "CoocaaDef.h"
#include "IDEServiceClient.h"
#include "MulticastSocket.h"

NS_COOCAA_BEGIN

class DEUDPServiceClient :public DEServiceClient
{
public:
	DEUDPServiceClient(std::string clientName);
	virtual ~DEUDPServiceClient();

	void init();
	void cleanup();
protected:
	void findSPs(int timeout = 1000u, const bool connectFirst = false);
	bool connectService(ServiceProvideInfo sp, int timeout);
	void disconnectService(ServiceProvideInfo sp);
	void handleMessage(DatagramPacket packet);

private:
	MulticastSocket* _multicastsocket;
	static const int ROUTE_LISTEN_PORT = 2015;
	SocketAddress _multisocketaddress;

	bool find(std::vector<ServiceProvideInfo>::iterator beg, std::vector<ServiceProvideInfo>::iterator end, ServiceProvideInfo info);
};

NS_COOCAA_END
#endif//__DEUDPSERVICE_CLIENT_H_