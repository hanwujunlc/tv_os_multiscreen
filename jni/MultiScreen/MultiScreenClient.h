/*
/*
* MultiScreenClient.h
*
*  Created on: Nov 27, 2015
*      Author: rico
*/
#ifndef _MULTISCREENCLIENT_H__
#define _MULTISCREENCLIENT_H__
#include "IMultiScreenCallBack.h"

namespace android {
	class MultiScreenClient : public BnMultiScreenCallBack {
	public:
		void			notifyCallback(int32_t msgType, int32_t ext1, int32_t ext2);
		void			dataCallback(int32_t msgType, const sp<IMemory>& data);
		void			connect();
		void			disconnect();

		virtual status_t onTransact(uint32_t code, const Parcel& data, Parcel* reply,uint32_t flags);
	};

}

#endif //_MULTISCREENCLIENT_H__