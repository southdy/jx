/*****************************************************************************************
//	����video����
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-7-14
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

#include "../Elem/WndWindow.h"
#include "..\..\..\KLVideo\KLVideo.h"

typedef u8 (*KLVideoSoundFn)(void PTR4* dd);
typedef HKLVIDEO (*KLVideoOpenFn)(const char PTR4* name,u32 flags, void PTR4* hWnd);
typedef void (*KLVideoCloseFn)(HKLVIDEO bnk);
typedef u32 (*KLVideoWaitFn)(HKLVIDEO bnk);
typedef void (*KLVideoToBufferFn)(HKLVIDEO bnk,void PTR4* buf,u32 left,u32 top,u32 Pitch,u32 destheight,u32 Flags);
typedef u32 (*KLVideoDoFrameFn)(HKLVIDEO bnk);
typedef void (*KLVideoNextFrameFn)(HKLVIDEO bnk);
typedef void (*KLVideoSetSndVolumeFn)(HKLVIDEO bnk,u32 nVolume);
typedef void (*KLVideoSndtrackFn)(u32 track);
typedef u32 (*KLVideoGetCurrentFrameFn)(HKLVIDEO bnk);

class KUiPlayVideo : protected KWndWindow
{
public:
	//----�������ͳһ�Ľӿں���----
	static KUiPlayVideo*	OpenWindow();							//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static void				CloseWindow(bool bDestroy);				//�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void				LoadScheme(const char* pszSettingFile, const char* pszSection);	//������淽��
	
	void					Setting(bool bProcessInput, bool bCloseWhenOver,
								KWndWindow* pCaller, unsigned int uParam = 0);
	int						OpenVideo(const char* pszVideoFile);
	void					CloseVideo(bool bFinished = true);
	void					SetPosition(int nLeft, int nTop);		//���ô���λ�ã��������
	void					SetSize(int nWidth, int nHeight);		//���ô��ڴ�С

	KUiPlayVideo();

private:
	//�����
	void	Breathe();
	//���봦����Ϣ�Լ�һЩ�ض�������Ϣ����Ӧ
	int	WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	//����
	void	PaintWindow();
	//��ʼ��
	int Initialize();
	//�ر�
	void Terminate();
	//���Ž���
	void OnPlayFinished();

	~KUiPlayVideo() {}
private:
	static KUiPlayVideo* m_pSelf;
	KWndWindow*		m_pCaller;
	unsigned int	m_uCallerParam;
	bool			m_bProcessInput;
	bool			m_bCloseWhenOver;

	bool			m_bInitialized;

	HKLVIDEO				  m_bnk;						//���ڲ��ŵ�Video���
	HMODULE					  m_hVideoDll;					//KLVideo.dll��ģ����
	KLVideoSoundFn            m_KLVideoSoundFn;
    KLVideoSndtrackFn         m_KLVideoSndtrackFn;
    KLVideoOpenFn             m_KLVideoOpen;
    KLVideoCloseFn            m_KLVideoClose;
    KLVideoWaitFn             m_KLVideoWait;
    KLVideoToBufferFn         m_KLVideoToBuffer;
    KLVideoDoFrameFn          m_KLVideoDoFrame;
    KLVideoNextFrameFn        m_KLVideoNextFrame;
	KLVideoGetCurrentFrameFn  m_KLVideoGetCurrentFrame;
	KLVideoSetSndVolumeFn	  m_KLVideoSetSoundVolume;

	char					  m_szBitmapName[16];
	unsigned int			  m_uBitmapId;
	short					  m_uBitmapIsPosition;

	int			m_nBlackWidth;
	int			m_nBlackHeight;
	int			m_nbAlreadyPaintBlack;
};
