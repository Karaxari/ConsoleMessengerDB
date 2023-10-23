#include "ConnectionDB.h"

ConnectionDB::ConnectionDB() {
    //std::cout << "Constructor\n";
    innitConnect();

    //Для создание и заполнения данными тестовых таблиц!
    //creatingAndFillingTables();
}

ConnectionDB::~ConnectionDB() {
    //std::cout << "Destructor\n";
    disconnect();
}

void ConnectionDB::setStatus(bool status) {
    _status = status;
}

bool ConnectionDB::getStatus() {
    return _status;
}

void ConnectionDB::innitConnect() {
    // Выделяем дескриптор для базы данных
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle)) {
        disconnect();
        return;
    }

    if (SQL_SUCCESS != SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0)) {
        disconnect();
        return;
    }

    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle)) {
        disconnect();
        return;
    }

    // Устанавливаем соединение с сервером  
    switch (SQLDriverConnect(sqlConnHandle,
        GetDesktopWindow(),
        (SQLWCHAR*)L"DRIVER={MySQL ODBC 8.1 ANSI Driver};SERVER=localhost;PORT=3306;DATABASE=testdb;UID=root;PWD=12345;",
        SQL_NTS,
        retconstring,
        1024,
        NULL,
        SQL_DRIVER_COMPLETE)) {


    case SQL_SUCCESS:
    case SQL_SUCCESS_WITH_INFO:
        //cout << "Successfully connected to SQL Server\n";
        break;

    case SQL_INVALID_HANDLE:
    case SQL_ERROR:
        //cout << "Could not connect to SQL Server\n";
        //goto COMPLETED;
        disconnect();
        return;

    default:
        break;
    }

    // Если соединение не установлено, то выходим из программы
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle)) {
        disconnect();
        return;
    }

    //cout << "\nExecuting T-SQL query...\n";

    setStatus(true);
}

void ConnectionDB::disconnect() {
    // Закрываем соединение и выходим из программы
    setStatus(false);
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    SQLDisconnect(sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);
}

bool ConnectionDB::setSQL(const wchar_t* sql, int type = 0) {
    // Если выполнение запроса с ошибками, то выходим из программы
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)sql, SQL_NTS)) {
        //cout << "Error querying SQL Server \n";
        //goto COMPLETED;
        //disconect();
        return false;
    }
    else {

        //Объявление структуры данных для результата запроса версии SQL
        SQLCHAR sqlVersion[SQL_RESULT_LEN];
        SQLLEN sql_str_length;
        // Переменная для хранения числа столбцов
        SQLSMALLINT    V_OD_colanz, V_OD_rowcount;
        SQLINTEGER   V_OD_err, V_OD_id;
        SQLCHAR     V_OD_buffer[200];

        if (type == 1) {
            V_OD_err = SQLBindCol(sqlStmtHandle, 1, SQL_INTEGER, &V_OD_id, sizeof(V_OD_id), nullptr);
            V_OD_err = SQLBindCol(sqlStmtHandle, 2, SQL_CHAR, &V_OD_buffer, SQL_RESULT_LEN, &sql_str_length);

            // Получим значение числа столбцов
            V_OD_err = SQLNumResultCols(sqlStmtHandle, &V_OD_colanz);
            std::cout << "Col count: " << V_OD_colanz << std::endl;

            V_OD_err = SQLRowCount(sqlStmtHandle, &sql_str_length);
            std::cout << "Row count: " << sql_str_length << std::endl;


            while (SQLFetch(sqlStmtHandle) != SQL_NO_DATA) {
                //Выведем на экран версию SQL          
                std::cout << "Id: " << V_OD_id << ", name: " << V_OD_buffer << std::endl;
            }
        }
    }
}

bool ConnectionDB::creatingSQL_Query(string str) {
    wchar_t* sql = new wchar_t[str.length() + 1];
    std::copy(str.begin(), str.end(), sql);
    sql[str.length()] = 0;

    bool status = setSQL(sql);

    delete[] sql;
    return status;
}

list<MessangerUser> ConnectionDB::selectThreeColumnTables(const wchar_t* sql) {
    list<MessangerUser> arrUsers;
    // Если выполнение запроса с ошибками, то выходим из программы
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)sql, SQL_NTS)) {
        cout << "Error querying SQL Server \n";
        //disconect();
        //return;
    }
    else {

        //Объявление структуры данных для результата запроса версии SQL
        SQLCHAR sqlVersion[SQL_RESULT_LEN];
        SQLLEN sql_str_length;
        // Переменная для хранения числа столбцов
        SQLSMALLINT    V_OD_colanz, V_OD_rowcount;
        SQLINTEGER   V_OD_err, V_OD_id;
        SQLCHAR     V_name_buf[255], V_login_buf[255], V_passwoed_buf[255];


        V_OD_err = SQLBindCol(sqlStmtHandle, 1, SQL_INTEGER, &V_OD_id, sizeof(V_OD_id), nullptr);
        V_OD_err = SQLBindCol(sqlStmtHandle, 2, SQL_CHAR, &V_name_buf, SQL_RESULT_LEN, &sql_str_length);
        V_OD_err = SQLBindCol(sqlStmtHandle, 3, SQL_CHAR, &V_login_buf, SQL_RESULT_LEN, &sql_str_length);
        V_OD_err = SQLBindCol(sqlStmtHandle, 4, SQL_CHAR, &V_passwoed_buf, SQL_RESULT_LEN, &sql_str_length);

        // Получим значение числа столбцов
        V_OD_err = SQLNumResultCols(sqlStmtHandle, &V_OD_colanz);
        std::cout << "Col count: " << V_OD_colanz << std::endl;

        V_OD_err = SQLRowCount(sqlStmtHandle, &sql_str_length);
        std::cout << "Row count: " << sql_str_length << std::endl;


        std::cout << "ID \t name \t login \t password \n";
        while (SQLFetch(sqlStmtHandle) != SQL_NO_DATA) {
            //Выведем на экран версию SQL          
            //std::cout << V_OD_id << '\t' << V_name_buf << '\t' << V_login_buf << '\t' << V_passwoed_buf << std::endl;
            string s1((const char*)V_name_buf), s2((const char*)V_login_buf), s3((const char*)V_passwoed_buf);
            MessangerUser user{ s1, s2, s3 };
            arrUsers.push_back(user);
        }

    }
    return arrUsers;
}

list<MessangerUser> ConnectionDB::selectAllUsers() {
    return selectThreeColumnTables(L"SELECT * from allUsers");
}

list<string> ConnectionDB::selectMessageGeneralChat() {
    //Костыль!!! здесь класс MessangerUser используется для получения данных из таблицы generalChat.
    list<MessangerUser> arrMessage = selectThreeColumnTables(L"SELECT * from generalChat");
    list<string> allMessage;
    for (MessangerUser message : arrMessage) {
        allMessage.push_back(message.getName() + ":" + message.getLogin() + ":" + message.getPassword());
    }
    return allMessage;
}

bool ConnectionDB::insertUser(MessangerUser user) {
    string str = "INSERT INTO allUsers(id, name, login, password) values(default, '" + user.getName() + "', '" + user.getLogin() + "', '" + user.getPassword() + "');";
    return creatingSQL_Query(str);
}

bool ConnectionDB::insertGeneralChatMessage(string login, string dateMessage, string message) {
    string str = "INSERT INTO generalChat(id, login, dateMessage, message) values(default, '" + login + "', '" + dateMessage + "', '" + message + "');";
    return creatingSQL_Query(str);
}

bool ConnectionDB::setSQL_SELECT(const wchar_t* sql) {
    return setSQL(sql, 1);
}

bool ConnectionDB::setSQL_INSERT(const wchar_t* sql) {
    return setSQL(sql);
}

void ConnectionDB::creatingAndFillingTables() {
    setSQL(L"CREATE TABLE allUsers(id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(255), login VARCHAR(255), password VARCHAR(255))");
    setSQL(L"INSERT INTO allUsers(id, name, login, password) values(default, 'kurt', 'kurt', '123qwe')");
    setSQL(L"INSERT INTO allUsers(id, name, login, password) values(default, 'klark', 'klark', '123qwe')");
    setSQL(L"INSERT INTO allUsers(id, name, login, password) values(default, 'кирил', 'kiril', '123qwe')");
    setSQL(L"INSERT INTO allUsers(id, name, login, password) values(default, 'Олег', 'oleg', 'oleg')");
    setSQL(L"INSERT INTO allUsers(id, name, login, password) values(default, 'kurt', '123qwe', 'exit')");
    setSQL(L"INSERT INTO allUsers(id, name, login, password) values(default, 'df', 'sdf', 'sdfg')");

    setSQL(L"CREATE TABLE generalChat(id INT AUTO_INCREMENT PRIMARY KEY, login VARCHAR(255), dateMessage DATETIME, message VARCHAR(255))");
    setSQL(L"INSERT INTO generalChat(id, login, dateMessage, message) values(default, 'klark', '2023-3-28 0:18:51', 'Добрый день! Как дела?')");
    setSQL(L"INSERT INTO generalChat(id, login, dateMessage, message) values(default, 'klark', '2023-3-28 0:19:9', 'Что нового на HabraHabr?')");
    setSQL(L"INSERT INTO generalChat(id, login, dateMessage, message) values(default, 'kurt', '2023-3-28 0:20:12', 'Добрый! Все как обычно')");
    setSQL(L"INSERT INTO generalChat(id, login, dateMessage, message) values(default, 'kurt', '2023-3-28 0:20:21', 'Ничего интересного пока нет')");
    setSQL(L"INSERT INTO generalChat(id, login, dateMessage, message) values(default, 'kurt', '2023-3-28 0:23:20', 'sd')");
    setSQL(L"INSERT INTO generalChat(id, login, dateMessage, message) values(default, 'oleg', '2023-3-28 20:52:16', 'Привет, я Олег')");
    setSQL(L"INSERT INTO generalChat(id, login, dateMessage, message) values(default, 'kurt', '2023-3-29 19:44:40', 'здарова народ')");
    setSQL(L"INSERT INTO generalChat(id, login, dateMessage, message) values(default, 'kurt', '2023-3-29 19:44:43', 'че так')");
    setSQL(L"INSERT INTO generalChat(id, login, dateMessage, message) values(default, 'kurt', '2023-3-29 19:44:46', 'тихо')");
    setSQL(L"INSERT INTO generalChat(id, login, dateMessage, message) values(default, 'kiril', '2023-3-29 19:46:24', 'Олеееег')");
}