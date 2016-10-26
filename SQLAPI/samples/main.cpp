#include <stdlib.h>
#include <stdio.h>

#include <iostream>

#include <SQLAPI.h>


int main(int argc, char *argv[])
{
    const SAString instance="bedlam-m.bedlam/ora111u";
    const SAString user="sys";
    const SAString password="java";
    try
    {
        SAConnection c;
        c.Connect(instance,user,password,SA_Oracle_Client);
        SACommand cmd1(&c,"CREATE TABLE sqlapi_char (c varchar(100))");
        //cmd1.Execute();

        SACommand cmd2(&c,"INSERT INTO sqlapi_char (c) VALUES (:c_value)");
        const SAString clobValue=SAString("\xc3\x84");
        cmd2.Param("c_value").setAsString()=clobValue;
        //cmd2.Execute();

        SACommand cmd3(&c,"SELECT c FROM sqlapi_clob");
        cmd3.Execute();
        if (cmd3.FetchNext())
        {
            std::cout << (const char*)cmd3[1].asString() << std::endl;
        }
        c.Disconnect();
    }
    catch (const SAException& e) {
        std::cout << "Exception: " << e.ErrClass() << "-" << e.ErrText().GetMultiByteChars() << "-" << e.ErrPos() << "-" << e.ErrNativeCode() << std::endl;
    }
    return 0;
}
