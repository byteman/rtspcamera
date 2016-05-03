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
		Log.e("camera","disconnect " + msg);
		DecodeStream.notifyClosed();
		return 0;
	}

	
}
