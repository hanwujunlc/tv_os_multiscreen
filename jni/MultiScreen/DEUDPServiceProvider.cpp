#include "DEUDPServiceProvider.h"
#include "UDPSocket.h"
#include "DEUDPConnector.h"

USING_NS_COOCAA;

DEUDPServiceProvider::DEUDPServiceProvider(const std::string& providerName, const  std::string& version, 
	const  std::string& devicetype)
	:IDEServiceProvider(providerName,version),
	_deviceType(devicetype),
	_multicastsocket(nullptr),
	_broadcastsocket(nullptr),
	_multisocketaddress("239.253.1.1"),
	_endBroadCast(false),
	_endRouteListen(false)
{
	COOCAALOGD("DEUDPServiceProvider DEUDPServiceProvider");
}

DEUDPServiceProvider::~DEUDPServiceProvider()
{

}

void DEUDPServiceProvider::startProvider()
{
	joinGroupAndBroadcast();
}

void DEUDPServiceProvider::endProvider()
{
	if (!_endRouteListen)
	{
		_endRouteListen = true;
		_multicastsocket->leaveGroup(_multisocketaddress );
		_multicastsocket->close();
		std::mutex _cmutex;
		std::unique_lock<std::mutex> lock(_cmutex);
		_syncRoute.wait(lock);

		delete _multicastsocket;
	}

	if(!_endBroadCast)
	{
		_endBroadCast = true;
		_broadcastsocket->close();
		std::mutex _bmutex;
		std::unique_lock<std::mutex> alock(_bmutex);
		_syncBoradCast.wait(alock);

		delete _broadcastsocket;
	}

}

// broadcast self ip, every 5s
void DEUDPServiceProvider::joinGroupAndBroadcast()
{

	_multicastsocket = new MulticastSocket();
	_multicastsocket->open(true);
	_multicastsocket->bind(SocketAddress(ROUTE_LISTEN_PORT));

	_multicastsocket->joinGroup(_multisocketaddress);
	COOCAALOGD("Router Listen localAddress = 239.253.1.1");

	_multicastsocket->setTTL(10u);
	auto route_thread = [this]()
	{
		while (!_endRouteListen)
		{
			DatagramPacket recedp = DatagramPacket(_MAX_SOCKET_LENGTH);

			unsigned int length = _multicastsocket->receive(recedp);
			if (length <= 0) {

			}
			else {
				std::string data((const char*)recedp.getData());
				if (data.length()>0) {
					COOCAALOGD("Router mode listen receive %s:%d = %s!\n",
						recedp.getAddress().getHostAddress().c_str(), recedp.getAddress().getPort(), data.c_str());
					handleMessage(recedp);

				}
				else {
					COOCAALOGD("Router mode listen receive null ");
				}
			}
		}
		_syncRoute.notify_one();
	};

	std::thread t(route_thread);
	t.detach();
}

void DEUDPServiceProvider::handleMessage(DatagramPacket packet)
{
	std::string data((const char*) packet.getData());
	data = data.substr(0, packet.getLength());

	DEData inData(data);
	std::string command = inData.getStringValue("cmd");
	std::string type = inData.getStringValue("type");
	//IFWLogger_i("fyb", "handleMessage command : " + command);
	if (!command.empty() && command.compare("FINDSP") ==0 && !type.empty() && type.compare("search")==0) {
		DEData outData, paramData;

		paramData.addValue("name", this->_spName);
		paramData.addValue("version", this->_spVersion);
		paramData.addValue("servicesName", "ServerService");
		if (_deviceType.length() == 0)
			paramData.addValue("deviceType", "NGBTVOS");
		else
			paramData.addValue("deviceType", _deviceType);

		outData.addValue("type", "search");
		outData.addValue("cmd", "FINDSP");
		outData.addValue("param", paramData.toString());
		COOCAALOGD(" --- FINDSP, params : %s " , paramData.toString().c_str());

		DatagramPacket dp(outData.toString());
		dp.setAddress(packet.getAddress());

		UDPSocket* socket = new UDPSocket();
		socket->open(true);
		int sendLen = socket->send(dp);
		if (sendLen <= 0) {
			COOCAALOGD("handleMessage SQ send msg error: sendLen = %d",sendLen);
		}
		socket->close();
		delete socket;

	}
	if (!command.empty() && command == "CONNECTSP" && !type.empty() && type.compare("connect") == 0) {
		std::string param = inData.getStringValue("param");
		DEData paramDEData(param);
		std::string serviceName = paramDEData.getStringValue("servicesName");
		std::string clientName = paramDEData.getStringValue("clientName");
		COOCAALOGD("handleMessage CQ, service : %s " , serviceName.c_str());
		COOCAALOGD("handleMessage CQ, clientName : %s " , clientName.c_str());
		if (isContainsService(serviceName)) {
			DEService *service = getService(serviceName);
			if (service!=nullptr)
			{
				DEData accepted_sendData, refused_sendData, accepted_paramData,refused_paramData;
				if (service->getMobileConnectorCount() >= 3u)
				{
					refused_paramData.addValue("response", "refused");
					refused_paramData.addValue("name", this->_spName);
					refused_paramData.addValue("reason", "maxNum");
					refused_sendData.addValue("type", "connect");
					refused_sendData.addValue("cmd", "CONNECTSP");
					refused_sendData.addValue("param",
						refused_paramData.toString());
					DatagramPacket dp_refused(refused_sendData.toString());
					dp_refused.setAddress(packet.getAddress());
					UDPSocket* socket = new UDPSocket();
					socket->open(true);
					socket->send(dp_refused);
					socket->close();
					delete socket;
				}
				else
				{
					std::string key = packet.getAddress().getHostAddress();
					unsigned int port = packet.getAddress().getPort();

					SocketAddress target;
					target.set(key, port);
					target.setHostName(clientName);

					DEUDPConnector* connector = (DEUDPConnector*)service->getConnector();
					if (connector!=nullptr)
					{
						service->addMobileTarget(key, target);

						accepted_paramData.addValue("response", "accepted");
						accepted_paramData.addValue("name", this->_spName);
						accepted_sendData.addValue("type", "connect");
						accepted_sendData.addValue("cmd", "CONNECTSP");
						accepted_sendData.addValue("param",
							accepted_paramData.toString());
						DatagramPacket dp_accepted(
							accepted_sendData.toString());
						dp_accepted.setAddress(packet.getAddress());
						connector->getSocket()->send(dp_accepted);
						if (_aliveListener!=nullptr)
						{
							_aliveListener->onAlive(target);
						}
					}
					else
					{
						refused_paramData.addValue("response", "refused");
						refused_paramData.addValue("name", this->_spName);
						refused_paramData.addValue("reason", "error");
						refused_sendData.addValue("type", "connect");
						refused_sendData.addValue("cmd", "CONNECTSP");
						refused_sendData.addValue("param",
							refused_paramData.toString());
						DatagramPacket dp_refused(refused_sendData.toString());
						dp_refused.setAddress(packet.getAddress());
						UDPSocket* socket = new UDPSocket();
						socket->open(true);
						socket->send(dp_refused);
						socket->close();

						delete socket;
					}
				}
			}
		}
	}
}
