/*******************************************************************************
// FileName			:	KFont3.cpp
// FileAuthor		:	Wooy
// FileCreateDate	:	2001-9-13 10:19:21
// FileDescription	:	������
// Revision Count	:	
*******************************************************************************/
#include "KFont3.h"
#include "string.h"
#include "../../../Engine/Src/Text.h"

//Handle to a vertex shader to KFontVertex vertex format
#define KF_FONTPOLY (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)


//����ȫ������һ���ǹ���һ����ͼ�豸������������ʵ�����ڵ�ʱ���ڶ���������ͼ�豸��
//�任�����Դ�Դ�����ļ��У��ѵ�һ�γ�ʼ������ʵ������ʱ���豸�϶�ΪΨһ�Ļ�ͼ�豸��
//��ֻ���һ���豸����ģ����ص���Ϣ��
//����Ժ󲻷�����������������ģ�����Ҫ�Ķ����룬����Ӧ�µ���Ҫ��

#define	ALP_CMP_CAP_HANT_BEEN_CHECKED	D3DCMP_NEVER

unsigned int		KFont3::ms_AlphaCmpMethod = ALP_CMP_CAP_HANT_BEEN_CHECKED;
LPDIRECT3DDEVICE9	KFont3::ms_pd3dDevice	= NULL;
unsigned int		KFont3::ms_AlphaRef1	= 0;
unsigned int		KFont3::ms_AlphaRef2	= 0;
fnRenderText		KFont3::ms_RenderText	= KFont3::RenderTextDirect;
unsigned int		KFont3::ms_uBorderColor	= 0xff000000;

//����ͼ���豸��֧�ֵ�alpha��ⷽ��
void KFont3::CheckAlphaCmpCaps()
{
	if (ms_pd3dDevice == NULL || ms_AlphaCmpMethod != ALP_CMP_CAP_HANT_BEEN_CHECKED)
		return;
	D3DCAPS9	Caps;
	if (FAILED(ms_pd3dDevice->GetDeviceCaps(&Caps)))
		return;

	KFontRes::EnableTextBorder(true);

	//�������жϸ��ݻ�ͼ�����Ч������
	if (Caps.AlphaCmpCaps & D3DPCMPCAPS_GREATEREQUAL)
	{
		ms_AlphaCmpMethod = D3DCMP_GREATEREQUAL;
		ms_AlphaRef1 = 0xE0;
		ms_AlphaRef2 = 0xF0;
		ms_RenderText = RenderTextCmpReverse;
	}
	else if (Caps.AlphaCmpCaps & D3DPCMPCAPS_GREATER)
	{
		ms_AlphaCmpMethod = D3DCMP_GREATER;
		ms_AlphaRef1 = 0xDf;
		ms_AlphaRef2 = 0xEf;
		ms_RenderText = RenderTextCmpReverse;
	}
	else if (Caps.AlphaCmpCaps & D3DPCMPCAPS_LESSEQUAL )
	{
		ms_AlphaCmpMethod = D3DCMP_LESSEQUAL;
		ms_AlphaRef1 = 0xFf;
		ms_AlphaRef2 = 0xEf;
		ms_RenderText = RenderTextCmp;
	}
	else if (Caps.AlphaCmpCaps & D3DPCMPCAPS_LESS)
	{
		ms_AlphaCmpMethod = D3DCMP_LESS;
		ms_AlphaRef1 = 0x100;
		ms_AlphaRef2 = 0xF0;
		ms_RenderText = RenderTextCmp;
	}
	else if (Caps.AlphaCmpCaps & D3DPCMPCAPS_EQUAL)
	{
		ms_AlphaCmpMethod = D3DCMP_NOTEQUAL;
		ms_AlphaRef1 = 0xFf;
		ms_AlphaRef2 = 0xEe;
		ms_RenderText = RenderTextCmp;
	}
	else if (Caps.AlphaCmpCaps & D3DPCMPCAPS_NOTEQUAL)
	{
		ms_AlphaCmpMethod = D3DCMP_NOTEQUAL;
		ms_AlphaRef1 = 0xEe;
		ms_AlphaRef2 = 0xFf;
		ms_RenderText = RenderTextCmp;
	}	
	else
	{
		ms_RenderText = RenderTextDirect;
		KFontRes::EnableTextBorder(false);
	}
}

