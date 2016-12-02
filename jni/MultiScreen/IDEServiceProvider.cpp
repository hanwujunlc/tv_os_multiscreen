#include "IDEServiceProvider.h"
#include "MulticastSocket.h"

USING_NS_COOCAA;

IDEServiceProvider::IDEServiceProvider(const std::string & providerName, const std::string & version):_spVersion(version), _spName(providerName),_aliveListener(nullptr)
{

}

IDEServiceProvider::~IDEServiceProvider()
{

}

int IDEServiceProvider::addService(DEService* const service)
{
	if(service!=nullptr)
	{
		_serviceMap.insert(std::make_pair(service->getServiceName(),service));
		service->init();
		if(_aliveListener!=nullptr)
		{
			service->setTargetAliveListener(_aliveListener);
		}
	}
	return 1;
}

void IDEServiceProvider::setProviderName(std::string name)
{
	_spName = name;
}

bool IDEServiceProvider::isContainsService(std::string serviceName)
{
	if(_serviceMap.size()>0)
	{
		std::map<std::string,DEService*>::iterator it=_serviceMap.begin();
		for(it;it != _serviceMap.end();++it)
		{
			if (it->first.compare(serviceName) == 0)
			{
				return true;
			}
		}
	}

	return false;
}

DEService* IDEServiceProvider::getService(std::string serviceName)
{
	if(_serviceMap.size()>0)
	{
		std::map<std::string,DEService*>::iterator it=_serviceMap.begin();
		for(it;it != _serviceMap.end();++it)
		{
			if (it->first.compare(serviceName) == 0)
			{
				return it->second;
			}
		}
	}

	return nullptr;
}

void IDEServiceProvider::init()
{
	//listen(); join group,listen find sp

	startProvider();
}

void IDEServiceProvider::cleanup()
{
	endProvider();
	if(_serviceMap.size()>0)
	{
		std::map<std::string,DEService*>::iterator it=_serviceMap.begin();
		for(it;it != _serviceMap.end();)
		{
			std::map<std::string,DEService*>::iterator it_tmp = it;
			bool is_first_element = false;
			if(it_tmp != _serviceMap.begin())
				it_tmp --;
			else
				is_first_element = true;

			DEService* curService = (*it).second;
			curService->cleanup();

			_serviceMap.erase(it);
			if(is_first_element)
				it = _serviceMap.begin();
			else
				it = ++ it_tmp;
		}
	}
}

void IDEServiceProvider::setTargetAliveListener(IDETargetAliveListener* alivelisten)
{
	_aliveListener = alivelisten;
}
