#ifndef __KWEATHERMGR_H__
#define __KWEATHERMGR_H__

#define WEATHERID_NOTHING	255

#ifdef _SERVER

#define MAX_WEATHER_NUM		4				// һ�ŵ�ͼ����������������Ŀ

struct KWeatherInfo
{
	int		m_weatherID;		// ����ID
	int		m_nOdds;			// ������������
	int		m_nLifeTimeMin;		// ������С����ʱ�䣬��λΪ��Ϸ֡��
	int		m_nLifeTimeMax;		// ����������ʱ�䣬��λΪ��Ϸ֡��
};

class KWeatherMgr
{
public:
public:
	KWeatherMgr();
	// ��.wor��weather.ini�ļ���ȡ��Ϣ����ɳ�ʼ��
	// IniFile:�Ѿ��򿪵�.wor�ļ�
	bool InitFromIni(KIniFile &IniFile);
	// ���ص�ǰ����ID
	int Activate();
private:
	bool			m_bInWeather;					// �Ƿ�������״̬��
	int				m_nWeatherNum;					// ����������Ŀ
	KWeatherInfo	m_weatherInfo[MAX_WEATHER_NUM];	// ������Ϣ����
	int				m_nFullOdds;					// ��Ӧ100%�ĸ���ֵ
	int				m_nCurWeather;					// ��ǰ����
	int				m_nHappenTimeMin;				// �����������ʱ����
	int				m_nHappenTimeMax;				// �����������ʱ����
	int				m_nLastHappentime;				// ��һ�������ķ���ʱ��
	int				m_nNextHappentime;				// ��һ�������ķ���ʱ��
	int				m_nWeatherDuration;				// ��������ʱ��
private:
};

#endif

#endif
