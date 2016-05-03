package cn.cloudwalk.camera;



public interface RtspObserver {
	 public boolean onConnect();
	 public boolean onConfigConfiged(int width,int height,int format);
	 public boolean onDisconnect();
	 public boolean onDataArrival(byte[] y, byte[] u, byte[] v);
}
