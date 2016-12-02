#include "MultiScreenClient.h"
#include <binder/IPCThreadState.h>  

using namespace android;
int main(int argc, char** argv)
{
	MultiScreenClient client;
	client.connect();

	ProcessState::self()->startThreadPool();
	IPCThreadState::self()->joinThreadPool();
}
