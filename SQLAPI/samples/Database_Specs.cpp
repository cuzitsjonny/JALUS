
bool TableExists(SAConnection &Connection, const char *sTable)
{
    bool bExist = true;
    SAString sSQL = "Select * from ";
    sSQL += sTable;
    try { SACommand cmd(&Connection, sSQL); cmd.Execute(); }
    catch(SAException &) {bExist = false;}
    return bExist;
}

const char* MapSADataType_tToString(SADataType_t eDataType)
{
	switch(eDataType)
	{
	case SA_dtUnknown:
		return "Unknown";
	case SA_dtShort:
		return "Short";
	case SA_dtLong:
		return "Long";
	case SA_dtDouble:
		return "Double";
	case SA_dtDateTime:
		return "DateTime";
	case SA_dtString:
		return "String";
	case SA_dtBytes:
		return "Bytes";
	case SA_dtLongBinary:
		return "Long Binary";
	case SA_dtLongChar:
		return "Long Char";
	case SA_dtBLob:
		return "BLob";
	case SA_dtCLob:
		return "CLob";
	case SA_dtCursor:		// will be avail in next version of SQLAPI++
		return "Ref Cursor";
	default:
		assert(false);
	}

	return "NYI";
}

void PrintTableList(SAConnection &Connection)
{
    SACommand cmd(&Connection);

	switch(Connection.Client())
	{
	case SA_SQLServer_Client:
		cmd.setCommandText(
			"select su.name + '.' + so.name from sysobjects so, sysusers su "
			"where so.uid = su.uid and so.type in ('S', 'U', 'V') order by 1");
		break;
	case SA_Oracle_Client:
		cmd.setCommandText(
			"select OWNER || '.' || OBJECT_NAME from SYS.ALL_OBJECTS "
			"where OBJECT_TYPE in ('TABLE', 'VIEW') order by 1");
		break;
	}

	cmd.Execute();
	while(cmd.FetchNext())
	{
		printf("Field name: %s\n", (const char*)cmd[1].asString());
	}
}

void PrintColumnsDescr(SAConnection &Connection, const char *sTable)
{
    SAString sSQL = "Select * from ";
    sSQL += sTable;
    try 
    { 
        SACommand cmd(&Connection, sSQL); 
        cmd.Execute();
        for(int i = 0 ; i < cmd.FieldCount(); ++i)
        {
            printf("Field name: %s, "
				"filed type: %s",
				"field size: %d, "
				"field precision: %d, "
				"field scale: %d, ",
				"nulls allowed?: %d\n",
				(const char*)cmd[i].Name(),
				MapSADataType_tToString(cmd[i].FieldType()),
				cmd[i].FieldSize(),
				cmd[i].FieldPrecision(),
				cmd[i].FieldScale(),
				!cmd[i].isFieldRequired());
        }
    }
    catch(SAException &) {}
}

