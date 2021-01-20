/**************************************************/
/*                                                */
/*  �ļ���:    S3PEquipEfficInfoDAO                */
/*  ����    :  EquipEfficInfo���ά����            */
/*                                                */
/*	����	 : Liu Wansong                        */
/*	�������� : 8/26/2002                          */
/*  �޸����� : 8/26/2002                          */
/**************************************************/

#include "S3PEquipEfficInfoDAO.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

S3PEquipEfficInfoDAO::S3PEquipEfficInfoDAO(S3PDBConnection *pConn)
{
	m_pConn=pConn;
}

S3PEquipEfficInfoDAO::~S3PEquipEfficInfoDAO()
{

}

S3PDBConnection * S3PEquipEfficInfoDAO::GetConnection()
{
	return m_pConn;
}

std::string S3PEquipEfficInfoDAO::GetTableName()
{
	return "EquipEfficInfo";
}