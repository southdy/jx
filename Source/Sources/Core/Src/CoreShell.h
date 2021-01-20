
/*****************************************************************************************
//	������Core�ӿڷ�������
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-9-12
------------------------------------------------------------------------------------------
	��磨�����ϵͳ��ͨ���˽ӿڴ�Core��ȡ��Ϸ�������ݡ�

	��������ӿں���CoreOperationRequest��CoreGetGameData��CoreDrawGameObj��CoreProcessInput
��CoreSetCallDataChangedNofify��
	CoreOperationRequest���ڷ��Ͷ���Ϸ�Ĳ������󡣲���uOperΪ������id����ֵΪ÷������
GAMEOPERATION_INDEX��ȡֵ֮һ������uParam�Լ�nParam�ľ��庬������uOper��ȡֵ���������
����ɹ����Ͳ������󣬺������ط�0ֵ�����򷵻�0ֵ����Щ����Ҫ��Core�������ܣ�Core��
�ͻ�����֤ͨ���˺������͵������ں�������֮����Ȼ��Ч��
	��֪��Ϸ���������ַ�ʽ��һ���ǵ��ýӿں���CoreGetGameData������ȡ������һ����ע��
֪ͨ����������Ϸ���ݱ����ʱ�򣬱�ע���֪ͨ�����ͻᱻ���ã���һ���ڵ���֪ͨ������ͬʱ
���ݸı����Ϸ���ݡ��������ַ�ʽ������������Χ������ͬ��
    �ӿں���CoreGetGameData����uDataId��ʾ��ȡ��Ϸ���ݵ�������������������ֵΪ÷������
GAMEDATA_INDEX��ȡֵ֮һ������uParam��nParam�Լ���������ֵ�ľ��庬������uDataId��ȡֵ
���������
    ע��֪ͨ�����Ľӿڷ���ΪCoreSetCallDataChangedNofify������pNotifyFuncΪ֪ͨ������
ָ�롣����ֵΪ��0ֵ��ʾע��ɹ��������ʾʧ�ܡ��������pNotifyFunc��ֵ��Ϊ0�����Ѿ�ע
���֪ͨ��������ȡ����ͨ��֪ͨ����֪ͨ������Ϸ���ݸı��ʱ�򣬲�һ��ͬʱͨ��֪ͨ����
�������ݸı䡣���ԭ����������ı�����������٣����Է���򵥵�ͨ��֪ͨ�����Ĳ������ݵģ�
����֪ͨ�������ݣ�����ֻ�Ƿ���֪ͨ���ѣ��������͸ı����Ϸ��

	CoreDrawGameObj���ڻ��Ƶ�����Ϸ���󡣲���uObjGenreָ�������������uIdָ�������id��
x��yָ�����Ʒ�Χ�����Ͻ����꣬Width��Heightnָ���˻��Ʒ�Χ�Ĵ�С��Param���ڶ���Ĳ�����
�ݣ��京�彫�����ھ���Ҫ���ƵĶ������͡�
*****************************************************************************************/

#pragma once

//=========================================================
// Core�ⲿ�ͻ���core��ȡ��Ϸ���ݵ�������������������
//=========================================================
//����������������ز���uParam��nParam�����ע����δ�ἰ���򴫵ݶ�ֵ0��
//����ر�ָ������ֵ���壬��ɹ���ȡ���ݷ���1��δ�ɹ�����0��
enum GAMEDATA_INDEX
{
	GDI_GAME_OBJ_DESC = 1,		//��Ϸ��������˵���ı���
	//uParam = (KUiObjAtContRegion*) ������Ϸ����Ľṹ���ݵ�ָ�룬����KUiObjAtContRegion::Region�����ݹ̶�Ϊ0�������塣
	//nParam = (KGameObjDesc*) ��ָ�Ľṹ�ռ����ڱ���ȡ�õ�����˵����
	
	GDI_GAME_OBJ_DESC_INCLUDE_TRADEINFO,	//��Ϸ��������˵���ı���(�������������Ϣ)
	//��������ͬGDI_GAME_OBJ_DESC

	GDI_GAME_OBJ_DESC_INCLUDE_REPAIRINFO,	//��Ϸ��������˵���ı���(�������������Ϣ)
	//��������ͬGDI_GAME_OBJ_DESC
	
	GDI_GAME_OBJ_LIGHT_PROP,	//����Ĺ�Դ��������
	//uParam = (KUiGameObject*) ������Ϸ����Ľṹ���ݵ�ָ��
	//nParam = to be def

	GDI_PLAYER_BASE_INFO,		//���ǵ�һЩ���ױ������
	//uParam = (KUiPlayerBaseInfo*)pInfo

	GDI_PLAYER_RT_INFO,			//���ǵ�һЩ�ױ������
	//uParam = (KUiPlayerRuntimeInfo*)pInfo

	GDI_PLAYER_RT_ATTRIBUTE,	//���ǵ�һЩ�ױ����������
	//uParam = (KUiPlayerAttribute*)pInfo

	GDI_PLAYER_IMMED_ITEMSKILL,	//���ǵ�����ʹ����Ʒ���书
	//uParam = (KUiPlayerImmedItemSkill*)pInfo

	GDI_PLAYER_HOLD_MONEY,		//��������Я����Ǯ
	//nRet = ��������Я����Ǯ

	GDI_PLAYER_IS_MALE,			//�����Ƿ�����
	//nRet = (int)(bool)bMale	�Ƿ�����

	// from now on, flying add this item, get the information whether
	// a player can ride a horse.
	GDI_PLAYER_CAN_RIDE,		//�����Ƿ����������
	//nRet = (int)bCanRide		�Ƿ����
	//0 - ����������Ŷ
	//1 - ����Ŷ

	GDI_ITEM_TAKEN_WITH,		//��������Я������Ʒ��
	//uParam = (KUiObjAtRegion*) pInfo -> KUiObjAtRegion�ṹ�����ָ�룬KUiObjAtRegion
	//				�ṹ���ڴ洢��Ʒ�����ݼ����������λ����Ϣ��
	//nParam = pInfo�����а���KUiObjAtRegion�ṹ����Ŀ
	//Return = �������ֵС�ڵ��ڴ������nParam����ֵ��ʾpInfo�����е�ǰ���ٸ�KUiObjAtRegion
	//			�ṹ���������Ч�����ݣ������ʾ��Ҫ����������ٸ�KUiObjAtRegion�ṹ������
	//			�Ź��洢ȫ��������Я������Ʒ��Ϣ��

	GDI_ITEM_IN_STORE_BOX,		//�����������Ʒ
	//����������ֵ����ͬGDI_ITEM_TAKEN_WITH��

	GDI_EQUIPMENT,				//����װ����Ʒ
	//uParam = (KUiObjAtRegion*)pInfo -> ����11��Ԫ�ص�KUiObjAtRegion�ṹ����ָ�룬
	//				KUiObjAtRegion�ṹ���ڴ洢װ�������ݺͷ���λ����Ϣ��
	//			KUiObjAtRegion::Region::h = 0
	//			KUiObjAtRegion::Region::v ��ʾ�����ĸ�λ�õ�װ��,��ֵΪ÷������
	//			UI_EQUIPMENT_POSITION��ȡֵ֮һ����ο�UI_EQUIPMENT_POSITION��ע�͡�
	//Return =  ��ֵ��ʾpInfo�����е�ǰ���ٸ�KUiObjAtRegion�ṹ���������Ч�����ݡ�

	GDI_TRADE_NPC_ITEM,			//npc�г������׵���Ʒ
	//uParam = (KUiObjAtContRegion*) pInfo -> KUiObjAtContRegion�ṹ�����ָ�룬KUiObjAtContRegion
	//				�ṹ���ڴ洢��Ʒ�����ݼ����������λ����Ϣ��
	//				����KUiObjAtContRegion::nContainerֵ��ʾ�ڼ�ҳ����Ʒ
	//nParam = pInfo�����а���KUiObjAtContRegion�ṹ����Ŀ
	//Return = �������ֵС�ڵ��ڴ������nParam����ֵ��ʾpInfo�����е�ǰ���ٸ�KUiObjAtContRegion
	//			�ṹ���������Ч�����ݣ������ʾ��Ҫ����������ٸ�KUiObjAtContRegion�ṹ������
	//			�Ź��洢ȫ����npc�г������׵���Ʒ��Ϣ��

	GDI_TRADE_ITEM_PRICE,		//������Ʒ�ļ۸�
	//uParam = (KUiObjAtContRegion*) pItemInfo -> ����ָ�����Ĵ����ĸ���Ʒ
	//nParam = (KUiItemBuySelInfo*) pPriceInfo -> ���ڽ�����Ʒ���ƽ��׼۸����Ϣ
	//Return = (int)(bool) �ɷ���

	GDI_REPAIR_ITEM_PRICE,		//������Ʒ�ļ۸�
	//uParam = (KUiObjAtContRegion*) pItemInfo -> ����ָ�����Ĵ����ĸ���Ʒ
	//nParam = (KUiItemBuySelInfo*) pPriceInfo -> ���ڽ�����Ʒ����������õ���Ϣ
	//Return = (int)(bool) �ɷ�����

	GDI_TRADE_OPER_DATA,		//���ײ�����ص�����
	//uParam = (UI_TRADE_OPER_DATA)eOper ���庬���UI_TRADE_OPER_DATA
	//nParam ����Ӧ���뺬����uParam��ȡֵ״������,��UI_TRADE_OPER_DATA��˵��
	//Return ���庬����uParam��ȡֵ״������,��UI_TRADE_OPER_DATA��˵��

	GDI_LIVE_SKILL_BASE,		//���ǵ����������
	//uParam = (KUiPlayerLiveSkillBase*) pInfo -> ���ǵ����������

	GDI_LIVE_SKILLS,			//�������յĸ��������
	//uParam = (KUiSkillData*) pSkills -> ����10��KUiSkillData���������ڴ洢��������ܵĵ����ݡ�

	GDI_FIGHT_SKILL_POINT,		//ʣ��ս�����ܵ���
	//Return = ʣ��ս�����ܵ���

	GDI_FIGHT_SKILLS,			//�������յĸ���ս������
	//uParam = (KUiSkillData*) pSkills -> ����50��KUiSkillData���������ڴ洢����ս�������ݡ�

	GDI_CUSTOM_SKILLS,			//���ǵ��Դ��书
	//uParam = (KUiSkillData*) pSkills -> ����5��KUiSkillData���������ڴ洢�����Դ��书�����ݡ�

	GDI_LEFT_ENABLE_SKILLS,		//���������������(������)����
	//uParam = (KUiSkillData*) pSkills -> ����65��KUiSkillData���������ڴ洢�����ܵ����ݡ�
	//								KUiSkillData::nLevel��������������ʾ�ڵڼ���
	//Return = ������Ч���ݵ�Skills����Ŀ

	GDI_RIGHT_ENABLE_SKILLS,	//���������Ҽ��ļ���
	//uParam = (KUiSkillData*) pSkills -> ����65��KUiSkillData���������ڴ洢�����ܵ����ݡ�
	//								KUiSkillData::nLevel��������������ʾ�ڵڼ���
	//Return = ������Ч���ݵ�Skills����Ŀ

	GDI_NEARBY_PLAYER_LIST,		//��ȡ��Χ��ҵ��б�
	//uParam = (KUiPlayerItem*)pList -> ��Ա��Ϣ�б�
	//			KUiPlayerItem::nData = 0
	//nParam = pList�����а���KUiPlayerItem�ṹ����Ŀ
	//Return = �������ֵС�ڵ��ڴ������nParam����ֵ��ʾpList�����е�ǰ���ٸ�KUiPlayerItem
	//			�ṹ���������Ч�����ݣ������ʾ��Ҫ����������ٸ�KUiPlayerItem�ṹ������
	//			�Ź��洢ȫ����Ա��Ϣ��

	GDI_NEARBY_IDLE_PLAYER_LIST,//��ȡ��Χ�µ������������ҵ��б�
	//��������ͬGDI_NEARBY_PLAYER_LIST

	GDI_NEARBY_NOT_FRIEND_LIST,//��ȡ��Χ�Ǻ��ѵ�����б�
	//��������ͬGDI_NEARBY_PLAYER_LIST

	GDI_PLAYER_LEADERSHIP,		//����ͳ˧������ص�����
	//uParam = (KUiPlayerLeaderShip*) -> ����ͳ˧������ص����ݽṹָ��

	GDI_ITEM_IN_ENVIRO_PROP,	//�����Ʒ��ĳ������λ�õ�����״̬
	//uParam = (KUiGameObject*)pObj����nParam==0ʱ����Ʒ����Ϣ
	//uParam = (KUiObjAtContRegion*)pObj����nParam!=0ʱ����Ʒ����Ϣ
	//			��ʱKUiObjAtContRegion::Region�����ݹ̶�Ϊ0�������塣
	//nParam = (int)(bool)bJustTry  �Ƿ�ֻ�ǳ��Է���
	//Return = (ITEM_IN_ENVIRO_PROP)eProp ��Ʒ������״̬
	
