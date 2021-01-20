/*****************************************************************************************
//  ͼ�ε��ڴ�����Ĳ���
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-11
*****************************************************************************************/
#include "ImageOperation.h"
#include "crtdbg.h"

static unsigned int l_uRGBBitMask32 = 0x07e0f81f;

void RIO_Set16BitImageFormat(int b565)
{
	l_uRGBBitMask32 = b565 ? 0x07e0f81f : 0x03e07c1f;
}

struct KRClipperInfo
{
	int			x;			// �ü����X����
	int			y;			// �ü����Y����
	int			width;		// �ü���Ŀ��
	int			height;		// �ü���ĸ߶�
	int			left;		// �ϱ߽�ü���
	int			top;		// ��߽�ü���
	int			right;		// �ұ߽�ü���
};


static int  RIO_ClipCopyRect(int nX, int nY, int nSrcWidth, int nSrcHeight, int nDestWidth, int nDestHeight, KRClipperInfo* pClipper)
{
	_ASSERT(pClipper);
	// ��ʼ���ü���
	pClipper->x = nX;
	pClipper->y = nY;
	pClipper->width = nSrcWidth;
	pClipper->height = nSrcHeight;
	pClipper->top = 0;
	pClipper->left = 0;
	pClipper->right = 0;

	// �ϱ߽�ü�
	if (pClipper->y < 0)
	{
		pClipper->y = 0;
		pClipper->top = -nY;
		pClipper->height += nY;
	}
	if (pClipper->height <= 0)
		return 0;
	
	// �±߽�ü�
	if (pClipper->height > nDestHeight - pClipper->y)
		pClipper->height = nDestHeight - pClipper->y;
	if (pClipper->height <= 0)
		return 0;

	// ��߽�ü�
	if (pClipper->x < 0)
	{
		pClipper->x = 0;
		pClipper->left = -nX;
		pClipper->width += nX;
	}
	if (pClipper->width <= 0)
		return 0;

	// �ұ߽�ü�
	if (pClipper->width > nDestWidth - pClipper->x)
	{
		pClipper->right = pClipper->width + pClipper->x - nDestWidth;
		pClipper->width -= pClipper->right;
	}
	if (pClipper->width <= 0)
		return 0;
	
	return 1;
}


#define		read_alpha_2_ebx_run_length_2_eax		\
				{	xor		eax, eax			}	\
				{	xor		ebx, ebx			}	\
				{	mov		al,	 byte ptr[esi]	}	\
				{	inc		esi					}	\
				{	mov		bl,  byte ptr[esi]	}	\
				{	inc		esi					}

#define		copy_pixel_use_eax	/*ebxָ���ɫ��*/	\
				{	movzx	eax, byte ptr[esi]	}	\
				{	add		eax, eax			}	\
				{	inc		esi					}	\
				{	mov		ax, [ebx + eax]		}	\
				{	mov		[edi], ax			}	\
				{	inc		edi					}	\
				{	inc		edi					}

#define		mix_2_pixel_color_use_eabdx									\
				{	push	ecx					}						\
				{	mov     ebx, pPalette		}						\
				{	movzx	eax, byte ptr[esi]	}						\
				{	mov		ecx, l_uRGBBitMask32}						\
				{	add		eax, eax			}						\
				{	inc		esi					}						\
				{	mov     dx, [ebx + eax]		}	/*edx = ...rgb*/	\
				{	mov		ax, dx				}	/*eax = ...rgb*/	\
				{	shl		eax, 16				}	/*eax = rgb...*/	\
				{	mov		ax, dx				}	/*eax = rgbrgb*/	\
				{	and		eax, ecx			}	/*eax = .g.r.b*/	\
				{	mov		dx, [edi]			}	/*edx = ...rgb*/	\
				{	mov		bx, dx				}	/*ebx = ...rgb*/	\
				{	shl		ebx, 16				}	/*ebx = rgb...*/	\
				{	mov		bx, dx				}	/*ebx = rgbrgb*/	\
				{	and		ebx, ecx			}	/*ebx = .g.r.b*/	\
				{	add		eax, ebx			}						\
				{	add		ebx, ebx			}						\
				{	add		eax, ebx			}						\
				{	shr		eax, 2				}	/*c = (3xc1+c2)/4*/	\
				{	and     eax, ecx			}	/*eax = .g.r.b*/	\
				{	mov     dx, ax				}	/*edx = ...r.b*/	\
				{	shr     eax, 16				}	/*eax = ....g.*/	\
				{	or      ax, dx				}	/*eax = ...rgb*/	\
				{	mov		[edi], ax			}						\
				{	inc		edi					}						\
				{	inc		edi					}						\
				{	pop		ecx					}

#define		mix_2_pixel_color_alpha_use_eabdx							\
				{	push	ecx					}						\
				{	mov     ebx, pPalette		}						\
				{	movzx	eax, byte ptr[esi]	}						\
				{	mov		ecx, l_uRGBBitMask32}						\
				{	add		eax, eax			}						\
				{	inc		esi					}						\
				{	mov     dx, [ebx + eax]		}	/*edx = ...rgb*/	\
				{	mov		ax, dx				}	/*eax = ...rgb*/	\
				{	shl		eax, 16				}	/*eax = rgb...*/	\
				{	mov		ax, dx				}	/*eax = rgbrgb*/	\
				{	and		eax, ecx			}	/*eax = .g.r.b*/	\
				{	mov		dx, [edi]			}	/*edx = ...rgb*/	\
				{	mov		bx, dx				}	/*ebx = ...rgb*/	\
				{	shl		ebx, 16				}	/*ebx = rgb...*/	\
				{	mov		bx, dx				}	/*ebx = rgbrgb*/	\
				{	and		ebx, ecx			}	/*ebx = .g.r.b*/	\
				{	mov		edx, nAlpha			}						\
				{	imul	eax, edx			}	/*eax = c1*nAlpha*/	\
				{	neg		edx					}						\
				{	add		edx, 32				}						\
				{	imul	ebx, edx			} /*ebx=c2*(32-nAlpha)*/\
				{	add		eax, ebx			}						\
				{	shr		eax, 5				}	/*c=(c1*nAlpha+c2*(32-nAlpha))/32*/ \
				{	and     eax, ecx			}	/*eax = .g.r.b*/	\
				{	mov     dx, ax				}	/*edx = ...r.b*/	\
				{	shr     eax, 16				}	/*eax = ....g.*/	\
				{	or      ax, dx				}	/*eax = ...rgb*/	\
				{	mov		[edi], ax			}						\
				{	inc		edi					}						\
				{	inc		edi					}						\
				{	pop		ecx					}


