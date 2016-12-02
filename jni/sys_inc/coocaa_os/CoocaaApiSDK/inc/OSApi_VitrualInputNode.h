#ifndef __OSAPI_VITRUAL_INPUT_NODE_H_
#define __OSAPI_VITRUAL_INPUT_NODE_H_
#include "CoocaaDef.h"

#include "OSApiListenerController.h"
#include "OSApi_Base.h"
NS_COOCAA_BEGIN

typedef enum {
	SENSOR_TYPE_ACCELEROMETER = 1,
	SENSOR_TYPE_MAGNETIC_FIELD = 2,
	SENSOR_TYPE_TYPE_GYROSCOPE = 4
} SENSOR_TYPE_t ;

class OSApi_VitrualInputNode:public OSApi_Base
{
	/*
	* ÿһ��api�����ж������ֵ��
	*
	* 1���������named pipeͨ������һ�����������Ҵ��������سɹ��ġ���ʱ����Ҫ���ж���Ĵ���uiengine�����д�������л���
	* 0���������named pipeͨ������һ�����������������ϲ�����ԭ�򷵻ش���Ľ������ʱ����Ҫ�����������Ҫ����һ����ʾ�򼴿ɡ�
	* -1�������named pipeͨ�Ŵ��󣬿�������Ϊ�ϲ�Ŀǳ����������Ҫ�������һ��named pipe���з���������ϵͳ�ȶ��ԡ�
	*/
public:
	static const std::string SIMULATE_KEY_DOWN;
	static const std::string SIMULATE_KEY_UP;
	static const std::string SIMULATE_KEYCLICK;
	static const std::string SIMULATE_BUTTON_UP;
	static const std::string SIMULATE_BUTTON_DOWN;
	static const std::string SIMULATE_BUTTON_CLICK;
	static const std::string SIMULATE_SCROLL_UP;
	static const std::string SIMULATE_SCROLL_DOWN;

	static const std::string SIMULATE_MOUSE_MOVE;
	static const std::string SIMULATE_MOUSE_MOVETO;
	static const std::string SIMULATE_TOUCH;

	static const std::string SIMULATE_SENSOR_ACCELEROMETER;
	static const std::string SIMULATE_SENSOR_TYPE_MAGNETIC_FIELD;
	static const std::string SIMULATE_SENSOR_TYPE_TYPE_GYROSCOPE;

	OSApi_VitrualInputNode(OSApiListenerController* controller);
	virtual ~OSApi_VitrualInputNode();

	void simulate_key_down(int keyCode);
	void simulate_key_up(int keyCode);
	void simulate_key_click(int keyCode);

	void simulate_button_up(int keyCode);
	void simulate_button_down(int keyCode);
	void simulate_button_click(int keyCode);

	void simulate_scroll_up();
	void simulate_scroll_down();

	void simulate_mouse_move(float x_rel,float y_rel);
	void simulate_mouse_moveto(float x_abs,float y_abs);

	void simulate_touch(float x[],float y[],int status);
	//���������͵�VirtualEvent
	void simulate_sensor(SENSOR_TYPE_t sensortype,long timestemp,float sensordata[],int size);

private:
	OSApi_VitrualInputNode(const OSApi_VitrualInputNode&);
	OSApi_VitrualInputNode& operator = (const OSApi_VitrualInputNode&);

	void simulateKeyCode(std::string forwhat,int keyCode);
};

NS_COOCAA_END


#endif //__OSAPI_VITRUAL_INPUT_NODE_H_