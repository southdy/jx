/*******************************************************************************
// FileName			:	KFontRes.cpp
// FileAuthor		:	Wooy
// FileCreateDate	:	2001-9-12 9:57:55
// FileDescription	:	����ͼ����Դ��
// Revision Count	:	2002-9-4��д����Ϊ����d3d��Wooy
*******************************************************************************/
#include "KFontRes.h"

#define	DESIRE_TEXTURE_SIDE_WIDTH	512

int KFontRes::m_bEnableTextBorder = true;

//����/�������ֱ�ԵЧ��
void KFontRes::EnableTextBorder(bool bEnable)
{
	m_bEnableTextBorder = (bEnable != false);
}

/*!*****************************************************************************
// Purpose		:  ���캯��
*****************************************************************************/
KFontRes::KFontRes()
{
	m_nFontW = 0;
	m_nFontH = 0;
	m_nNumCountH = 0;
	m_nMaxCount = 0;
	m_pd3dDevice   = NULL;
	m_pCharTexture = NULL;
	m_nTextureSideWidth = 512;
}

/*!*****************************************************************************
// Purpose		:  ��������
*****************************************************************************/
KFontRes::~KFontRes()
{
	Terminate();
}

/*!*****************************************************************************
// Function		: KFontRes::Init
// Purpose		: ��ʼ��
// Return		: �Ƿ�ɹ�
// Argumant		: const char* pszFontFile      -> �ֿ��ļ���
// Argumant		: LPDIRECT3DDEVICE8 pd3dDevice -> directx 3d device�ӿڵ�ʵ����ָ��
*****************************************************************************/
bool KFontRes::Init(const char* pszFontFile, LPDIRECT3DDEVICE9 pd3dDevice)
{
	Terminate();
	if ((m_pd3dDevice = pd3dDevice) == NULL)
		return false;
	if (pszFontFile == NULL)
		return false;

	//�����ֿ��ļ�
	if (!m_FontData.Load(pszFontFile))
	{
		Terminate();
		return false;
	}

	m_FontData.GetInfo(m_nFontW, m_nFontH);

	HRESULT	hr;
    D3DCAPS9 d3dCaps;
    m_pd3dDevice->GetDeviceCaps(&d3dCaps);
	
    if(d3dCaps.MaxTextureWidth >= DESIRE_TEXTURE_SIDE_WIDTH)
		m_nTextureSideWidth = DESIRE_TEXTURE_SIDE_WIDTH;
	else
		m_nTextureSideWidth = d3dCaps.MaxTextureWidth;

    // Create a new texture for the font
    hr = m_pd3dDevice->CreateTexture( m_nTextureSideWidth, m_nTextureSideWidth, 1,
                                      0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, &m_pCharTexture, NULL);
    if (FAILED(hr))
	{
		Terminate();
		return false;
	}

	//����ͼ��ɾ�
/*	{
		//ȡ����ͼ���ݿ��ָ��
	    D3DLOCKED_RECT d3dlr;
		if (m_pCharTexture->LockRect( 0, &d3dlr, 0, 0 ) != D3D_OK)
		{
			Terminate();
			return false;
		}
		for(unsigned int v = 0; v < m_nTextureSideWidth; v++)
		{
			memset(d3dlr.pBits, 0, m_nTextureSideWidth * 4);
			d3dlr.pBits = (BYTE*)d3dlr.pBits + d3dlr.Pitch;
		}
		m_pCharTexture->UnlockRect(0);
	}*/

	//�ַ����Լ��ַ��߼�һ����Ϊ�ڰ��ַ�����ͼ�δ�����ͼʱ�����ַ���һ�����صļ��
	m_nNumCountH = m_nTextureSideWidth / (m_nFontW + 1);
	m_nMaxCount = (m_nTextureSideWidth / (m_nFontH + 1)) * m_nNumCountH;
	return true;
}

/*!*****************************************************************************
// Function		: KFontRes::GetInfo
// Purpose		: �õ�����Ŀ�ȸ߶ȣ���ͼ���Ի�����ַ��ĸ���
// Argumant		: int &nWidth	�ַ���
// Argumant		: int &nHeight	�ַ���
// Argumant		: int &nHCount	��ͼ��һ�д洢�ַ��ĸ���
// Argumant		: int &nCount	�����ַ��ĸ���
*****************************************************************************/
void KFontRes::GetInfo(int &nWidth, int &nHeight, int& nHCount, int &nCount) const
{
	nWidth  = m_nFontW;
	nHeight = m_nFontH;
	nHCount = m_nNumCountH;
	nCount = m_nMaxCount;
}

/*!*****************************************************************************
// Function		: KFontRes::GetInfo
// Purpose		: �õ��ַ�����ͼ��Ŀ�ȸ߶ȣ�ˮƽ�������ͼ���Ի�����ַ��ĸ���
// Argumant		: float& fWidth		�ַ�����ͼ���ˮƽ���
// Argumant		: float& fHeight	�ַ�����ͼ���ˮƽ�߶�
// Argumant		: float& fHInterval	�ַ�����ͼ���ˮƽ���
// Argumant		: float& fVInterval	�ַ�����ͼ��Ĵ�ֱ���
// Argumant		: int& nCount		�����ַ��ĸ���
*****************************************************************************/
void KFontRes::GetInfo(float& fWidth, float& fHeight, float& fHInterval, float& fVInterval) const
{
	fWidth  = ((float)m_nFontW) / (float)m_nTextureSideWidth;
	fHeight = ((float)m_nFontH) / (float)m_nTextureSideWidth;
	fHInterval = ((float)(m_nFontW + 1)) / (float)m_nTextureSideWidth;
	fVInterval = ((float)(m_nFontH + 1)) / (float)m_nTextureSideWidth;
}