void RIO_CopySprToBuffer(void* pSpr, int nSprWidth, int nSprHeight, void* pPalette,
					 void* pBuffer, int nBufferWidth, int nBufferHeight,
					 int nX, int nY)
{
	_ASSERT(pSpr && pBuffer && pPalette);
	// �Ի���������вü�
	KRClipperInfo Clipper;
	if (RIO_ClipCopyRect(nX, nY, nSprWidth, nSprHeight, nBufferWidth, nBufferHeight, &Clipper) == 0)
		return;

	long nSprSkip = nSprWidth * Clipper.top + Clipper.left;
	long nSprSkipPerLine = Clipper.left + Clipper.right;
	long nBuffNextLine;

	__asm
	{
		//ebx = nPitch = nBufferWidth * 2
		mov		ebx, nBufferWidth
		add		ebx, ebx

		//ʹediָ��buffer�������edi = (char*)pBuffer + Clipper.y * nPitch + Clipper.x * 2;
		mov		edi, pBuffer
		mov		eax, Clipper.y
		mul		ebx
		mov		edx, Clipper.x
		add		eax, edx
		add		eax, edx
		add		edi, eax

		//nBuffNextLine = nPitch - Clipper.width * 2;
		mov		edx, Clipper.width
		add		edx, edx
		sub		ebx, edx
		mov		nBuffNextLine, ebx

		//ʹesiָ��ͼ���������,(����nSprSkip������ͼ������)
		mov		esi, pSpr
		//_SkipSpriteAheadContent_:
		{
			mov		edx, nSprSkip
			or		edx, edx
			jz		_SkipSpriteAheadContentEnd_

			_SkipSpriteAheadContentLocalStart_:
			{
				read_alpha_2_ebx_run_length_2_eax
				or		ebx, ebx
				jnz		_SkipSpriteAheadContentLocalAlpha_
				sub		edx, eax
				jg		_SkipSpriteAheadContentLocalStart_
				neg		edx
				jmp		_SkipSpriteAheadContentEnd_

				_SkipSpriteAheadContentLocalAlpha_:
				{
					add		esi, eax
					sub		edx, eax
					jg		_SkipSpriteAheadContentLocalStart_
					add		esi, edx
					neg		edx
					jmp		_SkipSpriteAheadContentEnd_
				}
			}
		}
		_SkipSpriteAheadContentEnd_:

		mov		eax, nSprSkipPerLine
		or		eax, eax
		jnz		_DrawPartLineSection_	//if (nSprSkipPerLine) goto _DrawPartLineSection_

		//_DrawFullLineSection_:
		{
			//��Ϊsprite�������ѹ���������е��˴�edx��Ϊ0����sprite�����ѹ����_DrawFullLineSection_���
			_DrawFullLineSection_Line_:
			{
				mov		edx, Clipper.width
				_DrawFullLineSection_LineLocal_:
				{
					read_alpha_2_ebx_run_length_2_eax

					or		ebx, ebx
					jnz		_DrawFullLineSection_LineLocal_Alpha_
					add		edi, eax
					add		edi, eax
					sub		edx, eax
					jg		_DrawFullLineSection_LineLocal_

					add		edi, nBuffNextLine
					dec		Clipper.height
					jnz		_DrawFullLineSection_Line_
					jmp		_EXIT_WAY_
				
					_DrawFullLineSection_LineLocal_Alpha_:
					{
						sub		edx, eax
						mov		ecx, eax
						mov     ebx, pPalette
						_DrawFullLineSection_CopyPixel_:
						{
							copy_pixel_use_eax
							loop	_DrawFullLineSection_CopyPixel_
						}
						or		edx, edx
						jnz		_DrawFullLineSection_LineLocal_

						add		edi, nBuffNextLine
						dec		Clipper.height
						jnz		_DrawFullLineSection_Line_
						jmp		_EXIT_WAY_
					}
				}
			}
		}

		_DrawPartLineSection_:
		{
			mov		eax, Clipper.left
			or		eax, eax
			jz		_DrawPartLineSection_SkipRight_Line_

			mov		eax, Clipper.right
			or		eax, eax
			jz		_DrawPartLineSection_SkipLeft_Line_
		}

		_DrawPartLineSection_Line_:
		{
			mov		eax, edx
			mov		edx, Clipper.width
			or		eax, eax
			jnz		_DrawPartLineSection_LineLocal_CheckAlpha_

			_DrawPartLineSection_LineLocal_:
			{
				read_alpha_2_ebx_run_length_2_eax

				_DrawPartLineSection_LineLocal_CheckAlpha_:
				or		ebx, ebx
				jnz		_DrawPartLineSection_LineLocal_Alpha_
				add		edi, eax
				add		edi, eax
				sub		edx, eax
				jg		_DrawPartLineSection_LineLocal_

				dec		Clipper.height
				jz		_EXIT_WAY_

				add		edi, edx
				add		edi, edx
				neg		edx
			}
				
			_DrawPartLineSection_LineSkip_:
			{
				add		edi, nBuffNextLine
				//����nSprSkipPerLine���ص�sprite����
				mov		eax, edx
				mov		edx, nSprSkipPerLine
				or		eax, eax
				jnz		_DrawPartLineSection_LineSkipLocal_CheckAlpha_
				_DrawPartLineSection_LineSkipLocal_:
				{
					read_alpha_2_ebx_run_length_2_eax
					
					_DrawPartLineSection_LineSkipLocal_CheckAlpha_:
					or		ebx, ebx
					jnz		_DrawPartLineSection_LineSkipLocal_Alpha_
					sub		edx, eax
					jg		_DrawPartLineSection_LineSkipLocal_
					neg		edx
					jmp		_DrawPartLineSection_Line_
					_DrawPartLineSection_LineSkipLocal_Alpha_:
					{
						add		esi, eax
						sub		edx, eax
						jg		_DrawPartLineSection_LineSkipLocal_
						add		esi, edx
						neg		edx
						jmp		_DrawPartLineSection_Line_
					}
				}
			}
			_DrawPartLineSection_LineLocal_Alpha_:
			{
				sub		edx, eax
				jle		_DrawPartLineSection_LineLocal_Alpha_Part_		//����ȫ����eax����ͬalphaֵ����㣬�����е��Ѿ���������

				mov		ecx, eax
				mov     ebx, pPalette

				_DrawPartLineSection_CopyPixel_:
				{
					copy_pixel_use_eax
					loop	_DrawPartLineSection_CopyPixel_
				}
				jmp		_DrawPartLineSection_LineLocal_
			}

			_DrawPartLineSection_LineLocal_Alpha_Part_:
			{
				mov		ecx, eax
				add		eax, edx
				mov     ebx, pPalette
				_DrawPartLineSection_CopyPixel_Part_:
				{
					copy_pixel_use_eax
					loop	_DrawPartLineSection_CopyPixel_Part_
				}
			
				dec		Clipper.height
				jz		_EXIT_WAY_
				neg		edx
				mov		ebx, 255	//�����Ҫȷ�е�ԭebx(alpha)ֵ������ǰͷpush ebx���˴�pop���
				jmp		_DrawPartLineSection_LineSkip_
			}
		}
				_DrawPartLineSection_SkipLeft_Line_:
		{
			mov		eax, edx
			mov		edx, Clipper.width
			or		eax, eax
			jnz		_DrawPartLineSection_SkipLeft_LineLocal_CheckAlpha_
			_DrawPartLineSection_SkipLeft_LineLocal_:
			{
				read_alpha_2_ebx_run_length_2_eax
				_DrawPartLineSection_SkipLeft_LineLocal_CheckAlpha_:
				or		ebx, ebx
				jnz		_DrawPartLineSection_SkipLeft_LineLocal_Alpha_
				add		edi, eax
				add		edi, eax
				sub		edx, eax
				jg		_DrawPartLineSection_SkipLeft_LineLocal_

				dec		Clipper.height
				jz		_EXIT_WAY_
			}
			
			_DrawPartLineSection_SkipLeft_LineSkip_:
			{
				add		edi, nBuffNextLine
				//����nSprSkipPerLine���ص�sprite����
				mov		edx, nSprSkipPerLine
				_DrawPartLineSection_SkipLeft_LineSkipLocal_:
				{
					read_alpha_2_ebx_run_length_2_eax
					or		ebx, ebx
					jnz		_DrawPartLineSection_SkipLeft_LineSkipLocal_Alpha_
					sub		edx, eax
					jg		_DrawPartLineSection_SkipLeft_LineSkipLocal_
					neg		edx
					jmp		_DrawPartLineSection_SkipLeft_Line_
					_DrawPartLineSection_SkipLeft_LineSkipLocal_Alpha_:
					{
						add		esi, eax
						sub		edx, eax
						jg		_DrawPartLineSection_SkipLeft_LineSkipLocal_
						add		esi, edx
						neg		edx
						jmp		_DrawPartLineSection_SkipLeft_Line_
					}
				}
			}
			_DrawPartLineSection_SkipLeft_LineLocal_Alpha_:
			{
				sub		edx, eax		;�Ȱ�eax���ˣ���������Ϳ��Բ���Ҫ����eax��
				mov		ecx, eax						
				mov     ebx, pPalette
				_DrawPartLineSection_SkipLeft_CopyPixel_:
				{
					copy_pixel_use_eax
					loop	_DrawPartLineSection_SkipLeft_CopyPixel_
				}
				or		edx, edx
				jnz		_DrawPartLineSection_SkipLeft_LineLocal_
				dec		Clipper.height
				jg		_DrawPartLineSection_SkipLeft_LineSkip_
				jmp		_EXIT_WAY_
			}
		}

		_DrawPartLineSection_SkipRight_Line_:
		{
			mov		edx, Clipper.width
			_DrawPartLineSection_SkipRight_LineLocal_:
			{
				read_alpha_2_ebx_run_length_2_eax
				or		ebx, ebx
				jnz		_DrawPartLineSection_SkipRight_LineLocal_Alpha_
				add		edi, eax
				add		edi, eax
				sub		edx, eax
				jg		_DrawPartLineSection_SkipRight_LineLocal_

				dec		Clipper.height
				jz		_EXIT_WAY_

				add		edi, edx
				add		edi, edx
				neg		edx
			}
			
			_DrawPartLineSection_SkipRight_LineSkip_:
			{
				add		edi, nBuffNextLine
				//����nSprSkipPerLine���ص�sprite����
				mov		eax, edx
				mov		edx, nSprSkipPerLine
				or		eax, eax
				jnz		_DrawPartLineSection_SkipRight_LineSkipLocal_CheckAlpha_
				_DrawPartLineSection_SkipRight_LineSkipLocal_:
				{
					read_alpha_2_ebx_run_length_2_eax
					
					_DrawPartLineSection_SkipRight_LineSkipLocal_CheckAlpha_:
					or		ebx, ebx
					jnz		_DrawPartLineSection_SkipRight_LineSkipLocal_Alpha_
					sub		edx, eax
					jg		_DrawPartLineSection_SkipRight_LineSkipLocal_
					jmp		_DrawPartLineSection_SkipRight_Line_
					_DrawPartLineSection_SkipRight_LineSkipLocal_Alpha_:
					{
						add		esi, eax
						sub		edx, eax
						jg		_DrawPartLineSection_SkipRight_LineSkipLocal_
						jmp		_DrawPartLineSection_SkipRight_Line_
					}
				}
			}
			_DrawPartLineSection_SkipRight_LineLocal_Alpha_:
			{
				sub		edx, eax
				jle		_DrawPartLineSection_SkipRight_LineLocal_Alpha_Part_		//����ȫ����eax����ͬalphaֵ����㣬�����е��Ѿ���������

				mov		ecx, eax				
				mov     ebx, pPalette
				_DrawPartLineSection_SkipRight_CopyPixel_:
				{
					copy_pixel_use_eax
					loop	_DrawPartLineSection_SkipRight_CopyPixel_
				}
				jmp		_DrawPartLineSection_SkipRight_LineLocal_
			}
			_DrawPartLineSection_SkipRight_LineLocal_Alpha_Part_:
			{
				add		eax, edx
				mov		ecx, eax
				mov     ebx, pPalette
				_DrawPartLineSection_SkipRight_CopyPixel_Part_:
				{
					copy_pixel_use_eax
					loop	_DrawPartLineSection_SkipRight_CopyPixel_Part_
				}
				neg		edx
				mov		ebx, 255	//�����Ҫȷ�е�ԭebx(alpha)ֵ������ǰͷpush ebx���˴�pop���
				dec		Clipper.height
				jg		_DrawPartLineSection_SkipRight_LineSkip_
				jmp		_EXIT_WAY_
			}
		}
		_EXIT_WAY_:
	}
}

