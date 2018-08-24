
#include "dev_manager.h"



int main(int argc, char *argv[])
{


	DevManager *m = new DevManager();
	int ret = m->Start();
	if (ret != ERR_SUCCESS)
	{
		return ret;
	}

	while (true) 
	{
		this_thread::sleep_for(chrono::seconds(1));
	}
	m->Stop();

	return 0;
}