/*!*****************************************************************************
// Function		: KFontRes::Terminate
// Purpose		: �������������
*****************************************************************************/
void KFontRes::Terminate()
{
	//�ͷ���ͼ
	if (m_pCharTexture)
	{
		m_pCharTexture->Release();
		m_pCharTexture = NULL;
	}
	m_pd3dDevice = NULL;
	m_FontData.Terminate();
	m_nNumCountH = 0;
	m_nMaxCount = 0;
}

/*!***************************************************************************
// Function		: KFontRes::Update
// Purpose		: ���»����ַ�
// Argumant		: unsigned short * pUpdates	�����ַ����뼰����ͼ��λ������
						����ĽṹΪ��Ӧ��ÿ�������ַ�������uint��ֵ��
						ǰһ����ʾ�ַ�����ͼ�е�λ�ã���һ��Ϊ�ַ��ı���
// Argumant		: int nCount	�����ַ��ĸ���
*****************************************************************************/
void KFontRes::Update(unsigned short * pUpdates, int nCount)
{
	int				i;
	int				nTexIndex;	//�ַ�����ͼ�������
	int				nX, nY;		//�ַ����Ͻ�����ͼ������

	if (pUpdates == NULL || nCount <= 0)
		return;

	//ȡ����ͼ���ݿ��ָ��
	D3DLOCKED_RECT d3dlr;
	if (m_pCharTexture->LockRect( 0, &d3dlr, 0, 0 ) != D3D_OK)
		return;
	//�������Ҫ���µ��ַ�
	for (i = 0; i < nCount; i++)
	{
		//ȡ���ַ�����ͼ���λ�ã��������
		nTexIndex = *pUpdates;
		if (nTexIndex >= m_nMaxCount)
		{
			pUpdates += 2;
			continue;
		}

		//ȡ���ַ����ֿ����������ָ��
		unsigned char* pCharacterData = m_FontData.GetCharacterData(
			*(((unsigned char*)pUpdates) + 2), *(((unsigned char*)pUpdates) + 3));
		pUpdates += 2;
		if (pCharacterData == NULL)
			continue;		
		
		//�����ַ����Ͻ�����ͼ������
		nX = (m_nFontW + 1) * (nTexIndex % m_nNumCountH);
		nY = (m_nFontH + 1) * (nTexIndex / m_nNumCountH);
		//������ͼ
		Update(pCharacterData,
			((BYTE*)d3dlr.pBits + d3dlr.Pitch * nY + nX * 2), d3dlr.Pitch);
	}
	m_pCharTexture->UnlockRect(0);
}

/*!*****************************************************************************
// Function		: KFontRes::Update
// Purpose		: ���µ����ַ��ĵ�������
// Argumant		: unsigned char* pCharData	ͼ�����ݿ����
// Argumant		: unsigned char* pTexData	��ͼ���ݿ����
// Argumant		: int nTexPitch		��ͼ���ݿ�pitch
*****************************************************************************/
void KFontRes::Update(unsigned char* pCharImage, unsigned char* pTexData, int nTexPitch)
{
	int		TextureOffset;		//��ͼ���ݻ��е�ƫ��
	int		nWidth, nHeight;

	nWidth = m_nFontW;
	nHeight = m_nFontH;

	//������ͼ���ݻ��е�ƫ��
	TextureOffset = nTexPitch - m_nFontW * 2;
	int bEnableTextBorder = m_bEnableTextBorder;

	__asm
	{
		//��ʼ�� EDI ָ����ͼ�������
		mov 	edi, pTexData
		//��ʼ�� ESI ָ��ͼ���������
		mov		esi, pCharImage
		//��
		mov		ebx, 0xffff

		mov		eax, bEnableTextBorder
		cmp		eax, 0
		je		without_border_start_line

		with_border_start_line:
		{
			//��
			mov		edx, nWidth

		with_border_same_alpha_block:
			xor		eax, eax
			mov     al, [esi]
			inc     esi
			mov		ecx, eax
			and		ecx, 0x1f			//eaxΪ������ͬalphaֵ�ĵ�ĸ���
			sub		edx, ecx
		
			shr		eax, 5				//eax��3bitΪalphaֵ

			cmp		eax, 0
			je		with_border_store_pixel

			cmp		eax, 7
			je		with_border_alpha_equal_7

			mov		eax, 0xefff
			jmp		with_border_store_pixel
		
		with_border_alpha_equal_7:
			mov		eax, ebx

		with_border_store_pixel:
			stosw
		    loop    with_border_store_pixel

			or		edx, edx
			jg		with_border_same_alpha_block
		
			add		edi, TextureOffset
			dec		nHeight
			jg		with_border_start_line
		}

		without_border_start_line:
		{
			//��
			mov		edx, nWidth

		without_border_same_alpha_block:
			xor		eax, eax
			mov     al, [esi]
			inc     esi
			mov		ecx, eax
			and		ecx, 0x1f			//eaxΪ������ͬalphaֵ�ĵ�ĸ���
			sub		edx, ecx
		
			shr		eax, 5				//eax��3bitΪalphaֵ

			cmp		eax, 7
			je		without_border_alpha_equal_7

			xor		eax, eax
			jmp		without_border_store_pixel
		
		without_border_alpha_equal_7:
			mov		eax, ebx

		without_border_store_pixel:
			stosw
		    loop    without_border_store_pixel

			or		edx, edx
			jg		without_border_same_alpha_block
		
			add		edi, TextureOffset
			dec		nHeight
			jg		without_border_start_line
		}
	}
}