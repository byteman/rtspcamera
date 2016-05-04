package cn.cloudwalk.camera;

import java.io.Closeable;
import java.nio.ByteBuffer;
import java.nio.channels.ClosedByInterruptException;
import java.util.Arrays;

import android.R.integer;
import android.media.MediaCodec;
import android.media.MediaCodec.CryptoException;
import android.media.MediaFormat;
import android.util.Log;

public class DecodeStream {
	
	private static DecodeStream api = null;
	
	private static boolean closed = false;
	static private RtspObserver observer=null;
	private static Thread _thread; 
	private static VideoDecoder _decoderDecoder;
	private static MediaInfo Information;
	public void setCallbak(RtspObserver obs)
	{
		observer = obs;
	}
	private static void stopThread()
	{
		closed = true;
	}
	private static void startThread()
	{
		closed = false;
		
		//连接成功后创建线程.或者连接成功后，线程开始工作.平时线程是睡眠的.
		_thread = new Thread(new Runnable() {
			
			@Override
			public void run() {
			
				// TODO Auto-generated method stub
				while(!closed)
				{
					int timeout_ms = 500;
					byte arr[] = GetCameraStreamJNI.get().GetImage(timeout_ms);
					
					if(arr==null) 
					{
						continue;
					}
					if(_decoderDecoder.writeInput(arr,1000000) < 0)
					{
						
					}
				
					byte[] outData=_decoderDecoder.getOutput(20000);
					while(outData != null)
					{
						if(observer!=null)
						{
							int size = 1920*1080;
							byte[] yarr = Arrays.copyOfRange(outData, 0, size );
		            		byte[] uarr = Arrays.copyOfRange(outData, size, size*5/4);
		            		byte[] varr = Arrays.copyOfRange(outData, size*5/4, size*3/2);
							observer.onDataArrival(yarr, uarr,varr);
						}
						outData=_decoderDecoder.getOutput(0);
					}
				
					
				}
				
				Log.e("Media", "decoder thread exit");
			}
		});
		_thread.start();
	}
	public int Start(String url,int timeout_ms){
		
		int err = GetCameraStreamJNI.get().Open(url,timeout_ms,true,true);
		//打开摄像头设置成异步，不是立即返回连接成功后，由jni层回调状态，根据返回的格式，再创建解码器
		//或者将解码器放到jni层去做.自动重连.连接成功或者断开后回调通知界面层.
		if(err != 0 ) 
		{
			return err;
		}

		return 0;
	}
	
	public MediaInfo GetInfo(){
		
		return Information;
	}

	
	static public void notifyClosed()
	{
	
	
		if(observer!=null)
		{
			observer.onDisconnect();
		}
		stopThread();
	}
	public int Closeurl()
	{
		closed  = true;
		stopThread();
		int ret = GetCameraStreamJNI.get().Close();
		Log.d("me", "ret="+ret);
		return ret;
	}
	
	public static DecodeStream get()
	{
		if(api == null)
			api = new DecodeStream();
		return api;
	}
	public static int notifyOpened() {
		// TODO Auto-generated method stub
		
		Information = GetCameraStreamJNI.get().GetMediaInfo();
		if(Information==null)
		{
			Log.e("Media","Can not get Information");
			return -1;
		}
		if(_decoderDecoder==null)
		{
			_decoderDecoder = new VideoDecoder();
	    
	    	_decoderDecoder.init(Information);
	    	
		}
		
		if(observer!=null)
		{
			observer.onConnect();
			observer.onConfigConfiged(Information.width, Information.height,Information.format);
		}
		startThread();
		return 0;
	}
	public void pause(boolean b) {
		// TODO Auto-generated method stub
		GetCameraStreamJNI.get().Pause(b);
	}
	
}