/*!*****************************************************************************
// Purpose		:  ���캯��
*****************************************************************************/
KFont3::KFont3()
{
	m_pTexture		= NULL;
	m_fFontWidth	= 14.0;
	m_fFontHeight	= 14.0;
	m_nOutputWidth  = 14;
	m_nOutputHeight = 14;
	m_nFontHalfWidth[0] = m_nFontHalfWidth[1] = 8;
	m_pStateBlockSaved     = NULL;
    m_pStateBlockDrawText  = NULL;
	m_nLastPosH		= 0;
	m_nLastPosV		= 0;
	m_bLoaded		= false;
	m_nRefCount		= 1;
}

/*!*****************************************************************************
// Purpose		:  ��������
*****************************************************************************/
KFont3::~KFont3()
{
	Terminate();
}

/*!*****************************************************************************
// Function		: KFont3::Init
// Purpose		: ��ʼ��
// Return		: bool �Ƿ�ɹ�
// Argumant		: void* pDrawDevice -> directx 3d device�ӿڵ�ʵ����ָ��
*****************************************************************************/
bool KFont3::Init(void* pDrawDevice)
{
	Terminate();

	if (pDrawDevice == NULL)
		return false;

	if (ms_pd3dDevice == NULL)
	{
		ms_AlphaCmpMethod = ALP_CMP_CAP_HANT_BEEN_CHECKED;
		ms_pd3dDevice = (LPDIRECT3DDEVICE9)pDrawDevice;
		CheckAlphaCmpCaps();
	}	

	return RestoreDeviceObjects();
}

/*!*****************************************************************************
// Function		: KFont3::Init
// Purpose		: �ͷŽӿ�ʵ������
*****************************************************************************/
void KFont3::Release()
{
	if ((--m_nRefCount) == 0)
		delete (this);
}

/*!*****************************************************************************
// Function		: KFont3::Load
// Purpose		: �����ֿ�
// Return		: bool �Ƿ�ɹ�
// Argumant		: cosnt char *pszFontFile �ֿ��ļ���
*****************************************************************************/
bool KFont3::Load(const char* pszFontFile)
{
	m_Resources.Terminate();
	m_pTexture = NULL;
	m_bLoaded = false;
	if (ms_pd3dDevice)
	{
	//��ʼ�������ֿ���Դ
		if (m_Resources.Init(pszFontFile, ms_pd3dDevice))
		{
			int		w, h;
			m_Resources.GetInfo(w, h);
			m_fFontWidth  = (float)w;
			m_fFontHeight = (float)h;
			m_pTexture = m_Resources.GetTexture();
			m_bLoaded = true;
		}
	}
	return m_bLoaded;
}

/*!*****************************************************************************
// Function		: KFont3::Terminate
// Purpose		: �������������
*****************************************************************************/
void KFont3::Terminate()
{
	if (ms_pd3dDevice)
    {
        if(m_pStateBlockSaved)
		{
			m_pStateBlockSaved->Release();
			m_pStateBlockSaved = NULL;
		}
		if(m_pStateBlockDrawText)
		{
			m_pStateBlockDrawText->Release();
			m_pStateBlockDrawText = NULL;
		}
		ms_pd3dDevice = NULL;
    }

	m_bLoaded = false;
	m_pTexture = NULL;
	m_Resources.Terminate();
}

