//---------------------------------------------------------------------------
// Sword3 Engine (c) 2003 by Kingsoft
//
// File:	KPlayerTong.h
// Date:	2003.08.12
// Code:	�߳�����
// Desc:	KPlayerTong Class
//---------------------------------------------------------------------------

#ifndef KPLAYERTONG_H
#define KPLAYERTONG_H

class KPlayerTong
{
	friend class KPlayer;
private:
	int			m_nPlayerIndex;		// ָ���Ӧ��player�����λ��
	int			m_nFlag;			// �Ƿ������
	int			m_nFigure;			// ����е����
	int			m_nCamp;			// �����Ӫ
	DWORD		m_dwTongNameID;		// ������ַ���ת���ɵ� dword
	char		m_szName[32];		// �������
	char		m_szTitle[32];		// ��ǰ�ƺ�
	int			m_nApplyTo;			// ����Ѿ���ĳ�����������룬������ playerindex

public:
	char		m_szMasterName[32];	// ��������

private:
	BOOL		CheckAcceptAddApplyCondition();	// �ж��Ƿ����ת�����˵ļ���������

public:
	// ��ʼ��
	void		Init(int nPlayerIdx);
	void		Clear();
	void		GetTongName(char *lpszGetName);
	DWORD		GetTongNameID();
	void		SetTongNameID(DWORD dwID) { m_dwTongNameID = dwID; };
	int			CheckIn() {return m_nFlag;};
	int			GetCamp() {return m_nCamp;};
	int			GetFigure() {return m_nFigure;};

	// �Ƿ���Ȩ����ѯ�ӳ���Ϣ
	BOOL		CanGetManagerInfo(DWORD dwTongNameID);
	// �Ƿ���Ȩ����ѯ������Ϣ
	BOOL		CanGetMemberInfo(DWORD dwTongNameID);

#ifndef _SERVER
	// ���봴�����
	BOOL		ApplyCreateTong(int nCamp, char *lpszTongName);
	// �õ�������֪ͨ�������
	void		Create(TONG_CREATE_SYNC *psCreate);
	// ���������
	BOOL		ApplyAddTong(DWORD dwNpcID);
	// �Ƿ���ܳ�Ա bFlag == TRUE ���� == FALSE ������
	void		AcceptMember(int nPlayerIdx, DWORD dwNameID, BOOL bFlag);
	// �����ᣬ��Ϊ��ͨ����
	BOOL		AddTong(int nCamp, char *lpszTongName, char *lpszTitle, char *lpszMaster);
	// ��������
	BOOL		ApplyInstate(int nCurFigure, int nCurPos, int nNewFigure, int nNewPos, char *lpszName);
	// ��������
	BOOL		ApplyKick(int nCurFigure, int nCurPos, char *lpszName);
	// ���봫λ
	BOOL		ApplyChangeMaster(int nCurFigure, int nPos, char *lpszName);
	// �����뿪���
	BOOL		ApplyLeave();

	/* �����ð����Ϣ
	if nInfoID == enumTONG_APPLY_INFO_ID_SELF ���ò���
	if nInfoID == enumTONG_APPLY_INFO_ID_MASTER nParam1 �����id
	if nInfoID == enumTONG_APPLY_INFO_ID_DIRECTOR nParam1 �����id
	if nInfoID == enumTONG_APPLY_INFO_ID_MANAGER nParam1 �����id nParam2 �ӿ�ʼ��Ч�ڼ��� nParam3 ��������
	if nInfoID == enumTONG_APPLY_INFO_ID_MEMBER nParam1 �����id nParam2 �ӿ�ʼ��Ч�ڼ��� nParam3 ��������
	if nInfoID == enumTONG_APPLY_INFO_ID_ONE lpszName ����
	if nInfoID == enumTONG_APPLY_INFO_ID_TONG_HEAD nParam1 NpcIndex
	*/
	BOOL		ApplyInfo(int nInfoID, int nParam1, int nParam2, int nParam3, char *lpszName = NULL);
	// �趨��������Ϣ
	void		SetSelfInfo(TONG_SELF_INFO_SYNC *pInfo);
	
	void        OpenCreateInterface();
#endif

#ifdef _SERVER
	// �жϴ�����������Ƿ���� if �ɹ� return == 0 else return error id
	int			CheckCreateCondition(int nCamp, char *lpszTongName);
	// �õ�relay֪ͨ����ᴴ���ɹ���������Ӧ����
	BOOL		Create(int nCamp, char *lpszTongName);
	// ͷ���Ƿ���Ҫ�����˱�־
	BOOL		GetOpenFlag();
	// ת��������������Է��ͻ���
	BOOL		TransferAddApply(DWORD dwNpcID);
	// ����Ϣ֪ͨ�ܾ�ĳ������
	void		SendRefuseMessage(int nPlayerIdx, DWORD dwNameID);
	// �жϱ��˼����Լ���������Ƿ����
	BOOL		CheckAddCondition(int nPlayerIdx);
	// �����ᣬ��Ϊ��ͨ����
	BOOL		AddTong(int nCamp, char *lpszTongName, char *lpszMasterName, char *lpszTitleName);
	// ���ͻ��˷����Լ��ڰ���е���Ϣ
	void		SendSelfInfo();
	// ����Ƿ�������Ȩ��
	BOOL		CheckInstatePower(TONG_APPLY_INSTATE_COMMAND *pApply);
	// ������
	void		BeInstated(STONG_SERVER_TO_CORE_BE_INSTATED *pSync);
	// ����Ƿ�������Ȩ��
	BOOL		CheckKickPower(TONG_APPLY_KICK_COMMAND *pKick);
	// ���߳����
	void		BeKicked(STONG_SERVER_TO_CORE_BE_KICKED *pSync);
	// ����Ƿ����뿪Ȩ��
	BOOL		CheckLeavePower(TONG_APPLY_LEAVE_COMMAND *pLeave);
	// �뿪���
	void		Leave(STONG_SERVER_TO_CORE_LEAVE *pLeave);
	// ����Ƿ���Ȩ��������
	BOOL		CheckChangeMasterPower(TONG_APPLY_CHANGE_MASTER_COMMAND *pChange);
	// ����Ƿ����������ܴ�λ
	BOOL		CheckGetMasterPower(STONG_SERVER_TO_CORE_CHECK_GET_MASTER_POWER *pCheck);
	// ��λ������ݸı�
	void		ChangeAs(STONG_SERVER_TO_CORE_CHANGE_AS *pAs);
	// ������
	void		ChangeMaster(char *lpszMaster);
	// ��½ʱ���ð����Ϣ
	void		Login(STONG_SERVER_TO_CORE_LOGIN *pLogin);

	void		DBSetTongNameID(DWORD dwID) { m_dwTongNameID = dwID; if (dwID) m_nFlag = 1;};

#endif
};
#endif
