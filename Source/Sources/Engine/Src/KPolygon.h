//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KPolygon.h
// Date:	2002.01.10
// Code:	Spe
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef KPolygon_H
#define KPolygon_H

#define	defMaxVertex	8
typedef struct
{
	int		nVertex;
	POINT	Pos[defMaxVertex];
}
TPolygon, *LPPolygon;

class ENGINE_API KPolygon  
{
private:
	TPolygon	m_Polygon;
public:
	KPolygon();
	BOOL		IsPointInPolygon(POINT pos);					//	��鵱ǰ���Ƿ��ڶ������
	BOOL		IsPointInPolygon(int x, int y);					//	��鵱ǰ���Ƿ��ڶ������
	BOOL		AddPointToVertex(POINT pos);					//	���Ӷ���
	BOOL		AddPointToVertex(int x, int y);					//	���Ӷ���
	int			GetNearVertex(POINT pos);						//	ȡ���뵱ǰ������Ķ�������
	int			GetVertexNumber() {return m_Polygon.nVertex;};	//	ȡ�ö���εĶ�����Ŀ
	BOOL		GetIndexVertex(int i, POINT* vertex);			//	ȡ�ö���εĵ�I����������
	BOOL		RemoveIndexVertex(int i);						//	�Ӷ������ȥ����һ������
	void		LoopVertex(int i);								//	�Ѷ���ζ���˳�����I��ѭ���仯
	void		Clear();										//	��ն������Ϣ
	void		GetCenterPos(POINT*	pos);						//	ȡ������λ��
	LPPolygon	GetPolygonPtr() {return &m_Polygon;};			//	ȡ�ö����ָ��
	BOOL		ShiftVertex(int nDir, int nDistance);			//	�����ⶥ����ʲô�����ƶ�
	int			GetLeftVertex();								//	ȡ�����󶥵������
	int			GetRightVertex();								//	ȡ�����Ҷ��������
};

#endif
