/* 
 * File:     KUiHelper.h
 * Desc:     �򵥰�������
 * Author:   flying
 * Creation: 2003/7/16
 */
//-----------------------------------------------------------------------------
#pragma once

#if !defined _UIHELPER1
#define _UIHELPER1

#include "../Elem/WndImage.h"

class KUiHelper : protected KWndImage
{
public:
	//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static KUiHelper* OpenWindow(bool bShow);
	//�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void				CloseWindow(bool bDestroy);
	static KUiHelper*		GetIfVisible();
	//������淽��
	void					LoadScheme(const char* pScheme);

	static void SelectInterfacePic(); //ѡ��Ҫ��ʾ���水ť�İ���ͼ
	static void SelectKeyboardPic();  //ѡ��Ҫ��ʾ��ݼ�����ͼ��
	static void SelectWuxingPic();    //ѡ��Ҫ��ʾ���м���ͼ��ͼƬ

private:
	KUiHelper();
	virtual ~KUiHelper();

	static KUiHelper* m_pSelf;
	static int ms_nImgFrame;
	int					WndProc(unsigned int uMsg, unsigned int uParam, int nParam);

	void Initialize();
	void	Breathe() {NextFrame();}
	virtual void	Show();									//��ʾ����
};


#endif