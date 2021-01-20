//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KSpriteCache.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Cache class
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KSprite.h"
#include "KSpriteCache.h"


KSpriteCache::KSpriteCache()
{

}

KSpriteCache::~KSpriteCache()
{
    Release();      // ���û����еĺ���,�ͷ����нڵ�
}

//---------------------------------------------------------------------------
// ����:	LoadNode
// ����:	
// ����:	
// ����:	void
//---------------------------------------------------------------------------
BOOL KSpriteCache::LoadNode(KCacheNode* lpNode)
{
	KSprite* pSprite = new KSprite;
	if (pSprite->Load(lpNode->GetName()))
		lpNode->m_lpData = pSprite;
	else
		lpNode->m_lpData = NULL;
	return lpNode->m_lpData != NULL;
}
//---------------------------------------------------------------------------
// ����:	FreeNode
// ����:	
// ����:	
// ����:	void
//---------------------------------------------------------------------------
void KSpriteCache::FreeNode(KCacheNode* lpNode)
{
	KSprite* pSprite = (KSprite*)lpNode->m_lpData;
	if (pSprite)
	{
		pSprite->Free();
		delete pSprite;
	}
	lpNode->m_lpData = NULL;
}
//---------------------------------------------------------------------------
