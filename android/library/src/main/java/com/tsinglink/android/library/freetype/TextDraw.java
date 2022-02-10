package com.tsinglink.android.library.freetype;

public class TextDraw {
    static {
        System.loadLibrary("freetype");
        System.loadLibrary("TextDraw");
    }
    private long ctx = 0;
    public  native void init(String fontPath,int size,float rotateDegree);
    public  native void release();
    public native void draw(String text,int x,int y,byte[]frame,int width, int height);
}
