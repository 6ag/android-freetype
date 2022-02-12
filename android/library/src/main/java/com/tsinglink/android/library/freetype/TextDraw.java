package com.tsinglink.android.library.freetype;

import java.nio.ByteBuffer;

public class TextDraw {
    static {
        System.loadLibrary("freetype");
        System.loadLibrary("TextDraw");
    }
    private long ctx = 0;
    public  native void init(String fontPath,int size,float rotateDegree);
    public  native void release();
    public native void draw(String text,int x,int y,byte[]frame,int width, int height);
    public native void drawBf(String text, int x, int y, ByteBuffer frame, int width, int height);
}
