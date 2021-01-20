/*****************************************************************************************
//	�������棬���洰��Ԫ����ϵͷ�ļ�
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-9
------------------------------------------------------------------------------------------
	����Ԫ�أ��ؼ����Ĺ�ͬ����ΪKWndWindow��Wnd_ǰ׺��ͷ��һ�麯������ʹ�����γ�һ���󼯺ϣ�
���һЩ�ض��Ĺ��ܡ�
    ����ϵͳ֧���ȼ�(HotKey)���ܣ����ͬʱ֧��24���ȼ����ã���������ֵ�ɱ��޸ġ�
*****************************************************************************************/

class KWndWindow;
struct KUiDraggedObject;
struct iKWndGameSpace;

enum CURSOR_INDEX
{
	CURSOR_NORMAL = 0,
	CURSOR_POINT_TO_ATTACKABLE,	//�Ƶ�����Ŀ����
	CURSOR_POINT_TO_OBJ_NPC,	//�Ƶ�һ��OBJ��NPC��
	CURSOR_PICKABLE,			//��ʰȡ
	CURSOR_SKILL_ENABLE,		//�Ƶ���ʹ�����漼�ܵĽ�ɫ��������
	CURSOR_BUY,					//����
	CURSOR_SELL,				//������
	CURSOR_REPAIR,				//�޶���
    CURSOR_USE,                 //�������ͼ��
	CURSOR_INDEX_COUNT,
};

//--------------------------------------------------------------------------
//	���Ʊ������ݺ���ԭ��
//--------------------------------------------------------------------------
//	������	x, y -> ���ָ�뵱ǰ������λ��
//			Obj -> ���϶��Ķ�����Ϣ�ṹ
//			nDropQeuryResult -> ��ǰĿ��ط����صĲ�ѯ�ɷ���������Ķ���Ľ��
//	���أ���ʾ�����걻�϶�������֮���Ƿ�Ҫ�����������ָ�롣������ָ�뵥ǰ��������״̬�򣬴˺�������ֵ������
typedef int (*fnDrawDraggedObj)(int x, int y, const KUiDraggedObject& Obj, int nDropQueryResult);

enum WND_LAYER_LIST
{
	WL_LOWEST,		//��ײ�
	WL_NORMAL,		//һ��
	WL_TOPMOST,		//���
};

void		Wnd_Cleanup();			//�������������ȫ���Ĵ���

void		Wnd_GetScreenSize(int& nWidth, int& nHeight);//��ȡ��Ļ��Χ��С
void		Wnd_SetScreenSize(int nWidth, int nHeight);  //������Ļ��Χ��С
void		Wnd_RenderWindows();						//���ƽ���ϵͳ
void		Wnd_AddWindow(KWndWindow* pWnd, WND_LAYER_LIST eLayer = WL_NORMAL);	//��Ӵ���
void		Wnd_OnWindowDelete(KWndWindow* pWnd);		//��Ӧ���ڵ�ɾ��
void		Wnd_SetGameSpaceWnd(KWndWindow* pWnd);	//����������Ϸ����
void		Wnd_GameSpaceHandleInput(bool bHandle);
void		Wnd_TransmitInputToGameSpace(unsigned int uMsg, unsigned int uParam, int nParam);
void		Wnd_ShowHideGameSpace(bool bShow);
//----�����¼�����----
//�������룬����ϵͳ��������������¼����򷵻ط�0ֵ�����򷵻�0ֵ��
void		Wnd_ProcessInput(unsigned int uMsg, unsigned int uParam, int nParam);
void		Wnd_Heartbeat();
//----���ָ�����----
void		Wnd_ShowCursor(int bShow);						//�ı����ָ����ʾ״̬
void		Wnd_SetCursorPos(int x, int y);					//�������ָ��λ��
void		Wnd_GetCursorPos(int* px, int* py);				//��ȡ���ָ��λ��
void		Wnd_LoadCursor(int nIndex, const char* pImgFile);//�������ָ����Դ
int			Wnd_SwitchCursor(int nIndex);					//�л���ǰ���ָ��ͼ��
void		Wnd_RestoreCursor();							//��ԭ���ͼ��
//----�϶�����----
//��ʼ�϶�����
int			Wnd_DragBegin(const KUiDraggedObject* pObj, fnDrawDraggedObj fnDrawObjFunc);
int			Wnd_GetDragObj(KUiDraggedObject* pObj);
void		Wnd_DragFinished();								//�����϶�״̬
//----�������뽹�㴰��----
void		Wnd_SetFocusWnd(KWndWindow* pWnd);				//�������뽹�㴰��
KWndWindow*	Wnd_GetFocusWnd();								//��ȡ���뽹�㴰��
//----��������¼�----
void		Wnd_SetCapture(KWndWindow* pWnd);				//���ø�����궯������
void		Wnd_ReleaseCapture();							//�ͷŴ��ڶ���궯���ĸ���
//----�����ڶ�ռ----
void		Wnd_SetExclusive(KWndWindow* pWnd);				//���ö�ռ����
void		Wnd_ReleaseExclusive(KWndWindow* pWnd);			//�ͷŴ��ڵĶ�ռ״̬
