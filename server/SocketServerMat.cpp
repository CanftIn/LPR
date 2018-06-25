#include "SocketMatTransmissionServer.h"  
  
int main()  
{  
    SocketMatTransmissionServer socketMat;  
    if (socketMat.socketConnect(6666) < 0)  
    {  
        return 0;  
    }  
  
    cv::Mat image;
	bool bfirst = false;
	int a = 0;
    while (1)  
    {  
        if(socketMat.receive(image) > 0)  
        {  
            cv::imshow("server video",image);
			a++;
			if(!bfirst){
				
				if(a > 100){
					imwrite("test.jpg",image);
					bfirst =true;
				}
			}
            cv::waitKey(1);  
        }
		
		if (send(socketMat.sockConn, "r", 1, 0) < 0)  
			{  
				printf("send image error: %s(errno: %d)\n", strerror(errno), errno);  
				return -1;  
			}    
		}  
  
    socketMat.socketDisconnect();  
    return 0;  
}  
