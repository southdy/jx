//#include "kstdafx.h"
#ifndef S3DBINTERFACE_H
#define S3DBINTERFACE_H
#define IN
#define OUT
#include <map>
#include <string>
#include <vector>
#define SAVE_ITEM_MAGICATTRIB_TO_DB

#ifndef MAGICATTRIB
#define MAGICATTRIB
struct KMagicAttrib
{
	int				nAttribType;					//��������
	int				nValue[3];						//���Բ���
	KMagicAttrib(){nValue[0] = nValue[1] = nValue[2] = nAttribType = 0;};
};
#else
struct KMagicAttrib;
#endif

class KTabFileCtrl;
class KIniFile;


struct TIntDBValue
{
	BYTE btUnitType;

};

struct TRoleBaseInfo
{
		//RoleBaseInfo
	DWORD dwId;
	char	szName[100];
	bool	bSex;
	char	szAlias[100];		
	char	caccname[100]                   ;
	BYTE	nFirstSect;
	BYTE	nSect;
	int		ijoincount                     ;
	int		isectrole                      ;
	int		igroupcode                     ;
	int		igrouprole                     ;
	int		irevivalid                     ;
	char	cpartnercode[100]                   ;
	int		isavemoney                                     ;
	int		imoney                                         ;
	int		ifiveprop                      ;
	int		iteam                          ;
	int		ifightlevel                    ;
	int		fightexp                      ;
	int		ileadlevel                     ;
	int		ileadexp                       ;
	int		iliveexp                       ;
	int		ipower                         ;
	int		iagility                       ;
	int		iouter                         ;
	int		iinside                        ;
	int		iluck                          ;
	int		imaxlife                       ;
	int		imaxstamina                    ;
	int		imaxinner                      ;
	int		ileftprop                      ;
	int		ileftfight                     ;
	int		ileftlife                      ;
	int		ifinishgame                    ;
	int		irevivalx                      ;
	int		irevivaly                      ;
	short	iarmorres                      ;
	short	iweaponres                     ;
	short	ihelmres                       ;
};

struct TRoleData
{
	TRoleBaseInfo BaseInfo;
	bool	bBaseNeedUpdate;//֪ͨ�Ƿ���Ҫ����
	//����ȡʱ���Offset����-1����ʾ�����¸�����
	//Additional Info
	short	nFightSkillCount;
	short	nLiveSkillCount;
	BYTE	nTaskCount;
	short	nItemCount;
	short	nFriendCount;
	int		dwTaskOffset;
	int		dwLSkillOffset;
	int		dwFSkillOffset;
	int		dwItemOffset;
	int		dwFriendOffset;
	int		dwDataLen;
	BYTE	pBuffer[1];
};

struct TDBTaskData
{
	BYTE	m_nTaskId;
	short	m_nTaskValue;
};

struct TDBItemData
{
	//BaseInfo
	int iid;
	char cusercode[100];
	int iequipclasscode;
	int ilocal                                      ;
	int ix                                            ;
	int iy                                            ;
	int iequipcode                                     ;
	int idetailtype                    ;
	int iparticulartype                ;
	int ilevel                         ;
	int iseries                        ;
	int iequipversion                  ;
	int irandseed                      ;
	int iparam2                        ;
	int iparam3                        ;
	int iparam5                        ;
	int iparam4                        ;
	int iparam6                        ;
	int iparam1                        ;
	int ilucky                         ;
#ifdef SAVE_ITEM_MAGICATTRIB_TO_DB
	BYTE BaseInfoCount;
	BYTE EfficCount;
	BYTE RequireInfoCount;
	BYTE pBuffer[1];
#endif
};

struct TDBFriendsData
{
	short FriendCount;
	void * pBuffer;
	short Offset[1];
};

struct TDBSkillData
{
	short	m_nSkillId;
	short	m_nSkillLevel;
};