	GDI_CHAT_SEND_CHANNEL_LIST,		//��ҿ��Է�����Ϣ������Ƶ�����б�
	//uParam = (KUiChatChannel*) pList -> KUiChatChannel�ṹ�����ָ��
	//nParam = pList�����а���KUiChatChannel�ṹ����Ŀ
	//Return = �������ֵС�ڵ��ڴ������nParam����ֵ��ʾpInfo�����е�ǰ���ٸ�KUiChatChannel
	//			�ṹ���������Ч�����ݣ������ʾ��Ҫ����������ٸ�KUiChatChannel�ṹ������
	//			�Ź��洢ȫ����ָ��Ƶ����Ϣ��

	GDI_CHAT_RECEIVE_CHANNEL_LIST,	//��ҿ��Զ��ĵ���ϢƵ�����б�
	//����������ֵ����ͬGDI_CHAT_SEND_CHANNEL_LIST

	GDI_CHAT_CURRENT_SEND_CHANNEL,	//��ȡ��ǰ������Ϣ��Ƶ������Ϣ
	//uParam = (KUiChatChannel*) pChannelInfo ָ���������Ƶ������Ϣ�Ľṹ�ռ�

	GDI_CHAT_GROUP_INFO,			//����ĺ��ѷ�����Ϣ
	//��nParam������������ʼ���ҵ�һ����Ч�ķ��飬���ظ÷������Ϣ�����������
	//uParam = (KUiChatGroupInfo*) pGroupInfo ������Ϣ
	//nParam = nIndex ����ȡ�ķ��������
	//Return = ʵ�ʷ������ݵķ�������������δ����򷵻�-1

	GDI_CHAT_FRIENDS_IN_AGROUP,		//����һ�����ѷ����к��ѵ���Ϣ
	//uParam = (KUiPlayerItem*)pList -> ��Ա��Ϣ�б�
	//			KUiPlayerItem::nData = (CHAT_STATUS)eFriendStatus ���ѵĵ�ǰ״̬
	//nParam = Ҫ��ȡ�б�ĺ��ѷ��������
	//Return = ��ֵ��ʾpList�����е�ǰ���ٸ�KUiPlayerItem�ṹ���������Ч������.

	GDI_PK_SETTING,					//��ȡpk����
	//Return = (int)(bool)bEnable	�Ƿ�����pk

	GDI_SHOW_PLAYERS_NAME,			//��ȡ��ʾ���������
	//Return = (int)(bool)bShow	�Ƿ���ʾ
	GDI_SHOW_PLAYERS_LIFE,			//��ȡ��ʾ���������
	//Return = (int)(bool)bShow	�Ƿ���ʾ
	GDI_SHOW_PLAYERS_MANA,			//��ȡ��ʾ���������
	//Return = (int)(bool)bShow	�Ƿ���ʾ
	
	GDI_PARADE_EQUIPMENT,				//�����װ����Ʒ,��Ϣ����ͬGDI_EQUIPMENT
	
	GDI_IMMEDIATEITEM_NUM,				//ȡ�ÿ����Ʒ����
	//uParam = nIndex��0��1��2��
};

//=========================================================
// Core�ⲿ�ͻ���core��ȡ��Ϸ���ݵ�������������������
//=========================================================
//����������������ز���uParam��nParam�����ע����δ�ἰ���򴫵ݶ�ֵ0��
enum GAMEDATA_CHANGED_NOTIFY_INDEX
{
	GDCNI_HOLD_OBJECT = 1,	//������ĳ������
	//uParam = (KUiGameObject*)pObject -> ������Ϣ�����ȡֵΪ0����ʾ���������õĶ���

	GDCNI_PLAYER_BASE_INFO,	//���ǵ�һЩ���ױ������, see also GDI_PLAYER_BASE_INFO
	GDCNI_PLAYER_RT_ATTRIBUTE,	//���ǵ�һЩ�ױ����������, see also GDI_PLAYER_RT_ATTRIBUTE
	GDCNI_PLAYER_IMMED_ITEMSKILL,//���ǵ�����ʹ����Ʒ���书
	//uParam = (KUiGameObject*)pInfo ��Ʒ�����书������
	//nParam = �ĸ�λ�õ���Ʒ�����书 ��nParam >=0 ʱ�� nParam��ʾ��nParam��������Ʒ
	//			��nParam = -1 ʱ����ʾΪ�����书��-2ʱΪ�����书

	GDCNI_PLAYER_BRIEF_PROP,//������ݵ�λ��һЩ�ؼ�������ı仯
	//uParam = (PLAYER_BRIEF_PROP)eProp �仯�����ȡֵΪö��PLAYER_BRIEF_PROP��ֵ֮һ��
	//nParam = ��������uParam�ľ��庬�����

	GDCNI_OBJECT_CHANGED,		//ĳ����Ʒ���������߼���
	//uParam = (KUiObjAtContRegion*)pInfo -> ��Ʒ���ݼ����������λ����Ϣ
	//          ��pInfo->eContainer == UOC_EQUIPTMENTʱ
	//				KUiObjAtRegion::Region::h ��ʾ���ڵڼ���װ��
	//				KUiObjAtRegion::Region::v ��ʾ�����ĸ�λ�õ�װ��,��ֵΪö������
	//				UI_EQUIPMENT_POSITION��ȡֵ֮һ����ο�UI_EQUIPMENT_POSITION��ע�͡�
	//nParam = bAdd -> 0ֵ��ʾ���������Ʒ����0ֵ��ʾ���������Ʒ
	
	GDCNI_CONTAINER_OBJECT_CHANGED,	//���������Ķ���ȫ������
	//uParam = (UIOBJECT_CONTAINER)eContainer;	//���λ��

	GDCNI_LIVE_SKILL_BASE,		//�������ֵ�仯

	GDCNI_FIGHT_SKILL_POINT,	//ʣ��ս�����ܵ����仯
	//nParam = �µ�ʣ��ս�����ܵ���

	GDCNI_SKILL_CHANGE,			//��������һ������/��������
	//uParam = (KUiSkillData*)pSkill -> �����յļ���
	//nParam = �¼�����ͬ��е�����λ��

	GDCNI_PLAYER_LEADERSHIP,	//����ͳ˧������ص����ݷ����仯
	
	GDCNI_TEAM,					//������Ϣ�����仯�������ɢ���������������飬�ӳ�Ȩ��ת�����˵ȣ���ע���˵Ķ���ȣ�
	//uParam = (KUiPlayerTeam*)pTeam -> ������Ϣ,����ΪNULL,��ʾ��������˶���

	GDCNI_TEAM_NEARBY_LIST,		//����������б�
	//uParam = (KUiTeamItem*)pList �������Ϣ������
	//nParam = nCount �������Ŀ

