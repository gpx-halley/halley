#include <jni.h>
#include <string.h>
#include <zlib.h>
#include <android/log.h>

#define printf LOGI
#define LOG_TAG "NativeSandDemo"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))

typedef struct zlib_type_s {
	uLong sourceLen;
	unsigned char *sourceData;
	uLong compressLen;
	unsigned char *compressData;
	uLong uncompressLen;
	unsigned char *uncompressData;
} zlib_type;

uLong uncompressBound(uLong sourceLen) {
	return sourceLen * 2;
}

int compressData(zlib_type *zlib_data) {
	uLong flen;
	unsigned char* fbuf = NULL;
	uLong bufLen;
	unsigned char* cbuf = NULL;

	fbuf = zlib_data->sourceData;
	flen = zlib_data->sourceLen;

	/* 获取压缩缓冲区大小 */
	bufLen = compressBound(zlib_data->sourceLen);
	if ((cbuf = (unsigned char*) malloc(sizeof(unsigned char) * bufLen)) == NULL) {
		printf("[%05d] No enough memory!\n", __LINE__);
		return -1;
	}

	/* 压缩数据 */
	if (compress(cbuf, &bufLen, fbuf, flen) != Z_OK) {
		printf("Compress data failed!\n");
		return -1;
	}

	zlib_data->compressData = cbuf;
	zlib_data->compressLen = bufLen;

	return 0;
}

int uncompressData(zlib_type *zlib_data) {
	uLong flen;
	unsigned char* fbuf = NULL;
	uLong clen;
	unsigned char* cbuf = NULL;

	fbuf = zlib_data->compressData;
	flen = zlib_data->compressLen;

	if (zlib_data->uncompressLen > flen)
		clen = zlib_data->uncompressLen;
	else
		clen = uncompressBound(flen);

	if ((cbuf = (unsigned char*) malloc(sizeof(unsigned char) * clen)) == NULL) {
		printf("[%05d] No enough memory!\n", __LINE__);
		return -1;
	}

	/* 解压缩数据 */
	if (uncompress(cbuf, &clen, fbuf, flen) != Z_OK) {
		printf("Uncompress data failed!\n");
		return -1;
	}

	zlib_data->uncompressData = cbuf;
	zlib_data->uncompressLen = clen;

	return 0;
}

extern "C" jstring Java_com_halley_sanddemo_MainActivity_stringFromJNI(
		JNIEnv* env, jobject thiz) {
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

	char buf[1000];
	memset(buf, 0, sizeof(buf));
	strcat(buf, "Hello from JNI !\nCompiled with ABI " ABI ".\n");

	strcat(buf, "zlib_version = ");
	strcat(buf, zlib_version);
	strcat(buf, "\n");

	zlib_type zlib_data;
	zlib_data.sourceData =
			(unsigned char*) "ZLIB压缩测试，compressData压缩uncompressData解压后能显示本字符串说明是OK的~";
	zlib_data.sourceLen = strlen((char*) zlib_data.sourceData);
	strcat(buf, "sourceData = ");
	strcat(buf, (char*)zlib_data.sourceData);
	strcat(buf, "\n");

	compressData(&zlib_data);
	strcat(buf, "compressData = ");
//	strcat(buf, (char*)zlib_data.compressData);
	strcat(buf, "\n");

	uncompressData(&zlib_data);
	strcat(buf, "uncompressData = ");
	strcat(buf, (char*)zlib_data.uncompressData);
	strcat(buf, "\n");

	return env->NewStringUTF(buf);
}
