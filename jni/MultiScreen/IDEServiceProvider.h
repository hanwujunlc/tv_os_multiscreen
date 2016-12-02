#ifndef __IDESERVICE_PROVIDER_H_
#define  __IDESERVICE_PROVIDER_H_

#include "CoocaaDef.h"
#include "ISocket.h"
#include "IDEService.h"
#include <map>

NS_COOCAA_BEGIN

class IDEServiceProvider
{
public:
	IDEServiceProvider(const std::string & providerName, const std::string & version);
	virtual ~IDEServiceProvider();
	int addService(DEService* const service);
	void init();

	void cleanup();
	void setTargetAliveListener(IDETargetAliveListener* alivelisten);

	void setProviderName(std::string name);
protected:

	virtual void startProvider() =0;
	virtual void endProvider() = 0;

	bool isContainsService(std::string serviceName);
	DEService* getService(std::string serviceName);

	std::string _spName;
	std::string _spVersion;

	IDETargetAliveListener* _aliveListener;

private:

	std::map<std::string,DEService*> _serviceMap;

};

NS_COOCAA_END

#endif//__IDESERVICE_PROVIDER_H_
