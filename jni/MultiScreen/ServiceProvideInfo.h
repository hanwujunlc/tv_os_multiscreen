#ifndef __DEVICEINFO_H_
#define __DEVICEINFO_H_
#include "CoocaaDef.h"
#include <string>
#include "ISocket.h"
NS_COOCAA_BEGIN

class ServiceProvideInfo
{
public:
	std::string spName;
	std::string spDeviceType;
	std::string spVersion;
	std::string serviceInfo;
	coocaa::SocketAddress spAddress;
};

NS_COOCAA_END

#endif//__DEVICEINFO_H_