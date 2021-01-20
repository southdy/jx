/*****************************************************************************************
//	�������棬���洰����ϵ�ṹ����������ڶ���
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-9
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

//============���ڷ��================
#define WND_S_VISIBLE				0x80000000	//�ɼ�
#define WND_S_DISABLED				0x40000000	//���ɲ���
#define	WND_S_TOPMOST				0x20000000	//�ö�����
#define	WND_S_MOVEALBE				0x10000000	//�ɱ��϶�
#define	WND_S_DISABLE				0x04000000	//���ڲ��ɱ�����
#define	WND_S_TOBEDESTROY			0x02000000	//window is to be destroy
#define	WND_S_MOVE_WITH_R_EDGE		0x01000000	//���游�������±߽�ı仯�ƶ�����
#define	WND_S_MOVE_WITH_B_EDGE		0x00800000	//���游�������±߽�ı仯�ƶ�����
#define	WND_S_SIZE_WITH_R_EDGE		0x00400000	//���游���ڴ�С�ı仯�������ڴ�С
#define	WND_S_SIZE_WITH_B_EDGE		0x00200000	//���游���ڴ�С�ı仯�������ڴ�С
#define	WND_S_SIZE_WITH_ALL_CHILD	0x00100000	//���Ӵ��ڵķ�Χ���Եļ�����Ϊ�Լ��ķ�Χ���Եļ���

extern	int WND_SHOW_DEBUG_FRAME_TEXT;

class KIniFile;

class KWndWindow
{
protected:
	//----���ڲ���----
	unsigned int m_Style;			//���ڷ��,��Wnd.h�д��ڷ��Ķ���
	int			m_Left;				//���Ͻ�X���꣬����ڸ�����
	int			m_Top;				//���Ͻ�Y���꣬����ڸ�����
	int			m_Width;			//���
	int			m_Height;			//�߶�
	int			m_nAbsoluteLeft;	//�������Ͻǵľ�������x
	int			m_nAbsoluteTop;		//�������Ͻǵľ�������y
#ifdef _DEBUG
	char		m_Caption[32];		//��������
#endif

	int			m_bMoving;
	int			m_nLastMouseHoldPosX;
	int			m_nLastMouseHoldPosY;

	//----���������ڵļ�����ϵ----
	KWndWindow*	m_pPreviousWnd;		//ǰһ��ͬ������
	KWndWindow*	m_pNextWnd;			//��һ��ͬ������
	KWndWindow*	m_pFirstChild;		//��һ���Ӵ���
	KWndWindow*	m_pParentWnd;		//������

public:
	KWndWindow();
	virtual	~KWndWindow();

	void	Destroy();
	virtual void UpdateData(){}

	//====�����صĺ���====
	// flying add this function
	virtual int		CloseWindow(bool bDestory){return 0;};

	virtual int		Init(KIniFile* pIniFile, const char* pSection);//��ʼ��
	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
	virtual int		PtInWindow(int x, int y);				//�ж�һ�����Ƿ��ڴ��ڷ�Χ��,������Ǿ�������

	virtual void	PaintWindow();							//�������
	void			PaintDebugInfo();

	virtual void	SetSize(int nWidth, int nHeight);		//���ô��ڴ�С

	//====������Ϊ����====
	void			BringToTop();							//�Ѵ����ö�
	void			GetPosition(int* pLeft, int* pTop);		//��ȡ����λ�ã��������
	void			SetPosition(int nLeft, int nTop);		//���ô���λ�ã��������
	void			GetAbsolutePos(int* pLeft, int* pTop);	//��ȡ����λ�ã���������
	void			GetSize(int* pWidth, int* pHeight);		//��ȡ���ڴ�С
	void			SetCursorAbove();						//ʹ���ָ�����ƶ��������ڴ˴����е�λ����
	void			GetAllChildLayoutRect(RECT* pRect);		//ȡ�ð��������Ӵ��ڷֲ��������С����

	virtual void	Show();									//��ʾ����
	virtual void	Hide();									//���ش���
	int				IsVisible();							//�жϴ����Ƿ���ʾ
	virtual	void	Enable(int bEnable);					//��ֹ��������ʹ���ڱ�����
//	void			SetStyle(DWORD	dwStyle);				//�޸Ĵ��ڷ��
	void			AddChild(KWndWindow* pChild);			//����Ӵ���
	void			AddBrother(KWndWindow* pBrother);		//����ֵܴ���

	virtual KWndWindow*	TopChildFromPoint(int x, int y);	//�õ�����ָ������λ�õ����ϲ㴰�ڣ����������Ϊ��������

	KWndWindow*		GetPreWnd() const { return m_pPreviousWnd; }	//�õ�ǰһ���ֵܴ���
	KWndWindow*		GetNextWnd() const { return m_pNextWnd; }		//�õ���һ���ֵܴ���
	KWndWindow*		GetParent() const { return m_pParentWnd; }		//�õ�������
	KWndWindow*		GetFirstChild() const { return m_pFirstChild; }	//�õ���һ���Ӵ���
	KWndWindow*		GetOwner();								//��ò����и����ڵ����ȴ���

	void			Paint();								//����
	void			LetMeBreathe();							//�ô��ڻ

	void			SplitSmaleFamily();						//���Լ������Ӵ��ڣ��Ӵ�������������������
	void			LeaveAlone();							//�������޴��������ң�һ��ǣ��

	//----�������ã���ȡ----
#ifdef _DEBUG
	void			SetCaption(char* pszCaption);
#endif
	int				IsDisable() { return (m_Style & WND_S_DISABLE); }
	int				GetStyle() { return m_Style; }
	int				SetStyle(unsigned int nStyle)
	{
		m_Style = nStyle;
		return m_Style; 
	}
	
protected:
	void			Clone(KWndWindow* pCopy);

private:
	virtual void	Breathe() {}							//���ڵĳ�����Ϊ
	void			OnLBtnDown(int x, int y);				//��Ӧ���������µĲ��������������Ϊ��������
	void			OnMoveWnd();							//��Ӧ�����������ƶ��Ĳ��������������Ϊ��������

private:
	void			AbsoluteMove(int dx, int dy);			//��������ĵ���
};

//���ַ�����ʾ����ɫ��ϢתΪ��ֵ��ʾ
unsigned int	GetColor(const char* pString);
//����ֵ��ʾ����ɫ��ϢתΪ�ַ�����ʾ
const char* GetColorString(unsigned int nColor);
