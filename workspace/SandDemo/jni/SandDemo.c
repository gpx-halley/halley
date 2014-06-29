#include <jni.h>
#include <string.h>

jstring Java_com_halley_sanddemo_MainActivity_stringFromJNI(JNIEnv* env, jobject thiz)
{
#if defined(__arm__)
  #if defined(__ARM_ARCH_7A__)
    #if defined(__ARM_NEON__)
      #define ABI "armeabi-v7a/NEON"
    #else
      #define ABI "armeabi-v7a"
    #endif
  #else
   #define ABI "armeabi"
  #endif
#elif defined(__i386__)
   #define ABI "x86"
#elif defined(__mips__)
   #define ABI "mips"
#else
   #define ABI "unknown"
#endif
//	return env->NewStringUTF("Hello from JNI !  Compiled with ABI " ABI ".");
    return (*env)->NewStringUTF(env, "中文测试\nHello from JNI !\nCompiled with ABI " ABI ".");
}
