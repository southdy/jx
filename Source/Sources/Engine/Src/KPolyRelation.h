//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KPolyRelation.cpp
// Date:	2002.03.11
// Code:	Spe
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef	KPolyRelation_H
#define	KPolyRelation_H
BOOL	ENGINE_API g_ColliSion(KPolygon Poly1, KPolygon Poly2);			//	�Ƿ���ײ
int		ENGINE_API g_SpaceRelation(KPolygon Poly1, KPolygon Poly2);		//	-1��Poly1��Poly2�ڵ�
																		//	0��	û�й�ϵ
																		//	1��	Poly1�ڵ�Poly2
BOOL	IntersectionX2Y(POINT p1, POINT p2, int x, int *y);
#endif