enum 
{
/*==============================================================*/
/* Table: EquipBaseInfo                                         */
/*==============================================================*/
   EQUIPBASEINFO_IID                                             ,
   EQUIPBASEINFO_IMAINID                                        ,
   EQUIPBASEINFO_ICLASS                                      ,
   EQUIPBASEINFO_IVALUE1                        ,
   EQUIPBASEINFO_IVALUE2                        ,
   EQUIPBASEINFO_IVALUE3                        ,
/*==============================================================*/
/* TABLE: EQUIPEFFICINFO_                                        */
/*==============================================================*/
   EQUIPEFFICINFO_IID                                             ,
   EQUIPEFFICINFO_IMAINID                                        ,
   EQUIPEFFICINFO_ICLASS                                      ,
   EQUIPEFFICINFO_IVALUE1                        ,
   EQUIPEFFICINFO_IVALUE2                        ,
   EQUIPEFFICINFO_IVALUE3                        ,

/*==============================================================*/
/* TABLE: EQUIPREQUIREINFO_                                     */
/*==============================================================*/
   EQUIPREQUIREINFO_IID                                             ,
   EQUIPREQUIREINFO_IMAINID                                        ,
   EQUIPREQUIREINFO_ICLASS                                      ,
   EQUIPREQUIREINFO_IVALUE1                        ,
   EQUIPREQUIREINFO_IVALUE2                        ,
   EQUIPREQUIREINFO_IVALUE3                        ,
/*==============================================================*/
/* TABLE: EQUIPMENTS                                            */
/*==============================================================*/
   EQUIPMENTS_IID                                             ,
   EQUIPMENTS_CUSERCODE                                         ,
   EQUIPMENTS_IEQUIPCLASSCODE                                   ,
   EQUIPMENTS_ILOCAL                                      ,
   EQUIPMENTS_IX                                            ,
   EQUIPMENTS_IY                                            ,
   EQUIPMENTS_IEQUIPCODE                                     ,
   EQUIPMENTS_IDETAILTYPE                    ,
   EQUIPMENTS_IPARTICULARTYPE                ,
   EQUIPMENTS_ILEVEL                         ,
   EQUIPMENTS_ISERIES                        ,
   EQUIPMENTS_IEQUIPVERSION                  ,
   EQUIPMENTS_IRANDSEED                      ,
   EQUIPMENTS_IPARAM2                        ,
   EQUIPMENTS_IPARAM3                        ,
   EQUIPMENTS_IPARAM5                        ,
   EQUIPMENTS_IPARAM4                        ,
   EQUIPMENTS_IPARAM6                        ,
   EQUIPMENTS_IPARAM1                        ,
   EQUIPMENTS_ILUCKY                         ,
 
/*==============================================================*/
/* TABLE: FIGHTSKILL                                            */
/*==============================================================*/
   FIGHTSKILL_IID                                             ,
   FIGHTSKILL_CUSERCODE                                         ,
   FIGHTSKILL_IFIGHTSKILL                                 ,
   FIGHTSKILL_IFIGHTSKILLLEVEL                            ,

/*==============================================================*/
/* TABLE: FRIENDLIST_                                           */
/*==============================================================*/
   FRIENDLIST_IID                                             ,
   FRIENDLIST_CUSERCODE                                         ,
   FRIENDLIST_CFRIENDCODE                                       ,


/*==============================================================*/
/* TABLE: LIFESKILL                                             */
/*==============================================================*/

