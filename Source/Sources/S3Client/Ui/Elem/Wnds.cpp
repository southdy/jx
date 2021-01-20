/*****************************************************************************************
//	�������棬���洰����ϵ�ṹ����������ڶ���
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-9
*****************************************************************************************/
#include "KWin32.h"

#include "KEngine.h"
#include "KWin32Wnd.h"
#include "../Elem/WndMessage.h"
#include "Wnds.h"
#include "WndWindow.h"
#include "UiCursor.h"
#include "WndGameSpace.h"
#include "MouseHover.h"
#include "PopupMenu.h"
#include "../UiBase.h"
#include "../ShortcutKey.h"

#include "CommCtrl.h"

int WND_SHOW_MOUSE_OVER_WND = false;

//=================
//		����վ
//=================
static struct KWndStation
{
	KWndWindow		LowLayerRoot;
	KWndWindow		NormalLayerRoot;
	KWndWindow		TopLayerRoot;
	KWndWindow*		pFocusWnd;
	KWndWindow*		pMouseOverWnd;
	KWndWindow*		pCaptureMouseWnd;
	KWndWindow*		pExclusiveWnd[4];

	KUiCursor		Cursor;
	SIZE			ScreenSize;

	struct KDragInfo
	{
		int					bDragging;
		KUiDraggedObject	DraggedObj;
		fnDrawDraggedObj	fnDrawObjFnc;
		int					nDropQeuryRsult;
	}		DragInfo;
	
	KWndWindow*		pGameSpaceWnd;	
	unsigned char	bGameSpaceNotHandleInput;
	bool			bPaintGameSpace;

	KWndStation()
	{
		pFocusWnd        = NULL;
		pMouseOverWnd    = NULL;
		pCaptureMouseWnd = NULL;
		pExclusiveWnd[0] = NULL;
		pExclusiveWnd[1] = NULL;
		pExclusiveWnd[2] = NULL;
		pExclusiveWnd[3] = NULL;
		pGameSpaceWnd	 = NULL;
		bGameSpaceNotHandleInput = 1;
		bPaintGameSpace = false;
		LowLayerRoot.Hide();
		NormalLayerRoot.Hide();
		TopLayerRoot.Hide();
		DragInfo.bDragging = false;
		DragInfo.DraggedObj.uGenre = 0;
		DragInfo.DraggedObj.uId = 0;
		ScreenSize.cx = 0;
		ScreenSize.cy = 0;
	}
}	s_WndStation;

static KWndWindow*	Wnd_GetActive(int x, int y, bool bBringToTop);

//--------------------------------------------------------------------------
//	���ܣ��������������ȫ���Ĵ���
//--------------------------------------------------------------------------
void Wnd_Cleanup()
{
	Wnd_Heartbeat();

	s_WndStation.pFocusWnd = NULL;
	s_WndStation.pMouseOverWnd = NULL;
	s_WndStation.pCaptureMouseWnd = NULL;
	s_WndStation.pExclusiveWnd[0] = NULL;
	s_WndStation.pExclusiveWnd[1] = NULL;
	s_WndStation.pExclusiveWnd[2] = NULL;
	s_WndStation.pExclusiveWnd[3] = NULL;
	s_WndStation.pGameSpaceWnd = NULL;

	s_WndStation.Cursor.Cleanup();
	s_WndStation.DragInfo.bDragging = false;
}

