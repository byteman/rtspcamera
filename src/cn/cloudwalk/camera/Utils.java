package cn.cloudwalk.camera;

import android.util.Log;

public class Utils {
	public static String byte2hex(byte [] buffer,int offset,int size){  
        String h = "";  
          
        int len = size>(buffer.length - offset)?(buffer.length - offset):size;
        
        for(int i = offset; i < len; i++){  
            String temp = Integer.toHexString(buffer[i] & 0xFF);  
            if(temp.length() == 1){  
                temp = "0" + temp;  
            }  
            h = h + " "+ temp;  
        }  
          
        return h;  
          
    }  
	public static void LOGD(final String tag, String message) {
		// 根据日志的级别判断是否打印日志
		 Log.d(tag, message);
	
		
	}
	public static void LOGD(String message) {
		// 根据日志的级别判断是否打印日志
		 Log.d("MyGL", message);
	
		
	}
	public static void LOGV(final String tag, String message) {

		Log.v(tag, message);

	}



	public static void LOGI(String tag, String message) {
		Log.i(tag, message);
	}

	

	public static void LOGW(String tag, String message) {
		Log.w(tag, message);
	}

	

	public static void LOGE(String tag, String message) {
		Log.e(tag, message);
	}

	
}
