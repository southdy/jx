//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KWavCodec.cpp
// Date:	2000.08.08
// Code:	Daniel Wang
// Desc:	Wave File Loading Class
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KMemBase.h"
#include "KWavCodec.h"
//---------------------------------------------------------------------------
#define STRICT_IMA
#define ISSTMAX 88
#define imaStateAdjust(c) (((c) < 4) ? -1 : (2 * (c) - 6))
//---------------------------------------------------------------------------
static BYTE imaStateAdjustTable[ISSTMAX + 1][8];
static const int imaStepSizeTable[ISSTMAX + 1] = {
	7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31, 34,
	37, 41, 45, 50, 55, 60, 66, 73, 80, 88, 97, 107, 118, 130, 143,
	157, 173, 190, 209, 230, 253, 279, 307, 337, 371, 408, 449, 494,
	544, 598, 658, 724, 796, 876, 963, 1060, 1166, 1282, 1411, 1552,
	1707, 1878, 2066, 2272, 2499, 2749, 3024, 3327, 3660, 4026,
	4428, 4871, 5358, 5894, 6484, 7132, 7845, 8630, 9493, 10442,
	11487, 12635, 13899, 15289, 16818, 18500, 20350, 22385, 24623,
	27086, 29794, 32767
};
//---------------------------------------------------------------------------
static void ImaInitTable();
static void ImaDecodeBlock(PBYTE lpIma, short* lpPcm, int nBlockAlign);
static void ImaDecodeSample(
   int ch,           /* channel number to decode, REQUIRE 0 <= ch < chans  */
   int chans,        /* total channels             */
   PBYTE ibuff,      /* input buffer[blockAlign]   */
   short* obuff,     /* obuff[n] will be output samples */
   int n,            /* samples to decode PER channel, REQUIRE n % 8 == 1  */
   int o_inc);       /* index difference between successive output samples */
//---------------------------------------------------------------------------
// ����:	ImaInitTable
// ����:	��ʼ�� imaStateAdjustTable
// ����:	void 
// ����:	void
//---------------------------------------------------------------------------
static void ImaInitTable()
{
	static BOOL bInited = FALSE;
	if (bInited)
		return;

	int i, j, k;
	for (i = 0; i <= ISSTMAX; i++) 
	{
		for (j = 0; j < 8; j++)
		{
			k = i + imaStateAdjust(j);
			if (k < 0)
				k = 0;
			else if (k > ISSTMAX)
				k = ISSTMAX;
			imaStateAdjustTable[i][j] = k;
		}
	}
	bInited = TRUE;
}
//---------------------------------------------------------------------------
// ����:	g_ImaAdpcmDecode
// ����:	����IMA-ADPCM
// ����:	lpIn		IMA����
//			nInLen		���泤��
//			lpOut		PCM����
//			nBlockAlign	PCM���С 11.1K=512 22.1k=1024 44.1k=2048
// ����:	ʵ�ʶ�ȡ����
//---------------------------------------------------------------------------
DWORD g_ImaAdpcmDecode(PVOID lpIn, int nInLen, PVOID lpOut, int nBlockAlign)
{
	LPBYTE lpIma = (LPBYTE)lpIn;
	LPBYTE lpPcm = (LPBYTE)lpOut;
	int blocks = nInLen / nBlockAlign;

	ImaInitTable();
	for (int i = 0; i < blocks; i++)
	{
		ImaDecodeBlock(lpIma, (short*)lpPcm, nBlockAlign);
		lpIma += nBlockAlign;
		lpPcm += 4 * (nBlockAlign - 7);
	}
	return blocks * 4 * (nBlockAlign - 7);
}
//---------------------------------------------------------------------------
// ����:	ImaDecodeBlock 
// ����:	�ֱ��ѹÿBlock��������������
// ����:	lpIma	��IMA_ADPCM��ʽ�ļ���ȡ�����ݿ�
//			lpPcm	��ѹ������ݻ���
// ����:	void
//---------------------------------------------------------------------------
static void ImaDecodeBlock(PBYTE lpIma, short* lpPcm, int nBlockAlign)
{
	// left chanle decode
	ImaDecodeSample(0, 2, lpIma, lpPcm + 0, nBlockAlign - 7, 2);
	// rigth chanle decode
	ImaDecodeSample(1, 2, lpIma, lpPcm + 1, nBlockAlign - 7, 2);
}
//---------------------------------------------------------------------------
// ����:	ImaDecodeSample
// ����:	��ѹ��IMA_ADPCM��ʽWAV�ļ���BLOCK�е�����
// ����:    ch     ������
//			chans  ��������
//			ibuff  ����Ļ�������ַ,ӦΪblock���׿�ʼbuffer[blockAlign] 
//			obuff  �������
//          n      ÿ����ÿblock������ 16λӦΪ1017
//          o_inc  ���������
// ����:	void
//---------------------------------------------------------------------------
static void ImaDecodeSample(
	int ch,           /* channel number to decode, REQUIRE 0 <= ch < chans  */
	int chans,        /* total channels             */
	PBYTE ibuff,      /* input buffer[blockAlign]   */
	short* obuff,     /* obuff[n] will be output samples */
	int n,            /* samples to decode PER channel, REQUIRE n % 8 == 1  */
	int o_inc)        /* index difference between successive output samples */
{
   	const unsigned char* ip;
	int i_inc;
	short* op;
	int i, val, state;
	int step, dp, c, cm;
	
	ip = ibuff + 4 * ch; /* input pointer to 4-byte block state-initializer   */
	i_inc = 4 * (chans - 1); /* amount by which to incr ip after each 4-byte read */
	val = (short)(ip[0] + (ip[1] << 8)); /* need cast for sign-extend */
	state = ip[2];
	if (state > ISSTMAX)
		state = 0;

	/* specs say to ignore ip[3] , but write it as 0 */
	ip += 4 + i_inc;
	
	op = obuff;
	*op = val; /* 1st output sample for this channel */
	op += o_inc;
	
	for (i = 1; i < n; i++)
	{	
		if (i & 1)
		{	/* 1st of pair */ 
			cm = * ip & 0x0f; //���Ļ���ȡ����λ
		} 
		else
		{
			cm = (*ip++) >> 4; //ȡ����λ��������һ��
			if ((i & 7) == 0)  /* ends the 8-sample input block for this channel */
				ip += i_inc;   /* skip ip for next group */ 
		}
		
		step = imaStepSizeTable[state];
		/* Update the state for the next sample */
		c = cm & 0x07;
		state = imaStateAdjustTable[state][c];
		
#ifdef STRICT_IMA // �ϸ��IMA����
		dp = 0;
		if (c & 4) dp += step;
		step = step >> 1;
		if (c & 2) dp += step;
		step = step >> 1;
		if (c & 1) dp += step;
		step = step >> 1;
		dp += step;
#else
		/* faster than bit-test & add on my cpu */
		dp = ((c + c + 1) * step) >> 3;
#endif
		if (c != cm) 
		{
			val -= dp;
			if (val < -0x8000)
				val = -0x8000;
		}
		else 
		{
			val += dp;
			if (val > 0x7fff)
				val = 0x7fff;
		}
		*op = val;
		op += o_inc;
	}
}
//---------------------------------------------------------------------------

