package com.tsinglink.android.library

import android.app.Application
import android.os.Environment
import android.util.Log
import androidx.test.core.app.ApplicationProvider
import androidx.test.ext.junit.runners.AndroidJUnit4
import com.tsinglink.android.library.freetype.TextDraw
import org.junit.Test
import org.junit.runner.RunWith
import java.io.File
import java.io.FileOutputStream

@RunWith(AndroidJUnit4::class)
class TextDrawTest {
    @Test
    fun draw(){
        val app = ApplicationProvider.getApplicationContext<Application>()
        val width = 640
        val height = 480
        val frame = ByteArray(width*height*3/2)
        val draw = TextDraw()
        draw.init("/system/fonts/NotoSansCJK-Regular.ttc",20,0.0f)
        draw.draw("""Log.i(TAG,"你好 hello ")""", 20, 30, frame, width, height)
        draw.draw("""Log.i(TAG,"こんにちは hello ")""", 20, 60, frame, width, height)

        draw.draw("你好 hello ", 20, 120, frame, width, height)
        draw.draw("こんにちは hello ", 20, 150, frame, width, height)
        draw.release()

        FileOutputStream(File(app.getExternalFilesDir(Environment.DIRECTORY_PICTURES),"640480.yuv")).use {
            it.write(frame)
        }
    }
}