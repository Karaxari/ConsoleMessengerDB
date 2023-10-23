#include "DataController.h"

DataController::DataController() {
    cout << "����������� DataController \n";
    dataLoader();
}

// �������� ������ ������������������ ������������� � ��������� � ����� ����
void DataController::dataLoader()
{
    // ���������� � ������� �� DB
    list<MessangerUser> arrUsers = connectionDB.selectAllUsers();
    for (MessangerUser user : arrUsers) {
        setUser(user);
    }

    list<string> arrMessage = connectionDB.selectMessageGeneralChat();
    for (string message : arrMessage) {
        setMessageGeneralChat(message);
    }

    // ���������� � ������� �� �����
    /*MessangerUser* tmpUser = new MessangerUser();
    list<string> arrData = tmpUser->readingDataFromFile(pathUsersData);
    for (string line : arrData) {
        vector<string> arr = tmpUser->parsingStringWithSpaceDelimiter(line);

        if (arr.size() == 3) {
            MessangerUser user{ arr[0], arr[1], arr[2] };
            setUser(user);
        }
        else {
            cout << "\n Warning! ������ � ��������� �����: " + pathUsersData << endl;
        }
    }

    arrData.clear(); //  ����� � �� ������, � ��� ���������������� ������ �������
    arrData = tmpUser->readingDataFromFile(pathGeneralChatData);

    for (string line : arrData) {
        setMessageGeneralChat(line);
    }*/
}

void DataController::setUser(MessangerUser user)
{
    _listOfUsers.insert_or_assign(user.getLogin(), user);
}

void DataController::setMessageGeneralChat(string message)
{
    _general�hat.push_back(message);
}

void DataController::loadUser(MessangerUser user)
{
    setUser(user);
    //���������� ������ ������������ � DB
    connectionDB.insertUser(user);
    

    //���������� ������ ������������ � �����
    //string data = user.getFullName();
    //user.writingDataToFile(pathUsersData, data);
}

// ������� ������ ������������� � �������, ������������� �������!
void DataController::outUsers()
{
    cout << "N: \t ��� \t ����� \t ������ \n";
    map <string, MessangerUser> ::iterator it = _listOfUsers.begin();
    for (int i = 0; it != _listOfUsers.end(); ++it, ++i) {
        cout << i + 1 << ":\t " + it->second.getName() + "\t " + it->second.getLogin() + "\t " + it->second.getPassword() << endl;
    }
}

// ����� ��������� �� ������ ����
void DataController::outMessagesGeneralChat()
{
    for (string message : _general�hat)
    {
        cout << message << endl;
    }
}

// �������� ������������ ������ � ������ ������������
bool DataController::authentication(string login, string password)
{
    if (_listOfUsers.count(login)) {
        if (password == _listOfUsers[login].getPassword()) {
            return true;
        }
    }
    return false;
}

//�������� �� ������������� ������������ � ������ �������
bool DataController::loginVerification(string login)
{
    if (_listOfUsers.count(login)) {
        return false; // ������������ � ����� ������� ��� ����������
    }
    return true; // ������ ����� �� ��������������� � �������
}

//����������� ������������ ����� ����� ������ � ������
void DataController::authorization(string login, string password)
{
    if (authentication(login, password)) // �������������� �������� ������������ ������
    {
        personalAccount(login);
    }
    else {
        cout << "\n �������� � �������! \n";
    }
}

// ��������� ���� � ������� ������ � ��������� �����
void DataController::personalAccount(string login)
{
    char choice{ '0' };

    for (;;) {
        system("cls");
        cout << " ����� ��������: \n 1 - ����� ��� \n 2 - ������ ���� \n 3 - ����� \n\n ������� ��������: ";
        cin >> choice;
        cout << '\n';

        switch (choice)
        {
        case '1': generalChat(login); break;
        case '2': _listOfUsers[login].loadingUserChats(); privateChat(login); break;
        case '3': return;
        default:
            system("cls");
            cin.clear();
            break;
        }
    }
}

// ��������� ������ ����, � ������� ��������� � ��������� ����� ���������
void DataController::generalChat(string login)
{
    char choice{ '0' };

    for (;;) {
        system("cls");
        outMessagesGeneralChat();

        cout << "\n ����� ��������: \n 1 - �������� ��������� \n 2 - ����� \n\n ������� ��������: ";
        cin >> choice;
        cout << '\n';

        switch (choice)
        {
        case '1': sendingMessagesGeneralChat(login); break;
        case '2': return;
        default:
            system("cls");
            cin.clear();
            break;
        }
    }
}

// �������� ��������� � ����� ���
void DataController::sendingMessagesGeneralChat(string login)
{
    string message = "", tmp_message = "";
    char temp = '0';

    for (;;)
    {
        temp = '0';
        system("cls");
        outMessagesGeneralChat();
        tmp_message = "";

        cout << "\n Enther - ����� ��������� ���������, ��� ������ ������� �����: exit \n";
        SetConsoleCP(1251);
        while (temp != '\n') {
            cin.get(temp);
            tmp_message += temp;
        }
        SetConsoleCP(866);

        if (tmp_message == "exit\n") {
            break;
        }
        
        //������ ��������� ������ ���� � DB
        if (tmp_message.size() > 1) {
            if (tmp_message.rfind("\n") != string::npos) {
                tmp_message.erase(tmp_message.rfind("\n"));
            }
            connectionDB.insertGeneralChatMessage(login, getCurrentTimeDB(), tmp_message);
            message = login + ":" + getCurrentTimeDB() + ":" + tmp_message;
            setMessageGeneralChat(message);
        }


        //������ ��������� ������ ���� � ����
        /*if (tmp_message.size() > 1) {
            message = login + ":" + getCurrentTime() + ": " + tmp_message;
            if (message.rfind("\n") != string::npos) {
                message.erase(message.rfind("\n"));
            }

            setMessageGeneralChat(message);
            _listOfUsers[login].writingDataToFile(pathGeneralChatData, message);
        }*/
    }
}

// ��������� ��������� ����� ������������
void DataController::privateChat(string login)
{
    char choice{ '0' };

    for (;;) {
        system("cls");

        cout << " ����� ��������: \n 1 - ������� ������ ����� \n 2 - ������� ����� ��� \n 3 - ����� \n\n ������� ��������: ";
        cin >> choice;
        cout << '\n';

        switch (choice)
        {
        case '1': _listOfUsers[login].userChatList(); break;
        case '2': createNewPrivateChat(login); break;
        case '3': return;
        default:
            cin.clear();
            break;
        }
    }
}

// �������� ������ ���������� ����
void DataController::createNewPrivateChat(string login)
{
    system("cls");
    string loginCompanion = "";

    for (;;) {

        cout << "\n ������� ����� ������������� � ������� ������ ������� ���, ���� exit ��� ������: ";
        cin >> loginCompanion;
        cout << '\n';

        if (loginCompanion == "exit") {
            return;
        }

        if (loginCompanion == login) {
            cout << " ������ ������� ��� � ����� � �����! \n";
            continue;
        }

        if (!loginVerification(loginCompanion)) {
            if (_listOfUsers[login].checkForChat(loginCompanion)) {
                _listOfUsers[login].createNewChat(loginCompanion);
                return;
            }
            else {
                cout << " ��� � ������ �������������: " + loginCompanion + " ��� ����������!\n";
            }
        }
        else {
            cout << " ������������ � �������: " + loginCompanion + " �� ������ � �������!\n";
        }
    }
}