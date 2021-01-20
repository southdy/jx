/*****************************************************************************************
//	���ָ�봰��
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-17
*****************************************************************************************/
#include "KWin32.h"
#include "KFile.h"
#include "KPakFile.h"

#include "UiCursor.h"
#include "string.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"

#include "../../../Represent/iRepresent/iRepresentShell.h"
extern iRepresentShell*	g_pRepresentShell;

//--------------------------------------------------------------------------
//	���ܣ����캯��
//--------------------------------------------------------------------------
KUiCursor::KUiCursor()
{
	m_bShow = true;
	m_nCurImage  = 0;
    m_nPositionH = 0;
	m_nPositionV = 0;
	
    #ifdef  KUI_USE_HARDWARE_MOUSE
	{
		for (int i = 0; i < MAX_CURSOR_IMAGE; i++)
		    m_CursorImages[i] = NULL;
	}
	#else   // KUI_USE_HARDWARE_MOUSE
	{
		m_nHotspotH = 0;
		m_nHotspotV = 0;
		for (int i = 0; i < MAX_CURSOR_IMAGE; i++)
		{
			IR_InitUiImageRef(m_CursorImages[i]);
			m_CursorImages[i].nType = ISI_T_SPR;
		}
	    SetCursor(NULL);
	}
	#endif
}

//���ȫ����Դ
void KUiCursor::Cleanup()
{
    #ifdef  KUI_USE_HARDWARE_MOUSE
	{
		::SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
		for (int i = 0; i < MAX_CURSOR_IMAGE; i++)
			m_CursorImages[i] = NULL;
	}
    #else
	{
		if (g_pRepresentShell)
		{
			for (int i = 0; i < MAX_CURSOR_IMAGE; i++)
			{
				if (m_CursorImages[i].uImage)
				{
					g_pRepresentShell->FreeImage(m_CursorImages[i].szImage);
					m_CursorImages[i].uImage = 0;
					m_CursorImages[i].szImage[0] = 0;
				}
			}
		}
	}
	#endif
}

//--------------------------------------------------------------------------
//	���ܣ���������
//--------------------------------------------------------------------------
KUiCursor::~KUiCursor()
{
	Cleanup();
}

//--------------------------------------------------------------------------
//	���ܣ��������ָ��λ��
//--------------------------------------------------------------------------
void KUiCursor::SetPosition(int h, int v)
{
	m_nPositionH = h;
	m_nPositionV = v;
}

//--------------------------------------------------------------------------
//	���ܣ���ȡ���ָ��λ��
//--------------------------------------------------------------------------
void KUiCursor::GetPosition(int& h, int& v)
{
	h = m_nPositionH;
	v = m_nPositionV;
}

//--------------------------------------------------------------------------
//	���ܣ��������ָ��ͼ��
//--------------------------------------------------------------------------

// Engine KPakFile

// Add by Freeway Chen in 2003.7.2
int KUiLoadCursor(const char* pszImgFile, HCURSOR &rhRetCursor)
{
    int nResult = false;
    int nRetCode = false;
    char szTempPath[MAX_PATH] = "";
    char szTempFileName[MAX_PATH] = "";
    unsigned char TempBuffer[4096];
    int nProcessSize = 0;

    KPakFile    SrcCursorFile;
    int         nSrcCursorFileOpenFlag = false;
    int         nSrcFileSize = 0;
    
    KFile       DstCursorFile;
    int         nDstCursorFileOpenFlag = false;

    rhRetCursor = NULL;

    if (!pszImgFile)
        goto Exit0;

    nRetCode = GetTempPath(MAX_PATH, szTempPath);
    if (0 == nRetCode)
        goto Exit0;
    
    nRetCode = GetTempFileName(szTempPath, "KSG_", 0, szTempFileName);
    if (0 == nRetCode)
        goto Exit0;

    nRetCode = DstCursorFile.Create(szTempFileName);
    if (!nRetCode)
        goto Exit0;

    nDstCursorFileOpenFlag = true;

    nRetCode = SrcCursorFile.Open((char *)pszImgFile);
    if (!nRetCode)
        goto Exit0;

    nSrcCursorFileOpenFlag = true;

    nSrcFileSize = SrcCursorFile.Size();

    while (nSrcFileSize > 0)
    {
        nProcessSize = SrcCursorFile.Read(TempBuffer, 4096);
        if (nProcessSize == 0)
            break;

        DstCursorFile.Write(TempBuffer, nProcessSize);

        nSrcFileSize -= nProcessSize;
    }

    if (nSrcCursorFileOpenFlag)
    {
        SrcCursorFile.Close();
        nSrcCursorFileOpenFlag = false;
    }

    if (nDstCursorFileOpenFlag)
    {
        DstCursorFile.Close();
        nDstCursorFileOpenFlag = false;
    }

    rhRetCursor = LoadCursorFromFile(szTempFileName);
    if (!rhRetCursor)
        goto Exit0;

    nResult = true;
Exit0:

    if (nSrcCursorFileOpenFlag)
    {
        SrcCursorFile.Close();
        nSrcCursorFileOpenFlag = false;
    }

    if (nDstCursorFileOpenFlag)
    {
        DstCursorFile.Close();
        nDstCursorFileOpenFlag = false;
    }

    if (szTempFileName[0])
    {
        DeleteFile(szTempFileName);
        szTempFileName[0] = '\0';
    }


    return nResult;
}



