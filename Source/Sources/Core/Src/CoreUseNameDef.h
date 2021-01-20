
#ifndef COREUSENAMEDEF_H
#define COREUSENAMEDEF_H

#define		MAX_PLAYER_IN_ACCOUNT			3

#define		SETTING_PATH					"\\settings"

//---------------------------- npc res ��� ------------------------------
#define		RES_INI_FILE_PATH				"\\settings\\npcres"
#define		RES_NPC_STYLE_PATH				"\\spr\\npcres\\style"
#define		RES_SOUND_FILE_PATH				"sound"

#define		NPC_RES_KIND_FILE_NAME			"\\settings\\npcres\\��������.txt"
#define		NPC_NORMAL_RES_FILE				"\\settings\\npcres\\��ͨnpc��Դ.txt"
#define		NPC_NORMAL_SPRINFO_FILE			"\\settings\\npcres\\��ͨnpc��Դ��Ϣ.txt"
#define		STATE_MAGIC_TABLE_NAME			"\\settings\\npcres\\״̬ͼ�ζ��ձ�.txt"
#define		PLAYER_RES_SHADOW_FILE			"\\settings\\npcres\\���Ƕ�����Ӱ��Ӧ��.txt"
#define		NPC_RES_SHADOW_FILE				"��ͨnpc������Ӱ��Ӧ��.txt"
#define		PLAYER_SOUND_FILE				"\\settings\\npcres\\���Ƕ���������.txt"
#define		NPC_SOUND_FILE					"\\settings\\npcres\\npc����������.txt"

#define		NPC_ACTION_NAME					"npc������.txt"
#define		ACTION_FILE_NAME				"������ű�.txt"

#define		PLAYER_MENU_STATE_RES_FILE		"\\settings\\npcres\\����״̬��ͼ�ζ��ձ�.txt"
#define		PLAYER_INSTANT_SPECIAL_FILE		"\\settings\\npcres\\˲����Ч.txt"

#define		SPR_INFO_NAME					"��Ϣ"
#define		KIND_NAME_SECT					"��������"
#define		KIND_NAME_SPECIAL				"����npc"
#define		KIND_NAME_NORMAL				"��ͨnpc"
#define		KIND_FILE_SECT1					"����˵���ļ���"
#define		KIND_FILE_SECT2					"������Ϊ������1"
#define		KIND_FILE_SECT3					"������Ϊ������2"
#define		KIND_FILE_SECT4					"������ͼ˳���"
#define		KIND_FILE_SECT5					"��Դ�ļ�·��"

//--------------------------- player ������� ------------------------------
// ע����������趨�ļ����밴 �� �� ľ ľ ˮ ˮ �� �� �� �� ��˳������
#define		FACTION_FILE					"\\settings\\faction\\�����趨.ini"

//--------------------------- player PK��� ------------------------------
#define		defPK_PUNISH_FILE				"\\settings\\npc\\player\\PKPunish.txt"

//--------------------------- player ������ ------------------------------
#define		defPLAYER_TONG_PARAM_FILE		"\\settings\\tong\\TongSet.ini"


//----------------------------- ������� ------------------------------
#define		CHAT_PATH						"\\chat"
#define		CHAT_TEAM_INFO_FILE_NAME		"Team.cht"
#define		CHAT_CHANNEL_INFO_FILE_NAME		"Channel.cht"

#define		CHAT_CHANNEL_NAME_ALL			"�������"
#define		CHAT_CHANNEL_NAME_SCREEN		"�������"
#define		CHAT_CHANNEL_NAME_SINGLE		"����"
#define		CHAT_CHANNEL_NAME_TEAM			"����"
#define		CHAT_CHANNEL_NAME_FACTION		"ͬ��"
#define		CHAT_CHANNEL_NAME_TONG			"����"
#define		CHAT_CHANNEL_NAME_SCREENSINGLE	"İ����"
#define		CHAT_CHANNEL_NAME_SYSTEM		"ϵͳ"

#define		CHAT_TAKE_CHANNEL_NAME_TEAM			"����Ƶ��"
#define		CHAT_TAKE_CHANNEL_NAME_FACTION		"����Ƶ��"
#define		CHAT_TAKE_CHANNEL_NAME_TONG			"���Ƶ��"
#define		CHAT_TAKE_CHANNEL_NAME_SCREENSINGLE	"˽��Ƶ��"

//---------------------------- player ��ֵ��� ------------------------------
#define		PLAYER_LEVEL_EXP_FILE			"\\settings\\npc\\player\\level_exp.txt"
#define		PLAYER_LEVEL_ADD_FILE			"\\settings\\npc\\player\\level_add.txt"
#define		PLAYER_LEVEL_LEAD_EXP_FILE		"\\settings\\npc\\player\\level_lead_exp.txt"
#define		BASE_ATTRIBUTE_FILE_NAME		"\\settings\\npc\\player\\NewPlayerBaseAttribute.ini"
#define		PLAYER_PK_RATE_FILE				"\\settings\\npc\\PKRate.ini"
#define		PLAYER_BASE_VALUE				"\\settings\\npc\\player\\BaseValue.ini"

// 00�н� 01Ů�� 02��ľ 03Ůľ 04��ˮ 05Ůˮ 06�л� 07Ů�� 08���� 09Ů��
#define		NEW_PLAYER_INI_FILE_NAME		"\\settings\\npc\\player\\NewPlayerIni%02d.ini"

//---------------------------- player ������� ------------------------------
#define		BUYSELL_FILE					"\\settings\\buysell.txt"
#define		GOODS_FILE						"\\settings\\goods.txt"

//----------------------------- ��Ʒ��� ------------------------------
#define		CHANGERES_MELEE_FILE			"\\settings\\item\\MeleeRes.txt"
#define		CHANGERES_RANGE_FILE			"\\settings\\item\\RangeRes.txt"
#define		CHANGERES_ARMOR_FILE			"\\settings\\item\\ArmorRes.txt"
#define		CHANGERES_HELM_FILE				"\\settings\\item\\HelmRes.txt"
#define		CHANGERES_HORSE_FILE			"\\settings\\item\\HorseRes.txt"
#define		ITEM_ABRADE_FILE				"\\settings\\item\\AbradeRate.ini"

//--------------- npc skill missles �趨�ļ�����������ģ�� -------------
#define		SKILL_SETTING_FILE				"\\settings\\Skills.txt"
#define		MISSLES_SETTING_FILE			"\\settings\\Missles.txt"
#define		NPC_SETTING_FILE				"\\settings\\NpcS.txt"
#define		NPC_GOLD_TEMPLATE_FILE			"\\settings\\npc\\NpcGoldTemplate.txt"

//---------------------------- object ��� ------------------------------
#define		OBJ_DATA_FILE_NAME		"\\settings\\obj\\ObjData.txt"
#define		MONEY_OBJ_FILE_NAME		"\\settings\\obj\\MoneyObj.txt"
#define		OBJ_NAME_COLOR_FILE		"\\settings\\obj\\ObjNameColor.ini"

//---------------------------- С��ͼ��� -------------------------------
#define		defLITTLE_MAP_SET_FILE	"\\Ui\\Default\\С��ͼ��ɫ.ini"

//----------------------------- ������� --------------------------------
#define		defINSTANT_SOUND_FILE	"\\settings\\SoundList.txt"
#define		defMUSIC_SET_FILE		"\\settings\\music\\MusicSet.txt"
#define		defMUSIC_FIGHT_SET_FILE	"\\settings\\music\\MusicFightSet.ini"

//------------------------------------------------------------------------
#define NPC_LEVELSCRIPT_FILENAME		"\\script\\npclevelscript\\npclevelscript.lua"
#define NPC_TEMPLATE_BINFILEPATH		"\\settings"
#define NPC_TEMPLATE_BINFILE			"NpcTemplate.Bin"
#define WEAPON_PHYSICSSKILLFILE			"\\settings\\�������������ձ�.txt"				
#define WEAPON_PARTICULARTYPE			"��ϸ���"
#define	WEAPON_DETAILTYPE				"�������"
#define	WEAPON_SKILLID					"��Ӧ�����ܱ��"
//-----------------------------------------------------------------------
#define	WORLD_WAYPOINT_TABFILE			"\\settings\\WayPoint.txt"
#define WORLD_STATION_TABFILE			"\\settings\\Station.txt"
#define WORLD_STATIONPRICE_TABFILE		"\\settings\\StationPrice.txt"
#define WORLD_WAYPOINTPRICE_TABFILE		"\\settings\\WayPointPrice.txt"
#define WORLD_DOCK_TABFILE				"\\settings\\Wharf.txt"
#define WORLD_DOCKPRICE_TABFILE			"\\settings\\WharfPrice.txt"
#define STRINGRESOURSE_TABFILE			"\\settings\\StringResource.txt"
#define PLAYER_RANK_SETTING_TABFILE		"\\settings\\RankSetting.txt"	
#define QUESTITEM_TABFILE				"\\settings\\item\\questkey.txt"

//---------------------------------����--------------------------
#define TASK_MISSION_SETTING_TABFILE	"\\settings\\task\\missions.txt"



//---------------------------- ��Ϣ��� ------------------------------
// �ر�ע�⣺�����ַ������Ȳ��ܳ���32�ֽڣ����� %d %s �Ƚ��վ��������Ժ�ĳ���
#define		MSG_GET_EXP						"�����%d�㾭��ֵ��"
#define		MSG_DEC_EXP						"����ʧ��%d�㾭��ֵ��"
#define		MSG_LEVEL_UP					"���ĵȼ�������%d��"
#define		MSG_LEADER_LEVEL_UP				"����ͳ�ʵȼ�������%d��"
#define		MSG_GET_ATTRIBUTE_POINT			"�����%d��Ǳ�ܣ�"
#define		MSG_GET_SKILL_POINT				"�����%d�����ܵ㣡"
#define		MSG_GET_ATTRIBUTE_SKILL_POINT	"�����%d��Ǳ�ܺ�%d�����ܵ㣡"

#define		MSG_TEAM_AUTO_REFUSE_INVITE		"�Զ��ܾ����˶�����������롣"
#define		MSG_TEAM_NOT_AUTO_REFUSE_INVITE	"��ʾ���˶�����������롣"
#define		MSG_TEAM_SEND_INVITE			"����%s����������룡"
#define		MSG_TEAM_GET_INVITE				"%s��������ӣ�"
#define		MSG_TEAM_REFUSE_INVITE			"%s�ܾ�������������룡"
#define		MSG_TEAM_CREATE					"��������һ֧���顣"
#define		MSG_TEAM_CREATE_FAIL			"���鴴��ʧ�ܡ�"
#define		MSG_TEAM_CANNOT_CREATE			"�����ڲ�����ӣ�"
#define		MSG_TEAM_TARGET_CANNOT_ADD_TEAM	"�Է����ڲ�����ӣ�"
#define		MSG_TEAM_OPEN					"���Ķ���������������¶�Ա��"
#define		MSG_TEAM_CLOSE					"���Ķ������ڲ���������¶�Ա��"
#define		MSG_TEAM_ADD_MEMBER				"%s��Ϊ���Ķ����ˡ�"
#define		MSG_TEAM_SELF_ADD				"������%s�Ķ��顣"
#define		MSG_TEAM_DISMISS_CAPTAIN		"����ɢ���Լ��Ķ��飡"
#define		MSG_TEAM_DISMISS_MEMBER			"%s��ɢ�˶��飡"
#define		MSG_TEAM_KICK_ONE				"%s�����������飡"
#define		MSG_TEAM_BE_KICKEN				"�������������飡"
#define		MSG_TEAM_APPLY_ADD				"%s���������飡"
#define		MSG_TEAM_APPLY_ADD_SELF_MSG		"���������%s�Ķ��飡"
#define		MSG_TEAM_LEAVE					"%s�뿪���顣"
#define		MSG_TEAM_LEAVE_SELF_MSG			"���뿪%s�Ķ��顣"
#define		MSG_TEAM_CHANGE_CAPTAIN_FAIL1	"�ӳ�����ʧ�ܣ�"
#define		MSG_TEAM_CHANGE_CAPTAIN_FAIL2	"%sͳ˧��������"
#define		MSG_TEAM_CHANGE_CAPTAIN_FAIL3	"���Ķ��鲻���ƽ������֣�"
#define		MSG_TEAM_CHANGE_CAPTAIN			"%s������Ϊ�ӳ���"
#define		MSG_TEAM_CHANGE_CAPTAIN_SELF	"����%s����Ϊ�ӳ���"

#define		MSG_CHAT_APPLY_ADD_FRIEND		"����%s������ѡ�"
#define		MSG_CHAT_FRIEND_HAD_IN			"%s�ѱ���Ϊ���ѡ�"
#define		MSG_CHAT_GET_FRIEND_APPLY		"%s�����Ϊ���ѣ�"
#define		MSG_CHAT_REFUSE_FRIEND			"%s�ܾ��������룡"
#define		MSG_CHAT_ADD_FRIEND_FAIL		"���%sΪ����ʧ�ܣ�"
#define		MSG_CHAT_ADD_FRIEND_SUCCESS		"����%s��Ϊ���ѣ�"
#define		MSG_CHAT_CREATE_TEAM_FAIL1		"���ѷ��鴴��ʧ�ܣ�"
#define		MSG_CHAT_CREATE_TEAM_FAIL2		"�Ѵ���ͬ���飡"
#define		MSG_CHAT_CREATE_TEAM_FAIL3		"�Ѵ�����������"
#define		MSG_CHAT_RENAME_TEAM_FAIL		"�����޸�ʧ�ܣ�"
#define		MSG_CHAT_DELETE_TEAM_FAIL1		"��ɾ��ʧ�ܣ�"
#define		MSG_CHAT_DELETE_TEAM_FAIL2		"����鲻����ɾ����"
#define		MSG_CHAT_FRIEND_ONLINE			"%s�����ˣ�"
#define		MSG_CHAT_FRIEND_OFFLINE			"%s�����ˣ�"
#define		MSG_CHAT_DELETE_FRIEND			"����%s����˺��ѹ�ϵ��"
#define		MSG_CHAT_DELETED_FRIEND			"%s��������˺��ѹ�ϵ��"
#define		MSG_CHAT_MSG_FROM_FRIEND		"%s������Ϣ��"
#define		MSG_CHAT_FRIEND_NOT_NEAR		"%s���ڸ�����"
#define		MSG_CHAT_FRIEND_NOT_ONLINE		"%s�����ߣ�"
#define		MSG_CHAT_TAR_REFUSE_SINGLE_TALK	"%sû�ж���˽��Ƶ����"

#define		MSG_SHOP_NO_ROOM				"�����ռ䲻�㣡"
#define		MSG_SHOP_NO_MONEY				"��Ǯ���㣡"

#define		MSG_NPC_NO_MANA					"�������㣡"
#define		MSG_NPC_NO_STAMINA				"�������㣡"
#define		MSG_NPC_NO_LIFE					"�������㣡"
#define		MSG_NPC_DEATH					"%s������������"

#define		MSG_OBJ_CANNOT_PICKUP			"������ʰȡ���˵���Ʒ��"
#define		MSG_MONEY_CANNOT_PICKUP			"������ʰȡ���˵�Ǯ��"
#define		MSG_OBJ_TOO_FAR					"��Ʒ̫Զ���޷�ʰȡ��"
#define		MSG_DEC_MONEY					"����ʧ��%d�����ӣ�"
#define		MSG_EARN_MONEY					"�������%d�����ӣ�"
#define		MSG_DEATH_LOSE_ITEM				"����ʧ����Ʒ%s��"
#define		MSG_ADD_ITEM					"�������%s��"
#define		MSG_ITEM_SAME_DETAIL_IN_IMMEDIATE	"������Ѿ���ͬ���͵���Ʒ�ˣ�"
#define		MSG_CAN_NOT_VIEW_ITEM			"���ڲ��ܲ쿴����װ����"
#define		MSG_ITEM_DAMAGED				"��װ����%s����ĥ���Ѿ������𻵣�"
#define		MSG_ITEM_NEARLY_DAMAGED			"��װ����%s�Ѿ��ӽ��𻵣��뼰ʱ����"

#define		MSG_TRADE_STATE_OPEN			"�����ڿ��Խ��ס�"
#define		MSG_TRADE_STATE_CLOSE			"�����ڲ��ܽ��ס�"
#define		MSG_TRADE_SELF_LOCK				"��������Ʒ�Ѿ�������"
#define		MSG_TRADE_SELF_UNLOCK			"��������Ʒ���������"
#define		MSG_TRADE_DEST_LOCK				"%s������Ʒ�Ѿ�������"
#define		MSG_TRADE_DEST_UNLOCK			"%s������Ʒ���������"
#define		MSG_TRADE_SUCCESS				"����%s���׳ɹ���"
#define		MSG_TRADE_FAIL					"����%s����ʧ�ܡ�"
#define		MSG_TRADE_SELF_ROOM_FULL		"���ı����ռ䲻�㣡"
#define		MSG_TRADE_DEST_ROOM_FULL		"%s�ı����ռ䲻�㣡"
#define		MSG_TRADE_SEND_APPLY			"����%s���뽻�ף�"
#define		MSG_TRADE_GET_APPLY				"%s�����������ף�"
#define		MSG_TRADE_REFUSE_APPLY			"%s�ܾ��������ף�"
#define		MSG_TRADE_TASK_ITEM				"������Ʒ���ܽ��ף�"

#define		MSG_PK_NORMAL_FLAG_OPEN			"PK���ش򿪣�"
#define		MSG_PK_NORMAL_FLAG_CLOSE		"PK���عرգ�"
#define		MSG_PK_VALUE					"��Ŀǰ��PKֵ��%d��"
#define		MSG_PK_ERROR_1					"�����ڴ��ڷ�ս��״̬��"
#define		MSG_PK_ERROR_2					"���ֲ��ܳ�ɱ��"
#define		MSG_PK_ERROR_3					"�����������д裡"
#define		MSG_PK_ERROR_4					"���������˳�ɱ��"
#define		MSG_PK_ERROR_5					"�����ܳ�ɱ���֣�"
#define		MSG_PK_ERROR_6					"�Է������ڷ�ս��״̬��"
#define		MSG_PK_ERROR_7					"�Է����������д裡"
#define		MSG_PK_ENMITY_SUCCESS_1			"����%s������ɱ��ϵ��"
#define		MSG_PK_ENMITY_SUCCESS_2			"10���Ӻ��ɱ��ʼִ�У�"
#define		MSG_PK_ENMITY_CLOSE				"���ĳ�ɱ��ϵ������"
#define		MSG_PK_ENMITY_OPEN				"��ɱ���ڿ�ʼִ�У�"

#define		MSG_TONG_CREATE_ERROR01			"�����������"
#define		MSG_TONG_CREATE_ERROR02			"�����Ӫ����"
#define		MSG_TONG_CREATE_ERROR03			"����Ա�������⽨����ᣡ"
#define		MSG_TONG_CREATE_ERROR04			"ɱ�ֲ��ܽ�����ᣡ"
#define		MSG_TONG_CREATE_ERROR05			"�������ĵȼ�������"
#define		MSG_TONG_CREATE_ERROR06			"��������ͳ˧�ȼ�������"
#define		MSG_TONG_CREATE_ERROR07			"���������Ҫ %d ��Ǯ��"
#define		MSG_TONG_CREATE_ERROR08			"����У����ܽ�����ᣡ"
#define		MSG_TONG_CREATE_ERROR09			"���������"
#define		MSG_TONG_CREATE_ERROR10			"���������Ҫ�Ľ�Ǯ������"
#define		MSG_TONG_CREATE_ERROR11			"��������Ϸ������İ��������"
#define		MSG_TONG_APPLY_CREATE			"�����봴����ᣡ"
#define		MSG_TONG_CREATE_SUCCESS			"��ᴴ���ɹ���"
#define		MSG_TONG_APPLY_ADD				"����������ᣡ"
#define		MSG_TONG_APPLY_ADD_ERROR1		"����Ա������������ᣡ"
#define		MSG_TONG_APPLY_ADD_ERROR2		"ɱ�ֲ��ܼ����ᣡ"
#define		MSG_TONG_APPLY_ADD_ERROR3		"����У����ܼ����ᣡ"
#define		MSG_TONG_REFUSE_ADD				"%s�ܾ��������ᣡ"
#define		MSG_TONG_ADD_SUCCESS			"�������ᣡ"
#define		MSG_TONG_CANNOT_LEAVE1			"���������뿪��ᣡ"
#define		MSG_TONG_CANNOT_LEAVE2			"���ϲ����뿪��ᣡ"
#define		MSG_TONG_BE_KICKED				"�����߳���ᣡ"
#define		MSG_TONG_LEAVE_SUCCESS			"�������뿪���ɹ���"
#define		MSG_TONG_LEAVE_FAIL				"�������뿪���ʧ�ܣ�"
#define		MSG_TONG_CHANGE_AS_MASTER		"��������Ϊ������"
#define		MSG_TONG_CHANGE_AS_MEMBER		"�����ڳ�Ϊ��ͨ���ڣ�"


#define		MESSAGE_SYSTEM_ANNOUCE_HEAD		"����"

enum enumMSG_ID
{
	enumMSG_ID_NONE = 0,
	enumMSG_ID_TEAM_KICK_One,
	enumMSG_ID_TEAM_DISMISS,
	enumMSG_ID_TEAM_LEAVE,
	enumMSG_ID_TEAM_REFUSE_INVITE,
	enumMSG_ID_TEAM_SELF_ADD,
	enumMSG_ID_TEAM_CHANGE_CAPTAIN_FAIL,
	enumMSG_ID_TEAM_CHANGE_CAPTAIN_FAIL2,
	enumMSG_ID_OBJ_CANNOT_PICKUP,
	enumMSG_ID_OBJ_TOO_FAR,
	enumMSG_ID_DEC_MONEY,
	enumMSG_ID_TRADE_SELF_ROOM_FULL,
	enumMSG_ID_TRADE_DEST_ROOM_FULL,
	enumMSG_ID_TRADE_REFUSE_APPLY,
	enumMSG_ID_TRADE_TASK_ITEM,
	enumMSG_ID_GET_ITEM,
	enumMSG_ID_ITEM_DAMAGED,
	enumMSG_ID_MONEY_CANNOT_PICKUP,
	enumMSG_ID_CANNOT_ADD_TEAM,
	enumMSG_ID_TARGET_CANNOT_ADD_TEAM,
	enumMSG_ID_PK_ERROR_1,
	enumMSG_ID_PK_ERROR_2,
	enumMSG_ID_PK_ERROR_3,
	enumMSG_ID_PK_ERROR_4,
	enumMSG_ID_PK_ERROR_5,
	enumMSG_ID_PK_ERROR_6,
	enumMSG_ID_PK_ERROR_7,
	enumMSG_ID_DEATH_LOSE_ITEM,
	enumMSG_ID_TONG_REFUSE_ADD,
	enumMSG_ID_TONG_BE_KICK,
	enumMSG_ID_TONG_LEAVE_SUCCESS,
	enumMSG_ID_TONG_LEAVE_FAIL,
	enumMSG_ID_TONG_CHANGE_AS_MASTER,
	enumMSG_ID_TONG_CHANGE_AS_MEMBER,
	enumMSG_ID_NUM,
};

//---------------------------- ���ָ����� ------------------------------
#define		MOUSE_CURSOR_NORMAL				0
#define		MOUSE_CURSOR_FIGHT				1
#define		MOUSE_CURSOR_DIALOG				2
#define		MOUSE_CURSOR_PICK				3
#define		MOUSE_CURSOR_USE				8
#endif