//--------------------------------------------------------------------------
//	���ܣ����ƴ���
//--------------------------------------------------------------------------
void Wnd_RenderWindows()
{
	int	bShowCursor = true;

	if (s_WndStation.pGameSpaceWnd && s_WndStation.bPaintGameSpace)
		s_WndStation.pGameSpaceWnd->Paint();
	s_WndStation.LowLayerRoot.Paint();
	s_WndStation.NormalLayerRoot.Paint();
	s_WndStation.TopLayerRoot.Paint();
	
	if (s_WndStation.DragInfo.bDragging &&
		s_WndStation.DragInfo.fnDrawObjFnc)
	{
		int h, v;
		s_WndStation.Cursor.GetPosition(h, v);
		if (s_WndStation.DragInfo.fnDrawObjFnc(h, v,
			s_WndStation.DragInfo.DraggedObj,
			s_WndStation.DragInfo.nDropQeuryRsult) == 0)
			bShowCursor = false;
	}
	KPopupMenu::PaintMenu();
	g_MouseOver.PaintMouseHoverInfo();

	if (WND_SHOW_MOUSE_OVER_WND && s_WndStation.pMouseOverWnd)
		s_WndStation.pMouseOverWnd->PaintDebugInfo();

	if (bShowCursor)
		s_WndStation.Cursor.Paint();
}

//--------------------------------------------------------------------------
//	���ܣ��ô��ڻ
//--------------------------------------------------------------------------
void Wnd_Heartbeat()
{
	s_WndStation.LowLayerRoot.LetMeBreathe();
	s_WndStation.NormalLayerRoot.LetMeBreathe();
	s_WndStation.TopLayerRoot.LetMeBreathe();
	if (s_WndStation.pExclusiveWnd[0])
		s_WndStation.pExclusiveWnd[0]->BringToTop();
}

//--------------------------------------------------------------------------
//	���ܣ���Ӵ���
//--------------------------------------------------------------------------
void Wnd_AddWindow(KWndWindow* pWnd, WND_LAYER_LIST eLayer/* = WL_NORMAL*/)
{
	if (eLayer == WL_LOWEST)
		s_WndStation.LowLayerRoot.AddBrother(pWnd);
	else if (eLayer == WL_TOPMOST)
		s_WndStation.TopLayerRoot.AddBrother(pWnd);
	else
		s_WndStation.NormalLayerRoot.AddBrother(pWnd);	
}

//--------------------------------------------------------------------------
//	���ܣ�����������Ϸ����
//--------------------------------------------------------------------------
void Wnd_SetGameSpaceWnd(KWndWindow* pWnd)
{
	s_WndStation.pGameSpaceWnd = pWnd;
}

void Wnd_GameSpaceHandleInput(bool bHandle)
{
/*	if (bHandle)
	{
		if (s_WndStation.bGameSpaceNotHandleInput)
			s_WndStation.bGameSpaceNotHandleInput --;
	}
	else
		s_WndStation.bGameSpaceNotHandleInput ++;
*/
	s_WndStation.bGameSpaceNotHandleInput = !bHandle;
}

void Wnd_ShowHideGameSpace(bool bShow)
{
	s_WndStation.bPaintGameSpace = bShow;
}

//--------------------------------------------------------------------------
//	���ܣ���Ӧ��������
//--------------------------------------------------------------------------
void Wnd_OnWindowDelete(KWndWindow* pWnd)
{
	if (pWnd)
	{		
		if (s_WndStation.pMouseOverWnd == pWnd)
			s_WndStation.pMouseOverWnd = NULL;
		if (s_WndStation.pCaptureMouseWnd == pWnd)
			s_WndStation.pCaptureMouseWnd = NULL;
		if (s_WndStation.pFocusWnd == pWnd)
			s_WndStation.pFocusWnd = NULL;
		if (s_WndStation.pExclusiveWnd[0] == pWnd)
		{
			s_WndStation.pExclusiveWnd[0] = s_WndStation.pExclusiveWnd[1];
			s_WndStation.pExclusiveWnd[1] = s_WndStation.pExclusiveWnd[2];
			s_WndStation.pExclusiveWnd[2] = s_WndStation.pExclusiveWnd[3];
			s_WndStation.pExclusiveWnd[3] = NULL;
		}
		else if (s_WndStation.pExclusiveWnd[1] == pWnd)
		{
			s_WndStation.pExclusiveWnd[1] = s_WndStation.pExclusiveWnd[2];
			s_WndStation.pExclusiveWnd[2] = s_WndStation.pExclusiveWnd[3];
			s_WndStation.pExclusiveWnd[3] = NULL;
		}
		else if (s_WndStation.pExclusiveWnd[2] == pWnd)
		{
			s_WndStation.pExclusiveWnd[2] = s_WndStation.pExclusiveWnd[3];
			s_WndStation.pExclusiveWnd[3] = NULL;
		}
		else if (s_WndStation.pExclusiveWnd[3] == pWnd)
			s_WndStation.pExclusiveWnd[3] = NULL;
		else
			KPopupMenu::OnWndDelete(pWnd);
	}
}