void KUiCursor::SetImage(int nIndex, const char* pImgFile)
{
    #ifdef  KUI_USE_HARDWARE_MOUSE
	{
		if (nIndex >= 0 && nIndex < MAX_CURSOR_IMAGE && pImgFile)
		{
			KUiLoadCursor(pImgFile, m_CursorImages[nIndex]);
			if (m_nCurImage == nIndex)
				SetCursor(m_CursorImages[m_nCurImage]);
		}
    }
    #else   // KUI_USE_HARDWARE_MOUSE
	{	
	    if (nIndex >= 0 && nIndex < MAX_CURSOR_IMAGE && pImgFile)
		{
			KUiImageRef* pCurrentImg = &m_CursorImages[nIndex];
			pCurrentImg->nFlipTime = 0;
			pCurrentImg->nInterval = 0;
			pCurrentImg->nNumFrames = 0;
			strncpy(pCurrentImg->szImage, pImgFile, sizeof(pCurrentImg->szImage));
			pCurrentImg->szImage[sizeof(pCurrentImg->szImage) - 1] = 0;
			pCurrentImg->bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
			KImageParam	Param;
			Param.nReferenceSpotX = 0;
			Param.nReferenceSpotY = 0;
			g_pRepresentShell->GetImageParam(pCurrentImg->szImage, &Param, pCurrentImg->nType);
			m_nHotspotH = Param.nReferenceSpotX;
			m_nHotspotV = Param.nReferenceSpotY;
		}
	}
    #endif
}

//--------------------------------------------------------------------------
//	���ܣ��л���ǰ���ָ��ͼ��
//--------------------------------------------------------------------------
int	KUiCursor::SwitchImage(int nIndex)
{
    #ifdef  KUI_USE_HARDWARE_MOUSE 
	{
        if (nIndex >= 0 && nIndex < MAX_CURSOR_IMAGE)
		{
			m_nCurImage = nIndex;
			if (m_CursorImages[m_nCurImage])
			{
				SetCursor(m_CursorImages[m_nCurImage]);
			}
			else
			{
				SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
			}
		}
		else
		{
			SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
		}
		return m_nCurImage;
	}
    #else   // KUI_USE_HARDWARE_MOUSE
	{
		if (nIndex >= 0 && nIndex < MAX_CURSOR_IMAGE)
			m_nCurImage = nIndex;
		return m_nCurImage;
	}    
    #endif
}

//--------------------------------------------------------------------------
//	���ܣ����¼������
//--------------------------------------------------------------------------
void	KUiCursor::RestoreCursor()
{
    SwitchImage(m_nCurImage);
}


//--------------------------------------------------------------------------
//	���ܣ��������ָ��
//--------------------------------------------------------------------------
void KUiCursor::Paint()
{
	#ifdef  KUI_USE_HARDWARE_MOUSE 
	{
	    NULL;
	}
    #else   // KUI_USE_HARDWARE_MOUSE
	{
	    if (m_bShow && g_pRepresentShell)
		{
			m_CursorImages[m_nCurImage].oPosition.nX = m_nPositionH - m_nHotspotH;
			m_CursorImages[m_nCurImage].oPosition.nY = m_nPositionV - m_nHotspotV;
    
			g_pRepresentShell->DrawPrimitives(1, &m_CursorImages[m_nCurImage], RU_T_IMAGE, true);
			IR_NextFrame(m_CursorImages[m_nCurImage]);
		}
	}
    #endif
}

//�������ָ�����ʾ״̬
void KUiCursor::Show(int bShow)
{
	m_bShow = bShow;

	#ifdef  KUI_USE_HARDWARE_MOUSE
		ShowCursor(m_bShow);
    #endif
}