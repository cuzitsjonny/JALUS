// samisc.h
//
//////////////////////////////////////////////////////////////////////

#if !defined(__SAMISC_H__)
#define __SAMISC_H__

SQLAPI_API char* sa_strncpy(char *dest, const char *src, size_t count);
SQLAPI_API wchar_t* sa_wcsncpy(wchar_t *dest, const wchar_t *src, size_t count);

SQLAPI_API char* sa_strncat(char *dest, const char *src, size_t count);
SQLAPI_API wchar_t* sa_wcsncat(wchar_t *dest, const wchar_t *src, size_t count);

SQLAPI_API SAChar *sa_strlwr(SAChar *src);
SQLAPI_API SAChar *sa_strupr(SAChar *src);

SQLAPI_API SAString sa_bin2hex(const void* pBuf, size_t nLenInBytes);

#define sa_min(x, y) ((x) < (y)? (x) : (y))
#define sa_max(x, y) ((x) > (y)? (x) : (y))

#if defined(SA_UNICODE)
#define sa_tcsncpy sa_wcsncpy
#define sa_tcsncat sa_wscncat
#define sa_tcscpy(x, y) wcscpy(x, y)
#define sa_tcslen(x) wcslen(x)
#define sa_tcsstr(x, y) wcsstr(x, y)
#define sa_tcschr(x, y) wcschr(x, y)
#define sa_istspace(x) iswspace(x)
#define sa_istdigit(x) iswdigit(x)
#define sa_istalpha(x) iswalpha(x)
#define sa_tcscmp(x, y) wcscmp(x, y)
#define sa_tcsncmp(x, y, z) wcsncmp(x, y, z)
#define sa_totlower(x) towlower(x)
#define sa_totupper(x) towupper(x)
#define sa_tcscoll(x, y) wcscoll(x, y)
#define sa_tcspbrk(x, y) wcspbrk(x, y)
#define sa_tcsrchr(x, y) wcsrchr(x, y)
#define sa_tcstol(x, y, z) wcstol(x, y, z)
#define sa_tcstoul(x, y, z) wcstoul(x, y, z)
#define sa_tcstod(x, y) wcstod(x, y)
#define sa_tstoi(x) (int)wcstol(x, NULL, 10)
#define sa_tstol(x) wcstol(x, NULL, 10)
#if (defined(_MSC_VER) &&  _MSC_VER >= 1400) || defined(__STDC_LIB_EXT1__)
#define sa_stscanf swscanf_s
#else
#define sa_stscanf swscanf
#endif
#define sa_tprintf wprintf
#define sa_tscanf wscanf
#define sa_gettchar getwchar
#ifdef SQLAPI_WINDOWS
#ifdef __BORLANDC__
#define sa_sntprintf_s snwprintf_s
#define sa_vsntprintf_s(x, y, z, j) vsnwprintf_s(x, y, z, j)
#else
#define sa_sntprintf_s _snwprintf_s
#define sa_vsntprintf_s(x, y, z, j) _vsnwprintf_s(x, y, y, z, j)
#endif
#define sa_sntprintf _snwprintf
#define sa_vsntprintf(x, y, z, j) _vsnwprintf(x, y, z, j)
#define sa_tcsicmp(x, y) _wcsicmp(x, y)
#else
#define sa_vsntprintf_s vsnwprintf_s
#define sa_vsntprintf(x, y, z, j) vswprintf(x, y, z, j)
#define sa_sntprintf_s swprintf_s
#define sa_sntprintf swprintf
#define sa_tcsicmp(x, y) wcscasecmp(x, y)
#endif // ! SQLAPI_WINDOWS
#define sa_csinc(x) (++(x))
#define sa_clen(x) (1)
#else
#define sa_tcsncpy sa_strncpy
#define sa_tcsncat sa_strncat
#define sa_tcscpy(x, y) strcpy(x, y)
#define sa_tcslen(x) strlen(x)
#define sa_tcsstr(x, y) strstr(x, y)
#define sa_tcschr(x, y) strchr(x, y)
#define sa_istspace(x) isspace((unsigned char)x)
#define sa_istdigit(x) isdigit((unsigned char)x)
#define sa_istalpha(x) isalpha((unsigned char)x)
#define sa_tcscmp(x, y) strcmp(x, y)
#define sa_tcsncmp(x, y, z) strncmp(x, y, z)
#define sa_totlower(x) tolower((unsigned char)x)
#define sa_totupper(x) toupper((unsigned char)x)
#define sa_tcscoll(x, y) strcoll(x, y)
#define sa_tcspbrk(x, y) strpbrk(x, y)
#define sa_tcsrchr(x, y) strrchr(x, y)
#define sa_tcstol(x, y, z) strtol(x, y, z)
#define sa_tcstoul(x, y, z) strtoul(x, y, z)
#define sa_tcstod(x, y) strtod(x, y)
#define sa_tstoi(x) atoi(x)
#define sa_tstol(x) atol(x)
#if (defined(_MSC_VER) &&  _MSC_VER >= 1400) || defined(__STDC_LIB_EXT1__)
#define sa_stscanf sscanf_s
#else
#define sa_stscanf sscanf
#endif
#define sa_tprintf printf
#define sa_tscanf scanf
#define sa_gettchar getchar
#ifdef SQLAPI_WINDOWS
#define sa_sntprintf _snprintf
#if defined(__BORLANDC__) && (__BORLANDC__  <= 0x0520)
#define sa_vsntprintf(x, y, z, j) vsprintf(x, z, j)
#else
#define sa_vsntprintf(x, y, z, j) _vsnprintf(x, y, z, j)
#endif
#ifdef __BORLANDC__
#define sa_sntprintf_s snprintf_s
#define sa_vsntprintf_s(x, y, z, j) vsnprintf_s(x, y, z, j)
#else
#define sa_sntprintf_s _snprintf_s
#define sa_vsntprintf_s(x, y, z, j) _vsnprintf_s(x, y, y, z, j)
#endif
#define sa_tcsicmp(x, y) _stricmp(x, y)
#else
#define sa_vsntprintf_s vsnprintf_s
#define sa_vsntprintf(x, y, z, j) vsnprintf(x, y, z, j)
#define sa_sntprintf snprintf
#define sa_sntprintf_s snprintf_s
#define sa_tcsicmp(x, y) strcasecmp(x, y)
#endif // ! SQLAPI_WINDOWS
#define sa_csinc(x) (++(x))
#define sa_clen(x) (1)
#endif

