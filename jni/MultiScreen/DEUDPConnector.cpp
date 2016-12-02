#include "DEUDPConnector.h"
#include <thread>
#include <mutex>

USING_NS_COOCAA;

DEUDPConnector::DEUDPConnector():_udpSocket(nullptr),_targetnickname(), _targetAddress(),_endthread(false)
{
}


DEUDPConnector::~DEUDPConnector()
{
	cleanup();
}

void DEUDPConnector::init()
{
	_udpSocket = new UDPSocket();
	_udpSocket->open(true);
	_udpSocket->bind(SocketAddress());

	auto recThread = [this]()
	{
		while(!_endthread)
		{
			DatagramPacket recedp(_MAX_SOCKET_LENGTH);
			unsigned int recevLen = _udpSocket->receive(recedp);
			if (recevLen == 0)
			{
				continue;
			}
			else if(recevLen<0)
			{
				_udpSocket->close();

				_udpSocket->open(true);
			}
			else
			{
				if (_connListener != NULL)
				{
					SocketAddress _address;
					_targetAddress.set(recedp.getAddress().getHostAddress(), recedp.getAddress().getPort());
					if (_connListener!=nullptr)
					{
						_connListener->onReceiveData(_targetAddress,(const char*)recedp.getData(),recedp.getLength());
					}
				}
			}
		}

		if (_udpSocket!=nullptr)
		{

			delete _udpSocket;
			_udpSocket = nullptr;
		}
		_sync.notify_one();
	};

	std::thread t(recThread);
	t.detach();
}

int DEUDPConnector::connect(coocaa::SocketAddress &target)
{
	return 1;
}

int DEUDPConnector::disconnect(coocaa::SocketAddress &target)
{
	return 1;
}

UDPSocket* DEUDPConnector::getSocket()
{
	return _udpSocket;
}

bool DEUDPConnector::isConnected()
{
	return _endthread?false:true;
}

unsigned int DEUDPConnector::send(const coocaa::SocketAddress & target, const char* data, const unsigned int dataLen)
{
	DatagramPacket dp(data);
	dp.setAddress(target);
	return _udpSocket->send(dp);
}

void DEUDPConnector::cleanup()
{
	_endthread = true;
	if(_udpSocket!=nullptr)
	{
		_udpSocket->close();

		std::mutex _mutex;
		std::unique_lock<std::mutex> lock(_mutex);
		_sync.wait(lock);
	}

}
