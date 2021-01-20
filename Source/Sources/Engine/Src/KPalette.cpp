//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KPalette.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	256 Color bitmap file class
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KColors.h"
#include "KPalette.h"
#include "KDDraw.h"
//---------------------------------------------------------------------------
static KPAL24 m_ColorStyle = {255, 255, 255};
//---------------------------------------------------------------------------
// ����:	SetColorStyle
// ����:	������ɫ���
// ����:	Red			��ɫ 0-255
//			Green		��ɫ 0-255
//			Blue		��ɫ 0-255
// ����:	void
//---------------------------------------------------------------------------
void g_SetColorStyle(BYTE Red, BYTE Green, BYTE Blue)
{
	m_ColorStyle.Red   = Red;
	m_ColorStyle.Green = Green;
	m_ColorStyle.Blue  = Blue;
}
//---------------------------------------------------------------------------
// ����:	Pal24ToPal16
// ����:	24λ��ɫ��ת��λ16λ��ɫ��
// ����:	pPal24		KPAL24ָ��
//			pPal16		KPAL16ָ��
//			nClolors	��ɫ��(0 - 256)
// ����:	void
//---------------------------------------------------------------------------
void g_Pal24ToPal16(KPAL24* pPal24, KPAL16* pPal16, int nColors)
{
	if (pPal24 && pPal16 && nColors > 0)
	{
		//	_ASSERT(pPal24 && pPal16 && g_pDirectDraw && count > 0);
		if (g_pDirectDraw->GetRGBBitMask16() == RGB_565)
		{
			_asm
			{
				mov		ecx, nColors
				mov		esi, pPal24
				mov		edi, pPal16
Start_Convert_565:
				{
					xor		ebx, ebx		//ebx��0
					mov		dx, [esi + 1]	//����GB
					mov		al, [esi]		//����R
					mov		bl, dl			//��G�ƶ���[bl]
					shr		eax, 3
					shr		ebx, 2
					shl		eax, 11			//Ŀ��r������
					shl		ebx, 5
					add		esi, 3
					add		eax, ebx		//Ŀ��RG���ϳɵ�ax��
					xor		ebx, ebx		//��ebx��0
					mov		bl, dh			//��B�ƶ�[bl]
					shr		ebx, 3
					add		eax, ebx		//��Ŀ��
					mov		[edi], ax
					add		edi, 2
					dec		ecx				//����count����
					jg		Start_Convert_565
				}
			}
		}
		else
		{
			_asm
			{
				mov		ecx, nColors
				mov		esi, pPal24
				mov		edi, pPal16
Start_Convert_555:
				{
					//ax���ڱ���Ŀ�����������15bit�Խ����Ӱ��
					xor		ebx, ebx		//ebx��0
					mov		dx, [esi + 1]	//����GB
					mov		al, [esi]		//����R
					mov		bl, dl			//��G�ƶ���[bl]
					shr		eax, 3
					shr		ebx, 3
					shl		eax, 10			//Ŀ��r������
					shl		ebx, 5
					add		esi, 3
					add		eax, ebx		//Ŀ��RG���ϳɵ�ax��
					xor		ebx, ebx		//��ebx��0
					mov		bl, dh			//��B�ƶ�[bl]
					shr		ebx, 3
					add		eax, ebx		//��Ŀ��
					mov		[edi], ax
					add		edi, 2
					dec		ecx				//����count����
					jg		Start_Convert_555
				}
			}
		}
	}
}