//--------------------------------------------------------------------------
//	���ܣ���ʼ�϶�����
//--------------------------------------------------------------------------
int Wnd_DragBegin(const KUiDraggedObject* pObj, fnDrawDraggedObj fnDrawObjFunc)
{
	//ԭ���и��������ڱ��϶�
	if (s_WndStation.DragInfo.bDragging == false && pObj && pObj->uGenre)
	{
		s_WndStation.DragInfo.DraggedObj = *pObj;
		s_WndStation.DragInfo.fnDrawObjFnc = fnDrawObjFunc;
		s_WndStation.DragInfo.bDragging = true;
		return true;
	}
	return false;
}

int	Wnd_GetDragObj(KUiDraggedObject* pObj)
{
	if (pObj)
	{
		if (s_WndStation.DragInfo.bDragging)
			memcpy(pObj, &s_WndStation.DragInfo.DraggedObj, sizeof(KUiDraggedObject));
		else
			memset(pObj, 0, sizeof(KUiDraggedObject));
	}
	return s_WndStation.DragInfo.bDragging;
}
//--------------------------------------------------------------------------
//	���ܣ������϶�״̬
//--------------------------------------------------------------------------
void Wnd_DragFinished()
{
	s_WndStation.DragInfo.bDragging = false;
}

//--------------------------------------------------------------------------
//	���ܣ���������
//--------------------------------------------------------------------------
void Wnd_ProcessInput(unsigned int uMsg, unsigned int uParam, int nParam)
{
	KWndWindow* pActiveWnd = NULL;
	static bool bLastCursorEventProcessedByGameSpace = false;

	//====���ָ����Ϣ====
	if ((uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST) || uMsg == WM_MOUSEHOVER)
	{
		int x = 0;
		int y = 0;

		if (uMsg != WM_MOUSEWHEEL)
		{
			x = LOWORD(nParam);
			y = HIWORD(nParam);
		}
		else
		{
			s_WndStation.Cursor.GetPosition(x, y);
		}
		//====�������ָ��ͼ��λ��====
		s_WndStation.Cursor.SetPosition(x, y);

		if (KPopupMenu::HandleInput(uMsg, uParam, nParam))
		{
			if (bLastCursorEventProcessedByGameSpace)
			{
				bLastCursorEventProcessedByGameSpace = false;
				g_UiBase.SetCurrentCursor();
			}
			return;
		}
		g_MouseOver.UpdateCursorPos(x, y);
		
		if (s_WndStation.pCaptureMouseWnd)
			pActiveWnd = s_WndStation.pCaptureMouseWnd;
		else if (s_WndStation.pExclusiveWnd[0] == NULL)
		{
			//====ȡ�������ָ�����ϲ㴰�����====
			//====�ñ�������Ĵ��ڳ�Ϊ���ϲ㴰�����====
			if ((uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN || uMsg == WM_MBUTTONDOWN) &&
				s_WndStation.DragInfo.bDragging == 0)
				pActiveWnd = Wnd_GetActive(x, y, true);
			else
				pActiveWnd = Wnd_GetActive(x, y, false);
		}
		else	//�ж�ռ����ʱ��Active���ھ�������
			pActiveWnd = s_WndStation.pExclusiveWnd[0];

		{
			KWndWindow* pTopWnd = NULL;
			if (pActiveWnd)			
				pTopWnd = pActiveWnd->TopChildFromPoint(x, y);	//ֻ����λ�����ָ���·������ϲ�Ĵ���
			if (pTopWnd != s_WndStation.pMouseOverWnd)
			{
				g_MouseOver.CancelMouseHoverInfo();
				KWndWindow* pOverParent = NULL;
				if (s_WndStation.pMouseOverWnd)
				{
					s_WndStation.pMouseOverWnd->WndProc(WND_M_MOUSE_LEAVE, 0, 0);
					pOverParent = s_WndStation.pMouseOverWnd->GetParent();
					if (pOverParent)
					{
						pOverParent->WndProc(WND_N_MOUSE_LEAVE_CHILD,
							(unsigned int)s_WndStation.pMouseOverWnd, 0);
					}
				}
				if (pTopWnd)
				{
					pTopWnd->WndProc(WND_M_MOUSE_ENTER, 0, 0);
					pOverParent = pTopWnd->GetParent();
					if (pOverParent)
					{
						pOverParent->WndProc(WND_N_MOUSE_ENTER_CHILD,
							(unsigned int)pTopWnd, 0);
					}
				}
				s_WndStation.pMouseOverWnd = pTopWnd;
			}

			if (s_WndStation.pFocusWnd && pTopWnd != s_WndStation.pFocusWnd &&
				(uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN || uMsg == WM_MBUTTONDOWN))
			{
				Wnd_SetFocusWnd(NULL);
			}
		}

		bool bPrecessedByGameSpace = false;
		if (s_WndStation.pMouseOverWnd)
			s_WndStation.pMouseOverWnd->WndProc(uMsg, uParam, nParam);
		else if (s_WndStation.pCaptureMouseWnd)
			s_WndStation.pCaptureMouseWnd->WndProc(uMsg, uParam, nParam);
		else if (s_WndStation.pExclusiveWnd[0] == NULL &&
			s_WndStation.bGameSpaceNotHandleInput == false && s_WndStation.pGameSpaceWnd)
		{
			s_WndStation.pGameSpaceWnd->WndProc(uMsg, uParam, nParam);
			bLastCursorEventProcessedByGameSpace = true;
			bPrecessedByGameSpace = true;
		}
		if (bPrecessedByGameSpace == false && bLastCursorEventProcessedByGameSpace)
		{
			bLastCursorEventProcessedByGameSpace = false;
			g_UiBase.SetCurrentCursor();
		}
	}
	else
	{//====����������Ϣ����ʵ���Ǽ���������Ϣ��====
		if (KPopupMenu::HandleInput(uMsg, uParam, nParam))
			return;

		if (s_WndStation.pFocusWnd && s_WndStation.pFocusWnd->WndProc(uMsg, uParam, nParam))
			return;
		if (s_WndStation.pExclusiveWnd[0])
		{
			s_WndStation.pExclusiveWnd[0]->WndProc(uMsg, uParam, nParam);
			return;
		}
		if (uMsg == WM_KEYDOWN || (uMsg == WM_KEYUP && uParam == VK_SNAPSHOT))
		{
			int nModifier = 0;
			if (GetKeyState(VK_CONTROL) & 0x8000)
				nModifier |= HOTKEYF_CONTROL;

			if (GetKeyState(VK_SHIFT) & 0x8000)
				nModifier |= HOTKEYF_SHIFT;

			if (GetKeyState(VK_MENU) & 0x8000)
				nModifier |= HOTKEYF_ALT;

			if (KShortcutKeyCentre::HandleKeyInput(uParam, nModifier))
				return;
		}
		else if (uMsg == WM_SYSKEYDOWN)
		{
			int nModifier = 0;
			if (GetKeyState(VK_CONTROL) & 0x8000)
				nModifier |= HOTKEYF_CONTROL;

			if (GetKeyState(VK_SHIFT) & 0x8000)
				nModifier |= HOTKEYF_SHIFT;

			if (GetKeyState(VK_MENU) & 0x8000)
				nModifier |= HOTKEYF_ALT;

			if (KShortcutKeyCentre::HandleKeyInput(uParam, nModifier))
				return;
		}

		int	bHandle = false;		
		for (int i = 0; i < 3 && bHandle == false; i++)
		{
			if (i == 0)
				pActiveWnd = s_WndStation.TopLayerRoot.GetNextWnd();
			else if (i == 1)
				pActiveWnd = s_WndStation.NormalLayerRoot.GetNextWnd();
			else
				pActiveWnd = s_WndStation.LowLayerRoot.GetNextWnd();

			while(pActiveWnd)
			{
				KWndWindow* pNextWnd = pActiveWnd->GetNextWnd();
				if (pActiveWnd->IsVisible() &&
					pActiveWnd->WndProc(uMsg, uParam, nParam))
				{
					bHandle = true;
					break;
				}
				pActiveWnd = pNextWnd;
			}
		}
		
		if (bHandle == false && s_WndStation.pGameSpaceWnd &&
			s_WndStation.bGameSpaceNotHandleInput == false)
		{
			s_WndStation.pGameSpaceWnd->WndProc(uMsg, uParam, nParam);
		}
	}
}

