/*******************************************************************************
File        : UiTongManager.h
Creator     : Fyt(Fan Zhanpeng)
create data : 08-29-2003(mm-dd-yyyy)
Description : ������Ľ���
********************************************************************************/

#if !defined(AFX_UITONGMANAGER_H__13BA213D_11EC_4F24_BF98_F51C3F414D6D__INCLUDED_)
#define AFX_UITONGMANAGER_H__13BA213D_11EC_4F24_BF98_F51C3F414D6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../elem/wndlabeledbutton.h"
#include "../elem/wndscrollbar.h"
#include "../elem/wndimage.h"
#include "../elem/wndlist2.h"

#include "../../Engine/Src/LinkStruct.h"
#include "../../Core/Src/GameDataDef.h"


struct KPilgarlicItem : public KTongMemberItem
{
	int nHashID;
	KPilgarlicItem *next;
    KPilgarlicItem *previous;
};


class KUiTongManager : KWndImage
{
public:
	KUiTongManager();
	virtual ~KUiTongManager();

	static        KUiTongManager* OpenWindow();      //�򿪴���
	static        KUiTongManager* OpenWindow(        //�򿪴��ڣ�ͬʱ���Ž��沼��
		                                     char* pszPlayerName);
	static        KUiTongManager* GetIfVisible();    //�������������ʾ���򷵻�ʵ��ָ��
	static void   CloseWindow(bool bDestory = TRUE); //�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void   LoadScheme(const char* pScheme);   //������淽��

	                                                 //�����µ�����
	static void   NewDataArrive(KUiGameObjectWithName *ParamInfo, KTongMemberItem *pIncome);

	                                                 //���ܰ����Ϣ
	static void   TongInfoArrive(KUiPlayerRelationWithOther* Relation, KTongInfo *pTongInfo);
	static int    RemoveTempFile();                  //ɾ����ʱ�ļ�

	static void   ResponseResult(                    //�Խ������Ӧ���
		                         KUiGameObjectWithName *pResult, int nbIsSucceed);

	
public:
	virtual int   WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void          ArrangeComposition(                //���ݴ����������֣����Ž��沼��
		                             char* pszPlayerName);


private:
	void          Initialize();

	void          PopupConfirmWindow(                //����ȷ�ϴ���
		                        const char* pszInfo, unsigned int uHandleID);

	static void   RequestData(int nType, int nIndex);//��Core������������
	static void   SaveNewData(                       //���µ����б���Ϣ�浽��ʱ�ļ�
		                        KUiGameObjectWithName *ParamInfo, KTongMemberItem *pIncome);

	static void   SaveWholeData(int nType);          //��ָ�����͵��б����������洢����

	static int    IsPlayerExist(                     //��ѯ�ض����ֵ�����Ƿ��Ѿ�����
		                        KPilgarlicItem *MeToCmp, KLinkStruct<KPilgarlicItem> *pData);

	KLinkStruct<KPilgarlicItem>*                     //��ѯ�ض����ֵ���ҵ�����λ��
		          FindPlayer(char *pszName);

	KLinkStruct<KPilgarlicItem>*                     //������Ҵ�����
		          HandleFind(KLinkStruct<KPilgarlicItem> *pData, char *pszName);

	void          LoadData(int nType);               //��������
	int           LoadDataHandler(                   //�������ϵĴ�����
		                        KLinkStruct<KPilgarlicItem> *pData,
							    const char* szSection);

	void          UpdateBtnTheme(                    //���°�ť���÷���
		                        int nType, BOOL IsDissable = FALSE);

	KLinkStruct<KPilgarlicItem>*                     //���ص�ǰ��ѡ����б���������Ӧ�����ݼ�
		          GetCurrentSelectedData();

	KPilgarlicItem*                                  //������ѡ��Ļ�Ա
		          GetSelectedPlayer();

	void          UpdateTongInfoView();              //���°��ѶϢ����ʾ
	void          UpdateTongListView();              //�����б���

	void          ClearTongData();                   //�Ѱ����Ϣ�����

	void          UpdateListCheckButton(int nType);  //�����б�CheckBox�Ĺ���
	void          OnAction(int nType);               //���ְ������Ĵ�����
	void          OnRefresh();                       //��Ӧˢ�°�ť�Ĳ�������

	void          ResponseLeave(                     //��Ӧ����Ĳ���
		                        KUiGameObjectWithName *pResult, int nbIsSucceed);

