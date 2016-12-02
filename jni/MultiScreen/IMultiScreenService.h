/*
* IMultiScreenService.h
*
*  Created on: Nov 27, 2015
*      Author: rico
*/
#ifndef _IMULTISCREENSERVICE_H__
#define _IMULTISCREENSERVICE_H__
#include <binder/IInterface.h>
#include <binder/IMemory.h>
#include <binder/Parcel.h>

#include <IMultiScreenCallBack.h>

namespace android {
	class IMultiScreenService : public IInterface
	{
	public:
		DECLARE_META_INTERFACE(MultiScreenService);

		//virtual void  sayhello(size_t size, const char* name) = 0;
		//virtual status_t connect(const sp<IMultiScreenCallBack>& multiScreenCallback) = 0;
		//multiscreenserver api
		virtual status_t startMultiScreenServer(const char* spName,const char* spVersion,const char* deviceType) = 0;
		virtual status_t stopMultiScreenServer() = 0;

		//multiscreenclient api
		virtual status_t startMultiScreenClient(const char* clientName) = 0;
		virtual status_t stopMultiScreenClient() = 0;
		virtual status_t findSPs() = 0;

		virtual status_t connect(const sp<IMultiScreenCallBack>& multiScreenCallback) = 0;
	};

	class BnMultiScreenService : public BnInterface<IMultiScreenService>
	{
	public:
		virtual status_t onTransact(uint32_t code, const Parcel& data, Parcel* reply,uint32_t flags = 0);
	};
}
#endif//_IMULTISCREENSERVICE_H__