	GDCNI_TRADE_START,			//����ҽ��׿�ʼ
	//uParam = (KUiPlayerItem*) pPlayer�Է�����Ϣ

	GDCNI_TRADE_DESIRE_ITEM,	//�Է������뽻�׵���Ʒ
	//uParam = (KUiObjAtRegion*) pObject -> ��Ʒ��Ϣ������������ϢΪ�ڽ��׽����е�����
	//nParam = bAdd -> 0ֵ��ʾ���٣�1ֵ��ʾ����
	//Remark : �����Ʒ�ǽ�Ǯ�Ļ�����KUiObjAtRegion::Obj::uId��ʾ�ѽ�Ǯ�����Ϊ���ֵ����nParam�����塣

	GDCNI_TRADE_OPER_DATA,		//���ײ�����ص�(״̬)���ݷ����仯
	//uParam = (const char*) pInfoText ��ʾ�����֣���"�Է��������"��

	GDCNI_TRADE_END,			//���׽���
	//nParam = (int)(bool)bTraded	�Ƿ�����˽���

	GDCNI_NPC_TRADE,			//��npc���׿�ʼ
	//nParam = (bool)bStart	�Ƿ���Ϊ���׿�ʼ�����ȡֵ�����ʾ���ף��رգ�����

	GDCNI_NPC_TRADE_ITEM,		//npc������Ʒ�ı仯
	//ͨ������iCoreShell::GetGameData,ʹ����uDataIdΪGDI_TRADE_NPC_ITEM������ȡ�µ���Ʒ��Ϣ��

	GDCNI_GAME_START,			//������Ϸ����

	GDCNI_GAME_QUIT,			//�뿪����Ϸ����

	GDCNI_QUESTION_CHOOSE,		//����ѡ��
	//uParam = (KUiQuestionAndAnswer*)pQuestionAndAnswer

	GDCNI_SPEAK_WORDS,			//npc˵������
	//uParam = (KUiInformationParam*) pWordDataList ָ��KUiInformationParam����
	//nParam = pWordDataList����KUiInformationParamԪ�ص���Ŀ

	GDCNI_INFORMATION,			//�µ���Ϣ����������ר�ŵ���Ϣ��
	//uParam = (KUiInformationParam*)pInformation	//��Ϣ����

	GDCNI_MSG_ARRIVAL,			//�£��������ݴ��ڣ���Ϣ����
	//uParam = (cons char*)pMsgBuff ��Ϣ���ݻ�����
	//nParam = (KUiMsgParam*)pMsgParam ��Ϣ����

	GDCNI_SEND_CHAT_CHANNEL,	//��ǰ������Ϣ��Ƶ���ı���

	GDCNI_CHAT_GROUP,			//������ѷ��鷢���仯

	GDCNI_CHAT_FRIEND,			//������ѷ����仯
	//nParam = nGroupIndex �������ѱ仯�ķ��������

	GDCNI_CHAT_FRIEND_STATUS,	//�������״̬�����仯
	//uParam = (KUiPlayerItem*)pFriend ����״̬�仯�ĺ���
	//			KUiPlayerItem::nData = (CHAT_STATUS)eFriendStatus ���ѵĵ�ǰ״̬
	//nParam = nGroupIndex ���������ڵ��������
	
	GDCNI_CHAT_MESSAGE,			//���з�����ѶϢ
	//uParam = (KUiChatMessage*)pMessage ��Ϣ����
	//nParam = (KUiPlayerItem*)pFriend   ����ѶϢ�ĺ���

	GDCNI_SYSTEM_MESSAGE,		//ϵͳ��Ϣ
	//uParam = (KSystemMessage*)pMsg ϵͳ��Ϣ������
	//nParam = (void*)pParamBuf ָ��һ�����������������������ݺ�������pMsg�����ݶ�����
	//			�ο�KSystemMessage��ע��˵�����������Ĵ�С��pMsg->byParamSize������

	GDCNI_NEWS_MESSAGE,			//������Ϣ
	//uParam = (KNewsMessage*)pMsg ������Ϣ����
	//nParam = (SYSTEMTIME*)pTime  ������Ϣ��ʱ����������庬��������������Ϣ�����Ͷ���

	GDCNI_SWITCH_CURSOR,		//�л����ָ��ͼ��
	//nParam = \Ui\???\���.ini�и�����ͼ��������ֵ

	GDCNI_OPEN_STORE_BOX,		//�򿪴�����
	
	GDCNI_SWITCHING_SCENEPLACE,	//��ͼ�л�
	//nParam = (int)(bool)bStart Ϊ��0ֵ��ʾ��ʼ��ͼ�л���Ϊ0ֵ��ʾ������ͼ�л�

	GDCNI_MISSION_RECORD,		//Ҫ���¼�´�������ʾ��Ϣ
	//uParam = (KMissionRecord*) pRecord ��¼����

	GDCNI_PK_SETTING,			//pk����״�������˱仯
	//nParam = (int)(bool)bEnable �Ƿ�����pk

	GDCNI_VIEW_PLAYERITEM,			//���������װ���������ѵ�
	//uParam = (KUiPlayerItem*)		//��ҵ� player data

	GDCNII_RANK_INDEX_LIST_ARRIVE,	//������id�б���
	//uParam = uCount ������id����Ŀ
	//nParam = (KRankIndex*)pIndexList ������id�����ṹ������

	GDCNII_RANK_INFORMATION_ARRIVE,	//����������ݵ���
	//uParam = ((unsigned short)usCount) | ((unsigned short)usIndexId << 16)
	//			usCount    �˴δ��������������Ŀ
	//			usIndexId  ������id����ʾ���ص������id�������������
	//nParam = (KRankMessage *) �б�����

	//====�������====
	GDCNI_TONG_INFO,				//ĳ�����ڵİ��ɵ���Ϣ
									//��Ӧ��GTOI_REQUEST_PLAYER_TONG����ѯĳ�����İ�ᣩ
	//uParam = (KUiPlayerRelationWithOther*) �Ǹ���ĳ����,�Լ�������˰��ɵĹ�ϵ
					//KUiPlayerRelationWithOther::nParam �Ƿ��ŵ����˿���
	//nParam = (KTongInfo*) pToneInfo	//���ɵ���Ϣ

	GDCNI_TONG_MEMBER_LIST,			//ĳ�������еĳ�Ա���б�
	//uParam = (KUiGameObjectWithName*) pTong �����������ĸ�����
							//KUiGameObjectWithName::szName ��������
							//KUiGameObjectWithName::nData  pMemberList�������ɳ�Ա�����ͣ��ο�TONG_MEMBER_FIGURE
							//KUiGameObjectWithName::nParam pMemberList�������ɳ�Ա�б������Ա����ʼ������
							//KUiGameObjectWithName::uParam pMemberList�������ɳ�Ա�б������Ա����Ŀ
	//nParam = (KTongMemberItem*) pMemberList