   LIFESKILL_IID                                             ,
   LIFESKILL_CUSERCODE                                         ,
   LIFESKILL_ILIFESKILL                                  ,
   LIFESKILL_ILIFESKILLLEVEL                             ,
/*==============================================================*/
/* TABLE: ROLEINFO                                             */
/*==============================================================*/
   ROLEINFO_IID                                             ,
   ROLEINFO_CUSERCODE                                         ,
   ROLEINFO_BSEX                                            ,
   ROLEINFO_CALIAS                         ,
   ROLEINFO_IFIRSTSECT                     ,
   ROLEINFO_ISECT                          ,
   ROLEINFO_IJOINCOUNT                     ,
   ROLEINFO_ISECTROLE                      ,
   ROLEINFO_IGROUPCODE                     ,
   ROLEINFO_IGROUPROLE                     ,
   ROLEINFO_IREVIVALID                     ,
   ROLEINFO_CPARTNERCODE                   ,
   ROLEINFO_ISAVEMONEY                                     ,
   ROLEINFO_IMONEY                                         ,
   ROLEINFO_IFIVEPROP                      ,
   ROLEINFO_ITEAM                          ,
   ROLEINFO_IFIGHTLEVEL                    ,
   ROLEINFO_FIGHTEXP                      ,
   ROLEINFO_ILEADLEVEL                     ,
   ROLEINFO_ILEADEXP                       ,
   ROLEINFO_ILIVEEXP                       ,
   ROLEINFO_IPOWER                         ,
   ROLEINFO_IAGILITY                       ,
   ROLEINFO_IOUTER                         ,
   ROLEINFO_IINSIDE                        ,
   ROLEINFO_ILUCK                          ,
   ROLEINFO_IMAXLIFE                       ,
   ROLEINFO_IMAXSTAMINA                    ,
   ROLEINFO_IMAXINNER                      ,
   ROLEINFO_ILEFTPROP                      ,
   ROLEINFO_ILEFTFIGHT                     ,
   ROLEINFO_ILEFTLIFE                      ,
   ROLEINFO_CACCNAME                       ,
   ROLEINFO_IFINISHGAME                    ,
   ROLEINFO_DLOGINTIME                     ,
   ROLEINFO_DBEGINDATE                     ,
   ROLEINFO_DENDDATE                       ,
   ROLEINFO_IREVIVALX                      ,
   ROLEINFO_IREVIVALY                      ,
   ROLEINFO_IARMORRES                      ,
   ROLEINFO_IWEAPONRES                     ,
   ROLEINFO_IHELMRES                       ,

/*==============================================================*/
/* TABLE: TASKLIST                                             */
/*==============================================================*/
   TASKLIST_IID                                             ,
   TASKLIST_CUSERCODE                                         ,
   TASKLIST_ITASKCODE                                   ,
   TASKLIST_IDEGREE                                     ,

};



//	Ini�ļ����ֶζ���---------------------------------------------------
#define SECTION_ROLE		"ROLE"	

#define SECTION_ITEMLIST	"ITEMS"	
#define SECTION_ITEM		"ITEM" // + id
#define KEY_COUNT			"COUNT"
#define KEY_BASELIST		"BLISTS"
#define KEY_EFFICLIST		"ELISTS"
#define KEY_REQUIRELIST		"RLISTS"

#define SECTION_BASEINFO	"IB" //IB1_2
#define SECTION_EFFICEINFO	"IE"
#define SECTION_REQUIREINFO	"IR"


#define SECTION_FRIENDLIST	"FRIENDS"
#define KEY_FRIEND			"F" // + id

#define SECTION_TASKLIST	"TASKS"
#define KEY_TASKID			"T" // + id
#define KEY_TASKVALUE		"V" // + id

#define SECTION_FIGHTSKILLLIST		"FSKILLS"
#define KEY_FIGHTSKILL		"S"
#define KEY_FIGHTSKILLLEVEL	"L"

#define SECTION_LIFESKILLLIST		"LSKILLS"
#define KEY_LIFESKILL				"S" // + id
#define	KEY_LIFESKILLLEVEL			"L" // + id
//------------------------------------------------------------------------------
#pragma warning (disable: 4512)
#pragma warning (disable: 4786)

#ifdef S3DBINTERFACE_EXPORTS
#define S3DBI_API __declspec(dllexport)
#else
#define S3DBI_API __declspec(dllimport)
#endif

