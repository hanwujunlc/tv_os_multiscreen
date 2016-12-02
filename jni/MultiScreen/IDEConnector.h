#ifndef __IDECONNECTOR_H_
#define __IDECONNECTOR_H_
#include "CoocaaDef.h"
#include "ISocket.h"
NS_COOCAA_BEGIN

class IDEConnectorListener
{
public:
	virtual ~IDEConnectorListener() {}
	virtual void onDisconnect(coocaa::SocketAddress &target) = 0;
	virtual void onConnect(coocaa::SocketAddress &target) = 0;
	virtual void onReceiveData(coocaa::SocketAddress &target, const char* data, unsigned int dataLen) = 0;
};

class IDEConnector
{
public:
	IDEConnector():_connListener(nullptr){}
	virtual ~IDEConnector(){}

	void setListener(IDEConnectorListener* const listener){
		this->_connListener = listener;
	}
	virtual int connect(coocaa::SocketAddress &target) = 0;
	virtual int disconnect(coocaa::SocketAddress &target) = 0;
	virtual bool isConnected() = 0;
	virtual unsigned int send(const coocaa::SocketAddress & target, const char* data, const unsigned int dataLen) = 0;
	virtual void cleanup() = 0;
	virtual void init() = 0;
protected:
	IDEConnectorListener* _connListener;
};
NS_COOCAA_END

#endif//__IDECONNECTOR_H
