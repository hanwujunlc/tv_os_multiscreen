#include "IDEServiceClient.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unistd.h>

USING_NS_COOCAA;

DEServiceClient::DEServiceClient(std::string clientName):_clientName(clientName), _endThread(true), _searchFlag(false)
{

}

DEServiceClient::~DEServiceClient()
{

}

void DEServiceClient::addService(DEService* service)
{
	if (service != nullptr)
	{
		_serviceMap.insert(std::make_pair(service->getServiceName(), service));
		service->init();
		service->setTargetAliveListener(this);
	}
}

void DEServiceClient::init()
{
	auto search_thread = [this]()
	{
		while (!_endThread)
		{
			if (_searchFlag)
			{
				_searchFlag = false;

				findSPs(3000, false);
			}
#ifdef _WIN32
			Sleep(5000);
#elif ANDROID
			sleep(5);
#endif // ANDROID
		}
	};

	std::thread t1(search_thread);
	t1.detach();
}

void DEServiceClient::cleanup()
{
	_endThread = true;
	_searchFlag = false;
}

void DEServiceClient::setDEServiceClientListener(DEServiceClientListener* listener)
{
	_listener = listener;
}

bool DEServiceClient::connectSP(ServiceProvideInfo sp, int timeout)
{
	return connectService(sp, timeout);
}

void DEServiceClient::findSPs()
{
	_searchFlag = true;
}


bool DEServiceClient::isContainsService(std::string serviceName)
{
	if (_serviceMap.size()>0)
	{
		std::map<std::string, DEService*>::iterator it = _serviceMap.begin();
		for (it; it != _serviceMap.end(); ++it)
		{
			if (it->first.compare(serviceName) == 0)
			{
				return true;
			}
		}
	}

	return false;
}

DEService* DEServiceClient::getService(std::string serviceName)
{
	if (_serviceMap.size()>0)
	{
		std::map<std::string, DEService*>::iterator it = _serviceMap.begin();
		for (it; it != _serviceMap.end(); ++it)
		{
			if (it->first.compare(serviceName) == 0)
			{
				return it->second;
			}
		}
	}

	return nullptr;
}

ServiceProvideInfo DEServiceClient::getConnectedSP()
{
	return _connectedSP;
}

std::vector<ServiceProvideInfo> DEServiceClient::getFoundSPs()
{
	return _foundedSps;
}

void DEServiceClient::disconnectSP(ServiceProvideInfo sp)
{
	disconnectService(sp);
}

void DEServiceClient::onDead(coocaa::SocketAddress& target)
{
	if (_listener != NULL)
	{
		if (_connectedSP.spAddress.getHostAddress().compare(target.getHostAddress()) == 0)
		{
			_listener->onDisconnected(_connectedSP);
		}
	}
}

void DEServiceClient::onAlive(coocaa::SocketAddress& target)
{

}