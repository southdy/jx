// S3P_MSSQLServer_Result.cpp: implementation of the S3P_MSSQLServer_Result class.
//
//////////////////////////////////////////////////////////////////////

#include "S3P_MSSQLServer_Result.h"
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
S3P_MSSQLServer_Result::~S3P_MSSQLServer_Result()
{
	m_pResult = NULL;
	assert(m_nAddRef == 0);
}

int S3P_MSSQLServer_Result::num_rows() const
{
	int iRet = 0;
	try
	{
		if (m_pResult)
			iRet = m_pResult->GetRecordCount();
	}
	catch( _com_error e )
	{
		//printf("[S3P_MSSQLServer_Result::num_rows]COM error: %s\r\n", e.ErrorMessage());
	}
	return iRet;
}

int S3P_MSSQLServer_Result::num_fields() const
{
	int iRet = 0;
	try
	{
		if (m_pResult)
		{
			FieldsPtr pFs = m_pResult->GetFields();
			iRet = pFs->GetCount();
		}
	}
	catch( _com_error e )
	{
		//printf("[S3P_MSSQLServer_Result::num_fields]COM error: %s\r\n", e.ErrorMessage());
	}
	return iRet;
}

void S3P_MSSQLServer_Result::data_seek(unsigned int offset, int nType) const
{
	try
	{
		if (NULL != m_pResult)
		{
			unsigned int iRows = num_rows();
			if (nType == begin)
			{
				m_pResult->MoveFirst();
				if (offset > 0 && offset < iRows)
				{
					m_pResult->Move(offset);
				}
			}
			else if (nType == end)
			{
				m_pResult->MoveLast();
			}
			else if (nType == next)
			{
				for (;offset > 0; offset--)
					m_pResult->MoveNext();
			}
			else if (nType == previous)
			{
				for (;offset > 0; offset--)
					m_pResult->MovePrevious();
			}
			else
				assert(0);
		}
	}
	catch ( _com_error e )
	{
		//printf("[S3P_MSSQLServer_Result::data_seek(%d, %d)]COM error: %s\r\n", offset, nType, e.ErrorMessage());
	}
}

bool S3P_MSSQLServer_Result::get_field_data(unsigned int nfieldindex, void* pData, unsigned long nsize)
{
	BOOL bRet = FALSE;
	try
	{
		if (NULL != m_pResult && nsize == sizeof(VARIANT))
		{
			if (num_rows() > 0 && nfieldindex >= 0 && nfieldindex < num_fields())
			{
				_variant_t varIndex;
				varIndex = (long)nfieldindex;
				FieldPtr pField = m_pResult->Fields->GetItem(varIndex);
				if (SUCCEEDED(pField->get_Value((VARIANT*)pData)))
					bRet = TRUE;
			}
		}
	}
	catch ( _com_error e )
	{
		//printf("[S3P_MSSQLServer_Result::get_field_data(%d)]COM error: %s\r\n", nfieldindex, e.ErrorMessage());
	}

	return bRet;
}

int S3P_MSSQLServer_Result::unuse()
{
	if (m_pResult)
	{
		try
		{
			if (adStateOpen ==
				(m_pResult->GetState() & adStateOpen))
			{
				m_pResult->Close();
			}
		}
		catch (_com_error e)
		{
			//printf("[S3PDB_MSSQLServer_Connection::Do(1)]COM error: %s\r\n", e.ErrorMessage());
		}
		m_nAddRef--;
	}

	return m_nAddRef;
}

int S3P_MSSQLServer_Result::GetResult( _Recordset** ppResult)
{
	if (ppResult == NULL)
		return m_nAddRef;
	if (m_pResult == NULL)
	{
		HRESULT hr = m_pResult.CreateInstance("ADODB.Recordset");
		if (FAILED(hr))
		{
			m_pResult = NULL;
		}
	}

	assert(m_nAddRef == 0);
	if (m_nAddRef == 0)
	{
		*ppResult = m_pResult;

		if (*ppResult)
			m_nAddRef++;
	}

	return m_nAddRef;
}
