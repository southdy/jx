// ***************************************************************************************
// �����еĶ������е���Ҷ�ṹ����
// Copyright : Kingsoft 2002
// Author    : wooy(wu yue)
// CreateTime: 2002-11-19
// ---------------------------------------------------------------------------------------
// ***************************************************************************************

#ifndef KIPOTLEAF_H_HEADER_INCLUDED_C2254C34
#define KIPOTLEAF_H_HEADER_INCLUDED_C2254C34

#include "SceneDataDef.h"
#include "../../../Represent/iRepresent/KRepresentUnit.h"
class KIpotBranch;

struct KIpotLeaf;
// ����ö�ٻص�����
typedef void (*ObjectsCallbackFn)(void* p, KIpotLeaf* pLeaf);

#define	POINT_LEAF_Y_ADJUST_VALUE	6

//##ModelId=3DD9B1EA02A3
//##Documentation
//## �����ж��󼯺�������Ҷ��
struct KIpotLeaf
{
	//##ModelId=3DD9B259020C
	//##Documentation
	//## �����ж��󼯺�������Ҷ����ʾ�Ķ��󣩵�����
	enum IPOTLEAF_TYPE
	{
		//##Documentation
		//## ������ͼ���ڽ�����
		IPOTL_T_BUILDIN_OBJ, 
		//##Documentation
		//## �����еķ���Դ
//		IPOTL_T_LIGHT, 
		//##Documentation
		//## ����ʱ(����)�����ڳ����Ķ���
		IPOTL_T_RUNTIME_OBJ
	};

	//##ModelId=3DD9B22D006E
	//##Documentation
	//## ��������Ҷ�ӵ�����
	IPOTLEAF_TYPE eLeafType;

	//##ModelId=3DD9DD1203A3
	//##Documentation
	//## ָ���������ϵ�Ҷ���ֵܡ�
	KIpotLeaf* pBrother;

	//��Ҷ��
	KIpotLeaf  *pLChild, *pRChild;

	//##ModelId=3DDBF7F8024B
	//##Documentation
	//## �����λ�����ꡣ
	POINT oPosition;

};

//## ö�ٰ�����Ҷ���еĶ���
void KIpotLeaf_EnumerateObjects(KIpotLeaf* pLeaf, void* p, ObjectsCallbackFn CallbackFn);
//����Ҷ�ӵĶ����
void KIpotLeaf_PaintObjectLayer(KIpotLeaf* pLeaf, RECT* pRepresentArea);
//����Ҷ�ӵĶ����֮���������
void KIpotLeaf_PaintNoneObjectLayer(KIpotLeaf* pLeaf, RECT* pRepresentArea, int eLayer);
//�Ƴ�ȫ����rtoҶ��
void KIpotLeaf_RemoveAllRtoLeafs(KIpotLeaf* pLeaf, KIpotLeaf* pPermanentRto);
//���
void KIpotLeaf_Clear(KIpotLeaf* pLeaf);


//##ModelId=3DD9EB4500C5
//##Documentation
//## ����������������ʱ����(�����ڳ����е��ǲ������ڳ�����ͼ�Ķ���)��Ҷ��
struct KIpotRuntimeObj : public KIpotLeaf
{
	//##ModelId=3DDBD5DB02DA
	//##Documentation
	//## �����ƬҶ������֦�ϣ������ժ������
	void Pluck();

	//##ModelId=3DD9EBFE032E
	//##Documentation
	//## ����������
	unsigned int uGenre;

	//##ModelId=3DD9EC170027
	//##Documentation
	//## �����id
	int nId;

	int	nPositionZ;

	int eLayerParam;

	//##ModelId=3DDBD19C0345
	//##Documentation
	//## Ҷ�ӵ����ڵ���֦��ָ��
	KIpotBranch* pParentBranch;

	//##Documentation
	//## Ҷ�ӵĸ�Ҷ��
	KIpotLeaf*	pParentLeaf;
	//pParentBranch��pParentLeafͬʱ���ֻ����һ��Ϊ��0��

	//##ModelId=3DDBD1DC03AB
	//##Documentation
	//## ����ͬһ֧��֦�ϵ���ǰ���Ҷ���ֵܡ�
	KIpotLeaf* pAheadBrother;
};

//##ModelId=3DD9B745022C
//##Documentation
//## ������ͼ�ϵ��ڽ�����һֱ���ڳ����У���Զ���ڣ�ֱ���������ĳ��������������ޡ�
struct KIpotBuildinObj : public KIpotLeaf
{
	//����һ����¡��
	KIpotBuildinObj* Clone(POINT* pDivisionPos);
	void PaintABuildinObject(RECT* pRepresentArea);

    //## ͼ��id
    unsigned int uImage;

	POINT	oEndPos;
	
	float	fAngleXY;
	float	fNodicalY;

    //## ͼ��ԭ����iImageStore�ڵ�λ�á�
    short 	nISPosition;
	bool	bClone;
	bool	bImgPart;

	struct KBuildinObj* pBio;
};

void  BuildinObjNextFrame(KBuildinObj* pBio);


#endif /* KIPOTLEAF_H_HEADER_INCLUDED_C2254C34 */
