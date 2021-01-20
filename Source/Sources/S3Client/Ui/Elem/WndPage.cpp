/*****************************************************************************************
//	���洰����ϵ�ṹ--ҳ�洰��
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-9
------------------------------------------------------------------------------------------
    ������M$ Win32����� PropertiePage
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "WndButton.h"
#include "WndPage.h"

//--------------------------------------------------------------------------
//	���ܣ����ں���
//--------------------------------------------------------------------------
//		�����ҳ���л���ť����ҳ��հ���ʱ��Ҫ����Ϣת����PageSet����
//--------------------------------------------------------------------------
int KWndPage::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	if ((uMsg == WM_LBUTTONDOWN || uMsg == WM_MOUSEMOVE ||
		uMsg == WM_LBUTTONUP || uMsg == WND_N_BUTTON_CLICK) && m_pParentWnd)
	{
		return m_pParentWnd->WndProc(uMsg, uParam, nParam);
	}
	return KWndImage::WndProc(uMsg, uParam, nParam);
}

//------------------------------------------------------------------------------------------
//		���ҳ�漯�ϴ���
//------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------
//	���ܣ����캯��
//--------------------------------------------------------------------------
KWndPageSet::KWndPageSet()
{
	m_pPageBtnPairList = NULL;
	m_nNumPage = 0;
	m_nAcitvePage = -1;
}

//--------------------------------------------------------------------------
//	���ܣ���������
//--------------------------------------------------------------------------
KWndPageSet::~KWndPageSet()
{
	if (m_pPageBtnPairList)
	{
		free(m_pPageBtnPairList);
		m_pPageBtnPairList = NULL;
	}
	m_nNumPage = 0;
}

//--------------------------------------------------------------------------
//	���ܣ����ں���
//--------------------------------------------------------------------------
int KWndPageSet::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	if (uMsg != WND_N_BUTTON_CLICK)
		return KWndImage::WndProc(uMsg, uParam, nParam);
	OnPageBtnClick((KWndWindow*)uParam);
	return 0;
}

//--------------------------------------------------------------------------
//	���ܣ����ҳ��
//--------------------------------------------------------------------------
bool KWndPageSet::AddPage(KWndPage* pPage, KWndButton* pPageBtn)
{
	if (pPage && pPageBtn)
	{
		AddChild(pPage);
		KWndPageBtnPair* pNewList = (KWndPageBtnPair*)realloc(m_pPageBtnPairList, sizeof(KWndPageBtnPair) * (m_nNumPage + 1));
		if (pNewList)
		{
			m_pPageBtnPairList = pNewList;
			m_pPageBtnPairList[m_nNumPage].pPage = pPage;
			m_pPageBtnPairList[m_nNumPage].pPageBtn = pPageBtn;
			m_nNumPage++;
			if ((m_nNumPage) == 1)	//ֻ������ռ����ҳ��
				ActivePage(0);
			else
			{
				pPage->Hide();				
				int	nActivePage = m_nAcitvePage;
				m_nAcitvePage = -1;
				ActivePage(nActivePage);
			}
			return true;
		}
	}
	return false;
}

//--------------------------------------------------------------------------
//	���ܣ�����ָ����ҳ��
//--------------------------------------------------------------------------
bool KWndPageSet::ActivePage(int nPageIndex)
{
	if (nPageIndex >= 0 && nPageIndex < m_nNumPage && nPageIndex != m_nAcitvePage)
	{
		if (m_nAcitvePage >= 0)
			m_pPageBtnPairList[m_nAcitvePage].pPage->Hide();
		m_nAcitvePage = nPageIndex;
		m_pPageBtnPairList[m_nAcitvePage].pPage->Show();
		for (int i = 0; i < m_nNumPage; i++)
		{
			m_pPageBtnPairList[i].pPageBtn->SplitSmaleFamily();
			AddChild(m_pPageBtnPairList[i].pPageBtn);
			m_pPageBtnPairList[i].pPageBtn->CheckButton(i == nPageIndex);
		}
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------
//	���ܣ���Ӧ�����л�ҳ�水ť�Ĳ���
//--------------------------------------------------------------------------
void KWndPageSet::OnPageBtnClick(KWndWindow* pBtn)
{
	for (int i = 0; i < m_nNumPage; i++)
	{
		if (m_pPageBtnPairList[i].pPageBtn == pBtn)
		{
			if (i != m_nAcitvePage)
				ActivePage(i);
			for (i = 0; i < m_nNumPage; i++)
				m_pPageBtnPairList[i].pPageBtn->CheckButton(m_pPageBtnPairList[i].pPageBtn == pBtn);
			break;
		}
	}
}

//--------------------------------------------------------------------------
//	���ܣ�����ָ����ҳ��
//--------------------------------------------------------------------------
KWndPage* KWndPageSet::GetActivePage()
{
	if (m_nAcitvePage >= 0)
		return m_pPageBtnPairList[m_nAcitvePage].pPage;
	else
		return NULL;
}