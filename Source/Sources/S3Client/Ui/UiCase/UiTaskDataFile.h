/* 
 * File:     UiTaskDataFile.h
 * Desc:     ��������ļ�����
 * Author:   flying
 * Creation: 2003/7/19
 */
//-----------------------------------------------------------------------------
/*************************************************************************
  �����Ļ��䣬����㡣���õ��ǹ⣬�������ĬĬ��ʧ������̫�������棬
  Ҳ���н�����һ�졣���֮�£������Ķ��ݣ�����һ�����ǡ����ݵ�һ���У�
  ���ǻ������ܶ�Ҵҹ��ͣ����ǻᰮ�����ޣ�Ϊ�Լ�������Ŭ���ܶ����ܶ�
  ���ѻ��뿪���ǣ�Ҳ�������ʶ�ܶ����ѡ�����֮��������ĳ��ߡ�ÿ��
  ���ִ��ϵ�ҹ�������ͺ�����ӳ��ҹ�գ�ҹ������ô����õĻ��߲�����
  ���ǹ⡣Ҳ���´λ��ܼ�����Ҳ�����е�һЩ����Զ��ʧ������Ӧ����΢Ц
  ����������Ϊ�����ζ���ط꣬�����ط��ϣ��������Ӧ�������е���
  ����ϧ�మ���ˣ���Ϊҹ��������һ���ǹ������Լ���һ�����Ҹ�������Ӧ
  �üǵ������ѣ���Ϊ������������;�е�һ���֡�����Ӧ�ò����ߵĶ�Զ��
  ǣ���Լ��ĸ�ĸ����Ϊ������ö�˵������Ӧ�ó�����������ĳ�������Ϊ
  �����������շֿ��������Ľ����У�����һ����Զ�����Ļ��䣬��ȻҲ��
  �����Ѿ��ֿ���������ߣ���Ȼʧȥ�����²��ٻ�ͷ����Ȼ��Ҷ������ڷ�
  æ�ĳ��У�����Ӧ�����ĵ׵��������һ��ĬĬ��ף����ǣ�ң���Ϊ����
  ������Ļ��䣬���εζ�������ʧ������ĳ�����׵�ҹ������ǰ��΢
  �������ϸ��Ҳ��������ĸ��֡������ľ��ѿ������������ɫ������Ļ�
  Ҷ�ͻ����ڷ���Ʈɢ������������ѩ�ͺ����ҹ����Щ�ڲ��ϵķ�������
  ҹ���еĹ���һ������л���ɻ����죬���������˭�������ǲ���Ҫ������
  �ĸп�����ЩҪ��΢Цȥ��ԡ���ʹ���ᣬҲ���������������ֻ������
  ��ֽ�����С�
  ������Ե����棬������һ����Ϸ����������һ�������������������λ�
  ��������У�����Ҳ�������������е����顣Ҳ���ڽ�����ĳһ�죬���ǻ�
  �뿴���Լ��߹���·�����������ˣ�������ս���͹�ͬ��������Ѫ��̤����
  ɽˮ��ľ��ϣ���ҿ�����������ס���䣬ֻ��ϣ����ʱ�򣬲�Ҫ��ׯ��������
  ��֪�����Լ��ε����������Ǻ����ε��Լ���
  ���������У��͸������е����ѣ����Ρ�
                                     -- flying
                                   2003/7/19  3:28  ��ɽ��
 *************************************************************************/
/*
 * File format specification
 * 1 file header: 
 *   this section specifies the number of
 *   system record count and the bytes of 
 *   personal record.
 * 2 personal record:
 *   this section starts with one byte data
 *   as a flag, and keeps nPersonalRecordBytes
 *   bytes data.
 * 3 system record:
 *   this section also starts with one byte
 *   data to flag the beginning of system record.
 *   and then followed by nSystemRecordCount
 *   elements of TASK_SYSTEM_RECORD type.
 * 4 the whole file end with a byte flag as EOF.
 *                        flying 
 */
#if !defined _TASKDATAFILE_H
#define _TASKDATAFILE_H

#include <time.h>
#define		TASK_FILE_FLAG				0x46445400	//"\0TDF"

// data type definition.
struct TASK_FILE_HEADER 
{
	char	cFlag[sizeof(int)];
	int		nPersonalRecordBytes;
	int		nSystemRecordCount;
	int		nReserved;
};

struct TASK_SYSTEM_RECORD
{
	time_t			tTime;
	unsigned int	uReserved;
	int				nContentLen;
	union
	{
		char		cBuffer[4];
		const char*	pBuffer;	
	};
};


// Classsssssssssssssssssssss
class KTaskDataFile
{
public:
	static void		LoadData();
	static bool		SaveData();

	static void		ClearAll();

	static int		GetPersonalRecord(char* pszBuffer, int nBufferSize);
	static bool		SetPersonalRecord(const char* pstrRecord, int nLen);

	static void		ClearSystemRecords();
	static int		GetSystemRecordCount();
	static bool		InsertSystemRecord(TASK_SYSTEM_RECORD* pRecord);
	static bool		RemoveSystemRecord(int nIndex);
	static const TASK_SYSTEM_RECORD*	GetSystemRecord(int nIndex);

private:
	struct KPersonalRecord
	{
		int			nLen;
		char		cBuffer[4];
	};
	struct KTaskSystemRecordNode
	{
		KTaskSystemRecordNode*	pNext;
		TASK_SYSTEM_RECORD		Record;
	};

private:
	static void		GetFileName(char* pszFileName, int nLen);
	static void		AppendSystemRecord(KTaskSystemRecordNode* pNode);
	static void		InsertSystemRecord(KTaskSystemRecordNode* pNode);

private:
	static KPersonalRecord*			ms_pPersonalRecord;
	static KTaskSystemRecordNode*	ms_pSystemRecordList;
	static	int						ms_nSystemRecordCount;
};
#endif