// KTongControl.h: interface for the CTongControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KTONGCONTROL_H__62D04F9A_67CD_419B_B475_BF0F8727A91E__INCLUDED_)
#define AFX_KTONGCONTROL_H__62D04F9A_67CD_419B_B475_BF0F8727A91E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


struct STONG_MEMBER
{
	DWORD	m_dwNameID;
	char	m_szName[defTONG_STR_LENGTH];
};

typedef struct
{
	char		szName[defTONG_STR_LENGTH];			// �����
	int			MemberCount;						//һ�������
}TTongList;	//��������б�

class CTongControl
{
	friend class CTongSet;
	friend class CTongDB;
public:
	// ���ڴ������
	CTongControl(int nCamp, char *lpszPlayerName, char *lpszTongName);
	// �������ݿ�����������
	CTongControl(TTongList sList);
	virtual ~CTongControl();

private:

	int			m_nCamp;								// �����Ӫ
	DWORD		m_dwMoney;								// ����ʽ�
	int			m_nCredit;								// �������
	int			m_nLevel;								// ���ȼ�
	int			m_nDirectorNum;							// ��������
	int			m_nManagerNum;							// �ӳ�����
	int			m_nMemberNum;							// ��������

	DWORD		m_dwNameID;								// �����ID
	char		m_szName[defTONG_STR_LENGTH];			// �����
	char		m_szPassword[defTONG_STR_LENGTH];		// �������

	char		m_szMasterTitle[defTONG_STR_LENGTH];	// �����ƺ�
	char		m_szDirectorTitle[defTONG_MAX_DIRECTOR][defTONG_STR_LENGTH];// �����ϳƺ�
	char		m_szManagerTitle[defTONG_MAX_MANAGER][defTONG_STR_LENGTH];	// ���ӳ��ƺ�
	char		m_szNormalTitle[defTONG_STR_LENGTH];	// �����ڳƺ�

	DWORD		m_dwMasterID;							// ������ID
	char		m_szMasterName[defTONG_STR_LENGTH];		// ������

	DWORD		m_dwDirectorID[defTONG_MAX_DIRECTOR];	// ��������ID
	char		m_szDirectorName[defTONG_MAX_DIRECTOR][defTONG_STR_LENGTH];	// ��������

	DWORD		m_dwManagerID[defTONG_MAX_MANAGER];		// ���ӳ���ID
	char		m_szManagerName[defTONG_MAX_MANAGER][defTONG_STR_LENGTH];	// ���ӳ���

	STONG_MEMBER	*m_psMember;
	int				m_nMemberPointSize;

public:
	BOOL		SearchOne(char *lpszName, STONG_ONE_LEADER_INFO *pInfo);

	BOOL		AddMember(char *lpszPlayerName);		// ���һ����Ա

	BOOL		ChangePassword(char *lpOld, char *lpNew);	// �ı�������

	BOOL		GetTongHeadInfo(STONG_HEAD_INFO_SYNC *pInfo);	// �����Ϣ

	BOOL		GetTongManagerInfo(STONG_GET_MANAGER_INFO_COMMAND *pApply, STONG_MANAGER_INFO_SYNC *pInfo);

	BOOL		GetTongMemberInfo(STONG_GET_MEMBER_INFO_COMMAND *pApply, STONG_MEMBER_INFO_SYNC *pInfo);

	BOOL		Instate(STONG_INSTATE_COMMAND *pInstate, STONG_INSTATE_SYNC *pSync);

	BOOL		Kick(STONG_KICK_COMMAND *pKick, STONG_KICK_SYNC *pSync);

	BOOL		Leave(STONG_LEAVE_COMMAND *pLeave, STONG_LEAVE_SYNC *pSync);

	BOOL		AcceptMaster(STONG_ACCEPT_MASTER_COMMAND *pAccept);

	BOOL		GetLoginData(STONG_GET_LOGIN_DATA_COMMAND *pLogin, STONG_LOGIN_DATA_SYNC *pSync);
};

#endif // !defined(AFX_KTONGCONTROL_H__62D04F9A_67CD_419B_B475_BF0F8727A91E__INCLUDED_)