	GDCNI_TONG_ACTION_RESULT,       //���������صĽ��
	//uParam = (KUiGameObjectWithName*) pInfo ���ص�����
	                        //KUiGameObjectWithName::szName ������ҵ�����
							//KUiGameObjectWithName::nData  ���������࣬�ο�TONG_ACTION_TYPE
							//KUiGameObjectWithName::nParam   V
							//KUiGameObjectWithName::uParam   V
							//KUiGameObjectWithName::szString �⼸�������治ͬ������ͬ
	// nParam : �ɹ� 1 ʧ�� 0

	GDCNI_OPEN_TONG_CREATE_SHEET,   //֪ͨ����򿪰�ᴴ������
	//uParam = ~0 ��    0 �ر�
};

enum GAMEDEBUGCONTROL
{
	DEBUG_SHOWINFO = 1,
	DEBUG_SHOWOBSTACLE,
};

//=========================================================
// Core�ⲿ�ͻ���core�Ĳ����������������
//=========================================================
enum GAMEOPERATION_INDEX
{
	GOI_EXIT_GAME = 1,		//�뿪��Ϸ
	//uParam = bIpSpotExit

	GOI_SWITCH_OBJECT_QUERY,		//����
	//uParam = (KUiObjAtContRegion*)pObject1 -> �������Ʒ����ǰ����Ϣ
	//���������Ķ�������uParam = 0
	//nParam = (KUiObjAtContRegion*)pObject2 -> ���µ���Ʒ���������Ϣ
	//����޷��µĶ�������nParam = 0
	//nRet = bSwitchable -> �Ƿ�ɽ���

	GOI_SWITCH_OBJECT,		//����
	//uParam = (KUiObjAtContRegion*)pObject1 -> �������Ʒ����ǰ����Ϣ
	//nParam = (KUiObjAtContRegion*)pObject2 -> ���µ���Ʒ���������Ϣ
	//nRet = bSwitched -> �Ƿ񽻻���

	GOI_REJECT_OBJECT,		//������Ʒ
	//uParam = (KUiObjAtContRegion*)pObject -> ����������Ʒ

	GOI_MONEY_INOUT_STORE_BOX,	//��StoreBox��ȡǮ
	//uParam = (unsigned int)bIn Ϊ��0ֵʱ��ʾ���룬�����ʾȡ��
	//nParam = Ǯ������

	GOI_PLAYER_ACTION,		//���ִ��/ȡ��ĳ������
	//uParam = (PLAYER_ACTION_LIST)eAction ������ʶ

	GOI_PLAYER_RENASCENCE,		//�������
	//nParam = (int)(bool)bBackTown �Ƿ�س�

	GOI_INFORMATION_CONFIRM_NOTIFY,	//��Ϣ���ȷ�ϵ�֪ͨ

	GOI_QUESTION_CHOOSE,	//����ѡ���
	//nParma = nAnswerIndex

	GOI_USE_ITEM,			//ʹ����Ʒ
	//uParam = (KUiObjAtRegion*)pInfo -> ��Ʒ�������Լ���Ʒԭ���ڷŵ�λ��
	//nParam = ��Ʒʹ��ǰ���õ�λ�ã�ȡֵΪö������UIOBJECT_CONTAINER��

	GOI_WEAR_EQUIP,			//����װ��
	//uParam = (KUiObjAtRegion*)pInfo -> װ�������ݺͷ���λ����Ϣ
	//			KUiObjAtRegion::Region::h ��ʾ���ڵڼ���װ��
	//			KUiObjAtRegion::Region::v ��ʾ�����ĸ�λ�õ�װ��,��ֵΪ÷������
	//			UI_EQUIPMENT_POSITION��ȡֵ֮һ����ο�UI_EQUIPMENT_POSITION��ע�͡�

	GOI_USE_SKILL,			//ʩչ�书/����
	//uParam = (KUiGameObject*)pInfo -> ��������
	//nParam = (Ŀǰֻ��0ֵ��)���ָ�뵱ǰ���꣨�������꣩���������ڵ�16λ���������ڸ�16λ��(���ص�����)

	GOI_SET_IMMDIA_SKILL,	//������������
	//uParam = (KUiGameObject*)pSKill, ������Ϣ
	//nParam = ����λ�ã�0��ʾΪ������ܣ�1��ʾΪ�Ҽ�����

	GOI_TONE_UP_SKILL,		//��ǿһ�ּ��ܣ���һ�μ�һ��
	//uParam = ��������
	//nParam = (uint)����id

	GOI_TONE_UP_ATTRIBUTE,	//��ǿһЩ���Ե�ֵ��һ�μ�һ��
	//uParam = ��ʾҪ��ǿ�����ĸ����ԣ�ȡֵΪUI_PLAYER_ATTRIBUTE��÷��ֵ֮һ

	//============����������ң��������================
	GOI_TRADE_INVITE_RESPONSE,	//��Ӧ/�ܾ���������
	//uParam = (KUiPlayerItem*)pRequestPlayer ������������
	//nParam = (int)(bool)bAccept �Ƿ��������

	GOI_TRADE_DESIRE_ITEM,		//����һ������������Ʒ
	//uParam = (KUiObjAtRegion*) pObject -> ��Ʒ��Ϣ������������ϢΪ�ڽ��׽����е�����
	//nParam = bAdd -> 0ֵ��ʾ���٣�1ֵ��ʾ����
	//Remark : �����Ʒ�ǽ�Ǯ�Ļ�����KUiObjAtRegion::Obj::uId��ʾ�ѽ�Ǯ�����Ϊ���ֵ����nParam�����塣

	GOI_TRADE_WILLING,			//���޽�������
	//uParam = (const char*)pszTradMsg ���ڽ�����Ϣһ�仰����bWillingΪtrueʱ��Ч
	//nParam = (int)(bool)bWilling �Ƿ��ڴ�����(����)

	GOI_TRADE_LOCK,				//��������
	//nParam = (int)(bool)bLock �Ƿ�����

	GOI_TRADE,					//����
	//nParam = (int)(bool)bTrading
	
	GOI_TRADE_CANCEL,			//����ȡ��

	//============����npc���������================
	GOI_TRADE_NPC_BUY,			//��npc����Ʒ
	//uParam = (KUiGameObject*)pObj -> ��Ʒ��Ϣ

	GOI_TRADE_NPC_SELL,			//����Ʒ��npc
	//uParam = (KUiObjAtContRegion*)pObj -> ��Ʒ��Ϣ

