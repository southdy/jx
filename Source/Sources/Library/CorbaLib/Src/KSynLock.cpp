// KSynLock.cpp: implementation of the KSynLock class.
//
//////////////////////////////////////////////////////////////////////

#include "KSynLock.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

long KSynLock::Lock()	 //����Ƿ������Ѿ�������������ǵĻ����ص�ǰ��ֵ
{
	if (m_bLock)
		++m_nCount ;
	else 
	{
		if (m_nCurId == 0)//δ��ʹ��
		{
			m_bLock = TRUE;
			m_nCurId = 1;
			m_nCount  = 1;
			return m_nCurId;
		}
		else 
			return -1*(++m_nCount) ;
	}
	
	return -1 * m_nCount;
}

BOOL  KSynLock::UnLock(long id)
{
	//Ҫ������Ϊ��ǰʹ���ߣ����Է���
	if (id == m_nCurId)
	{
		m_bLock = FALSE;
		if (m_nCount <= m_nCurId)  
		{
			m_nCount = m_nCurId = 0;
			printf("\n\n����IndexΪ�㣡����\n\n");
		}
		else 
			m_nCurId ++;
		return TRUE;
	}
	
	return FALSE;
}

