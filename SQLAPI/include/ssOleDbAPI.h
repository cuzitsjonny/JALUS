// ssOleDbAPI.h
//
//////////////////////////////////////////////////////////////////////

#if !defined(__SSOLEDBAPI_H__)
#define __SSOLEDBAPI_H__

#include "SQLAPI.h"

// API header(s)
#include <oledb.h>

// API declarations
class ssOleDbAPI : public saAPI
{
public:
	ssOleDbAPI();

public:
	virtual void InitializeClient(const SAConnection *pConnection);
	virtual void UnInitializeClient(bool unloadAPI);

	virtual long GetClientVersion() const;

	virtual ISAConnection *NewConnection(SAConnection *pConnection);

protected:
	bool m_bProcessSQLServerErrorInfo;

	void ResetAPI();

public:
	bool& ProcessSQLServerErrorInfo();

	static void CheckAndFreePropertySets(ULONG cPropertySets, DBPROPSET *rgPropertySets);
	static void CheckHRESULT(HRESULT hr);
	void Check(HRESULT hrOLEDB, IUnknown * pIUnknown, REFIID riid) const;
	void Check(const SAString &sCommandText, HRESULT hrOLEDB, IUnknown * pIUnknown, REFIID riid) const;
};

class SQLAPI_API ssOleDbConnectionHandles : public saConnectionHandles
{
public:
	ssOleDbConnectionHandles();

	IDBInitialize *pIDBInitialize;
	IDBDataSourceAdmin *pIDBDataSourceAdmin;

	IDBCreateCommand *pIDBCreateCommand;
	ITransactionLocal *pITransactionLocal;

	bool compactEdition;
};

class SQLAPI_API ssOleDbCommandHandles : public saCommandHandles
{
public:
	ssOleDbCommandHandles();

	ICommandText *pICommandText;
	IMultipleResults *pIMultipleResults;
	IRowset *pIRowset;
};

extern const GUID SA_DBPROPSET_DATASOURCEINFO;

#endif // !defined(__SSOLEDBAPI_H__)
