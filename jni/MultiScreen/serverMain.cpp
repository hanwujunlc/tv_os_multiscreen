#include "MultiScreenService.h"
using namespace android;

int main(int argc, char** argv)
{
	MultiScreenService multiScreenService;
	multiScreenService.instantiate();

	ProcessState::self()->startThreadPool();
	IPCThreadState::self()->joinThreadPool();
}
