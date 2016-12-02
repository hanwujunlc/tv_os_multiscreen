#ifndef __OSAPI_MAINPAGE_SHELL_API_H_
#define __OSAPI_MAINPAGE_SHELL_API_H_
#include "CoocaaDef.h"

#include "OSApiListenerController.h"
#include "OSApi_Base.h"
NS_COOCAA_BEGIN

class ExpandDataStruct
{
public:
	std::string _id;
	std::string _title;
	std::string _content;
	std::string _clickaction;
};

class StatusBarStruct
{
public:
	std::string _weather;
	std::string _network;
	std::string _blueTooth;
	std::string _usb;
	std::string _sdcard;
	std::string _download;
    std::string _hotspot;
	std::string _msgbox;
	std::string _username;
	std::string _usericon;
    std::string _signed;
    std::string _blerc;
};

#define DEFAUKT_SERVER_URL "http://dev.tvos.skysrt.com"//"dev.tvos.skysrt.com"

#define DOMAIN_URL "/Framework/tvos/index.php?_r=personalized/Personalized/GetPanel&panel=home_main_coocaa5"


typedef std::function<void (std::string cmd_action,std::vector<ExpandDataStruct>)> ExpandDataCallBack;

typedef std::function<void (const StatusBarStruct&)> StatusBarDataCallBack;

class OSApi_MainPageShell:public OSApi_Base
{
	/*
	* ÿһ��api�����ж������ֵ��
	*
	* 1���������named pipeͨ������һ�����������Ҵ��������سɹ��ġ���ʱ����Ҫ���ж���Ĵ���uiengine�����д�������л���
	* 0���������named pipeͨ������һ�����������������ϲ�����ԭ�򷵻ش���Ľ������ʱ����Ҫ�����������Ҫ����һ����ʾ�򼴿ɡ�
	* -1�������named pipeͨ�Ŵ��󣬿�������Ϊ�ϲ�Ŀǳ����������Ҫ�������һ��named pipe���з���������ϵͳ�ȶ��ԡ�
	*/
public:

	OSApi_MainPageShell(OSApiListenerController* controller);
	virtual ~OSApi_MainPageShell();

	//android ��:��ҳ����
	static const std::string LAUNCH_LOCAL_MEIDA;
	static const std::string LAUNCH_SOURCELIST;
	static const std::string LAUNCH_TVSETTINGS;
	static const std::string LAUNCH_QRCODE;
	static const std::string FINISH_HOME_SHELL;
	static const std::string TERMINATE_HOME_SHELL;
	static const std::string LAUNCH_WITH_PROROCOL;

	static const std::string LAUNCH_MYHISTROY;
	static const std::string LAUNCH_MYGAME;
	static const std::string LAUNCH_MYAPPS;
    static const std::string LAUNCH_SCREEN_SAVER_APP;

	static const std::string GET_EXPAND_DATA_BYPROROCOL;
	static const std::string GET_WEBSERVICE_URL;

	static const std::string GET_STATUS_BAR_DATA;
	static const std::string IS_NET_CONNECTED;
    static const std::string GET_SCREEN_SAVER_TIME;

	static const std::string GET_CURRENT_LANGUAGE;

	//submitlog
	static const std::string SUBMITLOG_PAGEOPEN;
	static const std::string SUBMITLOG_PAGECLOSE;
	static const std::string SUBMITLOG_MODULECLICK;

	//check window focused
	static const std::string ISCURRENTWINDOWFOCUSED;

	/*//get unread msg count in message box
	static const std::string GET_UNREAD_MSG_COUNT;
	*/
	static const std::string LAUNCH_MESSAGEBOXAPP;
	static const std::string LAUNCH_BLUETOOTHSETTING;
	static const std::string LAUNCH_NETWORKSETTING;
	static const std::string LAUNCH_USERACCOUNTSETTING;

	//heart beat
	static const std::string HEART_BEAT;

	//bonus sign In
	static const std::string CLICKBONUS_SIGN_IN;

	//ble remote control
	static const std::string BLE_REMOTECONTROL_GUIDE;

	int lunchLocalMedia();
	int lunchSourceList();
	int lunchTvSettings();
	int lunchQrCode();

	int finishMainPageShell();

	void terminateHomeShell();

	//protocol
	int lunchWithProtocolParam(std::string cmd_param);

	//histroy, game, app
	int launchMyHistroy();
	int launchMyGame();
	int launchMyApps();

    // screen saver
    int launchScreenSaverApp();

	//get data struct
	void getExpandDataByProtocol(std::string cmd_param,OSAPICALLBACK expandCallBack);

	//get web url
	std::string getWebServiceUrl(std::string md5);

	//get state bar 
	void getStateBarDetails(OSAPICALLBACK statebarCallBack);

	bool isNetConnected();

    int getScreenSaverStartupTime();

	//currentlanguage
	std::string getCurrentLanguage();

	//submit log
	bool submitLog_PageOpen(std::string page_tag,std::string page_detail,int pageposition);
	bool submitLog_PageClose(std::string page_tag,std::string page_detail,int pageposition);
	bool submitLog_ModuleClick(std::string module_tag,std::string module_detail,int moduleposition);

	//check current focus on window
	bool isCurrentWindowFocused();

	//ȥ���ӿ�
	//int getUnReadMessageCount();

	int launchMessageBoxApp();
	int launchBlueToothSetting();
	int launchNetworkSetting();
	int launchUserAccountSetting();

	bool heartBeat();
	bool clickBounsSignIn(int x,int y,int width,int height);
	bool clickBleRemoteGuide();
private:
	OSApi_MainPageShell(const OSApi_MainPageShell&);
	OSApi_MainPageShell& operator = (const OSApi_MainPageShell&);

	int lunch(std::string module);

};

NS_COOCAA_END


#endif //__OSAPI_MAINPAGE_SHELL_API_H_