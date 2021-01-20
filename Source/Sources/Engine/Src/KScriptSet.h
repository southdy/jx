//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
// 
// File:	KScriptSet.h
// Date:	2001-10-31 10:50:28
// Code:	Romandou
// Desc:	
//---------------------------------------------------------------------------
#ifndef _KSCRIPTSET_H
#define _KSCRIPTSET_H
#include "KScript.h"
#include "KLuaScript.h"
#define MAXSCRIPTNUM 100 // ���ű���
#define KEY_NUMBER  0
#define KEY_STRING  1

struct TScript  
{
	char szKey[32];	//�ؼ��ֿ����ǽű���Ӧ���ļ���������
	DWORD nKey;//�ؼ���Ϊ����
	KScript * pScript;	//��Ӧ�Ľű�ʵ���ĵ�ַ
	TScript * pParent;		//��
	TScript * pLeftChild;	//����
	TScript * pRightChild;	//����
	TScript * pMoreRecent;	//��֮�����
	TScript * pLessRecent;	//��֮��ٵ�
};//�����ļ���������������Ϊ�ű���ʶ�����������


//�������ṹ��׼����
extern TScript * BTSearch(TScript * pParentTScript, TScript * pTScript, char * szKey, BOOL * pResult);
extern DWORD BTPreorder(TScript * pTScript);//����������������
extern TScript * BTInsert(TScript *pTScript, char * szKey);
extern TScript * BTFindLess(TScript * pTScript);
extern TScript * BTSearch(TScript * pParentTScript, TScript * pTScript, DWORD nKey, BOOL * pResult);
extern DWORD BTPreorder(TScript * pTScript);//����������������
extern TScript * BTInsert(TScript *pTScript, DWORD nKey);
extern TScript *  BTDelete(TScript * pTScript, TScript ** ppRootTScript, int nKeyStyle);


//---------------------------------------------------------------------------
class ENGINE_API KScriptSet //�ű����Ͽ�����
{		
public:		
	KScriptSet();
	~KScriptSet();

	KScriptSet(int Key_Style);

	
	KScript * GetScript(char * szKey, BOOL nNeedCreate, BOOL nNeedUpdateRecent);	//���ݹؼ��ֻ�ýű�	
	KScript * GetScript(DWORD nKey, BOOL nNeedCreate, BOOL nNeedUpdateRecent);
	DWORD  ListScriptsKey();
	DWORD  ListRecent(int order);
	
	TScript *  SearchScript(char * szKey, BOOL *pnResult);//���ҹؼ��ֵĽ�㣬��������*pnResult = 1�����ظý�㣻����*pnResult = 0,���ؽӽ��㡣
	TScript *  SearchScript(DWORD nKey, BOOL *pnResult);//���ҹؼ��ֵĽ�㣬��������*pnResult = 1�����ظý�㣻����*pnResult = 0,���ؽӽ��㡣
	BOOL DeleteScript(char * szKey);//ɾ���ؼ��ֵĽ��
	BOOL DeleteScript(DWORD szKey);//ɾ���ؼ��ֵĽ��
	
	DWORD GetCount(){return ListRecent(0);};	//��õ�ǰ�������
	int	  GetKeyStyle(){return m_nKeyStyle;	};
	
	virtual BOOL	Run(char * szKey);
	virtual BOOL	Run(DWORD nKey);

	virtual BOOL	RunFunction(DWORD nKey, char * szFuncName, char * szFormat, ...);
	virtual BOOL	RunFunction(char * szKey, char * szFuncName, char * szFormat, ...);
		 
//protected:
	TScript * m_pRootList;		//��������֧��
	TScript * m_pMostRecent;	//����ʹ�ýű�
	TScript * m_pLestRecent;	//����ʹ�ýű�
	
	virtual KScript *  CreateScript(char * szKey , int StackSize); 
	virtual KScript *  CreateScript(DWORD nKey , int StackSize); 
	virtual	char * GetScriptFileNameFromKey(char * szKey){ return szKey;};
	virtual char * GetScriptFileNameFromKey(DWORD nKey){return NULL;}
	
	virtual int  GetInitStackSize(char * szKey);//�ڽ��г�ʼ��ʱ����ýű��ѵĴ�С
	virtual int  GetInitStackSize(DWORD nKey);//�ڽ��г�ʼ��ʱ����ýű��ѵĴ�С
	
	TScript * InsertScript(char * szKey);//�����Ըùؼ��ֵĽ��
	TScript * InsertScript(DWORD nKey);//�����Ըùؼ��ֵĽ��
	BOOL DeleteScript(TScript * pTScript);//ɾ�����
	
	void UpdateRecent(BOOL bExistedScript, TScript * pTScript);
	int m_nKeyStyle ;//�ؼ��ֵ����� ������ �������ַ�����ֵ�ֱ�Ϊ KEY_NUMER KEY_STRING
	
	

};
//---------------------------------------------------------------------------
#endif
