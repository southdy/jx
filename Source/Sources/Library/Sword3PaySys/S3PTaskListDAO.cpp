/**************************************************/
/*                                                */
/*  �ļ���:    S3PTaskListDAO                */
/*  ����    :  Task_List���ά����            */
/*                                                */
/*	����	 : Liu Wansong                        */
/*	�������� : 8/26/2002                          */
/*  �޸����� : 8/26/2002                          */
/**************************************************/

#include "S3PTaskListDAO.h"
#include "S3PDBConnection.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

S3PTaskListDAO::S3PTaskListDAO(S3PDBConnection * pConn)
{
	m_pConn =pConn;
}

S3PTaskListDAO::~S3PTaskListDAO()
{
}

S3PDBConnection * S3PTaskListDAO::GetConnection()
{
	return m_pConn;
}

std::string S3PTaskListDAO::GetTableName()
{
	return "Task_List";
}