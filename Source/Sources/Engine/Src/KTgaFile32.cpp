//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KTgaFile32.cpp
// Date:	2000.08.08
// Code:	Daniel Wang
// Desc:	24bit TGA file class
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KMemClass.h"
#include "KColors.h"
#include "KPakFile.h"
#include "KTgaFile32.h"
//---------------------------------------------------------------------------
// ����:	Load
// ����:	��TGA�Ŀ�
// ����:	lpFileName	�ļ���
// ����:	TRUE		�ɹ�
//			FALSE		ʧ��
//---------------------------------------------------------------------------
BOOL KTgaFile32::Load2Buffer(LPSTR lpFileName)
{
	int					i, j;
	TGAFILEHEADER		FileHeader;
	KPakFile			File;
	KMemClass			Buffer;
	
	// open the file
	if (!File.Open(lpFileName))
		return FALSE;

	// read file head
	File.Read(&FileHeader, sizeof(FileHeader));

	if (FileHeader.ColorMapType)
	{
		if((FileHeader.ImageType != 2 && FileHeader.ImageType != 10) 
			|| FileHeader.ColorMapSpec[4] != 32)
		{
			return FALSE;	// ��32λɫ��TGA����TGA
		}
	}

	if (FileHeader.PixelDep != 32)
	{
		return FALSE;	// ��32λTGA
	}

	if(FileHeader.IDLength)
	{
		File.Seek(FileHeader.IDLength, FILE_CURRENT);
	}

	if(FileHeader.ColorMapType)
	{
		i= (FileHeader.ColorMapSpec[4] + 1) / 8 * (*(WORD*)(&FileHeader.ColorMapSpec[2]));
		File.Seek(i,FILE_CURRENT);
	}
	i=FileHeader.Width*FileHeader.Height*4;
	
	Buffer.Alloc(i);
	if(FileHeader.ImageType == 2)
	{
		int nReadSize = File.Read(Buffer.GetMemPtr(), i);
		if (nReadSize != i)
			return FALSE;	// �����ļ��д�
	}
	else
	{
		BYTE	TempData[4] ;
		int		TempState =0, TempCount =0 ;
		BYTE	*ppData ;
		ppData = (BYTE*)Buffer.GetMemPtr();
		do
		{
			j = File.Read( TempData, (TempState==0)?1:4 );
			if( j != ((TempState==0)?1:4) )
			{
				return FALSE;// �ļ��д�
			}
			
			if( !TempState )
			{
				TempState = (TempData[0] & 0x80)?1:2 ;
				TempCount = TempData[0] & 0x7F ;
			}
			else
			{
				if( TempState == 1 )	// RLE ѹ��
				{
					for(; TempCount>=0 ; TempCount -- )
					{
						*(ppData++) = TempData[0];
						*(ppData++) = TempData[1];
						*(ppData++) = TempData[2];
						*(ppData++) = TempData[3];
					}
				}
				else		// NON-RLE ѹ��
				{
					*(ppData++) = TempData[0];
					*(ppData++) = TempData[1];
					*(ppData++) = TempData[2];
					*(ppData++) = TempData[3];
					TempCount -- ;
				}
				
				if( TempCount < 0 )	
				{
					TempCount = 0 ;
					TempState = 0 ;
				}
			}
		} while( ppData < (BYTE*)Buffer.GetMemPtr() + i ) ;
	}
	m_nWidth	= FileHeader.Width;
	m_nHeight	= FileHeader.Height;

	// ��TGA��ʽ��BufferתΪ��׼��ʽ����Ϊ��B G R A��
	BYTE	*pTemp1, *pTemp2, *pTmp1;
	m_Buffer.Alloc(m_nWidth * m_nHeight * 4);

	pTemp1 = (BYTE*)Buffer.GetMemPtr();
	pTemp2 = (BYTE*)m_Buffer.GetMemPtr();

	int flag = (FileHeader.Desc&0x30)>>4;
	switch(flag)
	{
	case 0:	// ÿ�б�����˳���������ǵ���
		pTemp1 += m_nWidth * m_nHeight * 4;
		for (j = 0; j < m_nHeight; j++)
		{
			pTemp1 -= m_nWidth * 4;
			pTmp1 = pTemp1;
			for (i = 0; i < m_nWidth; i++)
			{
				pTemp2[0]	= pTmp1[2];
				pTemp2[1]	= pTmp1[1];
				pTemp2[2]	= pTmp1[0];
				pTemp2[3]	= pTmp1[3];
				pTmp1	+= 4;
				pTemp2	+= 4;
			}
		}
		break;
	case 1:	// ���е㶼�ǵ���
		pTemp1 += m_nWidth * m_nHeight * 4;
		for (i = 0; i < m_nWidth * m_nHeight; i++)
		{
			pTemp1 -= 4;
			pTemp2[0]	= pTemp1[2];
			pTemp2[1]	= pTemp1[1];
			pTemp2[2]	= pTemp1[0];
			pTemp2[3]	= pTemp1[3];
			pTemp2 += 4;
		}
		break;
	case 2:	// ���е㶼������
		for (i = 0; i < m_nWidth * m_nHeight; i++)
		{
			pTemp2[0]	= pTemp1[2];
			pTemp2[1]	= pTemp1[1];
			pTemp2[2]	= pTemp1[0];
			pTemp2[3]	= pTemp1[3];
			pTemp1	+= 4;
			pTemp2	+= 4;
		}
		break;
	case 3:	// �����������ģ�ÿ�б���ĵ��ǵ���
		for (j = 0; j < m_nHeight; j++)
		{
			pTemp1 += m_nWidth * 4;
			pTmp1 = pTemp1;
			for (i = 0; i < m_nWidth; i++)
			{
				pTmp1 -= 4;
				pTemp2[0]	= pTmp1[2];
				pTemp2[1]	= pTmp1[1];
				pTemp2[2]	= pTmp1[0];
				pTemp2[3]	= pTmp1[3];
				pTemp2 += 4;
			}
		}
		break;
	default:
		break;
	}
	return TRUE;
}