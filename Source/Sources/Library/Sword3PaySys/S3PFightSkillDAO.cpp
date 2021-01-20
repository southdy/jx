/**************************************************/
/*                                                */
/*  �ļ���:    S3PFightSkillDAO                   */
/*  ����    :  FightSkill���ά����            */
/*                                                */
/*	����	 : Liu Wansong                        */
/*	�������� : 8/26/2002                          */
/*  �޸����� : 8/26/2002                          */
/**************************************************/

#include "S3PFightSkillDAO.h"
#include "S3PDBConnection.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

S3PFightSkillDAO::S3PFightSkillDAO(S3PDBConnection * pConn)
{
	m_pConn = pConn;
}

S3PFightSkillDAO::~S3PFightSkillDAO()
{

}

S3PDBConnection * S3PFightSkillDAO::GetConnection()
{
	return m_pConn;
}

std::string S3PFightSkillDAO::GetTableName()
{
	return "FightSkill";
}