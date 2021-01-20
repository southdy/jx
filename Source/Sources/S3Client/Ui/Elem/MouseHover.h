/*****************************************************************************************
//	������ʾ����
//	Copyright : Kingsoft 2002-2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-12-23
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

#include "WndMessage.h"
#include "../../../Represent/iRepresent/KRepresentUnit.h"
#include "../../../core/src/GameDataDef.h"

class KMouseOver
{
public:
	KMouseOver();
	int		IsMoseHoverWndObj(void* pWnd, int nObj);
	void    SetMouseHoverInfo(void* pWnd, int nObj, int x, int y,
		                        bool bHeadTailImg, bool bFollowCursor);
    void    SetMouseHoverTitle(const char *pcszTitleText, int nTitleTextLen, UINT uColor);
	void    SetMouseHoverProp(const char *pcszPropText, int nPropTextLen, UINT uColor);
	void    SetMouseHoverDesc(const char *pcszDescText, int nDescTextLen, UINT uColor);
	void	CancelMouseHoverInfo();
	void	PaintMouseHoverInfo();
	void	LoadScheme(const char* pScheme);		//������淽��
	void	UpdateCursorPos(int nX, int nY);		//����λ�ø�����
	void	OnWndClosed(void* pWnd);
private:
	void	Update(int nX, int nY);
private:
	int		m_nImgWidth;	        //ͼ��ˮƽ����
	int		m_nImgHeight;	        //ͼ�δ�ֱ�߶�
	int		m_nLeft;		        //�������ϽǺ�����
	int		m_nTop;			        //�������Ͻ�������
	int		m_nWndWidth;	        //��̬�Ĵ���ˮƽ��
	int		m_nWndHeight;	        //��̬�Ĵ��ڴ�ֱ��

	int		m_nIndent;		        //������������߿��ˮƽ������룩
	int		m_nFontSize;	        //��������(��С)

	int     m_nApplyX;              //�ⲿ�ύ�ĸ������ڵ�X����
	int     m_nApplyY;              //�ⲿ�ύ�ĸ������ڵ�Y����
	int     m_nTitleLineNum;        //����(����)���ֵ�����
	int     m_nPropLineNum;         //���Բ��ֵ�����
	int     m_nDescLineNum;         //�������ֵ�����
	int     m_nMaxLineLen;          //������һ��������еĳ���

	unsigned int m_uTitleBgColor;   //����(����)������ɫ
	unsigned int m_uTitleColor;     //����(����)������ɫ
	unsigned int m_uPropBgColor;    //���Ա�����ɫ
	unsigned int m_uPropColor;      //����������ɫ
	unsigned int m_uDescBgColor;    //�������ֱ�����ɫ
	unsigned int m_uDescColor;      //��������������ɫ
	KRUImagePart m_Image;	        //�߿�ͼ��

	void*	m_pMouseHoverWnd;		//���ͣ���Ĵ���
	int		m_nObj;
	char    m_ObjTitle[GOD_MAX_OBJ_TITLE_LEN];
	char    m_ObjProp[GOD_MAX_OBJ_PROP_LEN];
	char	m_ObjDesc[GOD_MAX_OBJ_DESC_LEN];
	int		m_nTitleLen;
	int		m_nPropLen;
	int		m_nDescLen;
	bool	m_bHeadTailImg;			//�Ƿ���ʾ��ͷ���β�߿�ͼ
	bool	m_bFollowCursor;		//�Ƿ�������ָ���λ�ã�һ���ƶ�����ͬʱû�к��������ƣ�
	BOOL	m_bShow;
};

extern KMouseOver	g_MouseOver;

void SetHoverObjDescColor(unsigned int uColor);
void SetMouseHoverObjectDesc(void* pWnd, int nObj, unsigned int uGenre,
			unsigned int uId, int nContainer, int x, int y);
//���Ʊ��϶���Ϸ����ĺ���
int DrawDraggingGameObjFunc(int x, int y, const KUiDraggedObject& Obj, int nDropQueryResult);
