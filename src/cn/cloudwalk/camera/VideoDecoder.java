package cn.cloudwalk.camera;

import java.nio.ByteBuffer;
import java.util.Arrays;

import android.media.MediaCodec;
import android.media.MediaFormat;
import android.util.Log;

public class VideoDecoder {
	private final static int CODEC_H264 = 1;
	private final static int CODEC_MPEG = 2;
	private final static int CODEC_UNKNOW = 0;
	private static String MIME_TYPE = "video/avc";
	private MediaCodec mMediaCodec = null;
	private static MediaFormat mediaFormat = null;
	ByteBuffer[] outputBuffers;
	ByteBuffer[] inputBuffers;
	int mCount = 0;
	int offset = 0;

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

	public byte[] getOutput(int timeoutUS) {
		try {

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
			
			ByteBuffer outputBuffer = outputBuffers[outputBufferIndex];  
				
			outputBuffer.get(outData);
			mMediaCodec.releaseOutputBuffer(outputBufferIndex, true);
			
			return outData;
			
		} 
		catch (IllegalStateException e) {
			// TODO: handle exception
			Log.e("Media", "dequeueOutputBuffer error");
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
		}
		return 0;
	}
}