void RIO_CopySprToBufferAlpha(void* pSpr, int nSprWidth, int nSprHeight, void* pPalette,
					 void* pBuffer, int nBufferWidth, int nBufferHeight,
					 int nX, int nY)
{
	_ASSERT(pSpr && pBuffer && pPalette);
	// �Ի���������вü�
	KRClipperInfo Clipper;	
	if (RIO_ClipCopyRect(nX, nY, nSprWidth, nSprHeight, nBufferWidth, nBufferHeight, &Clipper) == 0)
		return;

	// pBufferָ����Ļ����ƫ��λ�� (���ֽڼ�)
	long nSprSkip = nSprWidth * Clipper.top + Clipper.left;
	long nSprSkipPerLine = Clipper.left + Clipper.right;
	int	 nBuffNextLine, nAlpha;

	__asm
	{
		//ebx = nPitch = nBufferWidth * 2
		mov		ebx, nBufferWidth
		add		ebx, ebx

		//ʹediָ��buffer�������edi = (char*)pBuffer + Clipper.y * nPitch + Clipper.x * 2;
		mov		edi, pBuffer
		mov		eax, Clipper.y
		mul		ebx
		mov		edx, Clipper.x
		add		eax, edx
		add		eax, edx
		add		edi, eax

		//nBuffNextLine = nPitch - Clipper.width * 2;
		mov		edx, Clipper.width
		add		edx, edx
		sub		ebx, edx
		mov		nBuffNextLine, ebx

		//ʹesiָ��ͼ���������,(����nSprSkip������ͼ������)
		mov		esi, pSpr
		//_SkipSpriteAheadContent_:
		{
			mov		edx, nSprSkip
			or		edx, edx
			jz		_SkipSpriteAheadContentEnd_

			_SkipSpriteAheadContentLocalStart_:
			{
				read_alpha_2_ebx_run_length_2_eax
				or		ebx, ebx
				jnz		_SkipSpriteAheadContentLocalAlpha_
				sub		edx, eax
				jg		_SkipSpriteAheadContentLocalStart_
				neg		edx
				jmp		_SkipSpriteAheadContentEnd_

				_SkipSpriteAheadContentLocalAlpha_:
				{
					add		esi, eax
					sub		edx, eax
					jg		_SkipSpriteAheadContentLocalStart_
					add		esi, edx
					neg		edx
					jmp		_SkipSpriteAheadContentEnd_
				}
			}
		}
		_SkipSpriteAheadContentEnd_:

		mov		eax, nSprSkipPerLine
		or		eax, eax
		jnz		_DrawPartLineSection_	//if (nSprSkipPerLine) goto _DrawPartLineSection_

		//_DrawFullLineSection_:
		{
			//��Ϊsprite�������ѹ���������е��˴�edx��Ϊ0����sprite�����ѹ����_DrawFullLineSection_���			
			_DrawFullLineSection_Line_:
			{
				mov		edx, Clipper.width
				_DrawFullLineSection_LineLocal_:
				{
					read_alpha_2_ebx_run_length_2_eax

					or		ebx, ebx
					jnz		_DrawFullLineSection_LineLocal_Alpha_
					add		edi, eax
					add		edi, eax
					sub		edx, eax
					jg		_DrawFullLineSection_LineLocal_

					add		edi, nBuffNextLine
					dec		Clipper.height
					jnz		_DrawFullLineSection_Line_
					jmp		_EXIT_WAY_
				
					_DrawFullLineSection_LineLocal_Alpha_:
					{
						sub		edx, eax
						mov		ecx, eax

						cmp		ebx, 255
						jl		_DrawFullLineSection_LineLocal_HalfAlpha_

						//_DrawFullLineSection_LineLocal_DirectCopy_:
						{
							mov     ebx, pPalette
							_DrawFullLineSection_CopyPixel_:
							{
								copy_pixel_use_eax
								loop	_DrawFullLineSection_CopyPixel_
							}
							or		edx, edx
							jnz		_DrawFullLineSection_LineLocal_
	
							add		edi, nBuffNextLine
							dec		Clipper.height
							jnz		_DrawFullLineSection_Line_
							jmp		_EXIT_WAY_
						}

						_DrawFullLineSection_LineLocal_HalfAlpha_:
						{
							push	edx
							shr		ebx, 3
							mov		nAlpha, ebx
							_DrawFullLineSection_HalfAlphaPixel_:
							{
								mix_2_pixel_color_alpha_use_eabdx
								loop	_DrawFullLineSection_HalfAlphaPixel_
							}
							pop		edx
							or		edx, edx
							jnz		_DrawFullLineSection_LineLocal_

							add		edi, nBuffNextLine
							dec		Clipper.height
							jnz		_DrawFullLineSection_Line_
							jmp		_EXIT_WAY_
						}
					}
				}
			}
		}

		_DrawPartLineSection_:
		{
			mov		eax, Clipper.left
			or		eax, eax
			jz		_DrawPartLineSection_SkipRight_Line_

			mov		eax, Clipper.right
			or		eax, eax
			jz		_DrawPartLineSection_SkipLeft_Line_
		}

		_DrawPartLineSection_Line_:
		{
			mov		eax, edx
			mov		edx, Clipper.width
			or		eax, eax
			jnz		_DrawPartLineSection_LineLocal_CheckAlpha_
			_DrawPartLineSection_LineLocal_:
			{
				read_alpha_2_ebx_run_length_2_eax
				_DrawPartLineSection_LineLocal_CheckAlpha_:
				or		ebx, ebx
				jnz		_DrawPartLineSection_LineLocal_Alpha_
				add		edi, eax
				add		edi, eax
				sub		edx, eax
				jg		_DrawPartLineSection_LineLocal_

				dec		Clipper.height
				jz		_EXIT_WAY_

				add		edi, edx
				add		edi, edx
				neg		edx
			}
			
			_DrawPartLineSection_LineSkip_:
			{
				add		edi, nBuffNextLine
				//����nSprSkipPerLine���ص�sprite����
				mov		eax, edx
				mov		edx, nSprSkipPerLine
				or		eax, eax
				jnz		_DrawPartLineSection_LineSkipLocal_CheckAlpha_
				_DrawPartLineSection_LineSkipLocal_:
				{
					read_alpha_2_ebx_run_length_2_eax
					
					_DrawPartLineSection_LineSkipLocal_CheckAlpha_:
					or		ebx, ebx
					jnz		_DrawPartLineSection_LineSkipLocal_Alpha_
					sub		edx, eax
					jg		_DrawPartLineSection_LineSkipLocal_
					neg		edx
					jmp		_DrawPartLineSection_Line_
					_DrawPartLineSection_LineSkipLocal_Alpha_:
					{
						add		esi, eax
						sub		edx, eax
						jg		_DrawPartLineSection_LineSkipLocal_
						add		esi, edx
						neg		edx
						jmp		_DrawPartLineSection_Line_
					}
				}
			}
			_DrawPartLineSection_LineLocal_Alpha_:
			{
				sub		edx, eax
				jle		_DrawPartLineSection_LineLocal_Alpha_Part_		//����ȫ����eax����ͬalphaֵ����㣬�����е��Ѿ���������

				mov		ecx, eax
				cmp		ebx, 255
				jl		_DrawPartLineSection_LineLocal_HalfAlpha_
						
				//_DrawPartLineSection_LineLocal_DirectCopy_:
				{
					mov     ebx, pPalette
					_DrawPartLineSection_CopyPixel_:
					{
						copy_pixel_use_eax
						loop	_DrawPartLineSection_CopyPixel_
					}
					jmp		_DrawPartLineSection_LineLocal_
				}
				
				_DrawPartLineSection_LineLocal_HalfAlpha_:
				{
					push	edx
					shr		ebx, 3
					mov		nAlpha, ebx
					_DrawPartLineSection_HalfAlphaPixel_:
					{
						mix_2_pixel_color_alpha_use_eabdx
						loop	_DrawPartLineSection_HalfAlphaPixel_
					}
					pop		edx
					jmp		_DrawPartLineSection_LineLocal_
				}
			}
			_DrawPartLineSection_LineLocal_Alpha_Part_:
			{
				mov		ecx, eax
				add		eax, edx
				cmp		ebx, 255
				jl		_DrawPartLineSection_LineLocal_HalfAlpha_Part_
					
				//_DrawPartLineSection_LineLocal_DirectCopy_Part_:
				{
					mov     ebx, pPalette
					_DrawPartLineSection_CopyPixel_Part_:
					{
						copy_pixel_use_eax
						loop	_DrawPartLineSection_CopyPixel_Part_
					}
			
					dec		Clipper.height
					jz		_EXIT_WAY_
					neg		edx
					mov		ebx, 255	//�����Ҫȷ�е�ԭebx(alpha)ֵ������ǰͷpush ebx���˴�pop���
					jmp		_DrawPartLineSection_LineSkip_
				}
				
				_DrawPartLineSection_LineLocal_HalfAlpha_Part_:
				{
					push	edx
					shr		ebx, 3
					mov		nAlpha, ebx
					_DrawPartLineSection_HalfAlphaPixel_Part_:
					{
						mix_2_pixel_color_alpha_use_eabdx
						loop	_DrawPartLineSection_HalfAlphaPixel_Part_
					}
					pop		edx
					neg		edx
					mov		ebx, 128
					dec		Clipper.height
					jg		_DrawPartLineSection_LineSkip_//�����Ҫȷ�е�ԭebx(alpha)ֵ������ǰͷpush ebx���˴�pop���
					jmp		_EXIT_WAY_
				}
			}
		}

		_DrawPartLineSection_SkipLeft_Line_:
		{
			mov		eax, edx
			mov		edx, Clipper.width
			or		eax, eax
			jnz		_DrawPartLineSection_SkipLeft_LineLocal_CheckAlpha_
			_DrawPartLineSection_SkipLeft_LineLocal_:
			{
				read_alpha_2_ebx_run_length_2_eax
				_DrawPartLineSection_SkipLeft_LineLocal_CheckAlpha_:
				or		ebx, ebx
				jnz		_DrawPartLineSection_SkipLeft_LineLocal_Alpha_
				add		edi, eax
				add		edi, eax
				sub		edx, eax
				jg		_DrawPartLineSection_SkipLeft_LineLocal_

				dec		Clipper.height
				jz		_EXIT_WAY_
			}
			
			_DrawPartLineSection_SkipLeft_LineSkip_:
			{
				add		edi, nBuffNextLine
				//����nSprSkipPerLine���ص�sprite����
				mov		edx, nSprSkipPerLine
				_DrawPartLineSection_SkipLeft_LineSkipLocal_:
				{
					read_alpha_2_ebx_run_length_2_eax
					or		ebx, ebx
					jnz		_DrawPartLineSection_SkipLeft_LineSkipLocal_Alpha_
					sub		edx, eax
					jg		_DrawPartLineSection_SkipLeft_LineSkipLocal_
					neg		edx
					jmp		_DrawPartLineSection_SkipLeft_Line_
					_DrawPartLineSection_SkipLeft_LineSkipLocal_Alpha_:
					{
						add		esi, eax
						sub		edx, eax
						jg		_DrawPartLineSection_SkipLeft_LineSkipLocal_
						add		esi, edx
						neg		edx
						jmp		_DrawPartLineSection_SkipLeft_Line_
					}
				}
			}
			_DrawPartLineSection_SkipLeft_LineLocal_Alpha_:
			{
				sub		edx, eax		;�Ȱ�eax���ˣ���������Ϳ��Բ���Ҫ����eax��
				mov		ecx, eax
				cmp		ebx, 255
				jl		_DrawPartLineSection_SkipLeft_LineLocal_nAlpha_
						
				//_DrawPartLineSection_SkipLeft_LineLocal_DirectCopy_:
				{
					mov     ebx, pPalette
					_DrawPartLineSection_SkipLeft_CopyPixel_:
					{
						copy_pixel_use_eax
						loop	_DrawPartLineSection_SkipLeft_CopyPixel_
					}
					or		edx, edx
					jnz		_DrawPartLineSection_SkipLeft_LineLocal_
					dec		Clipper.height
					jg		_DrawPartLineSection_SkipLeft_LineSkip_
					jmp		_EXIT_WAY_
				}

				_DrawPartLineSection_SkipLeft_LineLocal_nAlpha_:
				{
					push	edx
					shr		ebx, 3
					mov		nAlpha, ebx
					_DrawPartLineSection_SkipLeft_HalfAlphaPixel_:
					{
						mix_2_pixel_color_alpha_use_eabdx
						loop	_DrawPartLineSection_SkipLeft_HalfAlphaPixel_
					}
					pop		edx
					or		edx, edx
					jnz		_DrawPartLineSection_SkipLeft_LineLocal_
					dec		Clipper.height
					jg		_DrawPartLineSection_SkipLeft_LineSkip_
					jmp		_EXIT_WAY_
				}
			}
		}

		_DrawPartLineSection_SkipRight_Line_:
		{
			mov		edx, Clipper.width
			_DrawPartLineSection_SkipRight_LineLocal_:
			{
				read_alpha_2_ebx_run_length_2_eax
				or		ebx, ebx
				jnz		_DrawPartLineSection_SkipRight_LineLocal_Alpha_
				add		edi, eax
				add		edi, eax
				sub		edx, eax
				jg		_DrawPartLineSection_SkipRight_LineLocal_

				dec		Clipper.height
				jz		_EXIT_WAY_

				add		edi, edx
				add		edi, edx
				neg		edx
			}
			
			_DrawPartLineSection_SkipRight_LineSkip_:
			{
				add		edi, nBuffNextLine
				//����nSprSkipPerLine���ص�sprite����
				mov		eax, edx
				mov		edx, nSprSkipPerLine
				or		eax, eax
				jnz		_DrawPartLineSection_SkipRight_LineSkipLocal_CheckAlpha_
				_DrawPartLineSection_SkipRight_LineSkipLocal_:
				{
					read_alpha_2_ebx_run_length_2_eax
					
					_DrawPartLineSection_SkipRight_LineSkipLocal_CheckAlpha_:
					or		ebx, ebx
					jnz		_DrawPartLineSection_SkipRight_LineSkipLocal_Alpha_
					sub		edx, eax
					jg		_DrawPartLineSection_SkipRight_LineSkipLocal_
					jmp		_DrawPartLineSection_SkipRight_Line_
					_DrawPartLineSection_SkipRight_LineSkipLocal_Alpha_:
					{
						add		esi, eax
						sub		edx, eax
						jg		_DrawPartLineSection_SkipRight_LineSkipLocal_
						jmp		_DrawPartLineSection_SkipRight_Line_
					}
				}
			}
			_DrawPartLineSection_SkipRight_LineLocal_Alpha_:
			{
				sub		edx, eax
				jle		_DrawPartLineSection_SkipRight_LineLocal_Alpha_Part_		//����ȫ����eax����ͬalphaֵ����㣬�����е��Ѿ���������

				mov		ecx, eax
				cmp		ebx, 255
				jl		_DrawPartLineSection_SkipRight_LineLocal_HalfAlpha_
						
				//_DrawPartLineSection_SkipRight_LineLocal_DirectCopy_:
				{
					mov     ebx, pPalette
					_DrawPartLineSection_SkipRight_CopyPixel_:
					{
						copy_pixel_use_eax
						loop	_DrawPartLineSection_SkipRight_CopyPixel_
					}
					jmp		_DrawPartLineSection_SkipRight_LineLocal_
				}
				
				_DrawPartLineSection_SkipRight_LineLocal_HalfAlpha_:
				{
					push	edx
					shr		ebx, 3
					mov		nAlpha, ebx
					_DrawPartLineSection_SkipRight_HalfAlphaPixel_:
					{
						mix_2_pixel_color_alpha_use_eabdx
						loop	_DrawPartLineSection_SkipRight_HalfAlphaPixel_
					}
					pop		edx
					jmp		_DrawPartLineSection_SkipRight_LineLocal_
				}
			}
			_DrawPartLineSection_SkipRight_LineLocal_Alpha_Part_:
			{
				add		eax, edx
				mov		ecx, eax
				cmp		ebx, 255
				jl		_DrawPartLineSection_SkipRight_LineLocal_HalfAlpha_Part_
					
				//_DrawPartLineSection_SkipRight_LineLocal_DirectCopy_Part_:
				{
					mov     ebx, pPalette
					_DrawPartLineSection_SkipRight_CopyPixel_Part_:
					{
						copy_pixel_use_eax
						loop	_DrawPartLineSection_SkipRight_CopyPixel_Part_
					}
					neg		edx
					mov		ebx, 255	//�����Ҫȷ�е�ԭebx(alpha)ֵ������ǰͷpush ebx���˴�pop���
					dec		Clipper.height
					jg		_DrawPartLineSection_SkipRight_LineSkip_
					jmp		_EXIT_WAY_
				}
				
				_DrawPartLineSection_SkipRight_LineLocal_HalfAlpha_Part_:
				{
					push	edx
					shr		ebx, 3
					mov		nAlpha, ebx
					_DrawPartLineSection_SkipRight_HalfAlphaPixel_Part_:
					{
						mix_2_pixel_color_alpha_use_eabdx
						loop	_DrawPartLineSection_SkipRight_HalfAlphaPixel_Part_
					}
					pop		edx
					neg		edx
					mov		ebx, 128
					dec		Clipper.height
					jg		_DrawPartLineSection_SkipRight_LineSkip_//�����Ҫȷ�е�ԭebx(alpha)ֵ������ǰͷpush ebx���˴�pop���
					jmp		_EXIT_WAY_
				}
			}
		}
		_EXIT_WAY_:
	}
}