	GOI_TRADE_NPC_REPAIR,		//������Ʒ
	//uParam = (KUiObjAtContRegion*) pObj -> ��Ҫ����Ʒ����Ϣ

	GOI_TRADE_NPC_CLOSE,		//��������

	GOI_DROP_ITEM_QUERY,		//��ѯ�Ƿ���Զ�ĳ����������Ϸ����
	//uParam = (KUiGameObject*)pObject -> ��Ʒ��Ϣ
	//nParam = ���϶������ĵ�ǰ���꣨�������꣩���������ڵ�16λ���������ڸ�16λ��(���ص�����)
	//Return = �Ƿ���Է���
	
//	GOI_DROP_ITEM,				//������Ʒ����Ϸ����
	//��������ͬGOI_DROP_ITEM_QUERY����������ͬ
	//Return = �Ƿ�����������

	GOI_SEND_MSG,				//������Ϣ
	//uParam = (const char*)pMsgBuff ��Ϣ���ݻ�����
	//nParam = (KUiMsgParam*)pMsgParam ��Ϣ����
	
	//============�������================
	GOI_SET_SEND_CHAT_CHANNEL,	//���õ�ǰ������Ϣ��Ƶ��
	//uParam = (KUiChatChannel*) pChannelInfo Ҫ���õ�Ƶ������Ϣ
	
	GOI_SET_SEND_WHISPER_CHANNEL,//���õ�ǰ������Ϣ��Ƶ��Ϊ��ĳ�˵���
	//uParam = (KUiPlayerItem*) pFriend

	GOI_SET_RECEIVE_CHAT_CHANNEL,//���ö���/ȡ����ϢƵ��
	//uParam = (KUiChatChannel*) pChannelInfo Ҫ���õ�Ƶ������Ϣ
	//nParam = (int)(bool) bEnable �Ƿ���

	GOI_CHAT_GROUP_NEW,			//�½����������
	//uParam = (const char*) pGroupName
	//Return = (int)(bool) bSuccessed

	GOI_CHAT_GROUP_RENAME,		//���������������
	//uParam = (const char*) pGroupName
	//nParam = nIndex ������
	//Return = (int)(bool) bSuccessed

	GOI_CHAT_GROUP_DELETE,		//ɾ�����������
	//nParam = nIndex ������
	//Return = (int)(bool) bSuccessed

	GOI_CHAT_FRIEND_ADD,		//������������
	//uParam = (KUiPlayerItem*)pFriend
	//			KUiPlayerItem::nData = 0

	GOI_CHAT_FRIEND_DELETE,		//ɾ���������
	//uParam = (KUiPlayerItem*)pFriend
	//			KUiPlayerItem::nData = 0
	//nParam = nGroupIndex �������ڵ��������
	//Return = (int)(bool) bSuccessed

	GOI_CHAT_FRIEND_MOVE,		//�����Ƶ�����
	//uParam = (KUiPlayerItem*)pFriend
	//			KUiPlayerItem::nData = 0
	//nParam = nGroupIndex �������������������
	//Return = (int)(bool) bSuccessed

	GOI_CHAT_SET_STATUS,		//�л�����״̬
	//uParam = (CHAT_STATUS)eStatus �µ�����״̬

	GOI_CHAT_WORDS_TO_FRIEND,	//��������һ��ѶϢ
	//uParam = (KUiChatMessage*)pMessage ��Ϣ����
	//nParam = (KUiPlayerItem*)pFriend   ����ѶϢ�ĺ���

	GOI_CHAT_FRIEND_INVITE,		//�Ա���Ҫ���Լ�Ϊ���ѵĻظ�
	//uParam = (KUiPlayerItem*)pRequestPlayer ������������
	//nParam = (int)(bool)bAccept �Ƿ��������

	GOI_OPTION_SETTING,			//ѡ������
	//uParam = (OPTIONS_LIST)eOptionItem Ҫ���õ�ѡ��
	//nParam = (int)nValue ���õ�ֵ���京��������eOptionItem�ĺ���
	//					�ο�OPTIONS_LIST��ֵ��ע��

	GOI_PLAY_SOUND,				//��������
	//uParam = (const char*)pszFileName

	GOI_PK_SETTING,				//����PK
	//nParam = (int)(bool)bEnable	�Ƿ�����pk

	GOI_REVENGE_SOMEONE,		//��ɱĳ��
	//uParam = (KUiPlayerItem*) pTarget	��ɱĿ��

	GOI_SHOW_PLAYERS_NAME,		//��ʾ���������
	//nParam = (int)(bool)bShow	�Ƿ���ʾ
	GOI_SHOW_PLAYERS_LIFE,		//��ʾ���������
	//nParam = (int)(bool)bShow	�Ƿ���ʾ
	GOI_SHOW_PLAYERS_MANA,		//��ʾ���������
	//nParam = (int)(bool)bShow	�Ƿ���ʾ

	GOI_GAMESPACE_DISCONNECTED,	//��Ϸ����Ͽ�������
	
	GOI_VIEW_PLAYERITEM,		//���뿴���װ��
	//uParam = dwNpcID	��ҵ�m_dwID
	GOI_VIEW_PLAYERITEM_END,	//�����װ������

	GOI_FOLLOW_SOMEONE,			//����ĳ��
	//uParam = (KUiPlayerItem*) pTarget	����Ŀ��

	GOI_QUERY_RANK_INFORMATION,  //��ȡ��������
	//uParam = usIndexId �������id
};

//=========================================================
// Core�ⲿ�ͻ���core�ĳ�����ͼ��صĲ����������������
//=========================================================
//����������������ز���uParam��nParam�����ע����δ�ἰ���򴫵ݶ�ֵ0��
//����ر�ָ������ֵ���壬��ɹ���ȡ���ݷ���1��δ�ɹ�����0��
enum GAME_SCENE_MAP_OPERATION_INDEX
{
	GSMOI_SCENE_TIME_INFO,			//��ǰ���������ĵ���ʱ�价��
	//uParam = (KUiSceneTimeInfo*)pInfo

	GSMOI_SCENE_MAP_INFO,				//��ǰ���������ĳ����ĵ�ͼ��Ϣ
	//uParam = (KSceneMapInfo*) pInfo ���ڻ�ȡ��Ϣ�Ľṹ��������ָ��
	//Return = (int)(bool)bHaveMap ����ֵ��ʾ��ǰ�����Ƿ���С��ͼ���������0ֵʱ�� pInfo�ڷ��ص�ֵ������

