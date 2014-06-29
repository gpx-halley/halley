package com.halley.sanddemo;

import android.app.Activity;
import android.util.Log;
import android.widget.TextView;
import android.os.Bundle;

public class MainActivity extends Activity {

    /** Called when the activity is first created. */
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		try {
			TextView tv = (TextView) findViewById(R.id.HelloTV);
			tv.setText("Before JNI");
			tv.setText(stringFromJNI());
//			unimplementedStringFromJNI();
		} catch (final UnsatisfiedLinkError ule) {
			Log.e("stringFromJNI", ule.toString());
		} catch (final Exception e) {
			Log.e("stringFromJNI", "Exception");
		}
	}
	
	/* A native method that is implemented by the
	 * 'hello-jni' native library, which is packaged
	 * with this application.
	 */
	public native String  stringFromJNI();
	
    /* This is another native method declaration that is *not*
     * implemented by 'hello-jni'. This is simply to show that
     * you can declare as many native methods in your Java code
     * as you want, their implementation is searched in the
     * currently loaded native libraries only the first time
     * you call them.
     *
     * Trying to call this function will result in a
     * java.lang.UnsatisfiedLinkError exception !
     */
    public native String  unimplementedStringFromJNI();

    /* this is used to load the 'hello-jni' library on application
     * startup. The library has already been unpacked into
     * /data/data/com.example.hellojni/lib/libhello-jni.so at
     * installation time by the package manager.
     */
    static {
		try {
			System.loadLibrary("SandDemo");
		} catch (SecurityException se) {
			Log.e("System.loadLibrary", "SecurityException");
		} catch (Exception e) {
			Log.e("System.loadLibrary", "Exception");
		}
    }

}
