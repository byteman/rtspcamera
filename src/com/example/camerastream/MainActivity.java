package com.example.camerastream;

//import java.nio.ByteBuffer;

import java.util.Arrays;

import cn.cloudwalk.camera.DecodeStream;
import cn.cloudwalk.camera.MediaInfo;
import cn.cloudwalk.camera.RtspObserver;
import android.os.Bundle;
import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;
import android.opengl.GLSurfaceView;

public class MainActivity extends Activity implements RtspObserver{
	public static boolean detectOpenGLES20(Context context) {  
	    ActivityManager am = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);  
	    ConfigurationInfo info = am.getDeviceConfigurationInfo();  
	    return (info.reqGlEsVersion >= 0x20000);  
	}

	private GLFrameSurface mGLSurface;
	private GLFrameRenderer mGLFRenderer;  
	//SurfaceView mSurfaceView=null;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
       
        setContentView(R.layout.activity_main);
        
       
        if(!detectOpenGLES20(this))
        {
        	Toast.makeText(this, "do not support gels2", Toast.LENGTH_LONG);
        	return;
        }
        // mSurfaceView = (SurfaceView) findViewById(R.id.surfaceView1);

        mGLSurface = (GLFrameSurface) findViewById(R.id.surfaceView1);  
        mGLSurface.setEGLContextClientVersion(2);  
        DisplayMetrics displaysMetrics = new DisplayMetrics(); 
        getWindowManager().getDefaultDisplay().getMetrics(displaysMetrics);
        
        mGLFRenderer = new GLFrameRenderer(mGLSurface,displaysMetrics);  
        mGLSurface.setRenderer(mGLFRenderer);  
      
        Button btn = (Button) findViewById(R.id.button1);
        btn.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				DecodeStream.get().setCallbak(MainActivity.this);
				int err = DecodeStream.get().Start("rtsp://admin:admin123@192.168.1.200",2000);
				// int err = DecodeStream.get().Start("rtsp://192.168.1.245/8.264",2000);
		    	Log.e("camera", "ret="+err);
		    	if(err < 0) 
		    		return;
		    
		    	
				
				
			}
		});
    }



	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
		DecodeStream.get().pause(true);
	}



	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		DecodeStream.get().pause(false);
	}



	@Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }



	@Override
	public boolean onConnect() {
		// TODO Auto-generated method stub
		Log.e("camera","rtsp connect ok");		
		return false;
	}



	@Override
	public boolean onConfigConfiged(int width, int height, int format) {
		// TODO Auto-generated method stub
		mGLFRenderer.update(width, height);
		return false;
	}



	@Override
	public boolean onDisconnect() {
		// TODO Auto-generated method stub
		Log.e("camera","rtsp disconnect ");		
		return false;
	}



	@Override
	public boolean onDataArrival(byte[] y, byte[] u, byte[] v) {
		// TODO Auto-generated method stub
		mGLFRenderer.update(y, u, v);
		return false;
	}
    
}
