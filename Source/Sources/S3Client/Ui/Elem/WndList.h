/*****************************************************************************************
//	���洰����ϵ�ṹ--�б���
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-8
------------------------------------------------------------------------------------------
    �б���ֻ�г����֡�����ÿ����Թ����������ݡ�
	�б������õ�����Ϊһ�����飬�����ÿһ��Ԫ�ض�Ӧһ���б��ÿ������ԭ�صĴ洢�ռ��
С��ͬ��ÿ������Ԫ�صĽṹΪһ���������ַ�������Ϊ��һ�������ַ�����Ա�Ľṹ���ַ�������
���ڽṹ�е�����λ�á��б������ṩ���ʺ������Ի�ȡ��Щ����Ԫ�ؽṹ�е������γ�Ա��ֵ��
    �б��������г����б�����ĿС������Ԫ����Ŀʱ�����������б��ڶ������ĸ�����Ԫ�ؿ�ʼ
��ʾ��
*****************************************************************************************/
#pragma once
#include "WndWindow.h"

#define	WNDLIST_ES_HALIGN_CENTRE		0x0001		//ˮƽ����
#define	WNDLIST_ES_HALIGN_RIGHT			0x0002		//ˮƽ���Ҷ���
#define	WNDLIST_ES_HALIGN_FILTER		(WNDLIST_ES_HALIGN_RIGHT | WNDLIST_ES_HALIGN_CENTRE)
#define	WNDLIST_ES_HIGHLIGHT_ENABLE		0x0004		//�Բ�ͬ��ɫ��ʾ���ָ��ָ�����
#define WNDLIST_ES_MULTI_SEL			0x0008		//����ͬʱѡ�ж���

#define	NO_CUSTOM_COLOR					0xffffffff

class KWndScrollBar;

class KWndList : public KWndWindow
{
public:
	virtual int		Init(KIniFile* pIniFile, const char* pSection);//��ʼ��
	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
	virtual void	PaintWindow();							//���ƴ���
	//�����б���ʾ������
	void			SetContent(BYTE* pContent, int nNumItem, int nItemSize, int nStringOffset);
	void			GetString(int nItemIndex, char* pBuffer);//��ȡ�б�����ַ�������
	int				GetStrignLen(int nItemIndex);			//����б�����ַ������ݳ��ȡ�
	//���б��в���ĳ���ַ�����
	int				FindString(int nPrecedingStart, const char* pString);
	//����б���������ݽṹ��ĳ�������ͳ�Ա��ֵ
	int				GetItemDataInt(int nItemIndex, int nDataOffset);
	void			SetTopItemIndex(int nTopItemIndex);		//�����б��ڶ�����ʾ�������������
	int				GetTopItemIndex() { return m_nTopItemIndex; }//����б��ڶ�����ʾ�������������
	int				GetVisibleItemCount() const;			//����б�����ͬʱ��ʾ�������Ŀ
	int				GetCount()  { return m_nNumItem; }		//��ȡ�����������Ŀ
	int				GetCurSel() { return m_nSelItemIndex;  }//�õ���ǰѡ��������������
	int				SetCurSel(int nSel);
	int				IsItemSel(int nIndex);					//�ж�ĳ���Ƿ�ѡ��
	void			SetCustomColorOffset(int nOffset);
	void			EnableMultiSel(int bEnable);			//����/���ö�ѡ
	void			SetScrollbar(KWndScrollBar* pScroll);
	KWndList();
	~KWndList();
protected:
	void			Clone(KWndList* pCopy);
private:
	void			OnLButtonDown(int x, int y);			//��Ӧ����������
	void			OnRButtonDown(int x, int y);			//��Ӧ����Ҽ�����
	void			OnLButtonDClick(int x, int y);			//��Ӧ������double click
	void			OnMouseMove(int x, int y);				//��Ӧ������б������ƶ�
	int				GetIemIndexAtPoint(int x, int y);		//��õ�ָ��λ�õ�ѡ�������
	void			UpdateData();
private:
	KWndScrollBar*	m_pScrollbar;			//������

	BYTE*			m_pContent;				//�б�����������
	int				m_nNumItem;				//���������Ŀ
	int				m_nItemSize;			//������ṹ�Ĵ洢�ռ���
	int				m_nStringOffset;		//Ҫ��ʾ���ַ�����������ṹ�е�ƫ��λ��
	int				m_nColorOffset;			//Ҫ��ʾ���ַ�������ɫ��������ṹ�е�ƫ��λ�ã�
					//ֻ����WNDLIST_ES_CUSTOM_COLOR���ʱ������Ч��

	int				m_nTopItemIndex;		//���ڶ�����ʾ�������������
	int				m_nSelItemIndex;		//ѡ�е������������
	int				m_nHighLightItemIndex;	//��������ʾ�����ָ���·����������������

	unsigned int	m_ItemColor;			//��ʾ�������ı�����ɫ
	unsigned int	m_ItemBorderColor;		//��ʾ�������ı��ı�Ե��ɫ
	unsigned int	m_SelItemColor;			//��ѡ�е��������ı�����ɫ
	unsigned int	m_SelItemBorderColor;	//��ѡ�е��������ı��ı�Ե��ɫ
	unsigned int	m_HighLightColor;		//��������ʾ���������ı�����ɫ
	unsigned int	m_HighLightBorderColor;	//��������ʾ���������ı��ı�Ե��ɫ
	unsigned int	m_uSelItemBgColor;		//ѡ����ı���ɫ�����ֵΪ0xffffffff��ʾ�ޱ���ɫ
	int				m_nFontSize;			//��ʾ�����С
	
	int				m_nNumColumn;			//����ͬʱ��ʾ��������
	unsigned int	m_uIsItemSelStatus;		//��¼ÿ�����Ƿ�ѡ��
	unsigned int*	m_puItemSelStatusList;	//��¼���32�����ϵ����Ƿ�ѡ��
	unsigned int	m_uNumSelStatusList;	//m_upItemSelStatusList������unsigned int�ռ����Ŀ
};