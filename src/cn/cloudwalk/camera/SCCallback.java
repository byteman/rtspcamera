package cn.cloudwalk.camera;


import android.util.Log;

/**
 * 
 * @author
 * TODO SCMessageHandler
 */
public class SCCallback {
	

	static public int SCMessageHandler(int obj, int msg) 
	{
		
		if(obj == 0)
		{
			DecodeStream.notifyClosed();
		}
		else if(obj == 1)
		{
			DecodeStream.notifyOpened();
		}
		
		return 0;
	}

	
}
