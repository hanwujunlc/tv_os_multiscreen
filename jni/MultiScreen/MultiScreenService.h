/*
* MultiScreenService.h
*
*  Created on: Nov 27, 2015
*      Author: rico
*/
#ifndef _MULTISCREENSERVICE_H__
#define _MULTISCREENSERVICE_H__

#include <binder/BinderService.h>
#include <IMultiScreenService.h>

#include "IDEServiceProvider.h"
#include "DEServerService.h"


/*
namespace android {
	class MultiScreenService : public BinderService<MultiScreenService>, public BnMultiScreenService
	{
		friend class BinderService<MultiScreenService>;
	public:
		MultiScreenService() : BnMultiScreenService() { }
		virtual ~MultiScreenService() { }
		virtual void onFirstRef() { }

		static const char*  getServiceName() { return "multiscreen"; }
		virtual void        sayhello(size_t size, const char *name);
		virtual status_t connect(const sp<IMultiScreenCallBack>& multiScreenCallback);
		virtual status_t    onTransact(uint32_t code, const Parcel& data, Parcel* reply,uint32_t flags);

		sp<IMultiScreenCallBack> multiScreenCallback;
	};
}
*/

namespace android {
	class MultiScreenService : public BinderService<MultiScreenService>, public BnMultiScreenService /*,public ServerServiceListener*/
	{
		friend class BinderService<MultiScreenService>;
	public:
		MultiScreenService() : BnMultiScreenService() { }
		virtual ~MultiScreenService() { }
		virtual void onFirstRef() { }

		static const char*  getServiceName() { return "multiscreen"; }

		//multiscreenserver api
		virtual status_t startMultiScreenServer(const char* spName, const char* spVersion, const char* deviceType);
		virtual status_t stopMultiScreenServer();

		//multiscreenclient api
		virtual status_t startMultiScreenClient(const char* clientName);
		virtual status_t stopMultiScreenClient();
		virtual status_t findSPs();

		virtual status_t connect(const sp<IMultiScreenCallBack>& multiScreenCallback);
		virtual status_t    onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags);

		sp<IMultiScreenCallBack> multiScreenCallback;


// 		virtual void onQueryInfo(SocketAddress & target, const std::string & attribute, std::string& result);
// 		virtual	void execCmd(SocketAddress & target, const std::string & command, const std::string& params);
// 		virtual void onInput(const std::string& action, const std::string& params);

	private:
		coocaa::IDEServiceProvider* provider;
	};
}

#endif //_MULTISCREENSERVICE_H__


