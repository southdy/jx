/**************************************************/
/*                                                */
/*  �ļ���:    S3PFriendListDAO                */
/*  ����    :  Friend_List���ά����            */
/*                                                */
/*	����	 : Liu Wansong                        */
/*	�������� : 8/26/2002                          */
/*  �޸����� : 8/26/2002                          */
/**************************************************/

#include "S3PFriendListDAO.h"
#include "S3PDBConnection.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

S3PFriendListDAO::S3PFriendListDAO(S3PDBConnection * pConn)
{
	m_pConn=pConn;
}

S3PFriendListDAO::~S3PFriendListDAO()
{

}

S3PDBConnection * S3PFriendListDAO::GetConnection()
{
	return m_pConn;
}

std::string S3PFriendListDAO::GetTableName()
{
	return "Friend_List";
}