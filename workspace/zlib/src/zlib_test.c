//============================================================================
// Name        : zlib_test.c
// Author      : halley
// Version     :
// Copyright   : Elysion
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "zlib.h"

// ѹ�����ݸ�ʽ��
// Դ���ݳ���(4)+ѹ�����ݳ���(4)+ѹ������

typedef struct zlib_type_s {
	uLong uncompressLen;
	unsigned char *uncompressData;
	uLong compressLen;
	unsigned char *compressData;
} zlib_type;

uLong uncompressBound(uLong sourceLen) {
	return sourceLen * 2;
}

int compressFile(char *inFile, char *outFile) {
	FILE* file;
	uLong flen;
	unsigned char* fbuf = NULL;
	uLong clen;
	unsigned char* cbuf = NULL;

	if ((file = fopen(inFile, "rb")) == NULL) {
		printf("Can\'t open %s!\n", inFile);
		return -1;
	}
	/* װ��Դ�ļ����ݵ������� */
	fseek(file, 0L, SEEK_END); /* �����ļ�ĩβ */
	flen = ftell(file); /* ��ȡ�ļ����� */
	fseek(file, 0L, SEEK_SET);
	if ((fbuf = (unsigned char*) malloc(sizeof(unsigned char) * flen)) == NULL) {
		printf("No enough memory!\n");
		fclose(file);
		return -1;
	}
	fread(fbuf, sizeof(unsigned char), flen, file);
	fclose(file);

	/* ��ȡѹ����������С */
	clen = compressBound(flen);
	if ((cbuf = (unsigned char*) malloc(sizeof(unsigned char) * clen)) == NULL) {
		printf("No enough memory!\n");
		return -1;
	}

	/* ѹ������ */
	if (compress(cbuf, &clen, fbuf, flen) != Z_OK) {
		printf("Compress %s failed!\n", inFile);
		return -1;
	}

	/* ����ѹ��������ݵ�Ŀ���ļ� */
	if ((file = fopen(outFile, "wb")) == NULL) {
		printf("Can\'t create %s!\n", outFile);
		return -1;
	}
	fwrite(&flen, sizeof(uLong), 1, file); /* д��Դ�ļ����� */
	fwrite(&clen, sizeof(uLong), 1, file); /* д��Ŀ�����ݳ��� */
	fwrite(cbuf, sizeof(unsigned char), clen, file);
	fclose(file);

	free(fbuf);
	free(cbuf);

	return 0;
}

int uncompressFile(char *inFile, char *outFile) {
	FILE* file;
	uLong flen;
	unsigned char* fbuf = NULL;
	uLong clen;
	unsigned char* cbuf = NULL;

	if ((file = fopen(inFile, "rb")) == NULL) {
		printf("Can\'t open %s!\n", inFile);
		return -1;
	}

	fread(&clen, sizeof(uLong), 1, file);
	if ((cbuf = (unsigned char*) malloc(sizeof(unsigned char) * clen)) == NULL) {
		printf("No enough memory!\n");
		fclose(file);
		return -1;
	}

	fread(&flen, sizeof(uLong), 1, file);

	if ((fbuf = (unsigned char*) malloc(sizeof(unsigned char) * flen)) == NULL) {
		printf("No enough memory!\n");
		fclose(file);
		return -1;
	}
	fread(fbuf, sizeof(unsigned char), flen, file);
	fclose(file);

	/* ��ѹ������ */
	if (uncompress(cbuf, &clen, fbuf, flen) != Z_OK) {
		printf("Uncompress %s failed!\n", inFile);
		return -1;
	}

	/* ����ѹ��������ݵ�Ŀ���ļ� */
	if ((file = fopen(outFile, "wb")) == NULL) {
		printf("Can\'t create %s!\n", outFile);
		return -1;
	}
	fwrite(cbuf, sizeof(unsigned char), clen, file);
	fclose(file);

	free(fbuf);
	free(cbuf);

	return 0;
}

int compressData(zlib_type *zlib_data) {
	uLong flen;
	unsigned char* fbuf = NULL;
	uLong bufLen;
	unsigned char* cbuf = NULL;

	fbuf = zlib_data->uncompressData;
	flen = zlib_data->uncompressLen;

	/* ��ȡѹ����������С */
	bufLen = compressBound(zlib_data->uncompressLen);
	if ((cbuf = (unsigned char*) malloc(sizeof(unsigned char) * bufLen)) == NULL) {
		printf("No enough memory!\n");
		return -1;
	}

	/* ѹ������ */
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
		printf("No enough memory!\n");
		return -1;
	}

	/* ��ѹ������ */
	if (uncompress(cbuf, &clen, fbuf, flen) != Z_OK) {
		printf("Uncompress data failed!\n");
		return -1;
	}

	zlib_data->uncompressData = cbuf;
	zlib_data->uncompressLen = clen;

	return 0;
}

int main() {
	int iRet;
	char *sourceFileName = ".\\res\\source.data";
	char *compressFileName = ".\\res\\compress.data";
	char *uncompressFileName = ".\\res\\uncompress.data";
	zlib_type zlib_data;

	printf("zlib_version = %s\n", zlib_version);

	zlib_data.uncompressData =
			(unsigned char*) "ZLIBѹ�����ԣ�compressDataѹ��uncompressData��ѹ������ʾ���ַ���˵����OK��~";
	zlib_data.uncompressLen = strlen((char*) zlib_data.uncompressData);
	iRet = compressData(&zlib_data);
	printf("compressData return %d\n", iRet);
	if (iRet != 0)
		return iRet;

	iRet = uncompressData(&zlib_data);
	printf("uncompressData return %d\n", iRet);
	if (iRet != 0)
		return iRet;

	iRet = compressFile(sourceFileName, compressFileName);
	printf("compressFile return %d\n", iRet);
	if (iRet != 0)
		return iRet;

	iRet = uncompressFile(compressFileName, uncompressFileName);
	printf("uncompressFile return %d\n", iRet);
	if (iRet != 0)
		return iRet;

	return 0;
}
