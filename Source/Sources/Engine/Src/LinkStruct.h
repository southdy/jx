/********************************************************************
File        : LinkStruct.h
Creator     : Fyt(Fan Zhanpeng)
create data : ??-??-2002(mm-dd-yyyy)
Description : һά�����ģ����
*********************************************************************/
//////////////////////////////////////////////////////////////////////
//By Fyt(Fan Zhanpeng)
//*****************************************************************************
//Function List:
//GetCount() AddItem() GetItem() Current() InsertItem() RemoveItem() NextItem()
//PrevItem() Reset()   Begin()   End()     Clear()
//*****************************************************************************
//Description  :
//class T  �������Լ����͵�previous  ��  next ָ��

#if !defined(AFX_LINKSTRUCT_H__76A129C2_C1B9_4556_90E9_11288646EF8C__INCLUDED_)
#define AFX_LINKSTRUCT_H__76A129C2_C1B9_4556_90E9_11288646EF8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template <class T> class KLinkStruct
{
public:
	KLinkStruct()
	{
		lp_beginitem = NULL;
		lp_currentitem = NULL;
		lp_enditem = NULL;
		count = 0;
	}


	virtual ~KLinkStruct()
	{
		Clear();
	}

	int GetCount()
	{
		return count;
	}

        //�������м���һ��Ԫ��
	T*  AddItem(T it)
	{
		if( lp_beginitem == NULL )
		{
			lp_beginitem = new T;
			*lp_beginitem = it;
			lp_beginitem->previous = NULL;
			lp_beginitem->next = NULL;
			lp_currentitem = lp_beginitem;
			lp_enditem = lp_beginitem;
			count = 1;
		}
		else if( lp_beginitem == lp_enditem )
		{
			lp_enditem = new T;
			*lp_enditem = it;
			lp_beginitem->next = lp_enditem;
			lp_enditem->previous = lp_beginitem;
			lp_enditem->next = NULL;
			lp_currentitem = lp_enditem;
			count ++;
		}
		else
		{
			T * temp;
			temp = lp_enditem;
			lp_enditem = new T;
			*lp_enditem = it;
			temp->next = lp_enditem;
			lp_enditem->previous = temp;
			lp_enditem->next = NULL;
			lp_currentitem = lp_enditem;
			count ++;
		}
		return lp_enditem;
	}

	//��ȡ��which��Ԫ��
	T*  GetItem(int which)
	{
		int i;
		if( count == 0 )
			return NULL;
		else if( which >= count )
			which = count;
		else if( which < 0 )
			which = 0;
		if( which == count - 1 )
		{
			lp_currentitem = lp_enditem;
			return lp_enditem;
		}
		else if( which == 0 )
		{
			lp_currentitem = lp_beginitem;
			return lp_beginitem;
		}
		else
		{
			Reset();
			for( i=0;i < which;i++ )
			{
				NextItem();
			}
			return lp_currentitem;
		}
	}

        //���ص�ǰ�����Ԫ��
	T*  Current()
	{
		return lp_currentitem;
	}

        //��whereλ�ò���һ��itԪ��
	T*  InsertItem(int where,T it)
	{
		GetItem(where);
		return InsertItem(it);
	}

        //�ڵ�ǰ����Ԫ��ǰ����һ��itԪ��
	T*  InsertItem(T it)
	{
		T * temp,* tempnext;

		if( lp_beginitem == NULL )
			add_item(it);
		else if( lp_currentitem == lp_beginitem )
		{
			temp = new T;
			*temp = it;
			temp->previous = NULL;
			temp->next = lp_currentitem;
			lp_currentitem->previous = temp;
			lp_beginitem = temp;
			lp_currentitem = temp;
		}
		else
		{
			temp = new T;
			*temp = it;
			temp->previous = lp_currentitem->previous;
			temp->next = lp_currentitem;
			tempnext = temp->previous;
			tempnext->next = temp;
			lp_currentitem->previous = temp;
			lp_currentitem = temp;
		}
		return lp_currentitem;
	}

        //ɾ����which��Ԫ��
	int RemoveItem(int which)
	{
		GetItem(which);
		RemoveItem();
	}

        //ɾ����ǰ����Ԫ��
	int RemoveItem()
	{
		T * temp1,* temp2;
		if( lp_currentitem == NULL )
			return 0;
		if( lp_beginitem == lp_enditem )
		{
			delete lp_beginitem;
			lp_beginitem = NULL;
			lp_currentitem = NULL;
			lp_enditem = NULL;
			count = 0;
		}
		else if( lp_currentitem == lp_beginitem )
		{
			lp_beginitem = lp_currentitem->next;
			lp_beginitem->previous = NULL;
			delete lp_currentitem;
			lp_currentitem = lp_beginitem;
			count --;
		}
		else if( lp_currentitem == lp_enditem )
		{
			lp_enditem = lp_currentitem->previous;
			lp_enditem->next = NULL;
			delete lp_currentitem;
			lp_currentitem = lp_enditem;
			count --;
		}
		else
		{
			temp1 = lp_currentitem->previous;
			temp2 = lp_currentitem->next;
			temp1->next = temp2;
			temp2->previous = temp1;
			delete lp_currentitem;
			lp_currentitem = temp2;
			count --;
		}
		return 1;
	}

        //�ѽ����Ƶ���һ��Ԫ��
	T*  NextItem()
	{
		if( lp_currentitem == NULL )
			return NULL;
		lp_currentitem = lp_currentitem->next;
		return lp_currentitem;
	}

        //�ѽ����Ƶ���һ��Ԫ��
	T*  PrevItem()
	{
		if( lp_currentitem == NULL )
			return NULL;
		lp_currentitem = lp_currentitem->previous;
		return lp_currentitem;
	}

        //�ѽ����Ƶ���һ��Ԫ��
	T*  Reset()
	{
		lp_currentitem = lp_beginitem;
		return lp_currentitem;
	}

        //�ѽ����Ƶ���һ��Ԫ��
	T*  Begin()
	{
		return Reset();
	}

        //�ѽ����Ƶ����һ��Ԫ��
	T*  End()
	{
		lp_currentitem = lp_enditem;
		return lp_currentitem;
	}

    //�����������
    void Clear()
    {
        Reset();
        while(RemoveItem() != NULL);
        lp_beginitem = NULL;
        lp_currentitem = NULL;
        lp_enditem = NULL;
        count = 0;
    }

public:
    KLinkStruct<T>* next;
    KLinkStruct<T>* previous;

private:
    int count;
    T *lp_beginitem;
    T *lp_enditem;
    T *lp_currentitem;
};

#endif // !defined(AFX_LINKSTRUCT_H__76A129C2_C1B9_4556_90E9_11288646EF8C__INCLUDED_)
