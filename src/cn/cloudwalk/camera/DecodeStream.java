package cn.cloudwalk.camera;

import java.io.Closeable;
import java.nio.ByteBuffer;
import java.nio.channels.ClosedByInterruptException;
import java.util.Arrays;

import android.media.MediaCodec;
import android.media.MediaFormat;
import android.util.Log;

public class DecodeStream {
	
	private static DecodeStream api = null;
	
	private static MediaCodec mMediaCodec;
  
	private static String MIME_TYPE = "video/avc";


	private final static int TIME_INTERNAL = 30;
	private final static int CODEC_H264 = 1;
	private final static int CODEC_MPEG = 2;
	private final static int CODEC_UNKNOW = 0;
	private static MediaInfo Information = null;
	private static MediaFormat mediaFormat = null;

	private static boolean closed = false;
	static private RtspObserver observer=null;

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
		
		//连接成功后创建线程.或者连接成功后，线程开始工作.平时线程是睡眠的.
		new Thread(new Runnable() {
			
			@Override
			public void run() {
				int mCount = 0;
				int offset = 0;
				closed = false;
				// TODO Auto-generated method stub
				while(!closed)
				//	while(true)
				{
					int timeout_ms = 500;
					byte arr[] = GetCameraStreamJNI.get().GetImage(timeout_ms);
					
					if(arr==null) 
					{
						continue;
					}
					
					// Get input buffer index
					ByteBuffer[] inputBuffers  = mMediaCodec.getInputBuffers();
					if(inputBuffers == null)
					{
						
					}
					//Log.e("Media","begin dequeueInputBuffer");
					int inputBufferIndex = mMediaCodec.dequeueInputBuffer(100000);
					//Log.e("Media","end dequeueInputBuffer");
				
					if (inputBufferIndex >= 0) {
						ByteBuffer inputBuffer = inputBuffers[inputBufferIndex];
						inputBuffer.clear();
						inputBuffer.put(arr, offset, arr.length);
						
						mMediaCodec.queueInputBuffer(inputBufferIndex, 0, arr.length, mCount* TIME_INTERNAL, 0);
						
						mCount++;
					} else {
						Log.e("Media", "inputBufferIndex < 0");
						continue;
					}
					
					ByteBuffer[] outputBuffers = mMediaCodec.getOutputBuffers();  
					
					// Get output buffer index
					MediaCodec.BufferInfo bufferInfo = new MediaCodec.BufferInfo();
					//Log.e("Media","begin dequeueOutputBuffer");
					//max 40ms
					int outputBufferIndex = mMediaCodec.dequeueOutputBuffer(bufferInfo, 40000);
					//Log.e("Media","end dequeueOutputBuffer");
					if(outputBufferIndex < 0)
					{
						Log.e("Media", "outputBufferIndex = "+outputBufferIndex);
						if(outputBufferIndex == -1)
						{
							//try wait
						}
						else if(outputBufferIndex == -2)
						{
							//MediaFormat format = mMediaCodec.getOutputFormat();
							//output format changed
						}
						else if(outputBufferIndex == -3)
						{
							//outputBuffers = mMediaCodec.getOutputBuffers();
							//output buffer changed
						}
						else
						{
							//unkown error
						}
					}
					
					while(outputBufferIndex >=0)
					{
						byte outData[] = new byte[bufferInfo.size];
						ByteBuffer outputBuffer = outputBuffers[outputBufferIndex];  
							
						outputBuffer.get(outData);
						mMediaCodec.releaseOutputBuffer(outputBufferIndex, true);
						if(observer!=null)
						{
							byte[] yarr = Arrays.copyOfRange(outData, 0, 1080*1920);
		            		byte[] uarr = Arrays.copyOfRange(outData, 1080*1920, 1080*1920*5/4);
		            		byte[] varr = Arrays.copyOfRange(outData, 1080*1920*5/4, 1080*1920*3/2);
							observer.onDataArrival(yarr, uarr,varr);
						}
						outputBufferIndex = mMediaCodec.dequeueOutputBuffer(bufferInfo, 0);
						
					}
					
					
				}
					//Log.e("Media","Thread Quit0");
				mMediaCodec.stop();
				mMediaCodec.release();
				mMediaCodec = null;
				mediaFormat = null;
			}
		}).start();
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
		if(Information.format == CODEC_H264)MIME_TYPE="video/avc";
		else if(Information.format == CODEC_MPEG)MIME_TYPE="video/mp4v-es";
		else if(Information.format == CODEC_UNKNOW) return -1;

		if(mMediaCodec==null)
		{
			mMediaCodec = MediaCodec.createDecoderByType(MIME_TYPE);
			
			
	    	if (mMediaCodec == null)
	    	{
	    	
	    		return -1;
	    	}
	    	if(observer!=null)
	    	{
	    		observer.onConfigConfiged(Information.width, Information.height,Information.format);
	    	}

	    	mediaFormat = MediaFormat.createVideoFormat(MIME_TYPE,
	    			Information.width, Information.height);

	    	mMediaCodec.configure(mediaFormat, null,null, 0);

	    	mMediaCodec.start();
	    	
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

