//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
// 
// File:	KLuaScriptSet.h
// Date:	2001-10-18 11:56:34
// Code:	Romandou
// Desc:	
//---------------------------------------------------------------------------
#ifndef KLUASCRIPTSET_H
#define KLUASCRIPTSET_H
//---------------------------------------------------------------------------

/*
#include "KScriptSet.h"
#define MAXLUASCRIPTNUM  10
#define NUMPERCLEAR 3

//---------------------------------------------------------------------------
class ENGINE_API KLuaScriptSet : public KScriptSet
{
public:
	KLuaScriptSet();
	~KLuaScriptSet();

	KScript * CreateScript(char * szKey, int StackSize);
	KScript * CreateScript(DWORD nKey, int StackSize);
	BOOL ClearUpSet(void);//���нű�����⣬һ�����ֳ�����ִ�����
	DWORD ClearUpSet(DWORD);//���ָ�������Ľű�
	DWORD GetMaxScriptNum(){return m_nMaxScriptNum;	}
	DWORD GetNumPerClear(){return m_nNumPerClear;	}
	BOOL SetMaxScriptNum(DWORD nMaxCount = MAXLUASCRIPTNUM);
	BOOL SetNumPerClear(DWORD nNumPerClear = NUMPERCLEAR);


	virtual BOOL Run(DWORD nKey);
	virtual BOOL Run(char * szKey);
	virtual BOOL RunFunction(DWORD nKey, char * szFuncName, int nResults, char * szFormat, ...);
	virtual BOOL RunFunction(char * szKey, char * szFuncName, int nResults, char * szFormat, ...);
	

private:
	DWORD m_nMaxScriptNum;//���ű�������
	DWORD m_nNumPerClear;//ÿ������ű�����ʱ�������
};
//---------------------------------------------------------------------------
*/
#endif //KLUASCRIPTSET_H
