// ***************************************************************************************
// �����������ϵ���֦
// Copyright : Kingsoft 2002
// Author    : wooy(wu yue)
// CreateTime: 2002-11-19
// ---------------------------------------------------------------------------------------
// ***************************************************************************************

#ifndef KIPOTBRANCH_H_HEADER_INCLUDED_C22510AB
#define KIPOTBRANCH_H_HEADER_INCLUDED_C22510AB

struct KIpotLeaf;
struct KIpotRuntimeObj;
struct KIpotBuildinObj;

//##ModelId=3DD9DE49027E
//##Documentation
//## ��������������֦�Ľṹ��
class KIpotBranch
{
  friend struct KIpotRuntimeObj;
  public:

	//##ModelId=3DDAC4AC0238
	KIpotBranch();

	//##ModelId=3DDAC4AC0242
	~KIpotBranch();
	
	//##ModelId=3DDACD580262
	//##Documentation
	//## �����֦��
	void AddBranch(
		//##Documentation
		//## ��֦�������Ķ����ָ��
		KIpotBuildinObj* pBranchObj);

	//##ModelId=3DDACA8B015D
	//##Documentation
	//## ɾ����֦�����б�������������ʱ�������Ҷ��
	void RemoveAllRtoLeafs(
		//##Documentation
		//## ����ժ������Ҷ�ӵ���֦��
		KIpotLeaf* pPermanentRto);

	//##ModelId=3DDACA410279
	//##Documentation
	//## ������֦
	void PaintObjectLayer(RECT* pRepresentArea);
	
	//## ö�ٰ�������֦�еĶ���
	void EnumerateObjects(void* p, ObjectsCallbackFn CallbackFn);

	void PaintNoneObjectLayer(RECT* pRepresentArea, int eLayer);

	//##ModelId=3DDC082502F0
	//##Documentation
	//## �����֦�����е����ݡ�
	void Clear();

	//##ModelId=3DDC101202EE
	//##Documentation
	//## ������֦��Ӧ�Ķ����Լ������õ��߶Ρ�
	void AddAObject(
		//##Documentation
		//## ����֦�����ĳ����ڽ�����
		KIpotBuildinObj* pBranchObj);

	void SetLine(
		//##Documentation
		//## �߶ε�����һ��
		POINT* pPoint1,
		//##Documentation
		//## �߶ε�����һ��
		POINT* pPoint2);

	//##ModelId=3DE2A08602E2
	//##Documentation
	//## ɾ��ȫ��������Χ��rtoҶ�ӡ�
/*	void RemoveOutsideRtoLeafs(
		//##Documentation
		//## ָ����Χ
		RECT& rc);
*/
	//##ModelId=3DE3CBBB03E7
	//##Documentation
	//## ������߷�ʽ������ڽ�����
	void AddLeafLine(
		//##Documentation
		//## ���߷�ʽ������ڽ�����
		KIpotBuildinObj* pLeaf);

	//##ModelId=3DE3CC2401EA
	//##Documentation
	//## ����Ե㷽ʽ������ڽ�����
	void AddLeafPoint(
		//##Documentation
		//## �Ե㷽ʽ������ڽ�����
		KIpotLeaf* pLeaf);

  private:

	//##Documentation
	//## �����֦��
	void AddSubBranch(
		int nSubIndex, 
		//##Documentation
		//## ��֦�ṹ��ָ��
		KIpotBuildinObj* pBranchObj);

	void AddLineLeafToList(KIpotLeaf*& pFirst, KIpotBuildinObj* pLeaf);
	void AddPointLeafToList(KIpotLeaf*& pFirst, KIpotLeaf* pLeaf, KIpotLeaf* pParentLeaf);
	
	void PaintABranchObject(KIpotBuildinObj* pObject, RECT* pRepresentArea) const;

  private:

	//##ModelId=3DDA93F50055
	enum IPOT_BRANCH_FLAG
	{ 
		//##Documentation
		//## KIpotBranch�ṹʵ��������pLeftBranch��Ա�������ҷ�0��pLeftLeafs��Ա�����塣
		IPOT_BF_HAVE_LEFT_BRANCH = 1, 
		//##Documentation
		//## KIpotBranch�ṹʵ��������pRightBranch��Ա�������ҷ�0��pRightLeafs��Ա�����塣
		IPOT_BF_HAVE_RIGHT_BRANCH = 2
	};

  private:

	//##ModelId=3DDA74570346
	//##Documentation
	//## ����֦�ڵ�ı��
	unsigned int m_uFlag;

	//##ModelId=3DD9DE7300C6
	//##Documentation
	//## ����㣬��֦���ӵĴ���֦��
	KIpotBranch* m_pParent;

	union
	{
		//##ModelId=3DD9DEAF0307
		//##Documentation
		//## �ֲ���֦��
		//## ��pLeafsΪunion��ϵ������ȡֵͬһʱ��ֻ��һ�������塣
		KIpotBranch* m_pSubBranch[2];

		//##ModelId=3DD9E03503E4
		//##Documentation
		//## ������֦��Ҷ���б�
		//## ��pSubBranchΪunion��ϵ������ȡֵͬһʱ��ֻ��һ�������塣
		KIpotLeaf* m_pLeafs[2];
	};

	//##ModelId=3DDAB05102A3
	//##Documentation
	//## ��֦���ϲ�˵������
	POINT m_oHeadPoint;

	//##ModelId=3DDAB10F026A
	//##Documentation
	//## ��֦���²�˵������
	POINT m_oEndPoint;


	//##ModelId=3DE3CB760294
	//##Documentation
	//## ����֦�����ĳ����ڽ�����
	KIpotBuildinObj*  m_pFirstObject;
	KIpotBuildinObj** m_pObjectList;
	int		m_nNumObjects;
	int		m_nNumObjectSpace;

  private:

	static unsigned int	m_BranchFlag[2];

};



#endif /* KIPOTBRANCH_H_HEADER_INCLUDED_C22510AB */
