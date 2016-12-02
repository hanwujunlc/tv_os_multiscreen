#ifndef  __OSAPI_APPNAME_DEF_H_
#define  __OSAPI_APPNAME_DEF_H_

#include "CoocaaDef.h"
#include <string>

NS_COOCAA_BEGIN

/*
* ��ȷ��ϵͳ���е�appname��Ψһ,���Ҳ�Ҫ�� - �ָ������֡�
*/
class OSApi_AppNameDef
{
public:
	/*
	* �Ὺϵͳ5.0��ҳӦ�ý�������,bin����
	*/
	static const std::string PATH_TO_MAINPAGE_APPNAME_5_0;

	/*
	*�Ὺϵͳ5.0��ҳ�ǳ���Ӧ�ý������ƣ�apk����
	*/
	static const std::string PATH_TO_MAINPAGESHELL_APPNAME_5_0;

	/*
	*�Ὺϵͳ5.0��������ڵ���̣�bin����
	*/
	static const std::string PATH_TO_VITTUAL_INPUT_EVENT_NODE;

	/*
	*�Ὺϵͳ5.0�����������̣�bin����
	*/
	static const std::string PATH_TO_DESERVICE_BIN;

	/*
	*�Ὺϵͳ5.0 DEService��apk����
	*/
	static const std::string PATH_TO_DESERVICE_APPNAME_5_0;

	/*
	*�Ὺϵͳ5.0���������ͽ��̣�bin����
	*/
	static const std::string PATH_TO_PUSHSERVICE_BIN;

	/*
	*�Ὺϵͳ5.0 PushService��apk����
	*/
	static const std::string PATH_TO_PUSHSERVICE_APPNAME_5_0;

    /*
    * �Ὺϵͳ5.0 AirplayService��bin����
    */
    static const std::string PATH_TO_AIRPLAY_BIN;
	
	/*
    * �Ὺϵͳ5.0 AdapterService��bin����
    */
    static const std::string PATH_TO_ADAPTERSERVICE_BIN;
	
	/*
	*�Ὺϵͳ5.0 AdapterService��apk����
	*/
	static const std::string PATH_TO_ADAPTERSERVICE_APPNAME_5_0;
};

NS_COOCAA_END

#endif //__OSAPI_APPNAME_DEF_H