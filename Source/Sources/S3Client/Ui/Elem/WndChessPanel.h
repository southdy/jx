/*
 *  File:      WndChessPanel.h
 *  Dest:      �������
 *  Creator:   flying
 *  CopyRight: KingSoft
 *  Date:      2003-8-22
 */
#pragma once

#if !defined _FACESELECTOR
#define _FACESELECTOR

#include "../Elem/WndWindow.h"
#include "../Elem/UiImage.h"

class KWndChessPanel : public KWndWindow
{
public:
	//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static KWndChessPanel* OpenWindow(bool bShow);
	//�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void				CloseWindow(bool bDestroy);
	static void				Clear();
	static KWndChessPanel*	GetIfVisible();
	// Draw the grid
	void					PaintWindow();
	virtual			int	Init(KIniFile* pIniFile, const char* pSection);
	int				PtInWindow(int x, int y);
	int				WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
private:
	KWndChessPanel();
	virtual ~KWndChessPanel();
	static KWndChessPanel* m_pSelf;	
	
	void Initialize();
	
	int					m_nIndex;
	int					m_nLeft;
	int					m_nBottom;
	int					m_nRowCount;
	int					m_nCount;
	int					m_nBtnWidth;
	int					m_nBtnHeight;
	char				m_szImageName[128];
	unsigned int		m_nBgColor;
	unsigned int		m_nBgOver;
	unsigned int		m_nBgAlpha;
	unsigned int		m_nBorderColor;
	char				m_szFaceIniFile[32];
	bool				m_bMouse;
	int					m_nMouseX1;
	int					m_nMouseY1;
	int					m_nMouseX2;
	int					m_nMouseY2;
	int					m_nCurIndex;
	int					m_nCurX, m_nCurY;
};

#endif