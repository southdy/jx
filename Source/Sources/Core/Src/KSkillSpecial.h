#ifndef __KSKILLSPECIAL_H__
#define __KSKILLSPECIAL_H__
/*******************************************************************************
// FileName			:	KSkillSpecial.h
// FileAuthor		:	RomanDou
// FileCreateDate	:	2002-9-23 20:49:13
// FileDescription	:	
// Revision Count	:	
*******************************************************************************/
#ifndef _SERVER
#include "KMissleRes.h"

class KSkillSpecial
{
	//Class Member
public:
	int			m_nRegionId;		
	int			m_nCurrentMapX;			//	��ǰ��X����
	int			m_nCurrentMapY;			//	��ǰ��Y����
	int			m_nCurrentMapZ;			//	��ǰ��Z����
	int			m_nXOffset;				//	��ǰ��X����ƫ��
	int			m_nYOffset;				//	��ǰ��Y����ƫ��
	int			m_nPX;
	int			m_nPY;
	int			m_nPZ;
	int			m_nCurDir;
	int			m_nNpcIndex;
	DWORD		m_dwMatchID;
	TMissleRes  * m_pMissleRes;
	DWORD		m_nBeginTime;
	DWORD		m_nEndTime;
	BOOL		m_bFollowMissle;
	int			m_eStatus;
	
#ifdef TOOLVERSION	
	KCacheNode	* m_pSprNode ;
#endif
	KCacheNode  * m_pSndNode ;
	KRUImage	m_RUImage;
	//Class Function
public:
	KSkillSpecial();
	BOOL		Init();
	void		Remove();
	void		Draw(int nCurLifeFrame);
	void		Draw(int nX, int nY , int nZ, int nDir, int nAllFrame,  int nCurLifeFrame);
	void		SetAnimation(char * szFile, unsigned int uImage);
private:
	
};

class KSkillSpecialNode : public KNode 
{
public:
	KSkillSpecial * m_pSkillSpecial;
	KSkillSpecialNode(){m_pSkillSpecial = NULL;};
	~KSkillSpecialNode(){delete m_pSkillSpecial;};
};
extern KList SpecialMovieList;
#endif
#endif //__KSKILLSPECIAL_H__
