/*****************************************************************************************
//	�ر��ܵ�һЩ��������
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-7-26
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

#include "../../../Represent/iRepresent/iRepresentShell.h"

struct KOutputTextParam;
//�������tab�ָ���ַ����������Ƕ��У�����ֵ��ʾ������ݹ�����
//nLineWidth �ĵ�λ����Ļ���ص�
//KOutputTextParam::nSkipLine��֧��
int OutputTabSplitText(const char* pBuffer, int nCount, int nLineWidth,
					   int nFontId, KOutputTextParam* pParam);
//eType:�ļ����ͣ�bmp,jpg��
//nQuality:ͼ������(0-100)��ֻ��jpg������
void SaveScreenToFile(ScreenFileType eType = SCRFILETYPE_JPG, unsigned int nQuality = 100);
bool SetScrPicPath(const char* szPath);