/*
* MultiScreenService.cpp
*
*  Created on: Nov 27, 2015
*      Author: rico
*/
#define LOG_TAG "MulltiScreenServiceServer"

#include <unistd.h>
#include <sys/mman.h>
#include <utils/Log.h>
#include <binder/PermissionCache.h>
#include <private/android_filesystem_config.h>
#include "DEUDPServiceProvider.h"
#include "MultiScreenService.h"

namespace android {

/*	void* test_fcn(void* arg) {
		//std::this_thread::sleep_for(std::chrono::milliseconds(1000 * 3));
		sleep(3);
		MultiScreenService * service = (MultiScreenService *)arg;
		for (int i = 0; i < 5; ++i) {
			//std::cout << ">>>>>>>>>>>>>>>>>>>Thread  executing :" << i << std::endl;
			printf(">>>>>>>>>>>>>>>>>>>Thread  executing :%d\n", i);
			//std::this_thread::sleep_for(std::chrono::milliseconds(1000 * 2));
			sleep(2);

			service->multiScreenCallback->notifyCallback(1, 1, 1);
		}
		return 0;
	}


	void MultiScreenService::sayhello(size_t size, const char *name) {
		printf("in echoservice::sayhello.size=%d,name=%s\n", size, name);
	}


	*/

	status_t MultiScreenService::connect(const sp<IMultiScreenCallBack>& callback) {
		//创建一个定时线程，定时回调接口
		printf("-----------------------------------------in MultiScreenService::connect.will invoke notifyCallback\n");

		this->multiScreenCallback = callback;

		////创建新线程
		////   std::thread t1(test_fcn,this);

		//pthread_t tid;
		//pthread_create(&tid, NULL, &test_fcn, this);
		return NO_ERROR;
	}


	status_t MultiScreenService::startMultiScreenServer(const char* spName, const char* spVersion, const char* deviceType)
	{
		COOCAALOGD("MultiScreenService startMultiScreenServer");
		if (provider == NULL)
		{
			provider = new coocaa::DEUDPServiceProvider(spName,spVersion,deviceType);
			provider->init();

			coocaa::DEServerService* service = new coocaa::DEServerService();
			////service->setListener();
		}
	}

	status_t MultiScreenService::stopMultiScreenServer()
	{
		COOCAALOGD("MultiScreenService stopMultiScreenServer");
		if (provider != NULL)
		{
			provider->cleanup();
			delete provider;
		}
	}

	status_t MultiScreenService::startMultiScreenClient(const char* clientName)
	{

	}

	status_t MultiScreenService::stopMultiScreenClient()
	{

	}

	status_t MultiScreenService::findSPs()
	{
		COOCAALOGD("MultiScreenService findSPs");
	}

	status_t MultiScreenService::onTransact(uint32_t code, const Parcel& data, Parcel* reply,
		uint32_t flags)
	{
		return BnMultiScreenService::onTransact(code, data, reply, flags);
	}

}