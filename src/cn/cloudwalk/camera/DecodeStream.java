package cn.cloudwalk.camera;

import java.io.Closeable;
import java.nio.ByteBuffer;
import android.media.MediaCodec;
import android.media.MediaFormat;
import android.util.Log;

public class DecodeStream {
	
	private static DecodeStream api = null;
	
	private MediaCodec mMediaCodec;
  
	private String MIME_TYPE = "video/avc";

	private final String TAG = "MediaCodeSample";

	private final static int TIME_INTERNAL = 30;
	private final static int CODEC_H264 = 1;
	private final static int CODEC_MPEG = 2;
	private final static int CODEC_UNKNOW = 0;
	private MediaInfo Information = null;
	MediaFormat mediaFormat = null;

	static public  boolean disconnectd = true;
	public int Start(String url,int timeout_ms){
		if(disconnectd==false)
		{
			return 0;
		}
		int err = GetCameraStreamJNI.get().Open(url,timeout_ms,false);
		
		if(err != 0 ) return err;
		
		Information = GetCameraStreamJNI.get().GetMediaInfo();
		if(Information.format == CODEC_H264)MIME_TYPE="video/avc";
		else if(Information.format == CODEC_MPEG)MIME_TYPE="video/mp4v-es";
		else if(Information.format == CODEC_UNKNOW) return -1;
		if(mMediaCodec!=null)
		{
			String now_mime_type =  mediaFormat.getString(MediaFormat.KEY_MIME);
			int now_width  = mediaFormat.getInteger(MediaFormat.KEY_WIDTH);
			int now_height = mediaFormat.getInteger(MediaFormat.KEY_HEIGHT);
			if( (MIME_TYPE !=now_mime_type) || now_width!=Information.width || now_height!=Information.height)
			{
				mMediaCodec.stop();
				mMediaCodec = null;
				mediaFormat = null;
			}
			
			
		}
		if(mMediaCodec==null)
		{
			mMediaCodec = MediaCodec.createDecoderByType(MIME_TYPE);
			
	    	if (mMediaCodec == null)
	    	{
	    		Log.e(TAG, "createDecoder failed");
	    		return -1;
	    	}
	    	//MediaFormat mediaFormat = MediaFormat.createVideoFormat(MIME_TYPE,VIDEO_WIDTH, VIDEO_HEIGHT);
	    	mediaFormat = MediaFormat.createVideoFormat(MIME_TYPE,
	    			Information.width, Information.height);
	    	//mMediaCodec.configure(mediaFormat, mSurfaceView.getHolder().getSurface(),null, 0);
	    	mMediaCodec.configure(mediaFormat, null,null, 0);

	    	mMediaCodec.start();
	    	
		}
		new Thread(new Runnable() {
			
			@Override
			public void run() {
				int mCount = 0;
				int offset = 0;
				// TODO Auto-generated method stub
				while(!disconnectd)
				{
					int timeout_ms = 500;
					byte arr[] = GetCameraStreamJNI.get().GetImage(timeout_ms);
					
					if(arr==null) 
					{
						continue;
					}
					
					// Get input buffer index
					ByteBuffer[] inputBuffers  = mMediaCodec.getInputBuffers();
					
					int inputBufferIndex = mMediaCodec.dequeueInputBuffer(-1);

				
					if (inputBufferIndex >= 0) {
						ByteBuffer inputBuffer = inputBuffers[inputBufferIndex];
						inputBuffer.clear();
						inputBuffer.put(arr, offset, arr.length);
						//Log.e("Media","begin queueInputBuffer");
						mMediaCodec.queueInputBuffer(inputBufferIndex, 0, arr.length, mCount* TIME_INTERNAL, 0);
						//Log.e("Media","end queueInputBuffer");
						mCount++;
					} else {
						Log.e("Media", "inputBufferIndex < 0");
						continue;
					}
				}
				
			}
		}).start();
		disconnectd = false;	
		return 0;
	}
	
	public MediaInfo GetInfo(){
		
		return Information;
	}
	public boolean isDisconnected()
	{
		return disconnectd;
	}
	public byte[] GetImage(int timeout_ms){
		
		int mCount = 0;
		int offset = 0;
		if(disconnectd)
		{
			return null;
		}
		
		
		ByteBuffer[] outputBuffers = mMediaCodec.getOutputBuffers();  
		
		// Get output buffer index
		MediaCodec.BufferInfo bufferInfo = new MediaCodec.BufferInfo();
		//Log.e("Media","begin dequeueOutputBuffer");
		int outputBufferIndex = mMediaCodec.dequeueOutputBuffer(bufferInfo, -1);
		//Log.e("Media","end dequeueOutputBuffer");
		if(outputBufferIndex <0)
		{
			Log.e("Media", "outputBufferIndex < 0");
			return null;
		}
		byte outData[] = new byte[bufferInfo.size];
		ByteBuffer outputBuffer = outputBuffers[outputBufferIndex];  
			
		outputBuffer.get(outData);
	
		mMediaCodec.releaseOutputBuffer(outputBufferIndex, true);
		
		return outData;	
	}
	public void notifyClosed()
	{
		disconnectd = true;
	}
	public int Closeurl(){
		
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
	
}