//
struct S3DBI_RoleBaseInfo
{
	char szRoleName[32];
	int	 nSex;
	int	 nHelmType;
	int	 nArmorType;
	int	 nWeaponType;
	int	 nLevel;
};


struct S3DBI_DATAS
{
	char * strNames;
	char * strValues;
	int	   nDataNum;
};


enum  S3DBI_TABLENAME
{
	TABLE_ACCOUNT,
	TABLE_CARD,
	TABLE_EQUIPBASEINFO,
	TABLE_EQUIPEFFICINFO,
	TABLE_EQUIPMENT,
	TABLE_EQUIPREQUIREINFO,
	TABLE_FIGHTSKILL,
	TABLE_FRIEND,
	TABLE_LIFESKILL,
	TABLE_LIST,
	TABLE_ROLE,
	TABLE_TASK,
	TABLE_TASK_LIST,
};

enum S3DBI_LISTNAME
{
	LIST_TASK,
	LIST_FRIEND,
	LIST_FIGHT,
	LIST_LIFESKILL,
	LIST_EQUIPMENT,
	LIST_EQUIPMENTREQINFO,
	LIST_EQUIPMENTBASEINFO,
	LIST_EQUIPMENTEFFICINFO,
};
/*
extern DBTableKeyMap  g_RoleTableKeyMap;

typedef std::map<std::string, int>	DBTableKeyMap;

typedef std::map<int, std::string>	DBStringMap;
typedef std::map<int, int>			DBIntegerMap;
*/
/*struct TDBTaskData
{
	int m_nTaskId;
	int	m_nTaskValue;
};


struct TDBTableData
{
	DBStringMap			m_StringDatas;
	DBIntegerMap		m_IntegerDatas;
};
struct TDBEquiptData;
typedef std::vector<KMagicAttrib>		TMagicAttribArray;
typedef std::vector<TDBEquiptData>		TDBEquiptArray;
typedef std::vector<TDBSkillData>		TDBSkillArray;
typedef std::vector<std::string>					TDBFriendArray;
typedef std::vector<TDBTaskData>			TDBTaskArray;

extern DBTableKeyMap g_TableIntegerKeyMap;
extern DBTableKeyMap g_TableStringKeyMap;
struct TDBEquiptData : TDBTableData
{	
	TMagicAttribArray	m_mapRequestInfo;
	TMagicAttribArray	m_mapBaseInfo;
	TMagicAttribArray	m_mapEfficInfo;
};

struct TDBPlayerData :TDBTableData
{
	TDBEquiptArray		m_mapEquipt;
	TDBSkillArray		m_mapFightSkill;
	TDBSkillArray		m_mapLiveSkill;
	TDBFriendArray		m_mapFriend;
	TDBTaskArray		m_mapTask;
};

*/
#define S3_DB_TABLENAME  S3DBI_TABLENAME
S3DBI_API int S3DBI_InitDBInterface();			//��ʼ�����ݿ�����
S3DBI_API void S3DBI_ReleaseDBInterface();		//�ͷ����ݿ�����

//��ý�ɫ����Ϣ����Ini�ļ��ĸ�ʽ����
//S3DBI_API int	S3DBI_GetRoleInfo(IN char * strUser, IN OUT KIniFile * pIniFile);
S3DBI_API void *	S3DBI_GetRoleInfo(IN OUT BYTE * pRoleBuffer, IN char * strUser, IN OUT int &nBufLen);

//�����ɫ����Ϣ��������ݿⲻ���ڸ���ң������Ӹ����
//bAutoInsertWhenNoExistUser ��ΪTRUEʱ��ʾ�������Ҫ����ĸ���������ݿ��в����������Զ����뵽���ݿ��У�FALSE������ֱ�ӷ��ش���
//ע��INI�ļ�ֻ���Ž���Ҫ�Ķ������ݣ�����Ķ������ݽ��Զ�����ԭ״��
S3DBI_API int	S3DBI_SaveRoleInfo(IN BYTE * pRoleBuffer, IN char * strUser, BOOL bAutoInsertWhenNoExistUser);
S3DBI_API void S3DBI_ShowDebugInfo(IN BYTE * pRoleBuffer, char * pShowString);