	void          ResponseDismiss(                   //��Ӧ���˺�Ĳ���
		                        KUiGameObjectWithName *pResult, int nbIsSucceed);

	void          ResponseDemise(                    //��Ӧ��λ��Ĳ���
		                        KUiGameObjectWithName *pResult, int nbIsSucceed);
	
	void          ResponseAssign(                    //��Ӧ������Ĳ���
		                        KUiGameObjectWithName *pResult, int nbIsSucceed);

	void          ResponseApply(                     //��Ӧ�����Ĳ���
		                        KUiGameObjectWithName *pResult, int nbIsSucceed);


private:
	static        KUiTongManager* ms_pSelf;

	static int    m_nElderDataIndex;
	static int    m_nMemberDataIndex;
	static int    m_nCaptainDataIndex;

private:
	enum RESULT_TYPE
	{
		RESULT_T_DISMISS,
		RESULT_T_ASSIGN,
		RESULT_T_DEMISE,
		RESULT_T_LEAVE,
		RESULT_T_PASSWORD_TO_DEMISE,
	};

private:
	KWndLabeledButton                                //���ˣ���������λ��ť
		          m_BtnDismiss, m_BtnAssign, m_BtnDemise;

	KWndLabeledButton
		          m_BtnLeave, m_BtnRecruit;          //�����˰�ť

	KWndLabeledButton
		          m_BtnApply, m_BtnCancel;           //���룬ȡ����ť

	KWndLabeledButton                                //���ϡ��ӳ�����Ա�б�
		          m_BtnElderList, m_BtnCaptainList, m_BtnMemberList;

	KWndButton    m_BtnClose, m_BtnRefresh;

	KWndText32    m_TongName, m_MasterName;          //������֣��ϴ�����(����)
	KWndText32    m_Alignment;                       //��Ӫ����
	KWndText32    m_MemberCount, m_Money;            //�������ʽ�

	KWndList2     m_List;                            //�м���б�ؼ�
	KWndScrollBar m_ListScroll;                      //�б�Ĺ�����

	TONG_MEMBER_FIGURE
		          m_Relation;                        //��������������Ŀ����Ĺ�ϵ


private:
	KLinkStruct<KPilgarlicItem>
		          m_MemberData, m_CaptainData;       //��Ա�б�Ͷӳ��б������

	KLinkStruct<KPilgarlicItem> m_ElderData;         //�����б�
	static KTongInfo            m_TongData;          //������������


private:
	unsigned int  m_uLastRequestTongDataTime;        //��һ�����������ϵ�ʱ��
	unsigned int  m_uLastRequestElderListTime;       //��һ���������б��ʱ��
	unsigned int  m_uLastRequestMemberListTime;      //��һ����������б��ʱ��
	unsigned int  m_uLastRequestCaptainListTime;     //��һ������ӳ��б��ʱ��

	int           m_nRelation;                       //�����Ǹ���������򿪵İ��Ĺ�ϵ
	int           m_nIndex;                          //�������Ǹ���Ӧ����ҵ�Index
	char          m_szPlayerName[32];                //������򿪣�����һ���󶨵���ң�
	                                                 //����ˣ����Լ����������(��)��
	                                                 //���֡���һ���Ƕ�˵��ÿһ�δ򿪽���
	                                                 //����һ����ᣬ�����󶨵���

	int           m_nSelectIndex;                    //���б���ѡ�������±�
	int           m_nCurrentCheckBox;                //����ѡ���CheckBox

	char          m_szDismiss[64], m_szAssign[64];   //���˺�������ȷ�Ͽ���ַ���
	char          m_szDemise[64], m_szLeave[64];     //��λ���뿪��ȷ�Ͽ���ַ���
	char          m_szJustice[16], m_szBalance[16];  //�����е�������
	char          m_szEvil[16];                      //а��������
	char          m_szConfirm[16], m_szCancel[16];   //ȷ�ϴ����ϣ�ȷ�Ϻ�ȡ��������

	char          m_szPassword[32];                  //MD5����֮������봮
	char          m_szTargetPlayerName[32];          //����������ҵ�����
};

unsigned long StringToHash(const char *pString, BOOL bIsCaseSensitive = TRUE);

#endif // !defined(AFX_UITONGMANAGER_H__13BA213D_11EC_4F24_BF98_F51C3F414D6D__INCLUDED_)
