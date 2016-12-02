/*
* IMultiScreenCallBack.cpp
*
*  Created on: Nov 27, 2015
*      Author: rico
*/
#define LOG_TAG "IMultiScreenCallBack"
#include <utils/Log.h>
#include <stdint.h>
#include <sys/types.h>
#include <utils/Timers.h>
#include "IMultiScreenCallBack.h"

namespace android
{
	enum {
		NOTIFY_CALLBACK = IBinder::FIRST_CALL_TRANSACTION,
		DATA_CALLBACK
	};

	class BpMultiScreenCallBack : public BpInterface<IMultiScreenCallBack> {
	public:
		BpMultiScreenCallBack(const sp<IBinder>& impl)
			: BpInterface<IMultiScreenCallBack>(impl)
		{
		}

		// generic callback from multiscreen service to app
		virtual void notifyCallback(int32_t msgType, int32_t ext1, int32_t ext2)
		{
			ALOGV("BpMultiScreenCallBack::notifyCallback");
			Parcel data, reply;
			data.writeInterfaceToken(IMultiScreenCallBack::getInterfaceDescriptor());
			data.writeInt32(msgType);
			data.writeInt32(ext1);
			data.writeInt32(ext2);
			remote()->transact(NOTIFY_CALLBACK, data, &reply, IBinder::FLAG_ONEWAY);
		}

		// generic data callback from multiscreen service to app with raw data
		virtual void dataCallback(int32_t msgType, const sp<IMemory>& rawData)
		{
			ALOGV("dataCallback");
			Parcel data, reply;
			data.writeInterfaceToken(IMultiScreenCallBack::getInterfaceDescriptor());
			data.writeInt32(msgType);
			data.writeStrongBinder(rawData->asBinder());
			remote()->transact(DATA_CALLBACK, data, &reply, IBinder::FLAG_ONEWAY);
		}
	};

	IMPLEMENT_META_INTERFACE(MultiScreenCallBack, "com.ngbtvos.IMultiScreenCallBack");//implement meta interface

	// ----------------------------------------------------------------------
	status_t BnMultiScreenCallBack::onTransact(
		uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
	{
		switch (code) {
		case NOTIFY_CALLBACK: 
		{
			printf("--------------------BnEchoCallback::onTransact recv NOTIFY_CALLBACK!!!!!!!\n");
			ALOGV("NOTIFY_CALLBACK");
			CHECK_INTERFACE(IMultiScreenCallBack, data, reply);
			int32_t msgType = data.readInt32();
			int32_t ext1 = data.readInt32();
			int32_t ext2 = data.readInt32();
			notifyCallback(msgType, ext1, ext2);
			return NO_ERROR;
		} 
		break;
		case DATA_CALLBACK: 
		{
			ALOGV("DATA_CALLBACK");
			CHECK_INTERFACE(IEchoCallback, data, reply);
			int32_t msgType = data.readInt32();
			sp<IMemory> rawData = interface_cast<IMemory>(data.readStrongBinder());
			dataCallback(msgType, rawData);
			return NO_ERROR;
		}
		break;
		
		default:
			return BBinder::onTransact(code, data, reply, flags);
		}
	}

	// ----------------------------------------------------------------------------

}// namespace NGBTVOS