/*!*****************************************************************************
// Function		: KFont3::TextOut
// Purpose		: ��ʾ�ַ���
// Argumant		: cosnt char *pszText �ַ���
// Argumant		: int nCount  �ַ����ĳ���(BYTE)��Ĭ��ֵΪ-1����ʾ���ַ�������'\0'��β��
// Argumant		: int nX	  �ַ�����ʾ�������X���������ֵΪKF_FOLLOW��
//							  ����ַ����������ϴ��ַ��������λ��֮��
// Argumant		: int nY      �ַ�����ʾ�������Y, �������ֵΪKF_FOLLOW��
//							  ����ַ�����ǰһ������ַ�����ͬһ�е�λ�á�
// Argumant		: uint nColor �ַ�����ʾ��ɫ��Ĭ��Ϊ��ɫ����32bit����ARGB�ĸ�
//							  ʽ��ʾ��ɫ��ÿ������8bit��
// Argumant     : nLineWidth  �Զ����е��п����ƣ������ֵС��һ��ȫ���ַ����
//							  �����Զ����д���Ĭ��ֵΪ0���Ȳ����Զ����д���
*****************************************************************************/
void KFont3::OutputText(const char *pszText, int nCount/*= KF_ZERO_END*/,
					   int nX/*= KF_FOLLOW*/, int nY/*= KF_FOLLOW*/,
					unsigned int nColor /*=0xff000000*/, int nLineWidth/*= 0*/)
{
	if (pszText == NULL || m_bLoaded == false || ms_pd3dDevice == NULL)
		return;

	unsigned int	nNumChars = 0;
	unsigned char*	lpByte = (unsigned char*) pszText;
	int				nL, h;
	BOOL			bShowChar = false;
	int				nPos = 0;
	int				nDx = 0;
	int				nHalfIndex = 0;

	//Ҫ�ύ���ַ���
	unsigned short	nCommitChars[KCS_CHAR_NUM_LIMIT];
	//���ڻ����ַ��Ķ����
	KFontVertex	vCharPolys[KCS_CHAR_NUM_LIMIT * 6];

	if (nCount == KRF_ZERO_END)
		nCount = strlen(pszText);
	if (nX == KRF_FOLLOW)
		nX = m_nLastPosH;
	if (nY == KRF_FOLLOW)
		nY = m_nLastPosV;

	h = 0;
	if (nLineWidth < m_fFontWidth + m_fFontWidth)
		nLineWidth = 0; //�����Զ����д���

	//����ǰ����Ⱦ״̬�ı�
	{
		m_pStateBlockSaved->Capture();
		m_pStateBlockDrawText->Apply();
		ms_pd3dDevice->SetFVF(KF_FONTPOLY);
		ms_pd3dDevice->SetTexture(0, m_pTexture);
	}

	while (nPos < nCount)
	{
		bShowChar = false;

		//*********�ַ����ж��봦��*********
		if (lpByte[nPos] > 0x80 && nPos + 1 < nCount)
		{
			nCommitChars[nNumChars] = *(unsigned short*)(lpByte + nPos);
			nPos += 2;
			bShowChar = true;
			nDx = m_nOutputWidth;
		}
		else	//�����ַ�
		{
			nL = lpByte[nPos++];
			if (nL > 0x20 && nL < 0x5F)
			{
				nCommitChars[nNumChars] = 0xa3 + ((0x20 + nL) << 8);
				bShowChar = true;
			}
			else if (nL >= 0x5F && nL < 0x7F)
			{
				nCommitChars[nNumChars] = 0xa3 + ((0x21 + nL) << 8);
				bShowChar = true;
			}
			if (nL != 0x0a)
			{
				nDx = m_nFontHalfWidth[nHalfIndex];
				nHalfIndex ^= 1;
			}
			else
			{
				nDx = -1;
			}
		}
		//����Ҫ��ʾ���ַ�������һ���ı��Σ����ں�����ƴ��ַ���
		if (bShowChar)
		{
			int n = nNumChars * 6;
            KFontVertex *pCharPoly = &vCharPolys[n];

			//���û����ַ��������ε���Ϣ
			pCharPoly[0].x = ((float)(nX + h)) - 0.5f;
			pCharPoly[0].y = ((float)nY) - 0.5f;
			
			pCharPoly[4].x = pCharPoly[1].x = pCharPoly[0].x + m_fFontWidth;
			pCharPoly[4].y = pCharPoly[1].y = pCharPoly[0].y;
		
			pCharPoly[3].x = pCharPoly[2].x = pCharPoly[0].x;
			pCharPoly[3].y = pCharPoly[2].y = pCharPoly[0].y + m_fFontHeight;

			pCharPoly[5].x = pCharPoly[4].x;
			pCharPoly[5].y = pCharPoly[2].y;
			
			pCharPoly[0].z = 0.9f;  pCharPoly[0].w = 1.0f;  pCharPoly[0].color = nColor;
			pCharPoly[1].z = 0.9f;  pCharPoly[1].w = 1.0f;  pCharPoly[1].color = nColor;
			pCharPoly[2].z = 0.9f;  pCharPoly[2].w = 1.0f;  pCharPoly[2].color = nColor;
			pCharPoly[3].z = 0.9f;  pCharPoly[3].w = 1.0f;  pCharPoly[3].color = nColor;
			pCharPoly[4].z = 0.9f;  pCharPoly[4].w = 1.0f;  pCharPoly[4].color = nColor;
			pCharPoly[5].z = 0.9f;  pCharPoly[5].w = 1.0f;  pCharPoly[5].color = nColor;

			nNumChars++;

			if (nNumChars >= KCS_CHAR_NUM_LIMIT)
			{
				//���ֿ���Դ�ύ�ַ���
				m_Resources.CommitText(nCommitChars, nNumChars, vCharPolys);
				//�����ַ���
				ms_RenderText(vCharPolys, nNumChars * 2);
				nNumChars = 0;
			}
		}

		h += nDx;

		if (nDx == -1 || (nLineWidth && h + m_nOutputWidth > nLineWidth))
		{
			if (nPos < nCount && lpByte[nPos] == 0x0a && nDx != -1)	//�����Զ�����λ�ú�ǡ�ý���һ�����з���
				nPos++;
			h = 0;
			nY += m_nOutputHeight;
			nHalfIndex = 0;
		}
		else if (nDx > 0 && nLineWidth && h + nDx + m_nOutputWidth + m_nFontHalfWidth[nHalfIndex] >= nLineWidth)
		{
			const char* pNext = TGetSecondVisibleCharacterThisLine((const char*)lpByte, nPos, nCount);
			if (pNext && TIsCharacterNotAlowAtLineHead(pNext))
			{
				h = 0;
				nY += m_nOutputHeight;
				nHalfIndex = 0;
			}
		}
	}

	if (nNumChars)
	{
		//���ֿ���Դ�ύ�ַ���
		m_Resources.CommitText(nCommitChars, nNumChars, vCharPolys);
		//�����ַ���
		ms_RenderText(vCharPolys, nNumChars * 2);
		nNumChars = 0;
	}

	//���ƺ����Ⱦ״̬��ԭ
	{
		m_pStateBlockSaved->Apply();
	}
	m_nLastPosH = nX + h;
	m_nLastPosV = nY;
}

