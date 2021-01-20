//////////////////////////////////////////////////////////////////////////////////////
//	�ļ���			:	TextPic.h
//	������			:	������
//	����ʱ��		:	2003-6-22 23:01:40
//	�ļ�˵��		:	����Ƕ��ʽͼƬ
//////////////////////////////////////////////////////////////////////////////////////

#ifndef __TEXTPIC_H__
#define __TEXTPIC_H__

#include "../../Engine/Src/Text.h"
#include "UiImage.h"
#include <vector>
using namespace std;

struct iRepresentShell;
class KInlinePicSink : public IInlinePicEngineSink
{
public:
	KInlinePicSink();
	BOOL Init(iRepresentShell*);
	BOOL UnInit();
	

protected:
	iRepresentShell* m_pRepShell;
	vector<KUiImageRef>	m_Images;
	vector<KImageParam> m_ImageParams;

	vector<KUiImageRef> m_CustomImages;
	vector<KImageParam> m_CustomImageParams;

public:
	//��̬����ͼƬ,��ȡһ��WORD,��ͼƬ������
	long AddCustomInlinePic(unsigned short& wIndex, const char* szSprPathName);
	//��̬ж��ͼƬ
	long RemoveCustomInlinePic(unsigned short wIndex);
	
	long GetPicSize(unsigned short wIndex, int& cx, int& cy);
	long DrawPic(unsigned short wIndex, int x, int y);
};

#endif //__TEXTPIC_H__
