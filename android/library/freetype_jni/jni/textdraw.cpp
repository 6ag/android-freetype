#include "com_tsinglink_android_library_freetype_TextDraw.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <android/log.h>
#include <sys/endian.h>
#include "ConvertUTF.h"

#include <ft2build.h>
#include FT_FREETYPE_H


// https://freetype.org/freetype2/docs/tutorial/example2.cpp
/* Replace this function with something useful. */

void
draw_bitmap( FT_Bitmap*  bitmap,
             FT_Int      x,
             FT_Int      y,char *frame, int width, int height)
{
  FT_Int  i, j, p, q;
  FT_Int  x_max = x + bitmap->width;
  FT_Int  y_max = y + bitmap->rows;


  /* for simplicity, we assume that `bitmap->pixel_mode' */
  /* is `FT_PIXEL_MODE_GRAY' (i.e., not a bitmap font)   */

  for ( i = x, p = 0; i < x_max; i++, p++ )
  {
    for ( j = y, q = 0; j < y_max; j++, q++ )
    {
      if ( i < 0      || j < 0       ||
           i >= width || j >= height )
        continue;

      frame[j*width + i] |= bitmap->buffer[q * bitmap->width + p];
    }
  }
}


#if 0

int
main( int     argc,
      char**  argv )
{
  FT_Library    library;
  FT_Face       face;

  FT_GlyphSlot  slot;
  FT_Matrix     matrix;                 /* transformation matrix */
  FT_Vector     pen;                    /* untransformed origin  */
  FT_Error      error;

  char*         filename;
  char*         text;

  double        angle;
  int           target_height;
  int           n, num_chars;


  if ( argc != 3 )
  {
    fprintf ( stderr, "usage: %s font sample-text\n", argv[0] );
    exit( 1 );
  }

  filename      = argv[1];                           /* first argument     */
  text          = argv[2];                           /* second argument    */
  num_chars     = strlen( text );
  angle         = ( 0 / 360 ) * 3.14159 * 2;      /* use 25 degrees     */
  target_height = HEIGHT;

  error = FT_Init_FreeType( &library );              /* initialize library */
  /* error handling omitted */

  error = FT_New_Face( library, filename, 0, &face );/* create face object */
  /* error handling omitted */

  /* use 50pt at 100dpi */
  error = FT_Set_Char_Size( face, 10 * 64, 0,
                            100, 0 );                /* set character size */
  /* error handling omitted */

  /* cmap selection omitted;                                        */
  /* for simplicity we assume that the font contains a Unicode cmap */

  slot = face->glyph;

  /* set up matrix */
  matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
  matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
  matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
  matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );

  /* the pen position in 26.6 cartesian space coordinates; */
  /* start at (300,200) relative to the upper left corner  */
  pen.x = 10 * 64;
  pen.y = ( target_height - 80 ) * 64;

  for ( n = 0; n < num_chars; n++ )
  {
    /* set transformation */
    FT_Set_Transform( face, &matrix, &pen );

    /* load glyph image into the slot (erase previous one) */
    error = FT_Load_Char( face, text[n], FT_LOAD_RENDER );
    if ( error )
      continue;                 /* ignore errors */

    /* now, draw to our target surface (convert position) */
    draw_bitmap( &slot->bitmap,
                 slot->bitmap_left,
                 target_height - slot->bitmap_top );

    /* increment pen position */
    pen.x += slot->advance.x;
    pen.y += slot->advance.y;
  }

  show_image();

  FT_Done_Face    ( face );
  FT_Done_FreeType( library );

  return 0;
}
#endif

/* EOF */

struct Context{
  FT_Library    library;
  FT_Face       face;
  FT_Matrix     matrix;
};
/*
 * Class:     com_tsinglink_android_library_freetype_TextDraw
 * Method:    init
 * Signature: (Ljava/lang/String;I)V
 */
JNIEXPORT void JNICALL Java_com_tsinglink_android_library_freetype_TextDraw_init
  (JNIEnv *env, jobject obj, jstring fontPath, jint size, jfloat angle){
  
    Context *ctx = new Context;
    memset(ctx,0,sizeof(Context));
                 /* untransformed origin  */
  FT_Error error = FT_Init_FreeType( &ctx->library );              /* initialize library */

  jclass Exception = env->FindClass("java/lang/Exception");
  char msg[256];
  const char *filename = env->GetStringUTFChars(fontPath,NULL);
  
  jclass jclz = env->GetObjectClass(obj);
  jfieldID fid = env->GetFieldID(jclz,"ctx","J");
  /* error handling omitted */
  if (error){
      sprintf(msg, "FT_Init_FreeType failed with error:%d",error);
      goto FAIL;
  }
  error = FT_New_Face( ctx->library, filename, 0, &ctx->face );/* create face object */
    /* error handling omitted */
  if (error){
    sprintf(msg, "FT_New_Face failed with error:%d",error);
    goto FAIL;
  }

  /* use 50pt at 100dpi */
  // error = FT_Set_Char_Size( ctx->face, size * 64, 0,
  //                           100, 0 );                /* set character size */

  error = FT_Set_Pixel_Sizes( ctx->face, size, size);                /* set character size */
  if (error){
    sprintf(msg, "FT_Set_Char_Size failed with error:%d",error);
    goto FAIL;
  }
  __android_log_print(ANDROID_LOG_INFO,"TEXTDRAW","FT_Load_Char load %s ok. family:%s,style:%s ",filename,
    ctx->face->family_name,ctx->face->style_name);
  /* error handling omitted */

  /* cmap selection omitted;                                        */
  /* for simplicity we assume that the font contains a Unicode cmap */
       /* transformation matrix */
  ctx->matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
  ctx->matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
  ctx->matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
  ctx->matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );

    env->SetLongField(obj,fid,(jlong)ctx);
    env->ReleaseStringUTFChars(fontPath,filename);
    return;
