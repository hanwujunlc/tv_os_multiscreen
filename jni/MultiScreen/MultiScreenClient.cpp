/*
* MultiScreenService.cpp
*
*  Created on: Nov 27, 2015
*      Author: rico
*/


#include <binder/IServiceManager.h>
#include <MultiScreenService.h>
#include "MultiScreenClient.h"

namespace android {
	void MultiScreenClient::notifyCallback(int32_t msgType, int32_t ext1, int32_t ext2) {
		printf("---------------------------MultiScreenClient::notifyCallback....\n");
	}
	void MultiScreenClient::dataCallback(int32_t msgType, const sp<IMemory>& data) {
		printf("-------------------------------------------MultiScreenClient::dataCallback....\n");
	}

	void MultiScreenClient::connect() {
		printf("--------------------------BnMultiScreenCallBack::connect begin\n");
		sp<IServiceManager> sm(defaultServiceManager());

		sp<IBinder> binder = sm->getService(String16(MultiScreenService::getServiceName()));
		if (binder == NULL) {
			printf("get null binder !!!! \n");
			return;
		}
		sp<IMultiScreenService> proxyBinder = interface_cast<IMultiScreenService>(binder);

		printf("------------------------------before EchoClient::connect proxyBinder->connect(client);...\n");
		sp<MultiScreenClient> client = new MultiScreenClient();
		proxyBinder->connect(client);
		printf("------------------------------after EchoClient::connect proxyBinder->connect(client);...\n");
	}

	void MultiScreenClient::disconnect() {

	}

	status_t MultiScreenClient::onTransact(uint32_t code, const Parcel& data, Parcel* reply,
		uint32_t flags)
	{
		printf("--------------------- MultiScreenClient::onTransact\n");
		return BnMultiScreenCallBack::onTransact(code, data, reply, flags);
	}

}