/*!*****************************************************************************
// Function		: KFont3::RenderTextDirect
// Purpose		: �����ַ���
// Argumant		: KFontVertex* pPolyVertices  ���ڻ����ַ��Ķ���ζ�������
// Argumant		: unsigned int nNumPolys	  ���ڻ����ַ��Ķ������Ŀ
*****************************************************************************/
void KFont3::RenderTextDirect(KFontVertex* pPolyVertices, unsigned int nNumPolys)
{
	//_ASSERT(ms_pd3dDevice && pPolyVertices && nNumPolys);
	//�����ַ������
	ms_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, nNumPolys,
		pPolyVertices, sizeof(KFontVertex));
}

/*!*****************************************************************************
// Purpose		: �����ַ���
*****************************************************************************/
void KFont3::RenderTextCmp(KFontVertex* pPolyVertices, unsigned int nNumPolys)
{
	//_ASSERT(ms_pd3dDevice && pPolyVertices && nNumPolys);
	ms_pd3dDevice->SetRenderState(D3DRS_ALPHAREF, ms_AlphaRef1);
	ms_pd3dDevice->SetRenderState(D3DRS_ALPHAFUNC,  ms_AlphaCmpMethod);
	ms_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, nNumPolys,
		pPolyVertices, sizeof(KFontVertex));
	ms_pd3dDevice->SetRenderState(D3DRS_ALPHAREF, ms_AlphaRef2);
	ms_pd3dDevice->SetRenderState(D3DRS_ALPHAFUNC,  ms_AlphaCmpMethod);
	unsigned int nNumVertex = nNumPolys * 3;
	for (unsigned int i = 0; i < nNumVertex; i++)
		pPolyVertices[i].color = ms_uBorderColor;
	ms_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, nNumPolys,
		pPolyVertices, sizeof(KFontVertex));
}

/*!*****************************************************************************
// Purpose		: �����ַ���
*****************************************************************************/
void KFont3::RenderTextCmpReverse(KFontVertex* pPolyVertices, unsigned int nNumPolys)
{
	//_ASSERT(ms_pd3dDevice && pPolyVertices && nNumPolys);
	unsigned int i;
	unsigned int nNumVertex = nNumPolys * 3;
	ms_pd3dDevice->SetRenderState(D3DRS_ALPHAREF, ms_AlphaRef1);
	ms_pd3dDevice->SetRenderState(D3DRS_ALPHAFUNC,  ms_AlphaCmpMethod);
	unsigned int uColor = pPolyVertices[0].color;
	for (i = 0; i < nNumVertex; i++)
		pPolyVertices[i].color = ms_uBorderColor;
	ms_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, nNumPolys,
		pPolyVertices, sizeof(KFontVertex));
	ms_pd3dDevice->SetRenderState(D3DRS_ALPHAREF, ms_AlphaRef2);
	ms_pd3dDevice->SetRenderState(D3DRS_ALPHAFUNC,  ms_AlphaCmpMethod);
	for (i = 0; i < nNumVertex; i++)
		pPolyVertices[i].color = uColor;
	ms_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, nNumPolys,
		pPolyVertices, sizeof(KFontVertex));
}

