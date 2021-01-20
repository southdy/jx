#ifndef LUAFUNS_H
#define LUAFUNS_H
#include "KCore.h"
#include "KScriptList.h"
#include "KScriptCache.h"

extern CORE_API TLua_Funcs GameScriptFuns[];
extern CORE_API int	g_GetGameScriptFunNum();

extern CORE_API TLua_Funcs WorldScriptFuns[] ;
extern CORE_API int	g_GetWorldScriptFunNum();

//�ű��б�ʵ��
//Questions:���������ű��б������������.
extern  KScriptList		g_StoryScriptList;	//���������ű��б�
#endif

