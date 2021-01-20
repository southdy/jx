// D3D_Device.h
//	Contains the active D3D Device. CD3D_Shell & CD3D_Device are the main containers of D3D shtuff...

#ifndef __D3D_DEVICE_H__
#define __D3D_DEVICE_H__

#include <d3d9.h>

// PROTOTYPES
struct D3DAdapterInfo;
struct D3DDeviceInfo;
struct D3DModeInfo;

class CD3D_Device {
public:
	CD3D_Device()			{ Reset(); }
	~CD3D_Device()			{ FreeAll(); }

	// Creation/Destruction Routines...
	// �����豸
	bool					CreateDevice(D3DAdapterInfo* pAdapter,D3DDeviceInfo* pDevice,D3DModeInfo* pMode);
	bool					SetMode(D3DModeInfo* pMode);
	// �ͷ��豸 (and resets all that should be reset on device release)...
	void					FreeDevice();
	// �ͷ������豸����
	bool					ReleaseDevObjects();
	// �ָ��豸���󣬵����豸�����ReCreateObject����
	bool					RestoreDevObjects();
	// �����豸��ʼ״̬ (���ͷ��κζ���)...
	void					Reset();
	// �����豸������� (�ֲ����� - ������ ALT-Tab �����Ĳ���)...
	void					ResetDeviceVars();
	// �ͷ����г�Ա������Ȼ������
	void					FreeAll();

	// ״̬���úͶ�ȡ����
	D3DAdapterInfo*			GetAdapterInfo()	{ return m_pAdapter; }
	D3DDeviceInfo*			GetDeviceInfo()		{ return m_pDevice; }
	D3DModeInfo*			GetModeInfo()		{ return m_pMode; }
	void					SetDefaultRenderStates();
	void					SetupViewport(uint32 iLeft, uint32 iRight, uint32 iTop, uint32 iBottom, float fMinZ = 0.0f, float fMaxZ = 1.0f);

	// ��Ⱦ����
	// ��ʼ��Ⱦһ֡ǰ����
	static bool				Start3D();
	// һ֡��Ⱦ��ɺ����
	static bool				End3D();
	static bool				IsIn3D();
	bool					ResetDevice();

	// Cap info (some precomputed caps)...
	D3DCAPS9*				GetDeviceCaps()		{ return &m_DeviceCaps; }

	D3DVIEWPORT9			GetViewport() { return m_viewportData; }

	// Debug/Helper Functions...
	void					ListDeviceCaps();	// ConsolePrint the device caps...

	// ע��: m_pD3DDevice ��Ϊ���г�Ա��Ϊ�˿��ٷ���
    LPDIRECT3DDEVICE9		m_pD3DDevice;		// D3D ��Ⱦ�豸
private:
//	D3DFORMAT				GetDefaultDepthStencilFormat(uint32 iZBitDepth,uint32 iStencilBitDepth);
	D3DMULTISAMPLE_TYPE		GetDefaultMultiSampleType(uint32 Samples);
	void					SetPresentationParams(D3DPRESENT_PARAMETERS& PresentationParam,D3DModeInfo* pMode);

	// Device Info/Data...
	D3DPRESENT_PARAMETERS	m_PresentationParam;
	D3DCAPS9				m_DeviceCaps;		// �����豸����
	D3DAdapterInfo*			m_pAdapter;			// My Adaptor Info Pointer (Points into g_D3DShell's device list)
    D3DDeviceInfo*			m_pDevice;			// My Device Info Pointer (Points into g_D3DShell's device list)
    D3DModeInfo*			m_pMode;			// My Mode Info Pointer (Points into g_D3DShell's mode list)
    bool					m_bWindowed;
	RECT					m_rcViewport;		// Cache the viewport...
	D3DVIEWPORT9			m_viewportData;

	bool					m_bIn3D;			// Are we in between the Start3D and End3D calls...
};

extern CD3D_Device g_Device;					// The global D3D Device...
#define PD3DDEVICE (g_Device.m_pD3DDevice)		// Use for quick access to the D3DDevice...
static LPDIRECT3DDEVICE9 d3d_GetD3DDevice() { return PD3DDEVICE; }	// For the RenderStruct...

#endif