//---------------------------------------------------------------------------
// ����:	16λ��ɫ��ת��λ16λ��ɫ��
//---------------------------------------------------------------------------
void g_Pal16ToPal24(KPAL16* pPal16, KPAL24* pPal24, int nColors)
{
	if (pPal24 && pPal16 && nColors > 0)
	{
		//	_ASSERT(pPal24 && pPal16 && g_pDirectDraw && count > 0);
		if (g_pDirectDraw->GetRGBBitMask16() == RGB_565)
		{
			_asm
			{
				mov		ecx, nColors
				mov		esi, pPal16
				mov		edi, pPal24
Start_Convert_565:
				{
					mov		ax, [esi]		//����16bitɫ
					add		esi, 2
					mov		bx, ax			//����16bitɫ
					and		ax, 0xF814		//�õ�r0b
					add		edi, 3
					mov		dl, al			//dl = b
					sub		bx, ax			//bx = 0g0
					shl		dl, 3			//dl = B
					shr		bx, 3			//bl = G
					mov		[edi - 1], dl
					mov		[edi - 2], bl
					mov		[edi - 3], ah
					dec		ecx				//����count����
					jg		Start_Convert_565
				}
			}
		}
		else
		{
			_asm
			{
				mov		ecx, nColors
				mov		esi, pPal16
				mov		edi, pPal24
Start_Convert_555:
				{
					mov		ax, [esi]		//����16bitɫ
					add		esi, 2
					mov		bx, ax			//����16bitɫ
					and		ax, 0x7C14		//�õ�r0b
					add		edi, 3
					mov		dl, al			//dl = b
					sub		bx, ax			//bx = 0g0
					shl		dx, 11			//dx = B0
					shr		bx, 2			//bl = G
					add		ax, ax
					mov		dl, bl			//dx = BG
					mov		[edi - 3], ah
					mov		[edi - 2], dx
					dec		ecx				//����count����
					jg		Start_Convert_555
				}
			}
		}
	}
}

