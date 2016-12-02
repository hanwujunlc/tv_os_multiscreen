#ifndef __IDESERVICE_CLIENT_H_
#define __IDESERVICE_CLIENT_H_

#include "CoocaaDef.h"
#include "ISocket.h"
#include "IDEService.h"
#include <map>
#include <string>
#include <vector>
#include "ServiceProvideInfo.h"

NS_COOCAA_BEGIN

class DEServiceClientListener
{
public:
	virtual void onSearchingSPFinished(const  std::vector<ServiceProvideInfo> & spInfos) = 0;
	virtual void onConnected(const ServiceProvideInfo& spInfo) = 0;
	virtual void onDisconnected(const ServiceProvideInfo& spInfo) = 0;
	virtual void onConnectedRefused(const ServiceProvideInfo& spInfo) = 0;
};

class DEServiceClient :public IDETargetAliveListener
{
public:
	DEServiceClient(std::string clientName);
	virtual ~DEServiceClient();
	void addService(DEService* service);
	void init();
	void cleanup();
	void setDEServiceClientListener(DEServiceClientListener* listener);
	bool connectSP(ServiceProvideInfo sp, int timeout);
	void findSPs();
	ServiceProvideInfo getConnectedSP();
	std::vector<ServiceProvideInfo> getFoundSPs();
	void disconnectSP(ServiceProvideInfo sp);
	virtual void onDead(coocaa::SocketAddress& target);
	virtual void onAlive(coocaa::SocketAddress& target);

protected:
	DEServiceClientListener* _listener;
	virtual void findSPs(int timeout = 1000u, const bool connectFirst = false) = 0;
	virtual bool connectService(ServiceProvideInfo sp,int timeout) = 0;
	virtual void disconnectService(ServiceProvideInfo sp ) = 0;

	bool isContainsService(std::string serviceName);
	DEService* getService(std::string serviceName);

	std::vector<ServiceProvideInfo> _foundedSps;

private:
	std::string _clientName;
	ServiceProvideInfo _connectedSP;

	std::map<std::string, DEService*> _serviceMap;

	bool _endThread;
	bool _searchFlag;
};

NS_COOCAA_END

#endif//__IDESERVICE_CLIENT_H_