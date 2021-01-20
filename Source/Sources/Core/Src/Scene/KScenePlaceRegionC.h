// ***************************************************************************************
// ������ͼ����������������
// Copyright : Kingsoft 2002
// Author    : wooy(wu yue)
// CreateTime: 2002-11-11
// ---------------------------------------------------------------------------------------
// ***************************************************************************************

#ifndef KSCENEPLACEREGIONC_H_HEADER_INCLUDED_C22EB91D
#define KSCENEPLACEREGIONC_H_HEADER_INCLUDED_C22EB91D

#include "SceneDataDef.h"
struct KIpotRuntimeObj;
struct KIpotBuildinObj;
struct KRUImage;
class KPakFile;

//##ModelId=3DB8F89F02D5
//##Documentation
//## ������ͼ�е�һ�����򡣣��ͻ��˰棩
class KScenePlaceRegionC
{
  public:

	//##ModelId=3DD7E5A80351
	//##Documentation
	//## �����������ʱ��һЩ����(�ⲿ�������)
	enum KREGION_WORK_PARAM_PUB { 
		//##Documentation
		//## ��������ĺ����ȣ���λ����㣩
		RWPP_AREGION_WIDTH = 512, 
		//##Documentation
		//## ��������������ȣ���λ����㣩
		RWPP_AREGION_HEIGHT = 1024 };

  public:

	//##ModelId=3DD7EA9200B2
	KScenePlaceRegionC();

	//##ModelId=3DD7EA9200F8
	~KScenePlaceRegionC();
   
	//##ModelId=3DBDAC140299
	//##Documentation
	//## ָʾ�������Ӧ�ü��ص��������ݵ�������������
	bool ToLoad(
		//##Documentation
		//## ����ĺ�������
		int nIndexX, 
		//##Documentation
		//## �������������
		int nIndexY);

	//##ModelId=3DB901F101CD
	//##Documentation
	//## ����������Ϣ��
	//## �������̲ο�[Use Case View\����\�ͻ���\��������ļ���]��
	bool Load(
		//##Documentation
		//## ָ��洢������Ϣ��'����Ŀ¼'���ڵĸ�Ŀ¼�����ƵĻ�������
		const char* pszBaseFolderName);

	//##ModelId=3DD3DD2C0141
	//##Documentation
	//## ��ȡ�������������
	void GetRegionIndex(
		//##Documentation
		//## ��������
		int& nX, 
		//##Documentation
		//## ��������
		int& nY) const;

	//##ModelId=3DBF876400B2
	//##Documentation
	//## ��մ˶��������
	void Clear();

	//##ModelId=3DBF9582039A
	//##Documentation
	//## Ԥ��Ⱦ�ر��ͼ��
	bool PrerenderGround(bool bForce);

	//##ModelId=3DDBD8C80309
	//##Documentation
	//## ���Ƴ�������ر��
	void PaintGround();

	void PaintGroundDirect();

	//##ModelId=3DE29E9102B0
	//##Documentation
	//## ��ȡ�߿ն������Ļ�ͼ�����š�
	unsigned int GetAboveHeadLayer(
		//## ָ��߿ն�����б�
		KBuildinObj*& pObjsAboveHead);

	//##ModelId=3DE29F360221
	//##Documentation
	//## ���Ƹ߿�����
	static void PaintAboveHeadObj(
		KBuildinObj* pObj,
		RECT* pRepresentArea);

	//##ModelId=3DE33AB30318
	//##Documentation
	//## ��ȡ�������ڽ�������б�
	void GetBuildinObjs(
		//##Documentation
		//## ָ�򰴵㷽ʽ���������б�
		KIpotBuildinObj*& pObjsPointList, 
		//##Documentation
		//## ���㷽ʽ����������Ŀ
		unsigned int& nNumObjsPoint, 
		//##Documentation
		//## ָ���߷�ʽ���������б�
		KIpotBuildinObj*& pObjsLineList,
		//##Documentation
		//## ���߷�ʽ����������Ŀ
		unsigned int& nNumObjsLine, 
		//##Documentation
		//## ָ������ʽ���������б�
		KIpotBuildinObj*& pObjsTreeList, 
		//##Documentation
		//## ������ʽ����������Ŀ
		unsigned int& nNumObjsTree);

	//## ��ȡ�������ڽ�δ��ʼ���Ķ�����б�
	void GetBIOSBuildinObjs(
		KBuildinObj*& pObjsList, 
		unsigned int& nNumObjs 
    );

	//����ڽ���Դ����Ϣ
	unsigned int GetBuildinLights(
		KBuildInLightInfo*& pLights);

	void SetNestRegion(KScenePlaceRegionC* pNest);
	
	void EnterProcessArea(KRUImage* pImage);
	void LeaveProcessArea();
	KRUImage* GetPrerenderGroundImage() { return m_pPrerenderGroundImg; }

	long GetObstacleInfo(int nX, int nY);
	long GetObstacleInfoMin(int nX, int nY, int nOffX, int nOffY);
	void SetHightLightSpecialObject(unsigned int uBioIndex);
	void UnsetHightLightSpecialObject(unsigned int uBioIndex);

#ifdef SWORDONLINE_SHOW_DBUG_INFO
	void PaintObstacle();
#endif

