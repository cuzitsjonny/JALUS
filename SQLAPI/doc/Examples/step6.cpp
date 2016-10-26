#include <stdio.h>  // for printf
#include <SQLAPI.h> // main SQLAPI++ header

// forwards
void IntoFileReader(
	SAPieceType_t ePieceType,
	void *pBuf,
	size_t nLen,
	size_t nBlobSize,
	void *pAddlData);

int main(int argc, char* argv[])
{
    SAConnection con; // connection object
    SACommand cmd(
        &con,
        "Select fblob from test_tbl");    // command object
    
    try
    {
        // connect to database (Oracle in our example)
        con.Connect("test", "tester", "tester", SA_Oracle_Client);

        // Usage 1. Read whole BLob(s) into internal buffers
		// Select BLob from our test table
        cmd.Execute();
        // fetch results row by row and print results
        while(cmd.FetchNext())
        {
			// after fetching a row all Long/Lob fields are automatically read into internal buffers
			// just like other data types
			SAString s = cmd.Field("fblob").asBLob();
            printf("Size of BLob is %d bytes\n", s.GetLength());
        }

        // Usage 2. Read BLob in pieces providing user callback for BLob data processing
		// Select blob from our test table
        cmd.Execute();
		// do not automatically read this field into internal buffer (into corresponding SAField object)
		// we will provide a callback for BLob fetching after FetchNext
		cmd.Field("fblob").setLongOrLobReaderMode(SA_LongOrLobReaderManual);
        
		// fetch results row by row and print results
		SAString sFilename;
		int i = 0;
        while(cmd.FetchNext())
        {
			sFilename.Format("fblob%d.bin", ++i);
			// at that moment all fields are fetched except
			// those that set for manual retrieving
			// read them (fblob in our example) now
			if(!cmd.Field("fblob").isNull())
				cmd.Field("fblob").ReadLongOrLob(
					IntoFileReader,	// our callback to read BLob content into file
					10*1024,		// our desired piece size
					(void*)(const char*)sFilename	// additional data, filename in our example
					);
		}

        // commit changes on success
        con.Commit();

        printf("Rows with BLob field fetched!\n");
    }
    catch(SAException &x)
    {
        // SAConnection::Rollback()
        // can also throw an exception
        // (if a network error for example),
        // we will be ready
        try
        {
            // on error rollback changes
            con.Rollback();
        }
        catch(SAException &)
        {
        }
        // print error message
        printf("%s\n", (const char*)x.ErrText());
    }
    
    return 0;
}

static FILE *pFile = NULL;
size_t nTotalRead;
void IntoFileReader(
	SAPieceType_t ePieceType,
	void *pBuf,
	size_t nLen,
	size_t nBlobSize,
	void *pAddlData)
{
	if(ePieceType == SA_FirstPiece || ePieceType == SA_OnePiece)
	{
		nTotalRead = 0;
		const char *sFilename = (const char *)pAddlData;
		pFile = fopen(sFilename, "wb");
		if(!pFile)
			SAException::throwUserException(-1, "Can not open file '%s' for writing", sFilename);
	}

	fwrite(pBuf, 1, nLen, pFile);
	nTotalRead += nLen;

	// show progress
	printf("%d bytes of %d read\n", nTotalRead, nBlobSize);

    if(ePieceType == SA_LastPiece || ePieceType == SA_OnePiece)
	{
		fclose(pFile);
		pFile = NULL;
	}
}
