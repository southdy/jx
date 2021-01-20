/**************************************************/
/*                                                */
/*  �ļ���:    S3PLifeSkillDAO                */
/*  ����    :  LifeSkill���ά����            */
/*                                                */
/*	����	 : Liu Wansong                        */
/*	�������� : 8/26/2002                          */
/*  �޸����� : 8/26/2002                          */
/**************************************************/

#include "S3PLifeSkillDAO.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

S3PLifeSkillDAO::S3PLifeSkillDAO(S3PDBConnection *pConn)
{
	m_pConn=pConn;
}

S3PLifeSkillDAO::~S3PLifeSkillDAO()
{

}

S3PDBConnection * S3PLifeSkillDAO::GetConnection()
{
	return m_pConn;
}

std::string S3PLifeSkillDAO::GetTableName()
{
	return "LifeSkill";
}