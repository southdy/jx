#ifndef __epsJO_h__
#define __epsJO_h__

#include "epassapi.h"
#include "des.h"

/************************************************
* Function name		: epj_Init
* Description	    : 
* Return type		: int(true/flase) 
* Argument			: const char*pszPassword:�����û�������(��ʼ�����ǡ�rockey��)
* Remark			: ���ȵ���
************************************************/
int __stdcall epj_Init(const char*pszPassword);

/************************************************
* Function name		: epj_UnInit
* Description	    : 
* Return type		: void 
* Remark			: ������
************************************************/
void __stdcall epj_UnInit();

/************************************************
* Function name		: epj_ClearUSBKey
* Description	    : ���USBKEY�е�����
* Return type		: int(true/flase) 
* Remark			: ��дkeyʱ��Ҫ�����ǰ������
************************************************/
int __stdcall epj_ClearUSBKey();

/************************************************
* Function name		: epj_WriteKeyCode
* Description	    : ��key��д��һ������
* Return type		: int(true/flase) 
* Argument			: const char* lpExeData��������MD5 hash���õ���16�ֽ�ժҪ��Ϊ
						DES����Կ����lpKeyCode����
* Argument			: int nExeDataLen��lpExeData����
* Argument			: const char* lpKeyCode�������ݱ����ܺ�д��key��
* Argument			: int nKeyCodeLen��lpKeyCode�ĳ���
* Argument			: int nIndex��Ψһ��ʶ�����ܵ�����(lpKeyCode),���� >=0 && <256
* Remark			: USBKEYֻ��32K�Ĵ洢�ռ�
************************************************/
int __stdcall epj_WriteKeyCode(const char* lpExeData,int nExeDataLen,
					  const char* lpKeyCode,int nKeyCodeLen,int nIndex);

/************************************************
* Function name		: epj_ReadKeyCode
* Description	    : ��key�ж���һ������
* Return type		: int(true/flase) 
* Argument			: const char* lpExeData��������MD5 hash���õ���16�ֽ�ժҪ��Ϊ
						DES����Կ���ܴ�key�ж���������
* Argument			: int nExeDataLen��lpExeData����
* Argument			: |*out*|char** lppKeyCode����Ž��ܺ�����ݣ���::free()�ͷſռ�
* Argument			: |*out*|int* pnKeyCodeLen�����ܺ�����ݵĳ���
* Argument			: int nIndex��Ψһ��ʶ�����ܵ�����,���� >=0 && <256
* Remark			: ����lppKeyCode����Ҫ::free()��
************************************************/
int __stdcall epj_ReadKeyCode(const char* lpExeData,int nExeDataLen,
					 /*out*/char** lppKeyCode,/*out*/int* pnKeyCodeLen,int nIndex);

/************************************************
* Function name		: epj_IsUSBKeyExist
* Description	    : ���USNKEY�Ƿ����
* Return type		: int ��true�����ڣ�false�������ڻ�������
* Remark			: ��epj_Init()ʱ���ڴ�����һ�������(int)����ÿ��epj_ReadKeyCode�󽫴�
						�������򱻽������ݵ�ǰ4���ֽڣ���д�뵽key�У�����epj_IsUSBKeyExist()
						ʱ���Ƚ�key�е�ֵ�Ƿ����ڴ��е�ֵ��ȣ������������������д�뵽key��
************************************************/
int __stdcall epj_IsUSBKeyExist();

#endif//__epsJO_h__