/*
* IMultiScreenCallBack.h
*
*  Created on: Nov 27, 2015
*      Author: rico
*/
#ifndef _IMULTISCREENCALLBACK_H__
#define _IMULTISCREENCALLBACK_H__

#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>
#include <binder/IMemory.h>

namespace android
{
	class IMultiScreenCallBack :public IInterface {
	public:
		DECLARE_META_INTERFACE(MultiScreenCallBack);
		virtual void notifyCallback(int32_t msgType, int32_t ext1, int32_t ext2) = 0;
		virtual void dataCallback(int32_t msgType, const sp<IMemory>& data) = 0;
	};

	class BnMultiScreenCallBack :public BnInterface<IMultiScreenCallBack> {
	public:
		virtual status_t onTransact(uint32_t code,const Parcel& data,Parcel* reply,uint32_t flags = 0);
	};
}
#endif //_IMULTISCREENCALLBACK_H__