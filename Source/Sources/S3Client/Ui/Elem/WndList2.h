/*****************************************************************************************
//	���洰����ϵ�ṹ--�б���
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-9-11
------------------------------------------------------------------------------------------
    �б���ֻ�г����֡�����ÿ����Թ����������ݡ�
*****************************************************************************************/
#pragma once
#include "WndWindow.h"
#include "WndScrollBar.h"

#define	WNDLIST_ES_HIGHLIGHT_ENABLE		0x0001			//�Բ�ͬ��ɫ��ʾ���ָ��ָ�����
#define	WNDLIST_ERROR					-1

class KWndList2 : public KWndWindow
{
public:
	virtual int	Init(KIniFile* pIniFile, const char* pSection);//��ʼ��
	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
	virtual void	PaintWindow();							//���ƴ���
	int				AddString(int nIndex, const char* pString);//�����
	int				GetString(int nIndex, char* pBuffer, unsigned int nSize);//��ȡ�б�����ַ�������
	int				DeleteString(int nIndex);				//ɾ���б��е�һ��
	void			ResetContent();							//ɾ���б���ȫ��������
	//���б��в���ĳ���ַ�����
	int				FindString(int nPrecedingStart, const char* pString);
	int				SetItemData(int nIndex, int nData);		//�����б���������ݵ�ֵ
	int				GetItemData(int nIndex);				//����б���������ݵ�ֵ
	void			SetTopItemIndex(int nTopItemIndex);		//����б��ڶ�����ʾ�������������
	int				GetVisibleLineCount() const;			//����б�����ͬʱ��ʾ�������Ŀ
	int				GetCount()  { return m_nNumItem; }		//��ȡ�����������Ŀ
	int				GetCurSel() { return m_nSelItemIndex;  }//�õ���ǰѡ��������������
	int				SetCurSel(int nSel);
	void			SetScrollbar(KWndScrollBar* pScroll);
	KWndList2();
	virtual ~KWndList2();
protected:
	void			Clone(KWndList2* pCopy);
private:
	void			OnLButtonDown(int x, int y);			//��Ӧ����������
	void			OnMouseMove(int x, int y);				//��Ӧ������б������ƶ�
	
private:
	void**			m_pContent;				//�б�����������
	int				m_nItemSpace;			//���Ա����item����Ŀ
	int				m_nNumItem;				//���������Ŀ

	int				m_nTopItemIndex;		//���ڶ�����ʾ�������������
	int				m_nSelItemIndex;		//ѡ�е������������
	int				m_nHighLightItemIndex;	//��������ʾ�����ָ���·����������������

	unsigned int	m_ItemColor;			//��ʾ�������ı�����ɫ
	unsigned int	m_ItemBorderColor;		//��ʾ�������ı��ı�Ե��ɫ
	unsigned int	m_SelItemColor;			//��ѡ�е��������ı�����ɫ
	unsigned int	m_SelItemBorderColor;	//��ѡ�е��������ı��ı�Ե��ɫ
	unsigned int	m_HighLightColor;		//��������ʾ���������ı�����ɫ
	unsigned int	m_HighLightBorderColor;	//��������ʾ���������ı��ı�Ե��ɫ
	int				m_nFontSize;			//��ʾ�����С

	KWndScrollBar  *m_pScrollbar;
};