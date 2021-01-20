/********************************************************************
File        : UiStrengthRank.h
Creator     : Fyt(Fan Zhanpeng)
create data : 08-11-2003(mm-dd-yyyy)
Description : �������ڵ�ͷ�ļ�
*********************************************************************/

#if !defined(AFX_KUISTRENGTHORDER_H__7801F7C7_BAE8_4A49_BEA8_63B5F8FA0BB1__INCLUDED_)
#define AFX_KUISTRENGTHORDER_H__7801F7C7_BAE8_4A49_BEA8_63B5F8FA0BB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KIniFile.h"
#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndButton.h"
#include "../Elem/WndScrollBar.h"
#include "../Elem/WndList.h"
#include "../Elem/WndMessageListBox.h"
#include "../../../Engine/src/LinkStruct.h"
#include "../../Core/Src/GameDataDef.h"

#define MAX_RANK_MESSAGE_LENGTH 128

struct KRankIndexNode : public KRankIndex
{
	char szMsg[MAX_RANK_MESSAGE_LENGTH];
	int  nMsgLen;
	char szUnit[16];
	KRankIndexNode *next;
	KRankIndexNode *previous;
};

struct KRankNode : public KRankMessage
{
	KRankNode* next;
	KRankNode* previous;
};

class KUiStrengthRank : public KWndShowAnimate
{
public:
	KUiStrengthRank();
	virtual ~KUiStrengthRank();

	static        KUiStrengthRank* OpenWindow();    //�򿪴���
	static        KUiStrengthRank* OpenDefaultWindow(int nId = -1);
	static        KUiStrengthRank* GetIfVisible();  //�������������ʾ���򷵻�ʵ��ָ��
	static void   CloseWindow(bool bDestory = TRUE);//�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void   LoadScheme(const char* pScheme);  //������淽��

	//�µ�������������
	static void   NewRankArrive(unsigned int uParam, KRankMessage *pRankData);
	//�µ�������������
	static void   NewIndexArrive(unsigned int uParam, struct KRankIndex *pIndex);

	static int    AttachTempFile();                            //�����ʱ�ļ��Ƿ���ڣ��������򴴽�
	static void   RemoveTempFile();                            //ɾ����ʱ�ļ�

private:
	static void   StoreTempData( );                            //��������������¼����ʱ�ļ�
	static void   StoreNewDataToTempFile(unsigned int uParam,  //�����յ����������ϴ浽��ʱ�ļ�
		                                 KRankMessage *pInData);
	static void   StoreNewDataToTempFile(unsigned int uParam,  //�����յ����������ϴ浽��ʱ�ļ�
		                                 KRankIndex *pInData);

private:
	int	   WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	virtual void Breathe();                                    //�ף�Ϊʲô����Ҳ�У�

private:
	static KUiStrengthRank* m_pSelf;

private:
	void   Initialize();
//	void   UpdateIndexData();                   //��������������
	void   UpdateRankData(int nID);             //����ָ������������

	static void StoreTempIni();                 //��������������¼����ʱ�ļ�
	BOOL   LoadIndexIni(KIniFile *Ini);         //��INI��������������
	BOOL   LoadRankIni(KIniFile *Ini, int nID); //��INI��������������
                                                //���ǿ���
//	void   SendRobIndexToCore();                //��Core����������������
//	void   SendGetIndexToCore();                //����ͬ�ϣ���Core�����ȡ��������
	void   SendGetRankToCore(int nID);          //��Core���ͻ�ȡ�������ݵ�����
                                                //��Ҳ�ǿ���
	void   ConstructIndexView();                //��Index������(m_data)�����ȥm_indexList���棬����ʾ����
	void   ConstructRankView();                 //��Rank���������ݹ����ȥm_RankList���棬����ʾ����
	void   ReleasePic();                        //�ͷ�ͼ����Դ
	void   ReleaseData();                       //�ͷ����ݺ�������ʾ

	void   ArrangeDefaultData();                //����Ĭ�����ݵ���ʾ

	static void   AddRank(unsigned int uParam, KRankMessage *pRankData);
	static void   AddIndex(unsigned int uParam, struct KRankIndex *pIndex);

private:
	KWndButton				  m_BtnClose;
	KWndList				  m_IndexList;
	KWndScrollBar			  m_IndexScroll;
	KWndMessageListBox		  m_RankList;
	KWndScrollBar			  m_RankScroll;

	static KIniFile           m_IndexIDIni;
	static KLinkStruct<KRankIndexNode> m_IndexData; //�����������ݵ�����
	static KLinkStruct<KRankNode> m_RankData;       //һ���������ݵ�����

	unsigned int              m_uColor;
	static BOOL               m_bIsIndexDataChange;//�������ݸı�����
	static BOOL               m_bIsRankDataChange; //�������ݸı�����
	KRankIndexNode           *m_pCurrentIndexNode; //��ǰ������ʾ������������

	int                       m_nFontSize;          //�����С
	int                       m_nLineLen;           //����������г���
	int                       m_nDefaultID;          //Ĭ�ϵ��������ڵ�ID������ֻ��һ��������һ����������
	unsigned char            *m_pIDontThinkSo;      //����Ϊ��WndList�����ڣ���ΰ��İ��鰡~~(���Ǵ���WndList��SetContent������ָ��)
    unsigned short            m_usUpPic;            //������ͼƬ
	unsigned short            m_usSteadyPic;        //��ƽ��ͼƬ
	unsigned short            m_usDownPic;          //�½���ͼƬ

	int						  m_nLastIndex;
	unsigned int             *m_pLastGettimes;
	unsigned int              m_uLastGetIndexTime;
	unsigned int              m_uLastGetRankTime;

	char                      m_szUpPicPath[128];
	char                      m_szDownPicPath[128];
	char                      m_szSteadyPicPath[128];
};

int TDrawOutString(char *szBuf, int nBufLen, int nLineLen, int nFontSize);
#endif // !defined(AFX_KUISTRENGTHORDER_H__7801F7C7_BAE8_4A49_BEA8_63B5F8FA0BB1__INCLUDED_)
