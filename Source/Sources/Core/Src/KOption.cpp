#include "KCore.h"
#include "KSubWorldSet.h"
#include "KOption.h"
#include "../../Represent/iRepresent/iRepresentshell.h"

extern struct iRepresentShell*	g_pRepresent;

#ifndef _SERVER
KOption	Option;

void	KOption::SetGamma(int nGamma)
{
	m_nGamma = nGamma;
	g_pRepresent->SetGamma(nGamma);
}

void KOption::SetSndVolume(int nSndVolume)
{
	if (nSndVolume <= 3)
		m_nSndVolume = 0;
	else if (nSndVolume >= 100)
		m_nSndVolume = 100;
	else if (nSndVolume >= 40)
		m_nSndVolume = 80 + (nSndVolume - 50) * 2 / 5;
	else
		m_nSndVolume = 40 + nSndVolume;
}

//KMapMusic::SetGameVolume modify by wooy
/*����������Ļ�����������������ǿ�ȷ�ΧΪ0dB-140dB(Ҳ������Ϊ�� -5dB��130dB).��Ȼ�������˶���
����Ƶ�ʷ�Χ����Ƶ�򣩵���������ʹ����ٴ��˶�Ҳ���������������Ϊ�㣩�������˶��Ŀ���Ƶ���ڣ�
������������ǿ��һ���̶ȣ��˶�ͬ�����������ġ��������������˶��ոտ�������ʱ����ʱ������ǿ��
��Ϊ�����С���һ����1kHz����Ϊ׼���в������˶�������������ѹΪ0dB(һ�����0.3dB���и���),
��ʱ��������ȼ���Ϊ�㷽������������ǿ��ʹ�˶��е���ʹʱ�������ֵ��Ϊ��ʹ�С�������1kHz����Ϊ׼
�����в�����ʹ�˶��е���ʹʱ��ǿ��Լ�ﵽ140dB���ҡ�ʵ����������к�ʹ��������ѹ��Ƶ�ʱ仯�ġ�
���к�ʹ����Ƶ�ʱ仯�ĵ�������ߣ�����������ѷ���ߣ�֮�����������˶���������Χ��ͨ����Ϊ��
����1kHz������0dB-20dBΪ��������30dB-40dBΪ΢������50dB-70dBΪ��������80dB-100dBΪ��������
110dB-130dBΪ�������������1kHz�������������ͬһ�����������������������Ч����ѹ-Ƶ��ֵ,
���磬200Hz��30dB��������1kHz��10dB���������˶�������ȴ������ͬ����ȡ�С��0dB���кʹ���140ʹ��ʱ
Ϊ������������ʹ���˶�������Ƶ�ʷ�Χ���������˶�Ҳ���첻����*/
void KOption::SetMusicVolume(int nMusicVolume)
{
	if (nMusicVolume <= 3)
		m_nMusicVolume = 0;
	else if (nMusicVolume >= 100)
		m_nMusicVolume = 100;
	else if (nMusicVolume >= 40)
		m_nMusicVolume = 80 + (nMusicVolume - 50) * 2 / 5;
	else
		m_nMusicVolume = 40 + nMusicVolume;

	g_SubWorldSet.m_cMusic.SetGameVolume(m_nMusicVolume);
}

#endif