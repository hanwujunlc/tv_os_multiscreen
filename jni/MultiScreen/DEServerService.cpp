#include "DEServerService.h"

USING_NS_COOCAA;

DEServerService::DEServerService():DEService("ServerService")
{

}

DEServerService::~DEServerService()
{

}

void DEServerService::setListener(ServerServiceListener* listener)
{
	DEService::setListener(listener);
}

//override
void DEServerService::processData(coocaa::SocketAddress  & target,coocaa::DEData & revData)
{
	if (_serviceListener != nullptr) {
	 	ServerServiceListener* mServerListener =
	 			static_cast<ServerServiceListener*>(_serviceListener);
	 	if (mServerListener != nullptr) {
	 		std::string type = revData.getStringValue("type");
	 		if (type.length()>0) {
	 			if (type == "command") {
	 				mServerListener->execCmd(target,
	 						revData.getStringValue("cmd"),
	 						revData.getStringValue("param"));
	 			} else if (type == "info") {
	 				std::string cmd = revData.getStringValue("cmd");
	 				std::string result;
	 				mServerListener->onQueryInfo(target, cmd, result);
	 				if (result.length()>0) {
	 					notifyInfo(target, cmd, result);
	 				}
	 			} else if (type == "input") {
	 				mServerListener->onInput(revData.getStringValue("cmd"),
	 						revData.getStringValue("param"));
	 			}
	 		}
	 	}
	}
}

void DEServerService::notifyInfo(const std::string & cmd, const std::string & param)
{
	 	DEData data;
	 	data.addValue("type", "notify");
	 	data.addValue("cmd", cmd);
	 	data.addValue("param", param);
	 	sendData(data.toString().c_str(), data.toString().length());
}

void DEServerService::notifyInfo(SocketAddress & target, const std::string& cmd, const std::string& param)
{
	DEData data;
	data.addValue("type", "response");
	data.addValue("cmd", cmd);
	data.addValue("param", param);
	sendData(target, data.toString().c_str(), data.toString().length());
}

void DEServerService::writeAll(const std::string &jsonString)
{
	sendData(jsonString.c_str(), jsonString.length());
}

void DEServerService::writeTarget(SocketAddress & target,const std::string& jsonString)
{
	sendData(target, jsonString.c_str(), jsonString.length());
}