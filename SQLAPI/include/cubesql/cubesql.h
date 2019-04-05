/*
 *  cubesql.h
 *
 *	This file is the public interface for the cubeSQL Server SDK.
 *	You just need to include this header file in your projects.
 *
 *  (c) 2006-2011 SQLabs s.r.l. -- All Rights Reserved
 *  Author: Marco Bambini (MB)
 *
 */

#ifndef CUBESQLSDK_H
#define CUBESQLSDK_H

#ifdef __cplusplus
extern "C" {
#endif
	
#ifdef CUBESQL_BUILD_DLL
#define CUBESQL_APIEXPORT		__declspec(dllexport)
#else
#define CUBESQL_APIEXPORT
#endif

// custom boolean values
#ifndef kTRUE
#define kTRUE				1
#endif

#ifndef kFALSE
#define kFALSE				0
#endif
	
// default values
#define	kDEFAULT_PORT		4430
#define kDEFAULT_TIMEOUT	12
#define kNOERR				0
#define kERR				-1

// client side errors
#define kMEMORY_ERROR		-2
#define kPARAMETER_ERROR	-3
#define kPROTOCOL_ERROR		-4
#define kZLIB_ERROR			-5
#define kSSL_ERROR			-6
#define kSSL_CERT_ERROR		-7

// encryption flags used in cubesql_connect
#define kAESNONE			0
#define kAES128				2
#define kAES192				3
#define kAES256				4
#define kSSL				8
#define kSSL_AES128			kSSL+kAES128
#define kSSL_AES192			kSSL+kAES192
#define kSSL_AES256			kSSL+kAES256
	
// flag used in cubesql_cursor_getfield
#define	kCUBESQL_COLNAME	0
#define kCUBESQL_CURROW		-1
#define	kCUBESQL_COLTABLE	-2
#define kCUBESQL_ROWID		-666
	
// flag used in cubesql_cursor_seek
#define kCUBESQL_SEEKNEXT	-2
#define kCUBESQL_SEEKFIRST	-3
#define kCUBESQL_SEEKLAST	-4
#define kCUBESQL_SEEKPREV	-5

// SSL
#define kSSL_LIBRARY_PATH		1
#define kCRYPTO_LIBRARY_PATH	2
	
#ifndef int64
#ifdef WIN32
typedef __int64 int64;
#else
typedef long long int int64;
#endif
#endif
	
// column types coming from the server
enum {
	TYPE_None		= 0,
	TYPE_Integer	= 1,
	TYPE_Float		= 2,
	TYPE_Text		= 3,
	TYPE_Blob		= 4,
	TYPE_Boolean	= 5,
	TYPE_Date		= 6,
	TYPE_Time		= 7,
	TYPE_Timestamp	= 8,
	TYPE_Currency	= 9
};

// column types to specify in the cubesql_bind command (coltype)
#define kBIND_INTEGER	1
#define kBIND_DOUBLE	2
#define kBIND_TEXT		3
#define kBIND_BLOB		4
#define kBIND_NULL		5
#define kBIND_INT64		8
#define kBIND_ZEROBLOB	9
	
// define opaque datatypes
typedef struct csqldb csqldb;
typedef struct csqlc csqlc;
typedef struct csqlvm csqlvm;
typedef void (*trace_function) (const char*, void*);
	
// function prototypes
CUBESQL_APIEXPORT int		cubesql_connect (csqldb **db, const char *host, int port, const char *username, const char *password, int timeout, int encryption);
CUBESQL_APIEXPORT int		cubesql_connect_ssl (csqldb **db, const char *host, int port, const char *username, const char *password, int timeout, char *ssl_certificate_path);
CUBESQL_APIEXPORT void		cubesql_disconnect (csqldb *db, int gracefully);
CUBESQL_APIEXPORT int		cubesql_execute (csqldb *db, const char *sql);
CUBESQL_APIEXPORT csqlc		*cubesql_select (csqldb *db, const char *sql, int unused);
CUBESQL_APIEXPORT int		cubesql_commit (csqldb *db);
CUBESQL_APIEXPORT int		cubesql_rollback (csqldb *db);
CUBESQL_APIEXPORT int		cubesql_bind (csqldb *db, const char *sql, char **colvalue, int *colsize, int *coltype, int ncols);
CUBESQL_APIEXPORT int		cubesql_ping (csqldb *db);
CUBESQL_APIEXPORT void		cubesql_cancel (csqldb *db);
CUBESQL_APIEXPORT int		cubesql_errcode (csqldb *db);
CUBESQL_APIEXPORT char		*cubesql_errmsg (csqldb *db);
CUBESQL_APIEXPORT int64		cubesql_changes (csqldb *db);
CUBESQL_APIEXPORT void		cubesql_trace (csqldb *db, trace_function trace, void *arg);
	
CUBESQL_APIEXPORT csqlvm	*cubesql_vmprepare (csqldb *db, const char *sql);
CUBESQL_APIEXPORT int		cubesql_vmbind_int (csqlvm *vm, int index, int value);
CUBESQL_APIEXPORT int		cubesql_vmbind_double (csqlvm *vm, int index, double value);
CUBESQL_APIEXPORT int		cubesql_vmbind_text (csqlvm *vm, int index, char *value, int len);
CUBESQL_APIEXPORT int		cubesql_vmbind_blob (csqlvm *vm, int index, void *value, int len);
CUBESQL_APIEXPORT int		cubesql_vmbind_null (csqlvm *vm, int index);
CUBESQL_APIEXPORT int		cubesql_vmbind_int64 (csqlvm *vm, int index, int64 value);
CUBESQL_APIEXPORT int		cubesql_vmbind_zeroblob (csqlvm *vm, int index, int len);
CUBESQL_APIEXPORT int		cubesql_vmexecute (csqlvm *vm);
CUBESQL_APIEXPORT csqlc		*cubesql_vmselect (csqlvm *vm);
CUBESQL_APIEXPORT int		cubesql_vmclose (csqlvm *vm);
	
CUBESQL_APIEXPORT int		cubesql_cursor_numrows (csqlc *c);
CUBESQL_APIEXPORT int		cubesql_cursor_numcolumns (csqlc *c);
CUBESQL_APIEXPORT int		cubesql_cursor_currentrow (csqlc *c);
CUBESQL_APIEXPORT int		cubesql_cursor_seek (csqlc *c, int index);
CUBESQL_APIEXPORT int		cubesql_cursor_iseof (csqlc *c);
CUBESQL_APIEXPORT int		cubesql_cursor_columntype (csqlc *c, int index);
CUBESQL_APIEXPORT char		*cubesql_cursor_field (csqlc *c, int row, int column, int *len);
CUBESQL_APIEXPORT int64		cubesql_cursor_rowid (csqlc *c, int row);
CUBESQL_APIEXPORT int64		cubesql_cursor_int64 (csqlc *c, int row, int column, int64 default_value);
CUBESQL_APIEXPORT int		cubesql_cursor_int (csqlc *c, int row, int column, int default_value);
CUBESQL_APIEXPORT double	cubesql_cursor_double (csqlc *c, int row, int column, double default_value);
CUBESQL_APIEXPORT char		*cubesql_cursor_cstring (csqlc *c, int row, int column);
CUBESQL_APIEXPORT char		*cubesql_cursor_cstring_static (csqlc *c, int row, int column, char *static_buffer, int bufferlen);	
CUBESQL_APIEXPORT void		cubesql_cursor_free (csqlc *c);

// private functions
int		cubesql_connect_token (csqldb **db, const char *host, int port, const char *username, const char *password,
							   int timeout, int encryption, char *token, int useOldProtocol, const char *ssl_certificate,
							   const char *root_certificate, const char *ssl_certificate_password, const char *ssl_chiper_list);
int		cubesql_connect_old_protocol (csqldb **db, const char *host, int port, const char *username, const char *password, int timeout, int encryption);
void	cubesql_clear_errors (csqldb *db);
csqldb	*cubesql_cursor_db (csqlc *cursor);
csqlc	*cubesql_cursor_create (csqldb *db, int nrows, int ncolumns, int *types, char **names);
int		cubesql_cursor_addrow (csqlc *cursor, char **row, int *len);
int		cubesql_cursor_columntypebind (csqlc *c, int index);
void	cubesql_setuserptr (csqldb *db, void *userptr);
void	*cubesql_getuserptr (csqldb *db);
void	cubesql_settoken (csqldb *db, char *token);
void	cubesql_sethostverification (csqldb *db, char *hostverification);
char	*cubesql_gettoken (csqldb *db);
void	cubesql_setpath (int type, char *path);
void	cubesql_seterror (csqldb *db, int errcode, char *errmsg);
const char *cubesql_sslversion (void);
	
#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif

#endif
