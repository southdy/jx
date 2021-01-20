//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KDrawFade.cpp
// Date:	2000.08.08
// Code:	Zroc,Daphnis
// Desc:	Fade Drawing Functions
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDDraw.h"
#include "KCanvas.h"
#include "KDrawFade.h"
//---------------------------------------------------------------------------
// ����:	DrawFade
// ����:	��������˥��
// ����:	node, canvas
// ����:	void
//---------------------------------------------------------------------------
void g_DrawFade(void* node, void* canvas)
{
	KDrawNode* pNode = (KDrawNode *)node;
	KCanvas* pCanvas = (KCanvas *)canvas;

	long nX = pNode->m_nX;// x coord
	long nY = pNode->m_nY;// y coord
	long nWidth = pNode->m_nWidth;// width of sprite
	long nHeight = pNode->m_nHeight;// height of sprite
	long nAlpha = pNode->m_nAlpha;// aplha level

	// ���ALPHAֵ
	if (nAlpha < 0 || nAlpha > 31)
		return;

	// �Ի���������вü�
	KClipper Clipper;
	if (!pCanvas->MakeClip(nX, nY, nWidth, nHeight, &Clipper))
		return;

	int nPitch;
	void* lpBuffer = pCanvas->LockCanvas(nPitch);
	if (lpBuffer == NULL)
		return;

	// ������Ļ��һ�е�ƫ��
	long ScreenOffset = nPitch - Clipper.width /2 * 4;//4�ı���

	// setup local var
	DWORD dwRGBMask1, dwRGBMask2;
	if (pCanvas->m_nMask16 == RGB_555)
	{
		dwRGBMask1 = 0x03e07c1f;
		dwRGBMask2 = 0xfc1f03e0;
	}
	else
	{
		dwRGBMask1 = 0x07e0f81f;
		dwRGBMask2 = 0xf81f07e0;
	}

	__asm
	{
//---------------------------------------------------------------------------
// ���� EDI ָ����Ļ����ƫ���� (���ֽڼ�)
// edi = nPitch * Clipper.y + Clipper.x * 2 + lpBuffer
//---------------------------------------------------------------------------
		mov		eax, nPitch
		mov		ebx, Clipper.y
		mul		ebx
		mov     ebx, Clipper.x
		add		ebx, ebx
		add     eax, ebx
		mov		edi, lpBuffer
		add		edi, eax
//---------------------------------------------------------------------------
// ��ʼѭ�� 
//---------------------------------------------------------------------------
		mov		ecx, Clipper.height

loc_DrawFade_Loop1:
		push	ecx
		// ȡ�ÿ��
		mov		ecx, Clipper.width
		// һ�μ���������
		shr		ecx, 1

loc_DrawFade_Loop2:
		// ȡ������
		mov		eax, [edi]
		mov		ebx, eax
		// ���� _g1_r2_b2
		and		eax, dwRGBMask1
		// ���� r1_b1_g2_
		and		ebx, dwRGBMask2
		// _g1_r2_b2 * nAlpha
		imul	eax, nAlpha
		// _g1_r2_b2 / 32
		shr		eax, 5
		// r1_b1_g2_ / 32, Ϊ��ֹ���,�ȳ��ٳ�
		shr		ebx, 5
		// r1_b1_g2_ * nAlpha
		imul	ebx, nAlpha
		// ���� _g1_r2_b2
		and		eax, dwRGBMask1
		// ���� r1_b1_g2_
		and		ebx, dwRGBMask2
		// �õ������
		or		eax, ebx
		// ��ֵ
		mov		[edi], eax
		// ָ�����
		add		edi, 4
		// ѭ����������
		dec		ecx
		jnz		loc_DrawFade_Loop2
		// ѭ����һ������
		add		edi, ScreenOffset
		pop		ecx
		dec		ecx
		jnz		loc_DrawFade_Loop1
	}
	pCanvas->UnlockCanvas();
}
//---------------------------------------------------------------------------
