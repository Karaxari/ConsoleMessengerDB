#pragma once
#include "MessangerUser.h"
//#include <iostream>
//#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>

class ConnectionDB
{
private:

#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 1000
    bool _status = false;
    SQLHANDLE sqlConnHandle{ nullptr }; // ���������� ��� ���������� � ����� ������
    SQLHANDLE sqlEnvHandle{ nullptr }; // ���������� ��������� ���� ������
    SQLHANDLE sqlStmtHandle{ nullptr };  // ���������� ��� ���������� �������� � ���� ������
    SQLWCHAR retconstring[SQL_RETURN_CODE_LEN]{}; // ������ ��� ���� �������� �� ������� API ODBC

    void setStatus(bool status);
    void innitConnect();
    void creatingAndFillingTables();
    bool setSQL(const wchar_t* sql, int type);
    bool creatingSQL_Query(string str);
    void disconnect();
    list<MessangerUser> selectThreeColumnTables(const wchar_t* sql);

public:
    ConnectionDB();
    ~ConnectionDB();
    //void selectAllUsers();
    list<MessangerUser> selectAllUsers();
    list<string> selectMessageGeneralChat();
    bool insertUser(MessangerUser user);
    bool insertGeneralChatMessage(string login, string dateMessage, string message);

    bool setSQL_SELECT(const wchar_t* sql);
    bool setSQL_INSERT(const wchar_t* sql);
    bool getStatus();
};

