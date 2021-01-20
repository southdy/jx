/*****************************************************************************
					������ʾϵͳ֮����ӿ�

Ӧ�ã�
	����ϵͳ����ӿڣ�iFont��
	�ⲿ�ͻ����ô�ϵͳҪ����ͷ�ļ�iFont.h��

�ӿڹ��ܣ�
	������ʾ�ַ�����

ʹ�ã�
	1 �ӿ�ʹ�õ��ֿ��ļ���Դ��
		���ʽ�淶���ɽ��ɽ�ӽ���������ֿ��ļ��ĸ�ʽ�淶��ͬ��
		�����ʽ�淶�ɲο�KFontRes.h�ļ���
	2 �ӿ�ʹ��ǰ�ĳ�ʼ����
		ʹ�ýӿ�ǰ���ȵ�����ӿڷ���Init�������ʹ��Direct3D Device�����ʼ��ʱ
		����Direct3D Device�ӿڵ�ʵ����ָ�롣����ʱ���������Direct3D Device��
		��ʼ��֮�������ʹ��DirectGraph����...
		��ʼ����ʹ�Ѿ������ֿ����ݶ�ʧ��
    3 �����ֿ�
		�����ֿ���Ҫ�ڳɹ��ĳ�ʼ������֮�󣬷��������ֿ�ʧ�ܡ����Ѿ������ֿ�
		֮�����ֱ���ظ����ô˽ӿں������Ը����ֿ⣬����Ҫ�ڵ��ó�ʼ��������
	4 ʹ�ú�ӿڵĹرգ�
		������ʹ�ô˽ӿ�ʵ��ʱ������ӿڷ���Terminate�ر�����
		�ر�֮�������±���ʼ�������ֿ��Ա�ʹ�á�

	5 ��������ƣ��ַ���������ʱ�����ַ������俪ʼ���Ƶ�λ�ã�������ָ���ַ���
	  ����ɫ�����û��ָ����ɫ����Ĭ��Ϊ��ɫ�����ṩ��һ���ر��ܣ����ַ�����
	  ���������ǰһ��������ַ����ĺ��棬����Ҫ�˹��ܰ��ַ�����ʼ����λ�õ�
	  X��Y������ΪKFL_FOLLOW�����ˡ�
	  TextOut�����Ĳ���nLineWidth(�����)�������ֵΪ���ڵ���һ��ȫ���ַ����
	  �Ŀ�ȣ�����ʾ�ַ���ʱ���������п��Զ����С�
	6 ��ģ�������˵Ļ�ͼ�豸Ҫ��lost state����ָ�Ϊoperational state֮ǰ��Ҫ��
	  ��iKFont�Ľӿں���InvalidateDeviceObjectsʹDevice״̬�л�����Ӱ��Ļ�ͼ��
	  ���������ʧЧ������֮��Ҫ����iKFont�ӿڵ�RestoreDeviceObjects������һЩ
	  �豸������ݵĻָ�������

/*******************************************************************************
// FileName			:	KFont.h
// FileAuthor		:	Wooy
// FileCreateDate	:	2002-9-5
// FileDescription	:	����ӿ�
// Revision Count	:	
*******************************************************************************/

#ifndef __IFONT_H__
#define __IFONT_H__

#include "FontDef.h"

struct iFont
{	
	//��ʼ��
	virtual bool	Init(void* pDrawDevice) = 0;
	//�����ֿ�
	virtual bool	Load(const char* pszFontFile) = 0;
	//�������������
	virtual void	Terminate() =0;
	//���û���ʱ�ַ���Ե����ɫ����alphaΪ0��ʾ�ַ���Ե����������
	virtual void	SetBorderColor(unsigned int uColor) = 0;
	//��ȡ�����С��Ϣ
	virtual void	GetFontSize(int* pWidth, int* pHeight) = 0;
	//�����ַ����ƴ�С
	virtual void	SetOutputSize(int nOutputWith, int nOutputHeight) = 0;
	//��ʾ�ַ���
	virtual void	OutputText(const char* pszText, int nCount = KRF_ZERO_END,
					int nX = KRF_FOLLOW, int nY = KRF_FOLLOW,
					unsigned int nColor = 0xff000000, int nLineWidth = 0) = 0;
	//��¡һ���ӿڶ���ָ��
	virtual iFont* Clone() = 0;
	//�ͷŽӿڶ���
	virtual void Release() = 0;
};

#endif