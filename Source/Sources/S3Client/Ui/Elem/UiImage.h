/*****************************************************************************************
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-25
------------------------------------------------------------------------------------------
	�ṩ��ͼ����Դ���õ�ά��������
*****************************************************************************************/
#pragma once
#include "../../../Represent/iRepresent/KRepresentUnit.h"

//======================================
//		ͼ�ζ�������ýṹ
//======================================
struct KUiImageRef : public KRUImage
{
	int			nNumFrames;		//ͼ�ζ����ͼ��֡��Ŀ
	int			nInterval;		//֡���
	int			nFlipTime;		//��ǰ֡�л�������ʱ��
};

#define	IDF_LEFT_TO_RIGHT	0
#define IDF_RIGHT_TO_LEFT	1
#define IDF_TOP_TO_BOTTOM	2
#define IDF_BOTTOM_TO_TOP	3

//======================================
//		ͼ�ζ�������ýṹ
//======================================
struct KUiImagePartRef : public KRUImagePart
{
	int			Width;			//ͼ���
	int			Height;			//ͼ�ݿ�
	int			nDivideFashion;	//�ָʽ,ȡֵΪIDF_*֮һ
};

void	IR_InitUiImageRef(KUiImageRef& Img);
void	IR_InitUiImagePartRef(KUiImagePartRef& Img);
void	IR_UpdateTime();										//����ͼ�λ�֡������ʱ��
int		IR_NextFrame(KUiImageRef& Img);							//��֡����
void	IR_GetReferenceSpot(KUiImageRef& Img, int& h, int& v);	//���ͼ��ο��㣨һ����˵Ϊ���ģ�
void	IR_UpdateImagePart(KUiImagePartRef& Img, int nPartValue, int nFullValue);	//���û���ͼ�ľֲ�
int		IR_IsTimePassed(unsigned int uInterval, unsigned int& uLastTimer);			//�ж�ʱ���Ƿ��Ѿ�����
unsigned int IR_GetRemainTime(unsigned int uInterval, unsigned int uLastTimer);		//��ȡʣ��ʱ�䣬���ʱ���Ѿ�����/���ˣ�����ֵ��Ϊ0
unsigned int IR_GetCurrentTime();													//��ȡ��ǰ��ʱ��
