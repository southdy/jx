/**************************************************/
/*                                                */
/*  �ļ���:    S3PEquipmentsDAO                */
/*  ����    :  Equipments���ά����            */
/*                                                */
/*	����	 : Liu Wansong                        */
/*	�������� : 8/26/2002                          */
/*  �޸����� : 8/26/2002                          */
/**************************************************/

#include "S3PEquipmentsDAO.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

S3PEquipmentsDAO::S3PEquipmentsDAO(S3PDBConnection *pConn)
{
	m_pConn=pConn;
}

S3PEquipmentsDAO::~S3PEquipmentsDAO()
{

}

S3PDBConnection * S3PEquipmentsDAO::GetConnection()
{
	return m_pConn;
}

std::string S3PEquipmentsDAO::GetTableName()
{
	return "Equipments";
}