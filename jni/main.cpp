#include "RtspCameraCap.h"

int main(int argc, char* argv[])
{
	RtspCameraCap cam;
	int err = cam.open("rtsp://admin:admin123@192.168.10.200",0,0,0,0);
	//int err = cam.open("rtsp://192.168.1.118/life.264",0,0,0);
	if(err != 0)
	{
		printf("error open,and err=%d\n",err);
		return -1;
	}
	else{
		printf("correct err=%d\n",err);
		//return 0;
	}
	printf("123456\n");
	while(1)
	{
		media_packt* pkt = cam.getimage(1000);
		if(pkt == NULL) continue;
		printf("len=%d\r\n",pkt->size);
		delete pkt;
		//CORE::Core_Thread::sleep(100);
	}
	getchar();

}