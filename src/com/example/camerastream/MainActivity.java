package com.example.camerastream;

//import java.nio.ByteBuffer;

import java.util.Arrays;

import cn.cloudwalk.camera.DecodeStream;
import cn.cloudwalk.camera.MediaInfo;
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

public class MainActivity extends Activity {
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
				 int err = DecodeStream.get().Start("rtsp://admin:admin123@192.168.10.200",2000);
				// int err = DecodeStream.get().Start("rtsp://192.168.1.244/8.264",2000);
			    	Log.e("camera", "ret="+err);
			    	if(err < 0) return;
			    	MediaInfo ret = DecodeStream.get().GetInfo();
			    	
			    	Log.e("camera",ret.width+"");
			    	Log.e("camera",ret.height+"");
			    	Log.e("camera",ret.format+"");
			    	mGLFRenderer.update(ret.width, ret.height);
				// TODO Auto-generated method stub
				 Thread thread=new Thread(new Runnable()  
			        {  
			            @Override  
			            public void run()  
			            {  
			            	//decode("rtsp://admin:admin123@192.168.10.200");
			            	//decode("rtsp://192.168.1.118/life.264");
			            	while(true){
			            		byte image[] = DecodeStream.get().GetImage(500);
			            		if(DecodeStream.get().isDisconnected())
			            		{
			            			Log.e("camera","isDisconnected true");
			            			break;
			            		}
			            		if(image == null){
			            			Log.e("camera","image has nothing");
			            			
			            			continue;
			            		}
			            		byte[] yarr = Arrays.copyOfRange(image, 0, 1080*1920);
			            		byte[] uarr = Arrays.copyOfRange(image, 1080*1920, 1080*1920*5/4);
			            		byte[] varr = Arrays.copyOfRange(image, 1080*1920*5/4, 1080*1920*3/2);
			            		
			            		mGLFRenderer.update(yarr,uarr,varr);
			            		Log.e("camera","image="+image.length);
			            	}
			            }  
			        });  
			        thread.start();  
			}
		});
    }



	@Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }
    
}
