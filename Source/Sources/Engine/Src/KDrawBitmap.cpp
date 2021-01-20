//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KDrawBitmap.cpp
// Date:	2000.08.08
// Code:	Daniel Wang
// Desc:	Bitmap Drawing Functions
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KCanvas.h"
#include "KDrawBitmap.h"
//---------------------------------------------------------------------------
// ����:	DrawBitmap
// ����:	����256ɫλͼ
// ����:	void* node, void* canvas
// ����:	void
//---------------------------------------------------------------------------
void g_DrawBitmap(void* node, void* canvas)
{
	KDrawNode* pNode = (KDrawNode *)node;
	KCanvas* pCanvas = (KCanvas *)canvas;

	long nX = pNode->m_nX;// x coord
	long nY = pNode->m_nY;// y coord
	long nWidth = pNode->m_nWidth;// width of sprite
	long nHeight = pNode->m_nHeight;// height of sprite
	void* lpBitmap = pNode->m_pBitmap;// bitmap pointer
	void* lpPalette	= pNode->m_pPalette;// palette pointer

	// �Ի���������вü�
	KClipper Clipper;
	if (!pCanvas->MakeClip(nX, nY, nWidth, nHeight, &Clipper))
		return;

	int nPitch;
	void* lpBuffer = pCanvas->LockCanvas(nPitch);
	if (lpBuffer == NULL)
		return;

	// ������Ļ��һ�е�ƫ��
	long ScreenOffset = nPitch - Clipper.width * 2;

	// ����λͼ��һ�е�ƫ��
	long BitmapOffset = nWidth - Clipper.width;

	// ���ƺ����Ļ�����
	__asm
	{
//---------------------------------------------------------------------------
//  ���� EDI ָ����Ļ����ƫ���� (���ֽڼ�)
//  edi = nPitch * Clipper.y + nX * 2 + lpBuffer
//---------------------------------------------------------------------------
		mov		eax, nPitch
		mov		ebx, Clipper.y
		mul		ebx
		mov     ebx, Clipper.x
		add		ebx, ebx
		add     eax, ebx
		mov 	edi, lpBuffer
		add		edi, eax
//---------------------------------------------------------------------------
//  ��ʼ�� ESI ָ��ͼ��������� (���� Clipper.top ��ͼ������)
//  esi = nWidth * Clipper.top + Clipper.left + lpBitmap
//---------------------------------------------------------------------------
		mov		eax, nWidth
		mov		ecx, Clipper.top
		mul     ecx
		add     eax, Clipper.left
		mov		esi, lpBitmap
		add     esi, eax
//---------------------------------------------------------------------------
//  ��ʼ�� EBX ָ���ɫ����ʼ��ַ
//  ebx = lpPalette
//---------------------------------------------------------------------------
		mov     ebx, lpPalette
		cld

loc_DrawBitmap_Loop1:

		mov		ecx, Clipper.width

loc_DrawBitmap_Loop2:

		movzx	edx, byte ptr[esi]
		inc		esi
		mov     ax, [ebx + edx * 2]
		stosw
		dec		ecx
		jg		loc_DrawBitmap_Loop2

		add		edi, ScreenOffset
		add     esi, BitmapOffset
		dec		Clipper.height
		jg		loc_DrawBitmap_Loop1
	}
	pCanvas->UnlockCanvas();
}
//---------------------------------------------------------------------------

