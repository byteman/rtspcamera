package cn.cloudwalk.camera;

import android.util.Log;

public class Utils {
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
