/*
* IMultiScreenService.cpp
*
*  Created on: Nov 27, 2015
*      Author: rico
*/
#include <utils/Log.h>
#include <stdint.h>
#include <sys/types.h>
#include <binder/Parcel.h>
#include <IMultiScreenService.h>
#include <IMultiScreenCallBack.h>
#include "CooCaaLog.h"

namespace android {
	enum {
		START_MULTISCREENSERVER = IBinder::FIRST_CALL_TRANSACTION,
		STOP_MULTISCREENSERVER,
		START_MULTISCREENCLIENT,
		STOP_MULTISCREENCLIENT,
		FIND_SPS,
		CONNECT,
	};

	class BpMultiScreenService :public BpInterface<IMultiScreenService>
	{
	public:
		BpMultiScreenService(const sp<IBinder>& impl)
			: BpInterface<IMultiScreenService>(impl)
		{
		}

	/*	virtual void sayhello(size_t size, const char* name) {
			Parcel data, reply;
			data.writeInterfaceToken(IMultiScreenService::getInterfaceDescriptor());
			data.writeInt64((int64_t)size);
			data.writeCString(name);
			status_t status = remote()->transact(SAY_HELLO, data, &reply);
			size_t ret_size = (size_t)reply.readInt64();
			printf("in BpMultiScreenService.ret_size=%d\n", ret_size);
			const char* ret_name = reply.readCString();
			printf("in BpMultiScreenService.ret_name=%s\n", ret_name);
		}*/

		virtual status_t connect(const sp<IMultiScreenCallBack>& multiScreenCallback) {
			COOCAALOGD("BpMultiScreenService connect");
			Parcel data, reply;
			data.writeInterfaceToken(IMultiScreenService::getInterfaceDescriptor());
			data.writeStrongBinder(multiScreenCallback.get()->asBinder());

			status_t status;
			status = remote()->transact(CONNECT, data, &reply);
			if (status != OK) 
				return status;

			status = reply.readInt32();
			if (reply.readInt32() != 0) {
				printf("BpMultiScreenService::connect status=%d\n", status);
			}
			return status;
		}


		virtual status_t startMultiScreenServer(const char* spName, const char* spVersion, const char* deviceType)
		{
			COOCAALOGD("BpMultiScreenService startMultiScreenServer");
			Parcel data, reply;
			data.writeInterfaceToken(IMultiScreenService::getInterfaceDescriptor());
			data.writeCString(spName);
			data.writeCString(spVersion);
			data.writeCString(deviceType);
			status_t status = remote()->transact(START_MULTISCREENSERVER, data, &reply);
			if (status != OK)
				return status;

			status = reply.readInt32();
			if (reply.readInt32() != 0) {
				printf("BpMultiScreenService::START_MULTISCREENSERVER status=%d\n", status);
			}
			return status;
		}

		virtual status_t stopMultiScreenServer()
		{
			COOCAALOGD("BpMultiScreenService stopMultiScreenServer");
			Parcel data, reply;
			data.writeInterfaceToken(IMultiScreenService::getInterfaceDescriptor());
			status_t status = remote()->transact(STOP_MULTISCREENSERVER, data, &reply);
			if (status != OK)
				return status;

			status = reply.readInt32();
			if (reply.readInt32() != 0) {
				printf("BpMultiScreenService::STOP_MULTISCREENSERVER status=%d\n", status);
			}
			return status;
		}

		virtual status_t startMultiScreenClient(const char* clientName)
		{
			Parcel data, reply;
			data.writeInterfaceToken(IMultiScreenService::getInterfaceDescriptor());
			data.writeCString(clientName);
			status_t status = remote()->transact(START_MULTISCREENCLIENT, data, &reply);
			if (status != OK)
				return status;

			status = reply.readInt32();
			if (reply.readInt32() != 0) {
				printf("BpMultiScreenService::START_MULTISCREENCLIENT status=%d\n", status);
			}
			return status;
		}

