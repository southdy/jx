//*************************************************************************************
// ����ģ��Ĺ����������ͺͺ���
//*************************************************************************************

#ifndef __REPRESENTUTILITY_H__
#define __REPRESENTUTILITY_H__

#include <windows.h>

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }

// ��ʼ��Gdi+
bool InitGdiplus();
// �ر�Gdi+
void ShutdownGdiplus();

// ��argb����������Ϊjpg�ļ�
bool SaveBufferToJpgFile32(LPSTR lpFileName, PVOID pBitmap, int nPitch, 
										   int nWidth, int nHeight, unsigned int nQuality);

// ��rgb����������Ϊjpg�ļ�
bool SaveBufferToJpgFile24(LPSTR lpFileName, PVOID pBitmap, int nPitch, 
										   int nWidth, int nHeight, unsigned int nQuality);

#endif