FAIL:
    // ctx->
    delete ctx;
    env->ReleaseStringUTFChars(fontPath,filename);
    env->ThrowNew(Exception, msg); // Error Message
  }
  

/*
 * Class:     com_tsinglink_android_library_freetype_TextDraw
 * Method:    release
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_tsinglink_android_library_freetype_TextDraw_release
  (JNIEnv *env, jobject obj){
    jclass jclz = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(jclz,"ctx","J");
    jlong v = env->GetLongField(obj,fid);
    if (v == 0){
      return;
    }
    Context *ctx = (Context *)v;

    FT_Done_Face    ( ctx->face );
    FT_Done_FreeType( ctx->library );
    delete ctx;
    env->SetLongField(obj,fid,0);
  }
  

void utf8_to_utf32(const UTF8 *input, UTF32 *output, int utf_size, size_t count)
{
  ConvertUTF8toUTF32(&input,input + utf_size,&output, output + count,strictConversion);
}
/*
 * Class:     com_tsinglink_android_library_freetype_TextDraw
 * Method:    draw
 * Signature: (Ljava/lang/String;II)V
 */
JNIEXPORT void JNICALL Java_com_tsinglink_android_library_freetype_TextDraw_draw
  (JNIEnv *env, jobject obj, jstring text, jint x, jint y, jbyteArray jframe, jint width, jint height){
     jclass jclz = env->GetObjectClass(obj);
     jfieldID fid = env->GetFieldID(jclz,"ctx","J");
    jlong v = env->GetLongField(obj,fid);
    if (v == 0){
      return;
    }
    jclass Exception = env->FindClass("java/lang/Exception");
    char msg[256];
    const int num_chars = env->GetStringLength(text);
    const int utf_nb = env->GetStringUTFLength(text);
    const char *txt = env->GetStringUTFChars(text,0);
    UTF32 *utf32 = new UTF32[num_chars];
    // __android_log_print(ANDROID_LOG_INFO,"TEXTDRAW","utf8_to_utf32 utf_nb=%d, num_chars=%d",utf_nb, num_chars);
    utf8_to_utf32((const UTF8*)txt,utf32,utf_nb, num_chars);
    // __android_log_print(ANDROID_LOG_INFO,"TEXTDRAW","GetStringLength of %s result %d",(char *)txt,num_chars);
    const FT_ULong* charCodes = (FT_ULong* )utf32;
    jbyte *frame = env->GetByteArrayElements(jframe,0);
    // const int num_chars = strlen(txt);
    FT_Vector     pen;                    /* untransformed origin  */
    FT_Error      error;
    Context *ctx = (Context *)v;
    FT_Face face = ctx->face;    
    FT_GlyphSlot slot = face->glyph;


  FT_Matrix     &matrix = ctx->matrix;              

    /* the pen position in 26.6 cartesian space coordinates; */
  /* start at (300,200) relative to the upper left corner  */
  // pen.x = x * 64;
  // pen.y = ( height - y ) * 64;
  pen.x = x * 64;
  pen.y = ( height - y ) * 64 ;
  int n;
  for ( n = 0; n < num_chars; n++ )
  {
    /* set transformation */
    
    FT_Set_Transform( face, &matrix, &pen );
    /* load glyph image into the slot (erase previous one) */
    FT_ULong c = (FT_ULong)charCodes[n];
    // c = ntohs(c);
    // __android_log_print(ANDROID_LOG_INFO,"TEXTDRAW","FT_Load_Char of %u ",c);

    error = FT_Load_Char( face, c, FT_LOAD_RENDER );
    if ( error ){
      sprintf(msg,"FT_Load_Char %ulld error...", c);
      env->ThrowNew(Exception, msg);
    }
      

    /* now, draw to our target surface (convert position) */
    draw_bitmap( &slot->bitmap,
                 slot->bitmap_left,
                 height - slot->bitmap_top,(char *)frame,width,height);

    /* increment pen position */
    pen.x += slot->advance.x;
    pen.y += slot->advance.y;
  }

  // show_image();
  delete []utf32;
  env->ReleaseByteArrayElements(jframe, frame, 0);
  env->ReleaseStringUTFChars(text, txt);
  }