//---------------------------------------------------------------------------
// ����:	Pal32ToPal16
// ����:	32λ��ɫ��ת��λ16λ��ɫ��
// ����:	pPal32		KPAL32ָ��
//			pPal16		KPAL16ָ��
//			nClolors	��ɫ��(0 - 256)
// ����:	void
//---------------------------------------------------------------------------
void g_Pal32ToPal16(KPAL32* pPal32, KPAL16* pPal16, int nColors)
{
	while (nColors-- > 0)
	{
		*pPal16 = g_RGB(pPal32->Red, pPal32->Green, pPal32->Blue);
		pPal16++;
		pPal32++;
	}
}
//---------------------------------------------------------------------------
// ����:	Pal24ToPal16Style
// ����:	24λ��ɫ��ת��λ16λ��ɫ��
// ����:	pPal24		KPAL24ָ��
//			pPal16		KPAL16ָ��
//			nClolors	��ɫ��(0 - 256)
// ����:	void
//---------------------------------------------------------------------------
void g_Pal24ToPal16Style(KPAL24* pPal24, KPAL16* pPal16, int nColors)
{
	while (nColors-- > 0)
	{
		*pPal16 = g_RGB(
			pPal24->Red * m_ColorStyle.Red / 255,
			pPal24->Green * m_ColorStyle.Green / 255,
			pPal24->Blue * m_ColorStyle.Blue / 255);
		pPal16++;
		pPal24++;
	}
}
//---------------------------------------------------------------------------
// ����:	Pal32ToPal16Style
// ����:	32λ��ɫ��ת��λ16λ��ɫ��
// ����:	pPal32		KPAL32ָ��
//			pPal16		KPAL16ָ��
//			nClolors	��ɫ��(0 - 256)
// ����:	void
//---------------------------------------------------------------------------
void g_Pal32ToPal16Style(KPAL32* pPal32, KPAL16* pPal16, int nColors)
{
	while (nColors-- > 0)
	{
		*pPal16 = g_RGB(
			pPal32->Red * m_ColorStyle.Red / 255,
			pPal32->Green * m_ColorStyle.Green / 255,
			pPal32->Blue * m_ColorStyle.Blue / 255);
		pPal16++;
		pPal32++;
	}
}
//---------------------------------------------------------------------------
// ����:	Pal24ToPal16Gray
// ����:	24λ��ɫ��ת��λ16λ��ɫ��(�Ҷ�)
// ����:	pPal24		KPAL24ָ��
//			pPal16		KPAL16ָ��
//			nClolors	��ɫ��(0 - 256)
// ����:	void
//---------------------------------------------------------------------------
void g_Pal24ToPal16Gray(KPAL24* pPal24, KPAL16* pPal16, int nColors)
{
	int nGray;
	while (nColors-- > 0)
	{
		nGray = (pPal24->Red * 30 + pPal24->Green * 59 + pPal24->Blue * 11) / 100;
		*pPal16 = g_RGB(nGray, nGray, nGray);
		pPal16++;
		pPal24++;
	}
}
//---------------------------------------------------------------------------
// ����:	Pal32ToPal16Gray
// ����:	32λ��ɫ��ת��λ16λ��ɫ��(�Ҷ�)
// ����:	pPal32		KPAL32ָ��
//			pPal16		KPAL16ָ��
//			nClolors	��ɫ��(0 - 256)
// ����:	void
//---------------------------------------------------------------------------
void g_Pal32ToPal16Gray(KPAL32* pPal32, KPAL16* pPal16, int nColors)
{
	int nGray;
	while (nColors-- > 0)
	{
		nGray = (pPal32->Red * 30 + pPal32->Green * 59 + pPal32->Blue * 11) / 100;
		*pPal16 = g_RGB(nGray, nGray, nGray);
		pPal16++;
		pPal32++;
	}
}
//---------------------------------------------------------------------------
// ����:	Pal24ToPal16Lum
// ����:	24λ��ɫ��ת��λ16λ��ɫ��
// ����:	pPal24		KPAL24ָ��
//			pPal16		KPAL16ָ��
//			nClolors	��ɫ��(0 - 256)
// ����:	void
//---------------------------------------------------------------------------
void g_Pal24ToPal16Lum(KPAL24* pPal24, KPAL16* pPal16, int nColors)
{
	int lum, i;
	for (lum = 32; lum > 0; lum--)
	{
		for (i = 0; i < nColors; i++)
		{
			pPal16[i] = g_RGB(
				pPal24[i].Red * lum / 32,
				pPal24[i].Green * lum / 32,
				pPal24[i].Blue * lum / 32);
		}
		pPal16 += nColors * sizeof(KPAL16);
	}
}
//---------------------------------------------------------------------------
// ����:	Pal32ToPal16Lum
// ����:	32λ��ɫ��ת��λ16λ��ɫ��
// ����:	pPal32		KPAL32ָ��
//			pPal16		KPAL16ָ��
//			nClolors	��ɫ��(0 - 256)
// ����:	void
//---------------------------------------------------------------------------
void g_Pal32ToPal16Lum(KPAL32* pPal32, KPAL16* pPal16, int nColors)
{
	int lum, i;
	for (lum = 32; lum > 0; lum--)
	{
		for (i = 0; i < nColors; i++)
		{
			pPal16[i] = g_RGB(
				pPal32[i].Red * lum / 32,
				pPal32[i].Green * lum / 32,
				pPal32[i].Blue * lum / 32);
		}
		pPal16 += nColors * sizeof(KPAL16);
	}
}
//---------------------------------------------------------------------------
// ����:	Pal24ToPal32
// ����:	24λ��ɫ��ת��λ32λ��ɫ��
// ����:	pPal24		KPAL24ָ��
//			pPal32		KPAL32ָ��
//			nClolors	��ɫ��(0 - 256)
// ����:	void
//---------------------------------------------------------------------------
void g_Pal24ToPal32(KPAL24* pPal24, KPAL32* pPal32, int nColors)
{
	for (int i = 0; i < nColors; i++)
	{
		pPal32[i].Red   = pPal24[i].Red;
		pPal32[i].Green = pPal24[i].Green;
		pPal32[i].Blue  = pPal24[i].Blue;
		pPal32[i].Alpha = 0;
	}
}
//---------------------------------------------------------------------------
// ����:	Pal32ToPal24
// ����:	32λ��ɫ��ת��λ24λ��ɫ��
// ����:	pPal32		KPAL32ָ��
//			pPal24		KPAL24ָ��
//			nClolors	��ɫ��(0 - 256)
// ����:	void
//---------------------------------------------------------------------------
void g_Pal32ToPal24(KPAL32* pPal32, KPAL24* pPal24, int nColors)
{
	for (int i = 0; i < nColors; i++)
	{
		pPal24[i].Red   = pPal32[i].Red;
		pPal24[i].Green = pPal32[i].Green;
		pPal24[i].Blue  = pPal32[i].Blue;
	}
}
//---------------------------------------------------------------------------