	GSMOI_IS_SCENE_MAP_SHOWING,	//���ó�����С��ͼ�Ƿ���ʾ��״̬
	//uParam = uShowElem,		//��ʾ��Щ���ݣ�ȡֵΪSCENE_PLACE_MAP_ELEMö�ٵ�һ����������ϡ�
				//SCENE_PLACE_MAP_ELEM��GameDataDef.h�ж���
				//���С��ͼ������һЩ���ǻ����
	//nParam = ��16λ��ʾ��ʾ�Ŀ�ȣ���16λ��ʾ��ʾ�ĸ߶ȣ���λ�����ص㣩

	GSMOI_PAINT_SCENE_MAP,		//���Ƴ�����С��ͼ
	//uParam = (int)h ��ʾ������ʼ������Ļ�Ϻ��������꣨��λ�����ص㣩
	//nParam = (int)v ��ʾ������ʼ������Ļ�����������꣨��λ�����ص㣩

	GSMOI_SCENE_MAP_FOCUS_OFFSET,//����С��ͼ�Ľ��㣨/���ģ�
	//uParam = (int)nOffsetH	����С��ͼ�����ˮƽ���꣨��λ���������꣩
	//nParam = (int)nOffsetV	����С��ͼ����Ĵ�ֵ���꣨��λ���������꣩

	GSMOI_SCENE_FOLLOW_WITH_MAP,	//���ó����Ƿ����ŵ�ͼ���ƶ����ƶ�
	//nParam = (int)nbEnable �����Ƿ����ŵ�ͼ���ƶ����ƶ�
};

//=========================================================
// Core�ⲿ�ͻ���core�İ����صĲ����������������
//=========================================================
//����������������ز���uParam��nParam�����ע����δ�ἰ���򴫵ݶ�ֵ0��
//����ر�ָ������ֵ���壬��ɹ���ȡ���ݷ���1��δ�ɹ�����0��
enum GAME_TONG_OPERATION_INDEX
{
	GTOI_TONG_CREATE,			//�������
	//uParam = (const char*) pszTongName ��������
	//nParam = (NPCCAMP)enFaction �����Ӫ

	GTOI_TONG_IS_RECRUIT,		//��ѯĳ�˵����˿���
	//uParam = (KUiPlayerItme*) Ҫ��˭
	//Return = (int)(bool)		�Ƿ��ŵ����˿���

	GTOI_TONG_RECRUIT,          //���˿���
	//uParam = (int)(bool)bRecruit �Ƿ�Ը������

	GTOI_TONG_ACTION,           //�԰��ڳ�Ա���Ķ��������Լ�����Ĺ�ϵ�ĸı�
	//uParam = (KTongOperationParam*) pOperParam ����ʱ�Ĳ���
	//nParam = (KTongMemberItem*) pMember ָ���˲���������Ա������

	GTOI_TONG_JOIN_REPLY,       //���������Ĵ�
	//uPAram = (KUiPlayerItem *) pTarget   ���뷽
	//nParam : !=0ͬ��     ==0�ܾ�

	GTOI_REQUEST_PLAYER_TONG,	//��ѯĳ�����İ��
	//uParam = (KUiPlayerItem*) Ҫ��˭
	//nParam = (int)(bool)bReturnTongDetail �Ƿ�Ҫ�����Ǹ�������Ϣ

	GTOI_REQUEST_TONG_DATA,     //Ҫ��ĳ�����ĸ�������
	//uParam = (KUiGameObjectWithName*)pTong Ҫ��ѯ�İ��
			//KUiGameObjectWithName::szName ��������
			//KUiGameObjectWithName::nData ���ϵ����ֵ࣬ȡ��ö��TONG_MEMBER_FIGURE
			//			�б��������enumTONG_FIGURE_MASTER�Ļ�����Ҫ����ǰ�����Ѷ��
			//KUiGameObjectWithName::nParam ��ʼ������
};

//=========================================================
// Core�ⲿ�ͻ���core�������صĲ����������������
//=========================================================
//����������������ز���uParam��nParam�����ע����δ�ἰ���򴫵ݶ�ֵ0��
//����ر�ָ������ֵ���壬��ɹ���ȡ���ݷ���1��δ�ɹ�����0��
enum GAME_TEAM_OPERATION_INDEX
{
	//----��ȡ����----
	TEAM_OI_GD_INFO,				//�������ڵĶ�����Ϣ
	//uParam = (KUiPlayerTeam*)pTeam -> ������Ϣ
	//Return = bInTeam, ���Ϊ��0ֵ��ʾ�����ڶ����У�pTeam�ṹ�Ƿ������Ϣ��
	//					���Ϊ0ֵ��ʾ���ǲ��ڶ����У�pTeam�ṹδ�������Ч��Ϣ��

	TEAM_OI_GD_MEMBER_LIST,		//��ȡ�������ڶ����Ա�б�
	//uParam = (KUiPlayerItem*)pList -> ��Ա��Ϣ�б�
	//			KUiPlayerItem::nData = (int)(bool)bCaptain �Ƿ��Ƕӳ�
	//nParam = pList�����а���KUiPlayerItem�ṹ����Ŀ
	//Return = �������ֵС�ڵ��ڴ������nParam����ֵ��ʾpList�����е�ǰ���ٸ�KUiPlayerItem
	//			�ṹ���������Ч�����ݣ������ʾ��Ҫ����������ٸ�KUiPlayerItem�ṹ������
	//			�Ź��洢ȫ���ĳ�Ա��Ϣ��

	TEAM_OI_GD_REFUSE_INVITE_STATUS,//��ȡ�ܾ������״̬
	//Return = (int)(bool)bEnableRefuse Ϊ��ֵ��ʾ�ܾ�״̬��Ч�������ʾ���ܾ���

	//----��������----
	TEAM_OI_COLLECT_NEARBY_LIST,//��ȡ��Χ������б�

	TEAM_OI_APPLY,				//����������˶���
	//uParam = (KUiTeamItem*)	Ҫ�������Ķ������Ϣ

	TEAM_OI_CREATE,				//�������

	TEAM_OI_APPOINT,			//�����ӳ���ֻ�жӳ����ò���Ч��
	//uParam = (KUiPlayerItem*)pPlayer -> �¶ӳ�����Ϣ
	//			KUiPlayerItem::nData = 0

	TEAM_OI_INVITE,			//������˼�����飬ֻ�жӳ����ò���Ч��
	//uParam = (KUiPlayerItem*)pPlayer -> Ҫ������˵���Ϣ
	//			KUiPlayerItem::nData = 0

	TEAM_OI_KICK,				//�߳������һ����Ա��ֻ�жӳ����ò���Ч��
	//uParam = (KUiPlayerItem*)pPlayer -> Ҫ�߳��Ķ�Ա����Ϣ
	//			KUiPlayerItem::nData = 0

