/*****************************************************************************************
//	���洰����ϵ�ṹ--������Ϸ����Ĵ���
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-9-25
*****************************************************************************************/
#pragma once

#include "Windows.h"
#include "WndWindow.h"
#include "../Elem/WndMessage.h"

#define	OBJCONT_S_ENABLE_CLICK_EMPTY	0x00008000
#define	OBJCONT_S_ACCEPT_FREE			0x00004000
#define	OBJCONT_S_HAVEOBJBGCOLOR		0x00002000	//������Ʒʱ�Ƿ��б���ɫ
#define	OBJCONT_S_TRACE_PUT_POS			0x00001000
#define	OBJCONT_S_DISABLE_PICKPUT		0x00000800	//������������
#define	OBJCONT_F_MOUSE_HOVER			0x00000400

//============================
//	�����������������
//============================
class KWndObjectBox : public KWndWindow
{
public:
	KWndObjectBox();
	virtual int		Init(KIniFile* pIniFile, const char* pSection);	//��ʼ��
	void	LoadScheme(const char* pScheme);			///������淽��
	void	Celar();									//���������Ʒ
	void	SetObjectGenre(unsigned int uGenre);		//���ÿ������ɵĶ��������
	int		GetObject(KUiDraggedObject& Obj) const;		//��ȡ���ɵĶ�����Ϣ
	void	HoldObject(unsigned int uGenre, unsigned int uId, int nDataW, int nDataH);//�������ɵĶ���
	void	Clone(KWndObjectBox* pCopy);
	void	SetContainerId(int nId);
	void	EnablePickPut(bool bEnable);
protected:
	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
	int		DropObject(bool bTestOnly);				//������Ʒ
	void	PaintWindow();							//�������
	unsigned int		m_uAcceptableGenre;			//�ɽ��ɵĶ�������
	KUiDraggedObject	m_Object;
	int					m_nContainerId;
};

//============================
//	���ɶ���������������
//============================
class KWndObjectMatrix : public KWndWindow
{
public:
	KWndObjectMatrix();
	virtual ~KWndObjectMatrix();
	virtual int		Init(KIniFile* pIniFile, const char* pSection);	//��ʼ��
	void			Clear();									//���ȫ���Ķ�����Ʒ
	int				AddObject(KUiDraggedObject* pObject, int nCount);	//���Ӷ�����Ʒ
	int				RemoveObject(KUiDraggedObject* pOjbect);			//����һ��������Ʒ
	int				GetObject(KUiDraggedObject& Obj, int x, int y) const;//��ȡ���ɵ�ĳ��������Ϣ
//	int				GetObjects(KUiDraggedObject* pObjects, int nCount) const;//��ȡ���ɵĶ�����Ϣ
	void			EnableTracePutPos(bool bEnable);
	void			SetContainerId(int nId);
	void			EnablePickPut(bool bEnable);
protected:
	void			Clone(KWndObjectMatrix* pCopy);
private:
	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);	//���ں���
	void	PaintWindow();										//�������
	int		GetObjectAt(int x, int y);							//���ĳ��λ���ϵ���Ʒ������
	int		PickUpObjectAt(int x, int y);						//����ĳ��λ���ϵĶ���
	int		DropObject(int x, int y, bool bTestOnly);			//������Ʒ
	int		TryDropObjAtPos(const RECT& dor, KUiDraggedObject*& pOverlaped);//���Է�����Ʒ
	void	DropObject(int x, int y, KUiDraggedObject* pToPickUpObj);		//������Ʒ

protected:
	int				m_nNumUnitHori;		//�������
	int				m_nNUmUnitVert;		//�������
	int				m_nUnitWidth;		//������
	int				m_nUnitHeight;		//������
	int				m_nUnitBorder;		//���ӵı߿�Ŀ�߶�
	int				m_nNumObjects;		//���ɵĶ������Ŀ
	KUiDraggedObject* m_pObjects;		//���ɵĶ����б�
	int				m_nMouseOverObj;

	int				m_nPutPosX;
	int				m_nPutPosY;
	int				m_nPutWidth;
	int				m_nPutHeight;
	int				m_nContainerId;
};

void WndObjContainerInit(KIniFile* pIni);