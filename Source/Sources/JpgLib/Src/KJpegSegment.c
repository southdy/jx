//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KJpegSegment.cpp
// Date:	2000.08.08
// Code:	Daniel Wang
// Desc:	Jpeg ��ȡ Segment
// From:	Cloud Wu's JPEG Decoder
//---------------------------------------------------------------------------
#include <windows.h>
#include "KJpegLib.h"
//---------------------------------------------------------------------------
// ����:	skip_SEG
// ����:	����һ�������ݵĶ�
// ����:	stream		Jpeg������
// ����:	PBYTE		Jpeg������
//---------------------------------------------------------------------------
PBYTE jpeg_skip_SEG(PBYTE stream)
{
	PBYTE stream_end;
	WORD  seg_size;

	READ_WORD(seg_size, stream);
	stream_end = stream + seg_size - 2;
	return stream_end;
}
/****************************************************************************
SOF0: Start Of Frame 0:
~~~~~~~~~~~~~~~~~~~~~~~
  - $ff, $c0 (SOF0)
  - ���� (���ֽ�, ���ֽ�), 8+components*3
  - ���ݾ��� (1 byte) ÿ������λ��, ͨ���� 8 (����������֧�� 12 �� 16)
  - ͼƬ�߶� (���ֽ�, ���ֽ�), �����֧�� DNL �ͱ��� >0
  - ͼƬ��� (���ֽ�, ���ֽ�), �����֧�� DNL �ͱ��� >0
  - components ����(1 byte), �Ҷ�ͼ�� 1, YCbCr/YIQ ��ɫͼ�� 3, CMYK ��ɫͼ
    �� 4
  - ÿ�� component: 3 bytes
     - component id (1 = Y, 2 = Cb, 3 = Cr, 4 = I, 5 = Q)
     - ����ϵ�� (bit 0-3 vert., 4-7 hor.)
     - quantization table ��
****************************************************************************/
//---------------------------------------------------------------------------
// ����:	read_SOF
// ����:	read start of frame
// ����:	stream		Jpeg������
// ����:	PBYTE		Jpeg������
//---------------------------------------------------------------------------
PBYTE jpeg_read_SOF(PBYTE stream)
{
	PBYTE stream_end;
	WORD seg_size;
	BYTE id,q,i;
	WORD w;

	READ_WORD(seg_size, stream);
	stream_end = stream + seg_size - 2;

	jpeg_head.colordepth = READ_BYTE(stream);
	READ_WORD(w, stream);
	jpeg_head.height = w;
	READ_WORD(w, stream);
	jpeg_head.width = w;

	if ((jpeg_head.components = READ_BYTE(stream)) > 3) 
		return NULL; // ��֧�� 3 ���������

	for (i = 0; i < jpeg_head.components; i++)
	{
		if ((id=READ_BYTE(stream)) > 3)
			return NULL; //��֧�ֵ�ģʽ
		if (--id < 0)
			return NULL;
		q = READ_BYTE(stream);
		if ((jpeg_head.component[id].v = q & 0xf ) > 2) 
			return NULL; // ��֧�ֳ��� 2 �Ĳ�����
		if ((jpeg_head.component[id].h = q >> 4) > 2)
			return NULL; // ��֧�ֳ��� 2 �Ĳ�����
		jpeg_head.component[id].qtb = READ_BYTE(stream) & 3;
	}
	return stream_end;
}

/****************************************************************************
SOS: Start Of Scan:
~~~~~~~~~~~~~~~~~~~
  - $ff, $da (SOS)
  - ���� (���ֽ�, ���ֽ�), ������ 6+2*(ɨ���������������)
  - ɨ��������������� (1 byte), ���� >= 1 , <=4 (�����Ǵ��) ͨ���� 3
  - ÿ�����: 2 bytes
     - component id (1 = Y, 2 = Cb, 3 = Cr, 4 = I, 5 = Q), �� SOF0
     - ʹ�õ� Huffman ��:
	- bit 0..3: AC table (0..3)
	- bit 4..7: DC table (0..3)
  - ���� 3 bytes (???)
****************************************************************************/
//---------------------------------------------------------------------------
// ����:	read_SOS
// ����:	read start of scan
// ����:	stream		Jpeg������
// ����:	PBYTE		Jpeg������
//---------------------------------------------------------------------------
PBYTE jpeg_read_SOS(PBYTE stream)
{
	PBYTE stream_end;
	WORD seg_size;
	BYTE i,id,com,q;

	READ_WORD(seg_size, stream);
	stream_end = stream + seg_size - 2;

	if ((com = READ_BYTE(stream)) != jpeg_head.components)
		return NULL; // �����Ŀ����

	for (i = 0; i < com; i++)
	{
		if ((id = READ_BYTE(stream)) > 3)
			return NULL; //��֧�ֵ�ģʽ
		if (--id < 0)
			return NULL;
		q = READ_BYTE(stream);
		jpeg_head.component[id].act = (q & 3) | 4;
		jpeg_head.component[id].dct = (q >> 4) & 3;
	}
	return stream_end;
}
//---------------------------------------------------------------------------