void RIO_CopyBitmap16ToBuffer(void* pBitmap, int nBmpWidth, int nBmpHeight,
					 void* pBuffer, int nBufferWidth, int nBufferHeight,
					 int nX, int nY)
{
	// �Ի���������вü�
	KRClipperInfo Clipper;
	_ASSERT(pBitmap && pBuffer);
	if (RIO_ClipCopyRect(nX, nY, nBmpWidth, nBmpHeight, nBufferWidth, nBufferHeight, &Clipper) == 0)
		return;
	long nBitmapOffset, nBuffNextLine;

	// ���ƺ����Ļ�����
	__asm
	{
		//ebx = nPitch = nBufferWidth * 2
		mov		ebx, nBufferWidth
		add		ebx, ebx

		//ʹediָ��buffer�������edi = (char*)pBuffer + Clipper.y * nPitch + Clipper.x * 2;
		mov		edi, pBuffer
		mov		eax, Clipper.y
		mul		ebx
		mov		edx, Clipper.x
		add		eax, edx
		add		eax, edx
		add		edi, eax

		//esiָ��ͼ�����ݻ��ƵĿ�ʼλ��
		mov		esi, pBitmap
		mov		eax, Clipper.top
		mul		nBmpWidth
		add		eax, Clipper.left
		add		eax, eax
		add		esi, eax

		//���㻺������һ�е�ƫ��=nPitch - Clipper.width * 2;
		mov		edx, Clipper.width
		add		edx, edx
		sub		ebx, edx
		mov		nBuffNextLine, ebx

		// ����λͼ��һ�е�ƫ��
		mov		eax, nBmpWidth
		sub		eax, Clipper.width
		add		eax, eax

		mov		ebx, Clipper.height
		mov		edx, Clipper.width
		mov		ecx, edi
		sub		ecx, esi
		test	ecx, 2
		jz		_4BYTE_ALIGN_COPY_

		//_2BYTE_ALIGN_COPY_:
		{
			_2BYTE_ALIGN_COPY_LINE_:
			{
				mov		ecx, edx
				rep		movsw
				add		edi, nBuffNextLine
				add     esi, eax
				dec		ebx
				jne		_2BYTE_ALIGN_COPY_LINE_
				jmp		_EXIT_WAY_
			}
		}

		_4BYTE_ALIGN_COPY_:
		{
			mov		nBitmapOffset, eax
			_4BYTE_ALIGN_COPY_LINE_:
			{
				mov		eax, edx
				mov		ecx, edi
				shr		ecx, 1
				and		ecx, 1
				sub		eax, ecx
				rep		movsw
				mov		ecx, eax
				shr		ecx, 1
				rep		movsd
				adc		ecx, ecx
				rep		movsw
				add		edi, nBuffNextLine
				add     esi, nBitmapOffset
				dec		ebx
				jne		_4BYTE_ALIGN_COPY_LINE_
				jmp		_EXIT_WAY_
			}
		}
	_EXIT_WAY_:
	}
}


