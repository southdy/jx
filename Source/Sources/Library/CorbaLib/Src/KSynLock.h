#ifndef KSynLock_H
#define KSynLock_H

#include <Kengine.h>

class KSynLock
{
public:
	KSynLock(){m_nCount = 0; m_bLock = FALSE;m_nCurId = 0;};
	virtual ~KSynLock() {};
	BOOL m_bLock;
	long m_nCount;//���ʸ������ѱ���������
	long m_nCurId;//Ŀǰ���з���Ȩ�޵�����ֵ
	long Lock();	 //����Ƿ������Ѿ�������������ǵĻ����ص�ǰ��ֵ
	BOOL UnLock(long index);//����
	long GetCurId(){return m_nCurId;};
	long GetCount(){return m_nCount;};
	//	virtual void * GetData() = 0;
	
};

#endif //KSynLock_H
