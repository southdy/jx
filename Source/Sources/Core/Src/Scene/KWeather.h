
#ifndef _SERVER

#ifndef __KWEATHER_H__
#define __KWEATHER_H__

#include "..\GameDataDef.h"
#include "../../Represent/iRepresent/KRepresentUnit.h"
#include "SceneDataDef.h"
#include "../../Represent/iRepresent/iRepresentShell.h"

#pragma warning(disable : 4786)
#include <list>
#ifndef __CODEWARRIOR
using namespace std;
#endif

#define WEATHER_AREA_WIDTH		1200
#define WEATHER_AREA_HEIGHT		2000

struct KParticle
{
	KPosition3 m_vPos;				// ����λ��
	KPosition3 m_vSpeed;			// �����ٶ�
	KRColor	m_color;				// ������ɫ
};

class KWeather
{
public:

public:
	KWeather();
	virtual ~KWeather();
	void SetParticleNum(int nNum)
	{
		if(nNum < 0)
			return;
		m_nParticleNum = nNum;
	}

	void SetDownSpeed(float nSpeed)
	{
		m_nDownSpeed = nSpeed;
	}

	void SetWindSpeed(float nSpeed)
	{
		m_nWindSpeed = nSpeed;
	}

	void SetLifeTime(int nTime)
	{
		if(nTime < 0)
			return;
		m_nLifeTime = nTime * 20;
	}

	void SetFocusPos(int nX, int nY)
	{
		m_nFocusPosX = nX;
		m_nFocusPosY = nY;

		m_nLeftTopX = nX - WEATHER_AREA_WIDTH / 2;
		m_nLeftTopY = nY - WEATHER_AREA_HEIGHT / 2;
	}

	void ShutDown()
	{
		m_bIsShutingDown = true;
	}

	virtual bool IsShutDown() = 0;
	virtual bool ReadInfoFromIniFile(int nWeather) = 0;
	virtual void FilterAmbient(DWORD &dwLight){}
	virtual void Breath();
	virtual void Render(iRepresentShell *pRepresent) = 0;

protected:
	bool	m_bIsShutingDown;			// �Ƿ����ڹر�
	int		m_nStartTime;				// ������ʼʱ��,��Ϸ֡����
	int		m_nLifeTime;				// ����������ʱ�䣬��
	int		m_nParticleNum;				// ÿ�������������Ŀ
	float	m_nDownSpeed;				// �����ٶȣ�ÿ����Ϸ��λ��
	float	m_nWindSpeed;				// ������ף�ÿ����Ϸ��λ��

	int		m_nLeftTopX;				// �Ÿ������������Ͻ�X����
	int		m_nLeftTopY;				// �Ÿ������������Ͻ�Y����

	int		m_nFocusPosX;				// ��������X
	int		m_nFocusPosY;				// ��������Y

	list<KParticle> m_ParticleList;		// ��������
private:

};

class KWeatherRain : public KWeather
{
public:
	KWeatherRain();
	~KWeatherRain();

	void SetFlareTime(int nTime)
	{
		m_nFlareTime = nTime * 2 * 20;
	}
public:
	virtual bool IsShutDown()
	{
		return m_bIsShutDown;
	}
	virtual bool ReadInfoFromIniFile(int nWeatherID);
	virtual void FilterAmbient(DWORD &dwLight);
	virtual void Breath();
	virtual void Render(iRepresentShell *pRepresent);
private:
	int		m_nLenRate;					// ��㳤��Ϊ��󳤶ȵİٷֱ�
	bool	m_bIsShutDown;				// �Ƿ��Ѿ��ر�
	int		m_nFlareTime;				// ƽ������ʱ��һ�����磬��λΪ��Ϸ֡��
	int		m_nLastFlareTime;			// �ϴ�����ʱ�䣬��λΪ��Ϸ֡��
	int		m_nFlareDelay;				// ��һ���������һ�������ʱ�䣬��λΪ��Ϸ֡��
	int		m_nFlareState;				// ����׶Σ�Ϊ��1ʱΪ������
	int		m_nFlareStateStart;			// ����׶���ʼʱ�䣬��λΪ��Ϸ֡��
	int		m_nFlareStateDelay;			// ����׶�����ʱ�䣬��λΪ��Ϸ֡��
	KLColor m_flareColorAdd;			// ������׶ε����ڻ������ϵ�����
	float	m_fAmbientFactor;			// �������������

	char	m_szRainSound[80];			// ������
	char	m_szFlareSounds[3][80];		// ������
	KWavSound *m_pSound;
	int		m_nSoundVolume;				// ����������

private:
};

class KWeatherSnow : public KWeather
{
public:
	KWeatherSnow()
	{
		m_bIsShutDown = false;
	}

public:
	virtual bool IsShutDown()
	{
		return m_bIsShutDown;
	}
	virtual bool ReadInfoFromIniFile(int nWeatherID);
	virtual void Breath();
	virtual void Render(iRepresentShell *pRepresent);
private:
	bool	m_bIsShutDown;				// �Ƿ��Ѿ��ر�

private:
};

#endif

#endif