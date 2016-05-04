package cn.cloudwalk.camera;

public class GetCameraStreamJNI{
	private static GetCameraStreamJNI jni = null;
	
	public native int Open(String path,int timeout_ms,boolean auto_reconnect,boolean tcp);
	
	public native byte[] GetImage(int timeout_ms);
	public native int Pause(boolean _pause);
	public native int Close();
	public native MediaInfo GetMediaInfo();
	public static GetCameraStreamJNI get()
	{
		if(jni == null)
			jni = new GetCameraStreamJNI();
		return jni;
	}
	GetCameraStreamJNI()
	{
		System.loadLibrary("gnustl_shared");
		System.loadLibrary("ijkffmpeg");
		System.loadLibrary("GetCameraStream");
	}

	
}