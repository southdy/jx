/**************************************************/
/*                                                */
/*  �ļ���:    S3PEquipRequireInfoDAO             */
/*  ����    :  EquipRequireInfo���ά����            */
/*                                                */
/*	����	 : Liu Wansong                        */
/*	�������� : 8/26/2002                          */
/*  �޸����� : 8/26/2002                          */
/**************************************************/

#include "S3PEquipRequireInfoDAO.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

S3PEquipRequireInfoDAO::S3PEquipRequireInfoDAO(S3PDBConnection *pConn)
{
	m_pConn=pConn;
}

S3PEquipRequireInfoDAO::~S3PEquipRequireInfoDAO()
{

}

S3PDBConnection * S3PEquipRequireInfoDAO::GetConnection()
{
	return m_pConn;
}

std::string S3PEquipRequireInfoDAO::GetTableName()
{
	return "EquipRequireInfo";
}