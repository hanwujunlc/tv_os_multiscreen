LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE:= TVOSClient


LOCAL_C_INCLUDES := $(LOCAL_PATH)								\
					$(LOCAL_PATH)/../sys_inc/system/core/include	\
					$(LOCAL_PATH)/../sys_inc/frameworks/native/include	\
					$(LOCAL_PATH)/../sys_inc/external						\
					$(LOCAL_PATH)/../sys_inc/coocaa_os/LocalSocketIPC/inc	\
					$(LOCAL_PATH)/../sys_inc/coocaa_os/CoocaaApiSDK/inc		\
					$(LOCAL_PATH)/../sys_inc/coocaa_os/CoocaaApiSDK/inc		\
					$(LOCAL_PATH)/../common \

					
LOCAL_SRC_FILES += \
	../common/DataPacket.cpp \
	../common/DEData.cpp \
	../common/TCPSocket.cpp \
	../common/UDPSocket.cpp \
	../common/cJSON.c \


LOCAL_SRC_FILES += \
	clientTest.cpp


LOCAL_CFLAGS += -Wno-multichar -fno-rtti -fexceptions -fpermissive -std=c++11 -Wno-unused-parameter -Wno-reorder -DHAVE_SYS_UIO_H -DHAVE_SYS_LINUX_CAPABILITY_H
LOCAL_LDFLAGS += -fPIE -pie 

LOCAL_LDFLAGS += -L$(LOCAL_PATH)/../sys_so

LOCAL_LDFLAGS += -lcutils -lutils -lbinder 

    
LOCAL_MODULE_TAGS := optional
include $(BUILD_EXECUTABLE)