void RIO_CopySprToBuffer3LevelAlpha(void* pSpr, int nSprWidth, int nSprHeight, void* pPalette,
					 void* pBuffer, int nBufferWidth, int nBufferHeight, int nX, int nY)
{
	_ASSERT(pSpr && pBuffer && pPalette);
	// �Ի���������вü�
	KRClipperInfo Clipper;	
	if (RIO_ClipCopyRect(nX, nY, nSprWidth, nSprHeight, nBufferWidth, nBufferHeight, &Clipper) == 0)
		return;

	int	nPitch = nBufferWidth + nBufferWidth;
	// pBufferָ����Ļ����ƫ��λ�� (���ֽڼ�)
	pBuffer = (char*)pBuffer + Clipper.y * nPitch + Clipper.x * 2;
	long nBuffNextLine = nPitch - Clipper.width * 2;// next line add
	long nSprSkip = nSprWidth * Clipper.top + Clipper.left;
	long nSprSkipPerLine = Clipper.left + Clipper.right;

	__asm
	{
		//ʹediָ��buffer�������,ʹesiָ��ͼ���������,(����nSprSkip������ͼ������)
		mov		edi, pBuffer
		mov		esi, pSpr

		//_SkipSpriteAheadContent_:
		{
			mov		edx, nSprSkip
			or		edx, edx
			jz		_SkipSpriteAheadContentEnd_

			_SkipSpriteAheadContentLocalStart_:
			{
				read_alpha_2_ebx_run_length_2_eax
				or		ebx, ebx
				jnz		_SkipSpriteAheadContentLocalAlpha_
				sub		edx, eax
				jg		_SkipSpriteAheadContentLocalStart_
				neg		edx
				jmp		_SkipSpriteAheadContentEnd_

				_SkipSpriteAheadContentLocalAlpha_:
				{
					add		esi, eax
					sub		edx, eax
					jg		_SkipSpriteAheadContentLocalStart_
					add		esi, edx
					neg		edx
					jmp		_SkipSpriteAheadContentEnd_
				}
			}
		}
		_SkipSpriteAheadContentEnd_:

		mov		eax, nSprSkipPerLine
		or		eax, eax
		jnz		_DrawPartLineSection_	//if (nSprSkipPerLine) goto _DrawPartLineSection_

		//_DrawFullLineSection_:
		{
			//��Ϊsprite�������ѹ���������е��˴�edx��Ϊ0����sprite�����ѹ����_DrawFullLineSection_���			
			_DrawFullLineSection_Line_:
			{
				mov		edx, Clipper.width
				_DrawFullLineSection_LineLocal_:
				{
					read_alpha_2_ebx_run_length_2_eax

					or		ebx, ebx
					jnz		_DrawFullLineSection_LineLocal_Alpha_
					add		edi, eax
					add		edi, eax
					sub		edx, eax
					jg		_DrawFullLineSection_LineLocal_

					add		edi, nBuffNextLine
					dec		Clipper.height
					jnz		_DrawFullLineSection_Line_
					jmp		_EXIT_WAY_
				
					_DrawFullLineSection_LineLocal_Alpha_:
					{
						push	eax
						mov		ecx, eax

						cmp		ebx, 200
						jl		_DrawFullLineSection_LineLocal_HalfAlpha_

						//_DrawFullLineSection_LineLocal_DirectCopy_:
						{
							mov     ebx, pPalette
							_DrawFullLineSection_CopyPixel_:
							{
								copy_pixel_use_eax
								loop	_DrawFullLineSection_CopyPixel_
							}

							pop		eax
							sub		edx, eax
							jg		_DrawFullLineSection_LineLocal_
	
							add		edi, nBuffNextLine
							dec		Clipper.height
							jnz		_DrawFullLineSection_Line_
							jmp		_EXIT_WAY_
						}

						_DrawFullLineSection_LineLocal_HalfAlpha_:
						{
							push	edx							
							_DrawFullLineSection_HalfAlphaPixel_:
							{
								mix_2_pixel_color_use_eabdx
								loop	_DrawFullLineSection_HalfAlphaPixel_
							}
							pop		edx
							pop		eax
							sub		edx, eax
							jg		_DrawFullLineSection_LineLocal_

							add		edi, nBuffNextLine
							dec		Clipper.height
							jnz		_DrawFullLineSection_Line_
							jmp		_EXIT_WAY_
						}
					}
				}
			}
		}

		_DrawPartLineSection_:
		{
			_DrawPartLineSection_Line_:
			{
				mov		eax, edx
				mov		edx, Clipper.width
				or		eax, eax
				jnz		_DrawPartLineSection_LineLocal_CheckAlpha_

				_DrawPartLineSection_LineLocal_:
				{
					read_alpha_2_ebx_run_length_2_eax

					_DrawPartLineSection_LineLocal_CheckAlpha_:
					or		ebx, ebx
					jnz		_DrawPartLineSection_LineLocal_Alpha_
					add		edi, eax
					add		edi, eax
					sub		edx, eax
					jg		_DrawPartLineSection_LineLocal_

					dec		Clipper.height
					jz		_EXIT_WAY_

					add		edi, edx
					add		edi, edx
					neg		edx
				}
				
				_DrawPartLineSection_LineSkip_:
				{
					add		edi, nBuffNextLine
					//����nSprSkipPerLine���ص�sprite����
					mov		eax, edx
					mov		edx, nSprSkipPerLine
					or		eax, eax
					jnz		_DrawPartLineSection_LineSkipLocal_CheckAlpha_

					_DrawPartLineSection_LineSkipLocal_:
					{
						read_alpha_2_ebx_run_length_2_eax
						
						_DrawPartLineSection_LineSkipLocal_CheckAlpha_:
						or		ebx, ebx
						jnz		_DrawPartLineSection_LineSkipLocal_Alpha_
						sub		edx, eax
						jg		_DrawPartLineSection_LineSkipLocal_
						neg		edx
						jmp		_DrawPartLineSection_Line_

						_DrawPartLineSection_LineSkipLocal_Alpha_:
						{
							add		esi, eax
							sub		edx, eax
							jg		_DrawPartLineSection_LineSkipLocal_
							add		esi, edx
							neg		edx
							jmp		_DrawPartLineSection_Line_
						}
					}
				}

				_DrawPartLineSection_LineLocal_Alpha_:
				{
					cmp		eax, edx
					jnl		_DrawPartLineSection_LineLocal_Alpha_Part_		//����ȫ����eax����ͬalphaֵ����㣬�����е��Ѿ���������

					push	eax
					mov		ecx, eax
					cmp		ebx, 200
					jl		_DrawPartLineSection_LineLocal_HalfAlpha_
						
					//_DrawPartLineSection_LineLocal_DirectCopy_:
					{
						mov     ebx, pPalette
						_DrawPartLineSection_CopyPixel_:
						{
							copy_pixel_use_eax
							loop	_DrawPartLineSection_CopyPixel_
						}						
						pop		eax
						sub		edx, eax
						jmp		_DrawPartLineSection_LineLocal_
					}
					
					_DrawPartLineSection_LineLocal_HalfAlpha_:
					{
						push	edx
						_DrawPartLineSection_HalfAlphaPixel_:
						{
							mix_2_pixel_color_use_eabdx
							loop	_DrawPartLineSection_HalfAlphaPixel_
						}
						pop		edx
						pop		eax
						sub		edx, eax
						jmp		_DrawPartLineSection_LineLocal_
					}
				}

				_DrawPartLineSection_LineLocal_Alpha_Part_:
				{
					push	eax
					mov		ecx, edx
					cmp		ebx, 200
					jl		_DrawPartLineSection_LineLocal_HalfAlpha_Part_
						
					//_DrawPartLineSection_LineLocal_DirectCopy_Part_:
					{
						mov     ebx, pPalette
						_DrawPartLineSection_CopyPixel_Part_:
						{
							copy_pixel_use_eax
							loop	_DrawPartLineSection_CopyPixel_Part_
						}						
						pop		eax
				
						dec		Clipper.height
						jz		_EXIT_WAY_

						sub		eax, edx
						mov		edx, eax
						mov		ebx, 255	//�����Ҫȷ�е�ԭebx(alpha)ֵ������ǰͷpush ebx���˴�pop���
						jmp		_DrawPartLineSection_LineSkip_
					}
					
					_DrawPartLineSection_LineLocal_HalfAlpha_Part_:
					{
						push	edx
						_DrawPartLineSection_HalfAlphaPixel_Part_:
						{
							mix_2_pixel_color_use_eabdx
							loop	_DrawPartLineSection_HalfAlphaPixel_Part_
						}
						pop		edx
						pop		eax
						dec		Clipper.height
						jz		_EXIT_WAY_
						sub		eax, edx
						mov		edx, eax
						mov		ebx, 128
						jmp		_DrawPartLineSection_LineSkip_//�����Ҫȷ�е�ԭebx(alpha)ֵ������ǰͷpush ebx���˴�pop���
					}
				}
			}
		}
		_EXIT_WAY_:
	}
}