void Wnd_TransmitInputToGameSpace(unsigned int uMsg, unsigned int uParam, int nParam)
{
	s_WndStation.pGameSpaceWnd->WndProc(uMsg, uParam, nParam);
}

//--------------------------------------------------------------------------
//	���ܣ��Ѵ���ĳ��λ�õĴ��ڴ������ϲ�
//	���أ����ϲ㴰�ڵ�ָ��
//--------------------------------------------------------------------------
KWndWindow* Wnd_GetActive(int x, int y, bool bBringToTop)
{
	KWndWindow* pActive = NULL;
	for (int i = 0; i < 3; i++)
	{		
		KWndWindow*	pWnd = NULL;
		if (i == 0)
			pWnd = &s_WndStation.TopLayerRoot;
		else if (i == 1)
			pWnd = &s_WndStation.NormalLayerRoot;
		else
			pWnd = &s_WndStation.LowLayerRoot;

		while(pWnd = pWnd->GetNextWnd())
		{
			if (pWnd->IsVisible())
				pActive = pWnd;
		}
		while(pActive)
		{
			if (pActive->PtInWindow(x, y))
			{
				if (i == 1 && bBringToTop)
					pActive->BringToTop();
				break;
			}
			pActive = pActive->GetPreWnd();
		}
		if (pActive)
			break;
	}
	return pActive;
}

