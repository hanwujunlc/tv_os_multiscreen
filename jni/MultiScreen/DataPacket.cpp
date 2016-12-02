#include "DataPacket.h"

USING_NS_COOCAA;

DataPacket& DataPacket::operator= (const DataPacket & src)
{
	if (&src != this)
	{
		if (_dataBuffer != NULL)
		{
			_dataBuffer->release();
		}
		_dataBuffer = src._dataBuffer;
		_dataBuffer->obtain();
	}
	return *this;
}
