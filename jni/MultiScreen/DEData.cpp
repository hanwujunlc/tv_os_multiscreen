#include "DEData.h"

USING_NS_COOCAA;


DEData& DEData::operator= (const DEData & src)
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

std::string DEData::getStringValue(const std::string & key)
{
	cJSON *root = _dataBuffer->getBuffer();
	if (root)
	{
		cJSON* value = cJSON_GetObjectItem(root, key.c_str());
		if (value)
		{
			return value->valuestring;
		}
	}
	
	return std::string();
}

int DEData::getIntValue(const std::string&  key,int& result)
{
	cJSON *root = _dataBuffer->getBuffer();
	if (root)
	{
		cJSON* value = cJSON_GetObjectItem(root, key.c_str());
		if (value)
		{
			result = value->valueint;
			return 0;
		}
	}

	return -1;
}

int DEData::getLongValue(const std::string&  key,long& result)
{
	cJSON *root = _dataBuffer->getBuffer();
	if (root)
	{
		cJSON* value = cJSON_GetObjectItem(root, key.c_str());
		if (value)
		{
			result = static_cast<long>(value->valuedouble);
			return 0;
		}
	}

	return -1;
}

int DEData::getBooleanValue(const std::string&  key,bool& result)
{
	cJSON *root = _dataBuffer->getBuffer();
	if (root)
	{
		cJSON* value = cJSON_GetObjectItem(root, key.c_str());
		if (value)
		{
			if (value->valueint == cJSON_True)
			{
				result = true;
			}
			else
			{
				result = false;
			}
			return 0;
		}
	}

	return -1;
}

int DEData::getFloatValue(const std::string&  key,float& result)
{
	cJSON *root = _dataBuffer->getBuffer();
	if (root)
	{
		cJSON* value = cJSON_GetObjectItem(root, key.c_str());
		if (value)
		{
			//BUG: double to float!
			result = static_cast<float>(value->valuedouble);
			return 0;
		}
	}

	return -1;
}

int DEData::getDoubleValue(const std::string &  key,double& result)
{
	cJSON *root = _dataBuffer->getBuffer();
	if (root)
	{
		cJSON* value = cJSON_GetObjectItem(root, key.c_str());
		if (value)
		{
			result = value->valuedouble;
			return 0;
		}
	}

	return -1;
}

int DEData::getDEDataListValue(const std::string&  key,  std::vector<DEData>& list)
{

	cJSON *root = _dataBuffer->getBuffer();
	if (root)
	{
		cJSON* value = cJSON_GetObjectItem(root, key.c_str());
		if (value)
		{
			int size = cJSON_GetArraySize(value);
			for (int i = 0; i < size; i++)
			{
				DEData item(cJSON_GetArrayItem(value, i)->valuestring);
				list.push_back(item);
			}
			return 0;
		}

	}

	return -1;
}

int  DEData::getDEData(const std::string& key,DEData& data)
{
	cJSON *root = _dataBuffer->getBuffer();
	if (root)
	{
		cJSON* value = cJSON_GetObjectItem(root, key.c_str());
		if (value)
		{
			data = DEData(value->valuestring);
			return 0;
		}
	}

	return -1;

}

void DEData::addValue(const std::string&  key, const std::string& value)
{

	cJSON *root = _dataBuffer->getBuffer();
	cJSON_AddStringToObject(root, key.c_str(), value.c_str());
}

void DEData::addValue(const std::string&  key,const char value[])
{
	cJSON *root = _dataBuffer->getBuffer();
	cJSON_AddStringToObject(root, key.c_str(), value);
}

void DEData::addValue(const std::string& key, std::vector<DEData> value)
{
	cJSON *root = _dataBuffer->getBuffer();
	cJSON *arrayvalue = cJSON_CreateArray();
	for (std::vector<DEData>::iterator it = value.begin(); it != value.end(); ++it)
	{
		DEData curdata = (*it);
		cJSON_AddItemToArray(arrayvalue, curdata._dataBuffer->getBuffer());
	}
	cJSON_AddItemToObject(root, key.c_str(), arrayvalue);
}

void DEData::addValue(const  std::string&  key, const int& value)
{
	cJSON *root = _dataBuffer->getBuffer();
	cJSON_AddNumberToObject(root, key.c_str(), value);
}

void DEData::addValue(const std::string&  key, const long& value)
{
	cJSON *root = _dataBuffer->getBuffer();
	cJSON_AddNumberToObject(root, key.c_str(), value);
}

void DEData::addValue(const std::string& key, const bool& value)
{
	cJSON *root = _dataBuffer->getBuffer();
	cJSON_AddBoolToObject(root, key.c_str(), value);
}

void DEData::addValue(const std::string&  key, const float& value)
{
	cJSON *root = _dataBuffer->getBuffer();
	cJSON_AddNumberToObject(root, key.c_str(), value);
}

void DEData::addValue(const std::string&  key, const double& value)
{
	cJSON *root = _dataBuffer->getBuffer();
	cJSON_AddNumberToObject(root, key.c_str(), value);
}

void DEData::addValue(const std::string& key,DEData data)
{
	cJSON *root = _dataBuffer->getBuffer();
	cJSON_AddItemToObject(root, key.c_str(), data._dataBuffer->getBuffer());
}

std::string DEData::toString()
{
	return cJSON_Print(_dataBuffer->getBuffer());
}


bool DEData::isIntValue(const std::string& key)
{
	cJSON *root = _dataBuffer->getBuffer();
	if (root)
	{
		cJSON* value = cJSON_GetObjectItem(root, key.c_str());
		if (value)
		{
			return value->type == cJSON_Number;
		}
	}
	return false;
}

bool DEData::isStringValue(const std::string& key)
{
	cJSON *root = _dataBuffer->getBuffer();
	if (root)
	{
		cJSON* value = cJSON_GetObjectItem(root, key.c_str());
		if (value)
		{
			return value->type == cJSON_String;
		}
	}
	return false;
}