//�ı����ָ����ʾ״̬
void Wnd_ShowCursor(int bShow)
{
	s_WndStation.Cursor.Show(bShow);
}

//--------------------------------------------------------------------------
//	���ܣ��������ָ��λ��
//--------------------------------------------------------------------------
void Wnd_SetCursorPos(int x, int y)
{
	s_WndStation.Cursor.SetPosition(x, y);
	
	HWND	hWnd = g_GetMainHWnd();
	POINT	Pos;
	Pos.x = x;
	Pos.y = y;
	::ClientToScreen(hWnd, &Pos);
	::SetCursorPos(Pos.x, Pos.y);
}

//--------------------------------------------------------------------------
//	���ܣ���ȡ���ָ��λ��
//--------------------------------------------------------------------------
void Wnd_GetCursorPos(int* px, int* py)
{
	int h, v;
	s_WndStation.Cursor.GetPosition(h, v);
	if (px)
		*px = h;
	if (py)
		*py = v;
}

//--------------------------------------------------------------------------
//	���ܣ���ȡ��ǰ�ҽӵ����뽹�㴰��
//--------------------------------------------------------------------------
KWndWindow* Wnd_GetFocusWnd()
{
	return s_WndStation.pFocusWnd;
}

//--------------------------------------------------------------------------
//	���ܣ��ҽ����뽹�㴰��
//--------------------------------------------------------------------------
void Wnd_SetFocusWnd(KWndWindow* pWnd)
{
	if (s_WndStation.pFocusWnd != pWnd)
	{
		if (s_WndStation.pFocusWnd)
			s_WndStation.pFocusWnd->WndProc(WND_M_KILL_FOCUS, (unsigned int)pWnd, 0);
		if (pWnd)
			pWnd->WndProc(WND_M_SET_FOCUS, (unsigned int)s_WndStation.pFocusWnd, 0);
		s_WndStation.pFocusWnd = pWnd;
	}
}

