/*****************************************************************************************
//	������Ϣ
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-9
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

#define	WND_GAMESPACE			0xFFFFFFFF

#ifndef WM_MOUSEHOVER
	#define	WM_MOUSEHOVER	0x02A1
#endif

#define	WND_M_BASE				0x500	//WM_USER + 0x100

//------------------------------------------------
//	����һ����Ϣ
#define WND_M_OTHER_WORK_RESULT	WND_M_BASE + 1				//��������������
//uParam = �����������ִ�ж���Ĳ���������������������ô˲����������Լ����Է����Ķ������
//nParam = ȡֵΪWND_OPER_RESULT_0����WND_OPER_RESULT(n), nָ������һ������ֵ���京�����ݾ���Ľ���Ӧ���Լ�ȷ��
#define	WND_M_MENUITEM_SELECTED	WND_M_BASE + 2
#define	WND_M_MENUITEMRIGHT_SELECTED	WND_M_BASE + 3
//(KWndWindow*)uParam = ���ò˵��ĵĴ���
//	(short)(nParam & 0xffff) = ��ѡ�Ĳ˵�������������ѡ��ȡ����Ϊ-1
//	(short)((nParam >> 16) & 0xffff) = �����������ִ�ж���Ĳ���������������������ô˲����������Լ����Է����Ķ������
/*
#define	WND_M_CREATE			WND_M_BASE + 1
#define	WND_M_DESTROY			WND_M_BASE + 2
#define	WND_M_CLOSE				WND_M_BASE + 3*/
#define WND_M_MOUSE_ENTER		WND_M_BASE + 6				//���ָ����봰�ڷ�Χ
#define	WND_M_MOUSE_LEAVE		WND_M_BASE + 7				//���ָ���뿪���ڷ�Χ

#define	WND_N_MOUSE_ENTER_CHILD	WND_M_BASE + 8				//���ָ�����ĳ�Ӵ��ڷ�Χ
//(KWndWindow*)uParam = ���ָ�������Ӵ��ڵ�ָ��

#define	WND_N_MOUSE_LEAVE_CHILD	WND_M_BASE + 9				//���ָ���뿪ĳ�Ӵ��ڷ�Χ
//(KWndWindow*)uParam = ���ָ���뿪���Ӵ��ڵ�ָ��

#define	WND_M_SET_FOCUS			WND_M_BASE + 10				//�������뽹��
//(KWndWindow*)uParam = ԭ��ӵ�����뽹��Ĵ��ڵ�ָ��

#define	WND_M_KILL_FOCUS		WND_M_BASE + 11				//ʧȥ���뽹��
//(KWndWindow*)uParam = ��Ҫӵ�����뽹��Ĵ��ڵ�ָ��

#define	WND_M_CAPTURECHANGED	WND_M_BASE + 12				//ʧȥ�����ָ����Ϣ����

#define	WND_M_HOTKEY			WND_M_BASE + 13				//�ȼ�
//uParam = �ȼ�ID

#define WND_M_POPUPMENU			WND_M_BASE + 14				//�Ҽ��˵�����
//(KWndWindow*)uParam = ���Ҽ�������Ӵ��ڵ�ָ��
//	(short)(nParam & 0xffff) = x��Ļ����
//	(short)((nParam >> 16) & 0xffff) = y��Ļ����


#define WND_N_ITEM_PICKDROP		WND_M_BASE + 17				//����/������Ʒ
//(ITEM_PICKDROP_PLACE*)uParam = ��������Ʒ������λ�ã�������ƷҪ��������uParam = 0
//(ITEM_PICKDROP_PLACE*)nParam = ��������Ʒ�ķ���λ�ã����޶���Ҫ�����£���nParam = 0

#define WND_N_LEFT_CLICK_ITEM	WND_M_BASE + 18				//����㰴��Ʒ����
//(const KUiDraggedObject*)uParam = ���㰴����Ʒ����Ϣ
//(KWndWindow*)nParam = ��Ʒ���ڵĴ��ڵ�ָ��
#define WND_N_RIGHT_CLICK_ITEM	WND_M_BASE + 19				//�Ҽ��㰴��Ʒ����
//��������ͬWND_N_LEFT_CLICK_ITEM

#define WND_N_CHILD_MOVE		WND_M_BASE + 20				//�Ӵ��ڱ��϶���
//(KWndWindow*)uParam = ���϶��˵��Ӵ��ڵ�ָ��
//��nParam != 0 ʱ
//	(short)(nParam & 0xffff) = �����ƶ��ľ���
//	(short)((nParam >> 16) & 0xffff) = �����ƶ��ľ���
//��nParam == 0 ʱ����ʾ���ڵ��϶�����������

//------------------------------------------------
//	������Ϣ
#define WND_M_BUTTON_BASE			WND_M_BASE + 100
#define WND_N_BUTTON_CLICK			WND_M_BUTTON_BASE + 1	//�����ť����
//(KWndWindow*)uParam = ������İ�ť���ڵ�ָ��
//nParam = ����CheckBox���͵İ�ť���˲�����ʾ��ť�����Ƿ��ڰ���״̬

#define	WND_N_BUTTON_DOWN			WND_M_BUTTON_BASE + 2	//��ť�����²���
//(KWndWindow*)uParam = ������İ�ť���ڵ�ָ��

#define	WND_N_BUTTON_MR_DOWN		WND_M_BUTTON_BASE + 3	//����Ұ�������
//(KWndWindow*)uParam = ������İ�ť���ڵ�ָ��

#define	WND_N_BUTTON_HOLD			WND_M_BUTTON_BASE + 4	//��ť��������ס����
//(KWndWindow*)uParam = ����ס�İ�ť���ڵ�ָ��

#define WND_N_BUTTON_OVER			WND_M_BUTTON_BASE + 5	//����Ƶ���ť��
//(KWndWindow*)uParam = ��ť���ڵ�ָ��

#define WND_N_BUTTON_DB_CLICK		WND_M_BUTTON_BASE + 6	//˫����ť����
//(KWndWindow*)uParam = ������İ�ť���ڵ�ָ��
//����CheckBox���͵İ�ť�������ɴ���Ϣ

//------------------------------------------------
// Scroll Bar��Ϣ
#define WND_M_SCORLLBAR_BASE		WND_M_BASE + 200
#define WND_N_SCORLLBAR_POS_CHANGED	WND_M_SCORLLBAR_BASE + 1//����������λ�÷����仯
//(KWndWindow*)uParam = ������Ĺ��������ڵ�ָ��
//nParam = ����������λ�ù�������ֵ

//------------------------------------------------
// �ı��༭����Ϣ
#define	WND_M_EDIT_BASE				WND_M_BASE + 300
#define	WND_N_EDIT_CHANGE			WND_M_EDIT_BASE + 1		//�ı��༭�����ڵ����ݷ����˱仯
//(KWndWindow*)uParam = �ı��༭�򴰿ڵ�ָ��

#define	WND_N_EDIT_SPECIAL_KEY_DOWN	WND_M_EDIT_BASE + 2		//�ı��༭���ڻ���ض��İ�������
//(KWndWindow*)uParam = �ı��༭�򴰿ڵ�ָ��
//nParam = ������visual key code
//�ض��������� �س�����Esc�����Ʊ��tab�������ϼ�ͷ�������¼�ͷ����ǰ��ҳ������ҳ��

//------------------------------------------------
// �б�򴰿���Ϣ
#define	WND_M_LIST_BASE				WND_M_BASE + 400
#define	WND_N_LIST_ITEM_SEL			WND_M_LIST_BASE + 1		//�б��ѡ������任
//(KWndWindow*)uParam = �б�򴰿ڵ�ָ��
//nParam = ��ѡ����������
#define	WND_N_LIST_ITEM_ACTIVE		WND_M_LIST_BASE + 2		//�б��ѡ�������������enter��
//��������ͬWND_N_LIST_ITEM_SEL
#define	WND_N_LIST_ITEM_D_CLICK		WND_M_LIST_BASE + 3
//(KWndWindow*)uParam = �б�򴰿ڵ�ָ��
//nParam = ˫�����������
#define WND_N_LIST_ITEM_R_CLICK		WND_M_LIST_BASE + 4
//(KWndWindow*)uParam = �б�򴰿ڵ�ָ��
//nParam = �Ҽ�������������
#define	WND_N_LIST_ITEM_HIGHLIGHT	WND_M_LIST_BASE + 5		//�б��ѡ���ָ��ָ�������任
//(KWndWindow*)uParam = �б�򴰿ڵ�ָ��
//nParam = �б��ѡ���ָ��ָ����������

//====��������������ֵ����====
#define WND_OPER_RESULT_0		0		//����������صĽ��0
#define WND_OPER_RESULT(n)		(n)		//����������صĽ��

// flying add the macro so that my compile work can continue.
// in m$ header file, there's this condition: 
// #if (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
#if !defined WM_MOUSEWHEEL
#define WM_MOUSEWHEEL                   0x020A
#endif
#if !defined WHEEL_DELTA
#define WHEEL_DELTA                     120
#endif

//=========================
//	�������϶�����Ľṹ
//=========================
struct KUiDraggedObject
{
	unsigned int	uGenre;
	unsigned int	uId;
	int				DataX;
	int				DataY;
	int				DataW;
	int				DataH;
};

//=========================
//	��������λ�õĽṹ
//=========================
struct ITEM_PICKDROP_PLACE
{
	class KWndWindow*	pWnd;
	int					h;
	int					v;
};