#ifdef SQLAPI_WINDOWS
#ifdef __BORLANDC__
#define _strnicmp strnicmp
#define _stricmp stricmp
#endif
#elif SQLAPI_SCOOSR5
#define _strnicmp strnicmp
#define _stricmp stricmp
#else
#define _strnicmp strncasecmp
#define _stricmp strcasecmp
#endif	// defined(SQLAPI_WINDOWS)

#ifdef SQLAPI_WINDOWS
#include <windows.h>
#else
#if defined(SA_USE_PTHREAD)
#include <pthread.h>
#endif	// defined(SA_USE_PTHREAD)
#endif

class SQLAPI_API SAMutex
{
#ifdef  SQLAPI_WINDOWS
#ifdef SA_USE_MUTEXT_LOCK
	HANDLE m_hMutex;
#else
	CRITICAL_SECTION m_hCriticalSection;
#endif
#endif	// defined(SQLAPI_WINDOWS)

#if defined(SA_USE_PTHREAD)
	pthread_mutex_t m_mutex;	// mutex
	// we need additional machinery
	// to allow portable recursion
	int				m_locks;	// number of times owner locked mutex
	pthread_t		m_owner_thread;	//owner of mutex
	pthread_mutex_t	m_helper_mutex;	// structure access lock
#endif	// defined(SA_USE_PTHREAD)

public:
	SAMutex();
	virtual ~SAMutex();

	void Wait();
	void Release();
};

class SQLAPI_API SACriticalSectionScope
{
	SAMutex *m_pSAMutex;

public:
	SACriticalSectionScope(SAMutex *pSAMutex);
	virtual ~SACriticalSectionScope();
};

#endif // !defined(__SAMISC_H__)
