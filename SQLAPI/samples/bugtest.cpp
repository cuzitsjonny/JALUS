#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>

#ifdef WIN32
#include <windows.h>
#include <tchar.h>
#endif

#include <SQLAPI.h>
#include <samisc.h>

#include <string>
#include <iostream>

static FILE *pFile = NULL;
size_t nTotalBound;
size_t FromFileWriter(SAPieceType_t &ePieceType,
    void *pBuf, size_t nLen, void *pAddlData)
{
    if (ePieceType == SA_FirstPiece)
    {
        const char *sFilename = (const char *)pAddlData;
        pFile = fopen(sFilename, "rb");
        if (!pFile)
            SAException::throwUserException(-1, _TSA("Can not open file '%s'"),
            (const SAChar*)SAString(sFilename));
        nTotalBound = 0;
    }

    size_t nRead = fread(pBuf, 1, nLen, pFile);
    nTotalBound += nRead;

    // show progress
    printf("%d bytes of file bound\n", nTotalBound);

    if (feof(pFile))
    {
        if (ePieceType == SA_FirstPiece)
            ePieceType = SA_OnePiece;
        else
            ePieceType = SA_LastPiece;

        fclose(pFile);
        pFile = NULL;
    }
    return nRead;
}

SAString ReadWholeFile(const char *sFilename, bool binaryData)
{
    SAString s;
    char sBuf[1024];
    FILE *pFile = fopen(sFilename, binaryData ? "rb" : "rt");

    if (!pFile)
        SAException::throwUserException(-1, _TSA("Error opening file '%s'\n"),
        (const SAChar*)SAString(sFilename));

    do
    {
        size_t nRead = fread(sBuf, 1, sizeof(sBuf), pFile);
        s += SAString((const void*)sBuf, nRead);
    } while (!feof(pFile));

    fclose(pFile);
    return s;
}

SAString ReadWholeTextFile(const SAChar *szFilename)
{
    SAString s;
    char szBuf[32 * 1024];
    FILE *pFile = _tfopen(szFilename, _TSA("rb"));

    if (!pFile)
        SAException::throwUserException(-1, _TSA("Error opening file '%s'\n"),
        (const SAChar*)SAString(szFilename));

    do
    {
        size_t nRead = fread(szBuf, 1, sizeof(szBuf), pFile);
        s += SAString(szBuf, nRead);
    } while (!feof(pFile));

    fclose(pFile);
    return s;
}

void WriteWholeFile(const char *sFilename, const SAString& data)
{
    FILE *pFile = fopen(sFilename, "wb");
    size_t n, written = 0, len = data.GetBinaryLength();
    const void* pData = (const void*)data;

    sa_printf(_TSA("PRGLEN: %d\n"), len);

    if (!pFile)
        SAException::throwUserException(-1, _TSA("Error opening file '%s'\n"),
        (const SAChar*)SAString(sFilename));

    while (len > written) {
        n = fwrite((const char*)pData + written, 1, sa_min(1024, len - written), pFile);
        if (n <= 0)
            break;
        written += n;
    }

    fclose(pFile);
}

size_t nTotalRead;

void IntoFileReader(
    SAPieceType_t ePieceType,
    void *pBuf,
    size_t nLen,
    size_t nBlobSize,
    void *pAddlData)
{
    const char *sFilename = (const char *)pAddlData;

    if (ePieceType == SA_FirstPiece || ePieceType == SA_OnePiece)
    {
        nTotalRead = 0;

        pFile = fopen(sFilename, "wb");
        if (!pFile)
            SAException::throwUserException(-1, _TSA("Can not open file '%s' for writing"),
            (const SAChar*)SAString(sFilename));
    }

    fwrite(pBuf, 1, nLen, pFile);

    nTotalRead += nLen;

    if (ePieceType == SA_LastPiece || ePieceType == SA_OnePiece)
    {
        fclose(pFile);
        pFile = NULL;
        printf("%s : %d bytes of %d read\n",
            sFilename, nTotalRead, nBlobSize);
    }
}

//#include "Scrollable_Cursor.cpp"

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>

#ifdef WIN32
#include <windows.h>
#include <tchar.h>
#endif

#include <SQLAPI.h>
#include <samisc.h>

#include <string>
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
    SAConnection con;

    try
    {
        con.Connect(_TSA("localhost@test"), _TSA("root"), _TSA(""), SA_MySQL_Client);

        SACommand cmd(&con, _TSA("insert into t1(f1,f2) values(5, :1)"));
        cmd.setOption(_TSA("UseStatement")) = _TSA("TRUE");
        cmd.Param(1).setAsLongBinary() = _TSA("TRUE");
        //cmd.Param(1).setAsLongBinary(FromFileWriter, 0, "D:\\service-manual-acer-aspire_5542_5542g_5242__jv50_tr_.pdf");
        cmd.Execute();

        con.Commit();

        cmd.setCommandText(_TSA("select f2 from t1 where f1 = 1"));
        cmd.Execute();
        if (cmd.FetchNext())
        {
            printf("VAL: %s\n", cmd[1].asString().GetMultiByteChars());
        }
    }
    catch (SAException& x)
    {
        printf("ERROR:\n%s\n", x.ErrText().GetMultiByteChars());
    }


    return 0;
}