	TEAM_OI_LEAVE,				//�뿪����

	TEAM_OI_CLOSE,				//�ر���ӣ�ֻ�жӳ����ò���Ч��
	//nParam = (int)(bool)bClose Ϊ��ֵ��ʾ���ڹر�״̬�������ʾ�����ڹر�״̬

	TEAM_OI_REFUSE_INVITE,		//�ܾ����������Լ��������
	//nParam = (int)(bool)bEnableRefuse Ϊ��ֵ��ʾ�ܾ�״̬��Ч�������ʾ���ܾ���

	TEAM_OI_APPLY_RESPONSE,			//�Ƿ���׼���˼�����飬ֻ�жӳ����ò���Ч��
	//uParam = (KUiPlayerItem*)pPlayer -> Ҫ����׼������Ϣ
	//			KUiPlayerItem::nData = 0
	//nParam = (int)(bool)bApprove -> �Ƿ���׼��

	TEAM_OI_INVITE_RESPONSE,	//���������Ļظ�
	//uParam = (KUiPlayerItem*)pTeamLeader �����������Ķӳ�
	//nParam = (int)(bool)bAccept �Ƿ��������

};


//-------��Ϸ�������ݸı��֪ͨ����ԭ��---------
struct IClientCallback
{
	virtual void CoreDataChanged(unsigned int uDataId, unsigned int uParam, int nParam) = 0;
	virtual void ChannelMessageArrival(DWORD nChannelID, char* szSendName, const char* pMsgBuff, unsigned short nMsgLength, bool bSucc) = 0;
	virtual void MSNMessageArrival(char* szSourceName, char* szSendName, const char* pMsgBuff, unsigned short nMsgLength, bool bSucc) = 0;
	virtual void NotifyChannelID(char* ChannelName, DWORD channelid, BYTE cost) = 0;
	virtual void FriendInvite(char* roleName) = 0;
	virtual void AddFriend(char* roleName, BYTE answer) = 0;
	virtual void FriendStatus(char* roleName, BYTE state) = 0;
	virtual void FriendInfo(char* roleName, char* unitName, char* groupname, BYTE state) = 0;
	virtual void AddPeople(char* unitName, char* roleName) = 0;
};

struct _declspec (novtable) iCoreShell
{
	virtual	int	 GetProtocolSize(BYTE byProtocol) = 0;
	virtual int	 Debug(unsigned int uDataId, unsigned int uParam, int nParam) = 0;
	//����Ϸ���Ͳ���
	virtual int	 OperationRequest(unsigned int uOper, unsigned int uParam, int nParam) = 0;
	//���������豸�����������Ϣ
	virtual void ProcessInput(unsigned int uMsg, unsigned int uParam, int nParam) = 0;
	//��x, y��ָ��Player
	virtual int FindSelectNPC(int x, int y, int nRelation, bool bSelect, void* pReturn, int& nKind) = 0;
	//��Nameָ����Player
	virtual int FindSpecialNPC(char* Name, void* pReturn, int& nKind) = 0;
	//��x, y��ָ��Obj
	virtual int FindSelectObject(int x, int y, bool bSelect, int& nObjectIdx, int& nKind) = 0;
	virtual int ChatSpecialPlayer(void* pPlayer, const char* pMsgBuff, unsigned short nMsgLength) = 0;
	virtual void ApplyAddTeam(void* pPlayer) = 0;
	virtual void TradeApplyStart(void* pPlayer) = 0;
	virtual int UseSkill(int x, int y, int nSkillID) = 0;
	virtual int LockSomeoneUseSkill(int nTargetIndex, int nSkillID) = 0;
	virtual int LockSomeoneAction(int nTargetIndex) = 0;
	virtual int LockObjectAction(int nTargetIndex) = 0;
	virtual void GotoWhere(int x, int y, int mode) = 0;	//mode 0 is auto, 1 is walk, 2 is run
	virtual void Goto(int nDir, int mode) = 0;	//nDir 0~63, mode 0 is auto, 1 is walk, 2 is run
	virtual void Turn(int nDir) = 0;	//nDir 0 is left, 1 is right, 2 is back
	virtual int ThrowAwayItem() = 0;
	virtual int GetNPCRelation(int nIndex) = 0;

	//===========����Ϸ����ķ������==========
	//���ͼ��صĲ���, uOper��ȡֵ���� GAME_SCENE_MAP_OPERATION_INDEX
	virtual int	SceneMapOperation(unsigned int uOper, unsigned int uParam, int nParam) = 0;
	//������صĲ���, uOper��ȡֵ���� GAME_TONG_OPERATION_INDEX
	virtual int	TongOperation(unsigned int uOper, unsigned int uParam, int nParam) = 0;
	//�������صĲ�����uOper��ȡֵ���� GAME_TEAM_OPERATION_INDEX
	virtual int TeamOperation(unsigned int uOper, unsigned int uParam, int nParam) = 0;

	//����Ϸ�����ȡ����
	virtual int	 GetGameData(unsigned int uDataId, unsigned int uParam, int nParam) = 0;

	//������Ϸ����
	virtual void DrawGameObj(unsigned int uObjGenre, unsigned int uId, int x, int y, int Width, int Height, int nParam) = 0;
	//������Ϸ����
	virtual void DrawGameSpace() = 0;

	virtual DWORD GetPing() = 0;
	//virtual void SendPing() = 0;
	//������Ϸ�������ݸı��֪ͨ����
	virtual int	 SetCallDataChangedNofify(IClientCallback* pNotifyFunc) = 0;
	//��������ɴ���������Ϣ
	virtual void NetMsgCallbackFunc(void* pMsgData) = 0;
	//���û�ͼ�ӿ�ʵ����ָ��
	virtual void SetRepresentShell(struct iRepresentShell* pRepresent) = 0;
	virtual void SetMusicInterface(void* pMusicInterface) = 0;
	virtual void SetRepresentAreaSize(int nWidth, int nHeight) = 0;
	//�ճ����core���Ҫ���������򷵻�0�����򷵻ط�0ֵ
	virtual int  Breathe() = 0;
	//�ͷŽӿڶ���
	virtual void Release() = 0;
	virtual void SetClient(LPVOID pClient) = 0;
	
	virtual void SendNewDataToServer(void* pData, int nLength) = 0;
};

#ifndef CORE_EXPORTS

	//��ȡiCoreShell�ӿ�ʵ����ָ��
	extern "C" iCoreShell* CoreGetShell();

#else

	//���ⷢ����Ϸ�������ݷ����ı��֪ͨ
	void	CoreDataChanged(unsigned int uDataId, unsigned int uParam, int nParam);

#endif
