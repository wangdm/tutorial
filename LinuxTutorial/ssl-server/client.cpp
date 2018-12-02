
#include "alexa.h"
#include "socket.h"
#include "device.h"


int main (int argc, char* argv[])
{
	Device* dev = new Device();
	dev->name = "living room";
	dev->mac = "00626E6A2C5F";
	dev->userToken = "ecabb46160054d3eba56692e87949fb4";
	dev->devToken = "615a7d2d84164d27af08fa2a043fb85e";
	dev->type = "TEST";
	dev->rom = "1.0.0";

	dev->Connect("34.213.217.1", 443);
	dev->Start();

	dev->Stop();
	dev->Disconnect();

	delete dev;
}
