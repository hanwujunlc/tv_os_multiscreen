#include "DEUDPServiceClient.h"
#include "DEData.h"
#include "CooCaaLog.h"

USING_NS_COOCAA;

DEUDPServiceClient::DEUDPServiceClient(std::string clientName):
	DEServiceClient(clientName), 
	_multisocketaddress("239.253.1.1")
{

}

DEUDPServiceClient::~DEUDPServiceClient()
{

}

void DEUDPServiceClient::init()
{
	DEServiceClient::init();

	_multicastsocket = new MulticastSocket();
	_multicastsocket->open(true);
	_multicastsocket->bind(SocketAddress(ROUTE_LISTEN_PORT));

	_multicastsocket->joinGroup(_multisocketaddress);
	COOCAALOGD("Router Listen localAddress = 239.253.1.1");

	_multicastsocket->setTTL(10u);

}

void DEUDPServiceClient::cleanup()
{
	DEServiceClient::cleanup();

	_multicastsocket->leaveGroup(_multisocketaddress);
	_multicastsocket->close();
	delete _multicastsocket;
}

void DEUDPServiceClient::findSPs(int timeout = 1000u, const bool connectFirst = false)
{
	_foundedSps.clear();
	DEData data;
	data.addValue("cmd", "FINDSP");
	data.addValue("type", "search");
	DatagramPacket dp(data.toString());
	dp.setAddress(SocketAddress("239.253.1.1", ROUTE_LISTEN_PORT));
	_multicastsocket->send(dp);

	while (true)
	{
		int recelen = 0;
		DatagramPacket recedp = DatagramPacket(1000);
		if (_multicastsocket->receive(recedp, timeout) == 0)
		{
			break;
		}
		handleMessage(recedp);
	}
	if (_listener != NULL)
	{
		_listener->onSearchingSPFinished(_foundedSps);
	}
}

bool DEUDPServiceClient::connectService(ServiceProvideInfo sp, int timeout)
{
	return false;
}

void DEUDPServiceClient::disconnectService(ServiceProvideInfo sp)
{

}

void DEUDPServiceClient::handleMessage(DatagramPacket packet)
{
	std::string data((const char*)packet.getData());
	data = data.substr(0, packet.getLength());

	DEData inData(data);
	std::string command = inData.getStringValue("cmd");
	std::string type = inData.getStringValue("type");
	if (!command.empty() && command.compare("FINDSP") == 0 && !type.empty() && type.compare("search") == 0)
	{
		std::string param = inData.getStringValue("param");
		DEData paramData(param);
		std::string spName = paramData.getStringValue("name");
		std::string spVersion = paramData.getStringValue("version");
		std::string serviceName = paramData.getStringValue("servicesName");
		std::string deviceType = paramData.getStringValue("deviceType");
		ServiceProvideInfo spInfo;
		spInfo.serviceInfo = serviceName;
		spInfo.spAddress = packet.getAddress();
		spInfo.spDeviceType = deviceType;
		spInfo.spName = spName;
		spInfo.spVersion = spVersion;

		std::vector<ServiceProvideInfo>::iterator it;

		if (!find(_foundedSps.begin(), _foundedSps.end(), spInfo))
		{
			_foundedSps.push_back(spInfo);
		}
	}
}

bool DEUDPServiceClient::find(std::vector<ServiceProvideInfo>::iterator beg,std::vector<ServiceProvideInfo>::iterator end, ServiceProvideInfo info)
{
	while (beg != end)
	{
		if ((*beg).spAddress.getHostAddress().compare(info.spAddress.getHostAddress()) == 0)
			break;
		else
			++beg;
	}

	if (beg != end)
		return true;
	else
		return false;
}