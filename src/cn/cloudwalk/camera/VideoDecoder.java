package cn.cloudwalk.camera;

import java.nio.ByteBuffer;
import java.util.Arrays;

import android.media.MediaCodec;
import android.media.MediaCodecInfo;
import android.media.MediaFormat;
import android.util.Log;

public class VideoDecoder {
	private final static int CODEC_H264 = 1;
	private final static int CODEC_MPEG = 2;
	private final static int CODEC_UNKNOW = 0;
	private static String MIME_TYPE = "video/avc";
	private MediaCodec mMediaCodec = null;
	private static MediaFormat mediaFormat = null;
	private static MediaInfo mediaInfo = null;
	ByteBuffer[] outputBuffers;
	ByteBuffer[] inputBuffers;
	int mCount = 0;
	int offset = 0;
	int format = 0;
	public int writeInput(byte[] arr,int timeoutUS) {
		try {
			if(mMediaCodec==null)return -1;
			// Log.e("Media","begin dequeueInputBuffer");
			int inputBufferIndex = mMediaCodec.dequeueInputBuffer(timeoutUS);
			// Log.e("Media","end dequeueInputBuffer");

			if (inputBufferIndex >= 0) {
				ByteBuffer inputBuffer = inputBuffers[inputBufferIndex];
				inputBuffer.clear();
				inputBuffer.put(arr, offset, arr.length);

				mMediaCodec.queueInputBuffer(inputBufferIndex, 0, arr.length,
						0, 0);

				mCount++;
			} else {
				Log.e("Media", "inputBufferIndex < 0");
				return -2;
			}
		} catch (IllegalStateException e) {
			return -3;
		}
		return 0;
	}
	public byte[] swapYUV420SemiPlanartoYUV420Planar(byte[] yv12bytes, int width, int height) 
	{
		 byte[] i420bytes = new byte[yv12bytes.length];
		 System.arraycopy(yv12bytes, 0, i420bytes, 0, width*height); // Y
		
		 for (int i = 0; i < (width/4*height); i++)
		 {
			 i420bytes[i+width*height]   = yv12bytes[i*2+width*height];
			 i420bytes[i+width*height*5/4] = yv12bytes[i*2+1+width*height];
			
		 }
		  
		
		 return i420bytes;
	}
	
	public byte[] getOutput(int timeoutUS) {
		try {
			byte[] tmp = null;
			MediaCodec.BufferInfo bufferInfo = new MediaCodec.BufferInfo();
			// Log.e("Media","begin dequeueOutputBuffer");
			int outputBufferIndex = -1;
			// max 40ms

			outputBufferIndex = mMediaCodec.dequeueOutputBuffer(bufferInfo,
					timeoutUS);

			// Log.e("Media","end dequeueOutputBuffer");
			if (outputBufferIndex < 0) {
				//Log.e("Media", "outputBufferIndex = " + outputBufferIndex);
				if (outputBufferIndex == -1) {
					// try wait
				} else if (outputBufferIndex == -2) {
					MediaFormat format = mMediaCodec.getOutputFormat();
					mMediaCodec.configure(format, null, null, 0);
					// output format changed

				} else if (outputBufferIndex == -3) {
					outputBuffers = mMediaCodec.getOutputBuffers();
					// output buffer changed

				} else {
					// unkown error
				}
				return null;
			}
			byte outData[] = new byte[bufferInfo.size];
			//Log.e("Media","buffsize="+bufferInfo.size);
			ByteBuffer outputBuffer = outputBuffers[outputBufferIndex];  
				
			outputBuffer.get(outData);
			//tmp = swapYV12toI420(outData,mediaInfo.width,mediaInfo.height);
			
			if(format == MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420Planar)
			{
				tmp = outData;
			}
			else if(format == MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420SemiPlanar)
			{
				 tmp = swapYUV420SemiPlanartoYUV420Planar(outData,mediaInfo.width,mediaInfo.height);
			}
			
			mMediaCodec.releaseOutputBuffer(outputBufferIndex, true);
			
			return tmp;
			
		} 
		
		catch (IllegalStateException e) {
			// TODO: handle exception
			if(e!=null)
			{
				Log.e("Media", "dequeueOutputBuffer error"+ e.getMessage());
			}
			
			// mMediaCodec.stop();
			// mMediaCodec.start();
			return null;
		}
		

	}

	public int init(MediaInfo info) {
		if (info.format == CODEC_H264)
			MIME_TYPE = "video/avc";
		else if (info.format == CODEC_MPEG)
			MIME_TYPE = "video/mp4v-es";
		else if (info.format == CODEC_UNKNOW)
			return -1;

		if (mMediaCodec == null) {

			mMediaCodec = MediaCodec.createDecoderByType(MIME_TYPE);

			if (mMediaCodec == null) {

				return -1;
			}

			mediaFormat = MediaFormat.createVideoFormat(MIME_TYPE, info.width,
					info.height);

			mMediaCodec.configure(mediaFormat, null, null, 0);

			mMediaCodec.start();
			outputBuffers = mMediaCodec.getOutputBuffers();
			inputBuffers = mMediaCodec.getInputBuffers();
			format = mMediaCodec.getOutputFormat().getInteger(MediaFormat.KEY_COLOR_FORMAT);
			Log.e("Media", "format="+format);
			mediaInfo = info;
		}
		return 0;
	}
}
