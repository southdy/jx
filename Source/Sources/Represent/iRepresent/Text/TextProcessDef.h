#pragma once

typedef	int (*fnTextProcessEncodePlugin)();
typedef	int (*fnTextProcessDrawPlugin)();

struct KOutputTextParam
{
	int	nX;
	int	nY;
	int	nZ;
	short 	nSkipLine;
	short 	nNumLine;
	unsigned int Color;
	unsigned int BorderColor;
	unsigned short nVertAlign;	//0:����/1:�϶���/2:�¶���
	int bPicPackInSingleLine;

	//��һ�����캯��,��Ҫ��nVertAlign�ĳ�ʼ��[wxb 2003-7-10]
	KOutputTextParam()
	{
		nX = nY = nZ = 0;
		nSkipLine = nNumLine = 0;
		Color = 0;
		nVertAlign = 0;	//ȱʡΪ����
		bPicPackInSingleLine = false;
	}
};