//--------------------------------------------------------------------------
//	���ܣ����ø�����궯������
//--------------------------------------------------------------------------
void Wnd_SetCapture(KWndWindow* pWnd)
{
	if (s_WndStation.pCaptureMouseWnd != pWnd)
	{
		Wnd_ReleaseCapture();
		s_WndStation.pCaptureMouseWnd = pWnd;
	}
}

//--------------------------------------------------------------------------
//	���ܣ��ͷŴ��ڶ���궯���ĸ���
//--------------------------------------------------------------------------
void Wnd_ReleaseCapture()
{
	if (s_WndStation.pCaptureMouseWnd)
	{
		s_WndStation.pCaptureMouseWnd->WndProc(WND_M_CAPTURECHANGED, 0, 0);
		s_WndStation.pCaptureMouseWnd = NULL;
	}
}

//--------------------------------------------------------------------------
//	���ܣ����ö�ռ����
//--------------------------------------------------------------------------
void Wnd_SetExclusive(KWndWindow* pWnd)
{
	int i;
	if (pWnd)
	{
		if (s_WndStation.pFocusWnd && s_WndStation.pFocusWnd->GetOwner() != pWnd->GetOwner())
			Wnd_SetFocusWnd(NULL);

		for (i = 0; i < 4; i++)
		{
			if (s_WndStation.pExclusiveWnd[i] == pWnd)
			{
				for (int j = i; j > 0; j--)
					s_WndStation.pExclusiveWnd[j] = s_WndStation.pExclusiveWnd[j - 1];
				s_WndStation.pExclusiveWnd[0] = pWnd;
				return;
			}
		}
		s_WndStation.pExclusiveWnd[3] = s_WndStation.pExclusiveWnd[2];
		s_WndStation.pExclusiveWnd[2] = s_WndStation.pExclusiveWnd[1];
		s_WndStation.pExclusiveWnd[1] = s_WndStation.pExclusiveWnd[0];
		s_WndStation.pExclusiveWnd[0] = pWnd;
	}
}

//--------------------------------------------------------------------------
//	���ܣ�ȡ����ռ����
//--------------------------------------------------------------------------
void Wnd_ReleaseExclusive(KWndWindow* pWnd)
{
	if (pWnd)
	{
		for (int i = 0; i < 4; i++)
		{
			if (s_WndStation.pExclusiveWnd[i] == pWnd)
			{
				for (int j = i; j < 3; j++)
					s_WndStation.pExclusiveWnd[j] = s_WndStation.pExclusiveWnd[j + 1];
				s_WndStation.pExclusiveWnd[3] = NULL;
			}
		}
	}
}

//--------------------------------------------------------------------------
//	���ܣ��������ָ����Դ
//--------------------------------------------------------------------------
void Wnd_LoadCursor(int nIndex, const char* pImgFile)
{
	s_WndStation.Cursor.SetImage(nIndex, pImgFile);
}

//--------------------------------------------------------------------------
//	���ܣ��л���ǰ���ָ��ͼ��
//--------------------------------------------------------------------------
int Wnd_SwitchCursor(int nIndex)
{
	return s_WndStation.Cursor.SwitchImage(nIndex);
}


//������Ļ��Χ��С
void Wnd_SetScreenSize(int nWidth, int nHeight)
{
	s_WndStation.ScreenSize.cx = nWidth;
	s_WndStation.ScreenSize.cy = nHeight;
}

//��ȡ��Ļ��Χ��С
void Wnd_GetScreenSize(int& nWidth, int& nHeight)
{
	nWidth = s_WndStation.ScreenSize.cx;
	nHeight = s_WndStation.ScreenSize.cy;
}

//��ԭ���ͼ��
void	Wnd_RestoreCursor()
{
	s_WndStation.Cursor.RestoreCursor();
}