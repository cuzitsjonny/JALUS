//////////////////////////////////////////////////////////////////////
// cubeSqlAPI.h
//////////////////////////////////////////////////////////////////////

#if !defined(__CUBESQLAPI_H__)
#define __CUBESQLAPI_H__

#include <SQLAPI.h>
#include <samisc.h>

#include <cubesql.h>

typedef int (*cubesql_connect_t)(csqldb **db, const char *host, int port, const char *username, const char *password, int timeout, int encryption);
typedef int (*cubesql_connect_ssl_t)(csqldb **db, const char *host, int port, const char *username, const char *password, int timeout, char *ssl_certificate_path);
typedef void (*cubesql_disconnect_t)(csqldb *db, int gracefully);
typedef int (*cubesql_execute_t)(csqldb *db, const char *sql);
typedef csqlc* (*cubesql_select_t)(csqldb *db, const char *sql, int unused);
typedef int (*cubesql_commit_t)(csqldb *db);
typedef int (*cubesql_rollback_t)(csqldb *db);
typedef int (*cubesql_bind_t)(csqldb *db, const char *sql, char **colvalue, int *colsize, int *coltype, int ncols);
typedef int (*cubesql_ping_t)(csqldb *db);
typedef void (*cubesql_cancel_t)(csqldb *db);
typedef int (*cubesql_errcode_t)(csqldb *db);
typedef char* (*cubesql_errmsg_t)(csqldb *db);
typedef int64 (*cubesql_changes_t)(csqldb *db);
typedef void (*cubesql_trace_t)(csqldb *db, trace_function trace, void *arg);

typedef csqlvm* (*cubesql_vmprepare_t)(csqldb *db, const char *sql);
typedef int (*cubesql_vmbind_int_t)(csqlvm *vm, int index, int value);
typedef int (*cubesql_vmbind_double_t)(csqlvm *vm, int index, double value);
typedef int (*cubesql_vmbind_text_t)(csqlvm *vm, int index, char *value, int len);
typedef int (*cubesql_vmbind_blob_t)(csqlvm *vm, int index, void *value, int len);
typedef int (*cubesql_vmbind_null_t)(csqlvm *vm, int index);
typedef int (*cubesql_vmbind_int64_t)(csqlvm *vm, int index, int64 value);
typedef int (*cubesql_vmbind_zeroblob_t)(csqlvm *vm, int index, int len);
typedef int (*cubesql_vmexecute_t)(csqlvm *vm);
typedef csqlc* (*cubesql_vmselect_t)(csqlvm *vm);
typedef int (*cubesql_vmclose_t)(csqlvm *vm);

typedef int (*cubesql_cursor_numrows_t)(csqlc *c);
typedef int (*cubesql_cursor_numcolumns_t)(csqlc *c);
typedef int (*cubesql_cursor_currentrow_t)(csqlc *c);
typedef int (*cubesql_cursor_seek_t)(csqlc *c, int index);
typedef int (*cubesql_cursor_iseof_t)(csqlc *c);
typedef int (*cubesql_cursor_columntype_t)(csqlc *c, int index);
typedef char* (*cubesql_cursor_field_t)(csqlc *c, int row, int column, int *len);
typedef int64 (*cubesql_cursor_rowid_t)(csqlc *c, int row);
typedef int64 (*cubesql_cursor_int64_t)(csqlc *c, int row, int column, int64 default_value);
typedef int (*cubesql_cursor_int_t)(csqlc *c, int row, int column, int default_value);
typedef double (*cubesql_cursor_double_t)(csqlc *c, int row, int column, double default_value);
typedef char* (*cubesql_cursor_cstring_t)(csqlc *c, int row, int column);
typedef char* (*cubesql_cursor_cstring_static_t)(csqlc *c, int row, int column, char *static_buffer, int bufferlen);
typedef void (*cubesql_cursor_free_t)(csqlc *c);

// API declarations
class SQLAPI_API cubeSqlAPI : public saAPI
{
public:
	cubeSqlAPI();

public:
	cubesql_connect_t cubesql_connect;
	cubesql_connect_ssl_t cubesql_connect_ssl;
	cubesql_disconnect_t cubesql_disconnect;
	cubesql_execute_t cubesql_execute;
	cubesql_select_t cubesql_select;
	cubesql_commit_t cubesql_commit;
	cubesql_rollback_t cubesql_rollback;
	cubesql_bind_t cubesql_bind;
	cubesql_ping_t cubesql_ping;
	cubesql_cancel_t cubesql_cancel;
	cubesql_errcode_t cubesql_errcode;
	cubesql_errmsg_t cubesql_errmsg;
	cubesql_changes_t cubesql_changes;
	cubesql_trace_t cubesql_trace;

	cubesql_vmprepare_t cubesql_vmprepare;
	cubesql_vmbind_int_t cubesql_vmbind_int;
	cubesql_vmbind_double_t cubesql_vmbind_double;
	cubesql_vmbind_text_t cubesql_vmbind_text;
	cubesql_vmbind_blob_t cubesql_vmbind_blob;
	cubesql_vmbind_null_t cubesql_vmbind_null;
	cubesql_vmbind_int64_t cubesql_vmbind_int64;
	cubesql_vmbind_zeroblob_t cubesql_vmbind_zeroblob;
	cubesql_vmexecute_t cubesql_vmexecute;
	cubesql_vmselect_t cubesql_vmselect;
	cubesql_vmclose_t cubesql_vmclose;

	cubesql_cursor_numrows_t cubesql_cursor_numrows;
	cubesql_cursor_numcolumns_t cubesql_cursor_numcolumns;
	cubesql_cursor_currentrow_t cubesql_cursor_currentrow;
	cubesql_cursor_seek_t cubesql_cursor_seek;
	cubesql_cursor_iseof_t cubesql_cursor_iseof;
	cubesql_cursor_columntype_t cubesql_cursor_columntype;
	cubesql_cursor_field_t cubesql_cursor_field;
	cubesql_cursor_rowid_t cubesql_cursor_rowid;
	cubesql_cursor_int64_t cubesql_cursor_int64;
	cubesql_cursor_int_t cubesql_cursor_int;
	cubesql_cursor_double_t cubesql_cursor_double;
	cubesql_cursor_cstring_t cubesql_cursor_cstring;
	cubesql_cursor_cstring_static_t cubesql_cursor_cstring_static;
	cubesql_cursor_free_t cubesql_cursor_free;

	void SetTraceFunction(SAConnection& con, trace_function trace, void *arg);

public:
	virtual void InitializeClient(const SAConnection *pConnection);
	virtual void UnInitializeClient(bool unloadAPI);

	virtual long GetClientVersion() const;

	virtual ISAConnection *NewConnection(SAConnection *pConnection);

protected:
	void  *m_hLibrary;
	SAMutex m_loaderMutex;
	long m_nDLLVersionLoaded;

	void ResetAPI();
	void LoadAPI();
	void LoadStaticAPI();
};

class SQLAPI_API cubeSqlConnectionHandles : public saConnectionHandles
{
public:
	cubeSqlConnectionHandles();

	csqldb *pDb;
};

class SQLAPI_API cubeSqlCommandHandles : public saCommandHandles
{
public:
	cubeSqlCommandHandles();

	csqlvm *pVm;
	csqlc *pC;
};

#endif //__CUBESQLAPI_H__
