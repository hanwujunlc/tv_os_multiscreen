#ifndef __DESERVERSERVICE_H_
#define __DESERVERSERVICE_H_
#include "CoocaaDef.h"
#include "DEData.h"
#include "IDEService.h"
#include <string>

NS_COOCAA_BEGIN
class ServerServiceListener : public IDEServiceListener
{
public:
	virtual void onQueryInfo(SocketAddress & target, const std::string & attribute, std::string& result) = 0;
	virtual	void execCmd(SocketAddress & target,const std::string & command, const std::string& params) = 0;
	virtual void onInput(const std::string& action, const std::string& params) = 0;
};

class DEServerService:public DEService
{
public:
	DEServerService();
	virtual ~DEServerService();


	void setListener(ServerServiceListener* listener);
	//override
	void processData(coocaa::SocketAddress  & target,coocaa::DEData & revData);

	void notifyInfo(const std::string & cmd, const std::string & param);
	void notifyInfo(SocketAddress & target, const std::string& cmd, const std::string& param);
	void writeAll(const std::string &jsonString);
	void writeTarget(SocketAddress & target,const std::string& jsonString);
};

NS_COOCAA_END

#endif//__DESERVERSERVICE_H_