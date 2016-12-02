#include "IDEService.h"
#ifdef ANDROID
#include <unistd.h> 
#endif // ANDROID

#include "CooCaaLog.h"
#include "DEData.h"
#include "DEUDPConnector.h"

USING_NS_COOCAA;

DEService::DEService(const std::string& name):_serviceName(name),_conn(nullptr),_serviceListener(nullptr),_aliveListener(nullptr),_endThread(false)
{

}

DEService::~DEService()
{
	cleanup();
}

void DEService::init()
{
	_conn = new DEUDPConnector();
	_conn->init();
	if (_conn != nullptr) {
		_conn->setListener(this);
	}

	auto alivecheckthread = [this]()
	{
		while(!_endThread)
		{
			checkTargetTicks();
#ifdef _WIN32
			Sleep(5000);
#elif ANDROID
			sleep(5);
#endif // ANDROID

		}

		_sync.notify_one();
	};

	std::thread t(alivecheckthread);
	t.detach();

}

void DEService::checkTargetTicks()
{
	std::unique_lock<std::mutex> lock(_mutex);

	if (_mobiesMap.size()>0)
	{
		std::map<std::string,MobileTarget>::iterator it=_mobiesMap.begin();
		for(it;it != _mobiesMap.end();)
		{
			std::map<std::string,MobileTarget>::iterator it_tmp = it;
			bool is_first_element = false;
			if(it_tmp != _mobiesMap.begin())
				it_tmp --;
			else
				is_first_element = true;

			MobileTarget target = (*it).second;

			COOCAALOGD("currentTarget ip = %s,count = %d",target.ip.c_str(),target.cnt);
			if( target.cnt<=10)
			{
				target.cnt += 5;
				(*it).second = target;
				it++;
			}
			else
			{
				MobileTarget tmptarget = (*it).second;

				SocketAddress address;
				address.setHostName(tmptarget.name);
				address.set(tmptarget.ip,tmptarget.port);
				if(_aliveListener!=nullptr)
				{
					_aliveListener->onDead(address);
				}

				_mobiesMap.erase(it);

				if(is_first_element)
					it = _mobiesMap.begin();
				else
					it = ++ it_tmp;
			}
		}
	}

}

void DEService::cleanup()
{
	if(_conn!=nullptr)
	{
		_endThread = true;

		_conn->cleanup();
		delete _conn;
		_conn = nullptr;

		std::mutex _cmutex;
		std::unique_lock<std::mutex> lock(_cmutex);
		_sync.wait(lock);
	}
}

void DEService::onDisconnect(coocaa::SocketAddress &target)
{
	//no implement
}

void DEService::onConnect(coocaa::SocketAddress &target)
{
	//no implement
}

void DEService::onReceiveData(coocaa::SocketAddress &target, const char* data, unsigned int dataLen)
{

	std::string text(data, dataLen);

	if(text.length()>0)
	{
		COOCAALOGD("before service onReceiveData = %s",text.c_str());
		DEData received(text);
		COOCAALOGD("after service onReceiveData deserialize json ");

		std::string cmd = received.getStringValue("cmd");
		if (cmd.compare("aliveCheck")==0) {
			handleAliveCheck(target.getHostAddress(), target.getPort());
			DEData response;
			response.addValue("type", "response");
			response.addValue("cmd", "aliveCheck");
			response.addValue("param", "yes");
			response.addValue("serviceName", _serviceName);
			sendData(target, response.toString().c_str(),
				response.toString().length());
		} else if (cmd.compare("DISCONNECTSP")==0) {

			removeMobileTarget(target.getHostAddress());
			SocketAddress address(target);
			if(_aliveListener!=nullptr)
			{
				_aliveListener->onDead(target);
			}

		} else {
			processData(target, received);
		}
	}
	
}

void DEService::setListener(IDEServiceListener* listener)
{
	_serviceListener = listener;
}

void DEService::setTargetAliveListener(IDETargetAliveListener* aliveListener)
{
	_aliveListener = aliveListener;
}

std::string DEService::getServiceName()
{
	return _serviceName;
}

void DEService::addMobileTarget(const std::string & ip, coocaa::SocketAddress & addr)
{
	std::unique_lock<std::mutex> lock(_mutex);
	MobileTarget target(addr.getHostName(),addr.getHostAddress(),addr.getPort());
	_mobiesMap.insert(std::make_pair(ip,target));
}

void DEService::removeMobileTarget(const std::string & ip)
{
	std::unique_lock<std::mutex> lock(_mutex);
	std::map<std::string,MobileTarget>::iterator it=_mobiesMap.find(ip);
	if(it != _mobiesMap.end())
	{
		_mobiesMap.erase(it);
	}
}

unsigned int DEService::getMobileConnectorCount()
{
	std::unique_lock<std::mutex> lock(_mutex);
	return _mobiesMap.size();
}

bool  DEService::isExist(const std::string & ip)
{
	std::unique_lock<std::mutex> lock(_mutex);
	std::map<std::string,MobileTarget>::iterator it=_mobiesMap.find(ip);
	if(it != _mobiesMap.end())
	{
		return true;
	}
	return false;
}

IDEConnector* DEService::getConnector()
{
	return _conn;
}

unsigned int DEService::sendData(const char* data, const unsigned int len)
{
	std::unique_lock<std::mutex> lock(_mutex);
	std::map<std::string,MobileTarget>::iterator it=_mobiesMap.begin();
	int length = 0;
	for(;it!=_mobiesMap.end();it++)
	{
		MobileTarget target = (*it).second;

		SocketAddress address(target.ip,target.port);
		if(_conn!=nullptr)
		{
			length = _conn->send(address,data,len);
		}
	}
	return length;
}

unsigned int DEService::sendData(const SocketAddress & target, const char* data,const unsigned int dataLen)
{
	int length = 0;
	if(_conn!=nullptr)
	{
		length = _conn->send(target,data,dataLen);
	}
	return length;
}

void DEService::handleAliveCheck(const std::string & ip, unsigned int port)
{
	SocketAddress target;
	target.set(ip, port);

	std::map<std::string,MobileTarget>::iterator it= _mobiesMap.find(ip);
	if(it!=_mobiesMap.end())
	{
		MobileTarget target = (*it).second;
		target.cnt = 0;
		(*it).second = target;
	}
	else if (_mobiesMap.size()<=3)
	{
		addMobileTarget(ip, target);
	}
}