S3DBI_API int S3DBI_GetRoleListOfAccount(char * szAccountName, S3DBI_RoleBaseInfo * RoleBaseList, int nMaxCount);

//ɾ����ɫ
S3DBI_API int	S3DBI_DeleteRole(char * strUser);




S3DBI_API void*	S3DBI_GetList(S3DBI_LISTNAME listname,  char * strUser);
S3DBI_API void*	S3DBI_GetList(S3DBI_LISTNAME listname,  int iid);


//S3DBI_API int	S3DBI_GetListData(S3DBI_LISTNAME listname, void * pList, int i , ColumnAndValue &cav);
S3DBI_API int	S3DBI_GetCountOfList(S3DBI_LISTNAME listname, void * pList);
//S3DBI_API int	S3DBI_UpdateDataAtList(S3DBI_LISTNAME listname, void * pList, int i , ColumnAndValue &cav);
S3DBI_API int	S3DBI_DeleteDataAtList(S3DBI_LISTNAME listname, void * pList, int i );
//S3DBI_API int	S3DBI_InsertDataAtList(S3DBI_LISTNAME listname, void * pList, ColumnAndValue cav);
S3DBI_API void	S3DBI_ReleaseListBuffer(S3DBI_LISTNAME listname, void * pList);

S3DBI_API int	S3DBI_ClearList(S3DBI_LISTNAME listname, void * pList);

//S3DBI_API int	S3DBI_GetData(S3_DB_TABLENAME tablename, int nId, ColumnAndValue *pcav);
//S3DBI_API int	S3DBI_GetData(S3_DB_TABLENAME tablename, char * strUser , ColumnAndValue * pCav);
//S3DBI_API int	S3DBI_UpdateData(S3_DB_TABLENAME tablename, int nId, ColumnAndValue *pcav);
//S3DBI_API int	S3DBI_UpdateData(S3_DB_TABLENAME tablename, char * strUser, ColumnAndValue *pcav);
S3DBI_API int	S3DBI_DeleteData(S3_DB_TABLENAME tablename, int nId);
S3DBI_API int	S3DBI_DeleteData(S3_DB_TABLENAME tablename, char * strUser);
//S3DBI_API int	S3DBI_InsertData(S3_DB_TABLENAME tablename, int nId, ColumnAndValue cav);
//S3DBI_API int	S3DBI_InsertData(S3_DB_TABLENAME tablename, char * strUser, ColumnAndValue cav);


S3DBI_API int	S3DBI_GetListData(S3DBI_LISTNAME listname, void * pList, int i , KTabFileCtrl *pTabFile);
S3DBI_API int	S3DBI_DeleteDataAtList(S3DBI_LISTNAME listname, void * pList, int i );
S3DBI_API int	S3DBI_InsertDataAtList(S3DBI_LISTNAME listname, void * pList, KTabFileCtrl *pTabFile);
S3DBI_API int	S3DBI_GetData(S3_DB_TABLENAME tablename, int nId, KTabFileCtrl *pTabFile);
S3DBI_API int	S3DBI_GetData(S3_DB_TABLENAME tablename, char * strUser, KTabFileCtrl * pTabFile);
S3DBI_API int	S3DBI_UpdateData(S3_DB_TABLENAME tablename, int nId, KTabFileCtrl *pTabFile);
S3DBI_API int	S3DBI_UpdateData(S3_DB_TABLENAME tablename, char * strUser, KTabFileCtrl *pTabFile);
S3DBI_API int	S3DBI_InsertData(S3_DB_TABLENAME tablename,  KTabFileCtrl *pTabFile);
#endif
