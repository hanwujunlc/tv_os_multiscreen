#ifndef __DEDATA_H_
#define __DEDATA_H_
#include "CoocaaDef.h"
#include <string>
#include <vector>
//#include "json.h"
#include "cJSON.h"
#include <assert.h>
#include <atomic> 

NS_COOCAA_BEGIN

class DEData
{
public:

	DEData():_dataBuffer(nullptr)
	{
		_dataBuffer = new DEDataBuffer();
		assert(_dataBuffer != nullptr);
	}

	DEData(const DEData& srcData)
	{
		_dataBuffer = srcData._dataBuffer;
		_dataBuffer->obtain();
	}

	DEData(const std::string& deserializeStr)
	{
		_dataBuffer = new DEDataBuffer(deserializeStr);
		assert(_dataBuffer != nullptr);
	}

	virtual ~DEData()
	{
		if (_dataBuffer != nullptr)
		{
			_dataBuffer->release();
		}
	}

	DEData & operator=(const DEData & src);

	std::string getStringValue(const std::string & key);

	int getIntValue(const std::string&  key,int& result);

	int getLongValue(const std::string&  key,long& result);

	int getBooleanValue(const std::string&  key,bool& result);

	int getFloatValue(const std::string&  key,float& result);

	int getDoubleValue(const std::string &  key,double& result);

	int getDEDataListValue(const std::string&  key,  std::vector<DEData>& list);

	int  getDEData(const std::string& key,DEData& data);

	void addValue(const std::string&  key, const std::string& value);	

	void addValue(const std::string&  key,const char value[]);

	void addValue(const std::string& key, std::vector<DEData> value);

	void addValue(const  std::string&  key, const int& value);

	void addValue(const std::string&  key, const long& value);

	void addValue(const std::string& key, const bool& value);

	void addValue(const std::string&  key, const float& value);

	void addValue(const std::string&  key, const double& value);

	void addValue(const std::string& key,DEData data);

	std::string toString();

	bool isIntValue(const std::string& key);

	bool isStringValue(const std::string& key);

private:
	class DEDataBuffer
	{
	public:
		explicit DEDataBuffer()
			: _root(NULL),
			_refCnt()
		{
			_root = cJSON_CreateObject();
			_refCnt = 1;
		}
		explicit DEDataBuffer(const std::string& data)
			: _root(NULL),
			_refCnt()
		{
			_root = cJSON_CreateObject();
			if (data.length()>0)
			{
				_root = cJSON_Parse(data.c_str());
			}
			_refCnt = 1;
		}

		virtual ~DEDataBuffer() {
		}

		cJSON* getBuffer()
		{
			return _root;
		}
		
		inline void release()
		{
			assert(_refCnt > 0);
			if (--_refCnt == 0)
			{
				if (_root != NULL)
				{
					cJSON_Delete(_root);
					delete this;
				}
			}
		}

		inline void obtain()
		{
			assert(_refCnt > 0);
			++_refCnt;
		}
	private:
		cJSON *_root;
		std::atomic<int> _refCnt;
	};
	DEDataBuffer* _dataBuffer;

};

NS_COOCAA_END

#endif//__DEDATA_H_