		virtual status_t stopMultiScreenClient()
		{
			Parcel data, reply;
			data.writeInterfaceToken(IMultiScreenService::getInterfaceDescriptor());
			status_t status = remote()->transact(STOP_MULTISCREENCLIENT, data, &reply);
			if (status != OK)
				return status;

			status = reply.readInt32();
			if (reply.readInt32() != 0) {
				printf("BpMultiScreenService::STOP_MULTISCREENCLIENT status=%d\n", status);
			}
			return status;
		}

		virtual status_t findSPs()
		{
			Parcel data, reply;
			data.writeInterfaceToken(IMultiScreenService::getInterfaceDescriptor());
			status_t status = remote()->transact(FIND_SPS, data, &reply);
			if (status != OK)
				return status;

			status = reply.readInt32();
			if (reply.readInt32() != 0) {
				printf("BpMultiScreenService::FIND_SPS status=%d\n", status);
			}
			return status;
		}
	};

	IMPLEMENT_META_INTERFACE(MultiScreenService,"com.ngbtvos.IMultiScreenService");

	//------------------------------------

	status_t BnMultiScreenService::onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
	{
		switch (code) {

		//case SAY_HELLO: 
		//{
		//	CHECK_INTERFACE(IMultiScreenService, data, reply);
		//	size_t size = (size_t)data.readInt64();
		//	const char *name = data.readCString();
		//	sayhello(size, name);
		//	reply->writeInt64((int64_t)size);
		//	reply->writeCString(name);
		//	return NO_ERROR;
		//}
		//break;
		//case CONNECT: 
		//{
		//	CHECK_INTERFACE(IMultiScreenService, data, reply);
		//	//∂¡»°strongbinder
		//	sp<IMultiScreenCallBack> multiScreenCallBack =
		//		interface_cast<IMultiScreenCallBack>(data.readStrongBinder());
		//	status_t status = connect(multiScreenCallBack);
		//	reply->writeInt32(status);
		//	reply->writeInt32(1);
		//	return NO_ERROR;
		//}
		//break;
		case START_MULTISCREENSERVER:
		{
			CHECK_INTERFACE(IMultiScreenService, data, reply);
			const char *spName = data.readCString();
			const char *spVersion = data.readCString();
			const char *deviceType = data.readCString();
			COOCAALOGD("BnMultiScreenService onTransact START_MULTISCREENSERVER 1");
			status_t status = startMultiScreenServer(spName, spVersion, deviceType);
			COOCAALOGD("BnMultiScreenService onTransact START_MULTISCREENSERVER 2");
			reply->writeInt32(2);
			reply->writeInt32(1);
			return NO_ERROR;
		}
			break;
		case STOP_MULTISCREENSERVER:
		{
			CHECK_INTERFACE(IMultiScreenService, data, reply);
			status_t status = stopMultiScreenServer();
			reply->writeInt32(status);
			reply->writeInt32(1);
			return NO_ERROR;
		}
			break;
		case START_MULTISCREENCLIENT:
		{
			CHECK_INTERFACE(IMultiScreenService, data, reply);
			const char *clientName = data.readCString();
			status_t status = startMultiScreenClient(clientName);
			reply->writeInt32(status);
			reply->writeInt32(1);
			return NO_ERROR;
		}
			break;
		case STOP_MULTISCREENCLIENT:
		{
			CHECK_INTERFACE(IMultiScreenService, data, reply);
			status_t status = stopMultiScreenClient();
			reply->writeInt32(status);
			reply->writeInt32(1);
			return NO_ERROR;
		}
			break;
		case FIND_SPS:
		{
			CHECK_INTERFACE(IMultiScreenService, data, reply);
			status_t status = findSPs();
			reply->writeInt32(status);
			reply->writeInt32(1);
			return NO_ERROR;
		}
			break;
		case CONNECT:
		{
			CHECK_INTERFACE(IMultiScreenService, data, reply);
			//∂¡»°strongbinder
			sp<IMultiScreenCallBack> multiScreenCallBack =
				interface_cast<IMultiScreenCallBack>(data.readStrongBinder());
			status_t status = connect(multiScreenCallBack);
			reply->writeInt32(status);
			reply->writeInt32(1);
			return NO_ERROR;
		}
			break;
		default:
			return BBinder::onTransact(code, data, reply, flags);
		}
	}
}// namespace android