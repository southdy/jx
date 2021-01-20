/**************************************************/
/*                                                */
/*  �ļ���:    S3PRoleInfoDAO                */
/*  ����    :  Role_Info���ά����            */
/*                                                */
/*	����	 : Liu Wansong                        */
/*	�������� : 8/22/2002                          */
/*  �޸����� : 8/26/2002                          */
/**************************************************/

#include "S3PRoleInfoDAO.h"
#include "S3PDBConnection.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

S3PRoleInfoDAO::S3PRoleInfoDAO(S3PDBConnection * pConn)
{
	m_pConn = pConn;
}

S3PRoleInfoDAO::~S3PRoleInfoDAO()
{

}

std::string S3PRoleInfoDAO::GetTableName()
{
	return "Role_Info";
}

S3PDBConnection * S3PRoleInfoDAO::GetConnection()
{
	return m_pConn;
}