/*!*****************************************************************************
// Function		: KFont3::RestoreDeviceObjects
// Purpose		: ʹDevice״̬�л�����Ӱ��Ļ�ͼ�豸�������ʧЧ
*****************************************************************************/
void KFont3::InvalidateDeviceObjects()
{
	if(ms_pd3dDevice)
	{
		if(m_pStateBlockSaved)
		{
			m_pStateBlockSaved->Release();
			m_pStateBlockSaved = NULL;
		}
		if(m_pStateBlockDrawText)
		{
			m_pStateBlockDrawText->Release();
			m_pStateBlockDrawText = NULL;
		}
    }
	if(m_pStateBlockSaved)
	{
		m_pStateBlockSaved->Release();
		m_pStateBlockSaved = NULL;
	}
	if(m_pStateBlockDrawText)
	{
		m_pStateBlockDrawText->Release();
		m_pStateBlockDrawText = NULL;
	}
}

/*!*****************************************************************************
// Function		: KFont3::RestoreDeviceObjects
// Purpose		: ��D3d Device��lost state�ָ�Ϊoperational state��KFont3��
//					d3d Device������ݵĻָ�������
*****************************************************************************/
bool KFont3::RestoreDeviceObjects()
{
	if (ms_pd3dDevice == NULL)
		return false;
	// Create the state blocks for rendering text
	for (int which = 0; which < 2; which++)
	{
		ms_pd3dDevice->BeginStateBlock();
		ms_pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
		ms_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		ms_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,   D3DBLEND_SRCALPHA );
		ms_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,  D3DBLEND_INVSRCALPHA );
		ms_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE);
		ms_pd3dDevice->SetRenderState( D3DRS_ALPHAREF, 0x08);

		ms_pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC,  D3DCMP_GREATEREQUAL );
		ms_pd3dDevice->SetRenderState( D3DRS_FILLMODE,   D3DFILL_SOLID );
		ms_pd3dDevice->SetRenderState( D3DRS_CULLMODE,   D3DCULL_CCW );
		ms_pd3dDevice->SetRenderState( D3DRS_STENCILENABLE,    FALSE );
		ms_pd3dDevice->SetRenderState( D3DRS_CLIPPING,         TRUE );
		ms_pd3dDevice->SetRenderState( D3DRS_CLIPPLANEENABLE,  FALSE );
		ms_pd3dDevice->SetRenderState( D3DRS_VERTEXBLEND,      FALSE );
		ms_pd3dDevice->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE );
		ms_pd3dDevice->SetRenderState( D3DRS_FOGENABLE,        FALSE );
		ms_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		ms_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		ms_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		ms_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
		ms_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		ms_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		ms_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
		ms_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
		ms_pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
		ms_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
		ms_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
		ms_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
		ms_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

        if( which==0 )
            ms_pd3dDevice->EndStateBlock(&m_pStateBlockSaved);
        else
            ms_pd3dDevice->EndStateBlock(&m_pStateBlockDrawText);
    }
	return true;
}

void KFont3::GetFontSize(int* pWidth, int* pHeight)
{
	if (pWidth)
		*pWidth = (int)(m_fFontWidth);
	if (pHeight)
		*pHeight = (int)(m_fFontHeight);
}

//���û���ʱ�ַ���Ե����ɫ����alphaΪ0��ʾ�ַ���Ե����������
void KFont3::SetBorderColor(unsigned int uColor)
{
	ms_uBorderColor = uColor;
}

//�����ַ���������
void KFont3::SetOutputSize(int nOutputWith, int nOutputHeight)
{
	if (nOutputWith > 0)
		m_nOutputWidth = nOutputWith;
	else
		m_nOutputWidth = (int)(m_fFontWidth);
	if (nOutputHeight > 0)
		m_nOutputHeight = nOutputHeight;
	else
		m_nOutputHeight = (int)(m_fFontHeight);
	m_nFontHalfWidth[0] = m_nOutputWidth / 2;
	m_nFontHalfWidth[1] = (m_nOutputWidth + 1) / 2;
}

//��¡һ���ӿڶ���ָ��
iFont* KFont3::Clone()
{
	if (m_nRefCount < 0xffff)
	{
		m_nRefCount ++;
		return ((iFont*)this);
	}
	return NULL;
}