  private:
	
	//##Documentation
	//## �����������ʱ��һЩ����(���ڲ�ʹ��)
	//##ModelId=3DD7E4C101E9
	enum KREGION_WORK_PARAM
	{
		//##Documentation
		//## �ر���򻮷ָ��ӵ���Ŀ
		RWP_NUM_GROUND_CELL_H = 16, 
		//##Documentation
		//## �ر����򻮷ָ��ӵ���Ŀ
		RWP_NUM_GROUND_CELL_V = 16,

		RWP_NEST_REGION_0 = 1,
		//##Documentation
		//## Ĭ�ϵ�ͼ��ĺ��
		//## ��ͼ��ָ������ͼΪ���ϰ��ȼ��㻮�ֵ���С���ӵ�Ԫ��
		RWP_OBSTACLE_WIDTH = 32,
		//##Documentation
		//## Ĭ�ϵ�ͼ����ݿ�
		//## ��ͼ��ָ������ͼΪ���ϰ��ȼ��㻮�ֵ���С���ӵ�Ԫ��
		RWP_OBSTACLE_HEIGHT = 32,
	};

	//##ModelId=3DCE75E2003D
	//##Documentation
	//## ��������״̬ö��
	enum KREGION_STATUS { 
		//##Documentation
		//## ����������״̬��
		REGION_S_IDLE,
		//##Documentation
		//## �ȴ�ִ�м���
		REGION_S_TO_LOAD,
		//##Documentation
		//## ������������
		REGION_S_LOADING, 
		//##Documentation
		//## ����������ϣ����Ա�Ӧ��״̬��
		REGION_S_STANDBY,
		//##Documentation
		//## ���ݴ�����
		REGION_S_PROCESSING };

	struct KGroundLayerData
	{
		unsigned int uNumGrunode;	//�ر��ͼ�ε���Ŀ
		unsigned int uNumObject;	//�������������·��֮��Ķ������Ŀ
		KSPRCrunode* pGrunodes;
		KSPRCoverGroundObj* pObjects;
	};

	//##ModelId=3DE1311803A3
	//##Documentation
	//## �ڽ���������
	struct KBiosData
	{
		KBuildinObjFileHead	Numbers;
		KBuildinObj*		pBios;
		KIpotBuildinObj*	pLeafs;
		KBuildInLightInfo*	pLights;
	};
  private:

	//##ModelId=3DB90015018D
	//##Documentation
	//## �����ϰ���Ϣ��
	void LoadObstacle(		 
		//## �����ļ������ָ��
		KPakFile* pDataFile,
		//## �����ļ��Ĵ�С
		unsigned int uSize); 
	//##ModelId=3DBDADE00001
	//##Documentation
	//## ������Ʒ��Ϣ��
	bool LoadAboveGroundObjects(
		//## �����ļ������ָ��
		KPakFile* pDataFile,
		//## �����ļ��Ĵ�С
		unsigned int uSize); 
	//##ModelId=3DB9001D00BD
	//##Documentation
	//## ����ر���Ϣ
	bool LoadGroundLayer(
		//## �����ļ������ָ��
		KPakFile* pDataFile,
		//## �����ļ��Ĵ�С
		unsigned int uSize); 
	// �����¼�����Ϣ
	void LoadTrap(
		//## �����ļ������ָ��
		KPakFile* pDataFile,
		//## �����ļ��Ĵ�С
		unsigned int uSize); 

	void AddGrundCoverObj(KSPRCoverGroundObj* pObj);

  private:

	unsigned int	m_Flag;

	//##ModelId=3DBD531C02B2
	//##Documentation
	//## ״̬���
	KREGION_STATUS m_Status;
	
	//##ModelId=3DDC11C400A3
	//##Documentation
	//## Region���Ͻǵ�������������ͼ�е����꣨��λ�����ص㣩
	POINT m_LeftTopCornerScenePos;

	//##ModelId=3DBE392600F3
	//##Documentation
	//## ����������������������
	POINT m_RegionIndex;

	//##ModelId=3DE131AE01A0
	//##Documentation
	//## �ڽ���������
	KBiosData m_BiosData;

//-----�ر������-----
	KGroundLayerData	m_GroundLayerData;

	//##ModelId=3DD7F09402D4
	//##Documentation
	//## Ԥ����Ⱦ�ĵر��ͼ�Ρ�
	KRUImage* m_pPrerenderGroundImg;

	long		m_ObstacleInfo[RWP_NUM_GROUND_CELL_H][RWP_NUM_GROUND_CELL_V * 2];
	long		m_TrapInfo[RWP_NUM_GROUND_CELL_H][RWP_NUM_GROUND_CELL_V * 2];

};

#define	GROUND_IMG_OCCUPY_FLAG			Color.Color_b.b
#define	GROUND_IMG_OK_FLAG				Color.Color_b.g

#endif /* KSCENEPLACEREGIONC_H_HEADER_INCLUDED_C22EB91D */
