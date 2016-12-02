#ifndef __IDESERVICE_H_
#define __IDESERVICE_H_

#include "CooCaaLog.h"
#include "ISocket.h"
#include "DEData.h"
#include <string>
#include <map>
#include "IDEConnector.h"
#include <thread>
#include <condition_variable>

NS_COOCAA_BEGIN

class IDEServiceListener
{
public:
	virtual ~IDEServiceListener() {}
	virtual void onServiceActived(coocaa::SocketAddress & target) = 0;
	virtual void onServiceDeactived(coocaa::SocketAddress & target) = 0;
};

class IDETargetAliveListener {
public:
	virtual ~IDETargetAliveListener() {}
	virtual void onDead(coocaa::SocketAddress& target) = 0;
	virtual void onAlive(coocaa::SocketAddress& target) = 0;
};


class MobileTarget {
public:
	std::string name;
	std::string ip;
	unsigned int port;
	unsigned int cnt;

	MobileTarget(){}

	MobileTarget(const MobileTarget& src)
	{
		if(&src != this)
		{
			name = src.name;
			ip = src.ip;
			port = src.port;
			cnt = src.cnt;
		}
	}

	MobileTarget& operator = (const MobileTarget& src)
	{
		if (&src != this)
		{
			name = src.name;
			ip = src.ip;
			port = src.port;
			cnt = src.cnt;
		}
		return *this;
	}

	MobileTarget(std::string name, std::string ipaddr, unsigned int iPort) {
		this->name = name;
		this->ip = ipaddr;
		this->port = iPort;
		this->cnt = 0;
	}
};

class DEService: public IDEConnectorListener
{
public:
	DEService(const std::string& name);
	virtual ~DEService();
	
	void init();
	void cleanup();

	void onDisconnect(coocaa::SocketAddress &target);
	void onConnect(coocaa::SocketAddress &target);

	void onReceiveData(coocaa::SocketAddress &target, const char* data, unsigned int dataLen);
	virtual void processData(coocaa::SocketAddress  & target,coocaa::DEData & revData)= 0;

	void setListener(IDEServiceListener* listener);
	void setTargetAliveListener(IDETargetAliveListener* aliveListener);
	std::string getServiceName();

	void addMobileTarget(const std::string & ip, coocaa::SocketAddress & addr);
	void removeMobileTarget(const std::string & ip);
	unsigned int getMobileConnectorCount();
	bool  isExist(const std::string & ip);

	IDEConnector* getConnector();

protected:
	IDEServiceListener* _serviceListener;

	unsigned int sendData(const char* data, const unsigned int len);
	unsigned int sendData(const SocketAddress & target, const char* data,const unsigned int dataLen);

private:
	IDEConnector* _conn;
	std::string _serviceName;
	std::map<std::string,MobileTarget> _mobiesMap;

	IDETargetAliveListener* _aliveListener;

	std::mutex _mutex;
	std::condition_variable _sync;
	bool _endThread;

	void checkTargetTicks();
	void handleAliveCheck(const std::string & ip, unsigned int port);

};

NS_COOCAA_END
#endif//__IDESERVICE_H_
