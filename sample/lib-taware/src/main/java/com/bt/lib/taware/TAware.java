package com.bt.lib.taware;

public class TAware {


    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("taware");
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    /**
     * @param async 0 同步 1 异步
     */
    public static native void enableAwareThread(int async);

}
