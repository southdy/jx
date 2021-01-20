/**************************************************/
/*                                                */
/*  �ļ���:    S3PEquipBaseInfoDAO                */
/*  ����    :  EquipBaseInfo���ά����            */
/*                                                */
/*	����	 : Liu Wansong                        */
/*	�������� : 8/26/2002                          */
/*  �޸����� : 8/26/2002                          */
/**************************************************/

#include "S3PEquipBaseInfoDAO.h"
#include "S3PDBConnection.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

S3PEquipBaseInfoDAO::S3PEquipBaseInfoDAO(S3PDBConnection *pConn)
{
	m_pConn=pConn;
}

S3PEquipBaseInfoDAO::~S3PEquipBaseInfoDAO()
{

}

S3PDBConnection * S3PEquipBaseInfoDAO::GetConnection()
{
	return m_pConn;
}

std::string S3PEquipBaseInfoDAO::GetTableName()
{
	return "EquipBaseInfo";
}