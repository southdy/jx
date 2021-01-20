/**************************************************/
/*                                                */
/*  �ļ���:    S3PTableDAO.cpp                    */
/*  ����    :  һ����ά�����࣬ ������ÿ��        */
/*             �ֶε����ͣ�Ȼ����ݲ�ͬ�Ĺ���     */
/*             ��֯����Ӧ��sql���                */
/*                                                */
/*	����	 : Liu Wansong                        */
/*	�������� : 8/22/2002                          */
/*  �޸����� : 8/26/2002                          */
/**************************************************/

//////////////////////////////////////////////////////////////////////

#include "S3PTableDAO.h"
#include "S3PDBConnection.h"
#include "regexpr2.h"

using namespace std;
using namespace regex;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

S3PTableDAO::S3PTableDAO()
{

}

S3PTableDAO::~S3PTableDAO()
{

}

int S3PTableDAO::Add(S3PRow * row)
{
	std::string strQuery;
	std::string tableName = GetTableName();

	std::string strInsert;
	if ( row->GetExpLikeInsert(strInsert) > 0 )
	{
		strQuery = "insert into " + tableName + strInsert;
		S3PDBConnection * pConn = GetConnection();
		if(pConn)
		{
			if ( pConn->Do(strQuery.c_str()) )
			{
				// Success
				m_resAdd = *((ResNSel *)pConn->GetRes());
			}
			else
			{
				return -3;
			}
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -2;
	}

	return 1;
}

int S3PTableDAO::Update(S3PRow * row, S3PRow * where)
{
	std::string strQuery;
	std::string tableName = GetTableName();

	std::string strWhere;
	std::string strUpdate;
	if ( row->GetExpLikeUpdate(strUpdate) > 0 )
	{

		strQuery = "update " + tableName + " set " + strUpdate;
		if (where && where->GetExpLikeWhereAnd(strWhere) >0 )
		{
			strQuery += " where " + strWhere;
			S3PDBConnection * pConn = GetConnection();
			if(pConn)
			{
				if(pConn->Do(strQuery.c_str()))
				{
					//Success
				}
				else
				{
					return -3;
				}
			}
			else
			{
				return -1;
			}
		}
	}
	else
	{
		return -2;
	}

	return 1;
}

int S3PTableDAO::Delete(S3PRow * where)
{
	std::string strQuery;
	std::string tableName = GetTableName();

	strQuery = "delete from " + tableName;

	std::string strWhere;
	if (where && where->GetExpLikeWhereAnd(strWhere) > 0 )
	{
		strQuery += " where " + strWhere;
	}

	S3PDBConnection * pConn = GetConnection();
	if(pConn)
	{
		if(pConn->Do(strQuery.c_str()))
		{
			//Success
		}
		else
		{
			return -3;
		}
	}
	else
	{
		return -1;
	}
	return 1;
}

bool S3PTableDAO::HasItem( S3PRow* where )
{
	bool bRet = false;

	std::string strQuery;
	std::string tableName = GetTableName();

	strQuery = "select * from " + tableName;

	std::string strWhere;
	if ( where && where->GetExpLikeWhereAnd(strWhere) > 0 )
	{
		strQuery += " where " + strWhere;
	}

	S3PDBConnection * pConn = GetConnection();
	if( pConn )
	{
		S3PResult result;
		if ( Query( strQuery, result ) > 0 )
		{
			if ( result.size() > 0 )
			{
				bRet = true;
			}
		}
	}

	return bRet;
}

int S3PTableDAO::Query(std::string q, S3PResult & result)
{
	S3PDBConnection * pConn = GetConnection();
	if(pConn)
	{
		if(pConn->QueryBySql(q.c_str()))
		{
			//Success
			Result * pResult = (Result*)(pConn->GetRes());
			if (pResult)
			{
				result.SetResult(pResult);
			}
			else
			{
				return -4;
			}
		}
		else
		{
			return -3;
		}
	}
	else
	{
		return -1;
	}
	return 1;
}

int S3PTableDAO::AddGroup(const std::list<ColumnAndValue*> & group)
{
	BOOL bFirst = TRUE;
	std::string strQuery;

	std::list<ColumnAndValue*>::const_iterator i;
	int errCode = 0;
	try
	{
		for(i=group.begin();i!=group.end();i++)
		{
			S3PRow row(GetTableName(), (*i), GetConnection());
			if(bFirst)
			{
				std::string keys;
				if (row.GetExpLikeInsertKey(keys)>0)
				{
					strQuery = "insert into " + GetTableName() + keys + " values";
				}
				else
				{
					errCode = -1;
					throw(errCode);
				}
				bFirst = FALSE;
			}
			std::string values;
			if (row.GetExpLikeInsertValue(values)>0)
			{
				strQuery += values + ",";
			}
			else
			{
				errCode = -2;
				throw(errCode);
			}
		}

		subst_results results;
		rpattern pat(",$", "");
		
		pat.substitute(strQuery, results );
		
		S3PDBConnection * pConn = GetConnection();
		if(pConn)
		{
			if ( pConn->Do(strQuery.c_str()) )
			{
				// Success
			}
			else
			{
				errCode = -3;
				throw(errCode);
			}
		}
		else
		{
			errCode = -4;
			throw(errCode);
		}
	}
	catch(...)
	{
		return errCode;
	}

	return 1;
}

int S3PTableDAO::GetInsertID()
{
	return m_resAdd.insert_id;
}

int S3PTableDAO::Query(S3PDBConnection *pConn, string q, S3PResult &result)
{
	if(pConn)
	{
		if(pConn->QueryBySql(q.c_str()))
		{
			//Success
			Result * pResult = (Result*)(pConn->GetRes());
			if (pResult)
			{
				result.SetResult(pResult);
			}
			else
			{
				return -4;
			}
		}
		else
		{
			return -3;
		}
	}
	else
	{
		return -1;
	}
	return 1;
}
