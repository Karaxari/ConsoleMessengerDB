#include "MessangerUser.h"

// ����������� ��������� �������� ��������
string getCurrentTime()
{
    struct tm newtime;
    time_t now = time(0);
    localtime_s(&newtime, &now);
    string time = "[" + to_string(newtime.tm_year + 1900) + "-" + to_string(newtime.tm_mon + 1) + "-" + to_string(newtime.tm_mday) + " "
        + to_string(newtime.tm_hour) + ":" + to_string(newtime.tm_min) + ":" + to_string(newtime.tm_sec) + "]";
    return time;
}

string getCurrentTimeDB()
{
    struct tm newtime;
    time_t now = time(0);
    localtime_s(&newtime, &now);
    string time = to_string(newtime.tm_year + 1900) + "-" + to_string(newtime.tm_mon + 1) + "-" + to_string(newtime.tm_mday) + " "
        + to_string(newtime.tm_hour) + ":" + to_string(newtime.tm_min) + ":" + to_string(newtime.tm_sec);
    return time;
}

MessangerUser::~MessangerUser() {

}

// �������� ���������� �� ������ �� ������ ��������� ����� ������������ � �� ���������!
void MessangerUser::loadingUserChats()
{
    string pathFile = pathToUsersChats + getLogin() + ".txt";
    list<string> arrData = readingDataFromFile(pathFile);

    for (string line : arrData) {
        vector<string> arr = parsingStringWithSpaceDelimiter(line);

        if (arr.size() == 2) {
            setUserChat(arr[0], arr[1]);
        }
        else {
            cout << "\n Warning! ������ � ��������� �����: " + pathToUsersChats << endl;
        }
    }

    map <string, PrivateChat> ::iterator it = _listOfChats.begin();
    for (; it != _listOfChats.end(); ++it) {
        pathFile = pathPrivateChats + it->second.getChatID() + ".txt";
        arrData.clear(); //  ����� � �� ������, � ��� ���������������� ������ �������

        arrData = readingDataFromFile(pathFile);

        for (string line : arrData) {
            it->second.setMesageChat(line);
        }
    }
}

// ������ ���������� � ���� � ���� � ������� �� ���������� ����
void MessangerUser::addingUsersToChat(string chatID, string loginCompanion)
{
    string pathFile = pathToUsersChats + getLogin() + ".txt";
    string data = chatID + " " + loginCompanion;
    writingDataToFile(pathFile, data);

    pathFile = pathToUsersChats + loginCompanion + ".txt";
    data = chatID + " " + getLogin();
    writingDataToFile(pathFile, data);
}

// ����� ���� �� ������������ try cath, �� ��-�� �������� ��������� ������ � �� ��������� ��������  ����������� ����������� ���������
// ��������� ID ���� � ����� ����������� � ���� ������������
void MessangerUser::writingDataToFile(string file, string data)
{
    ofstream out(file, ios::app);
    if (out.is_open())  
    {
        out << data << endl;
    }
    else {
        cout << "�� ������� ������� ����: " + file;
    }
    out.close();
}

// ������ ������ �� ����� �� ���������� ����
list<string> MessangerUser::readingDataFromFile(string pathFile)
{
    list<string> arrData;
    string line;

    ifstream inUsersData(pathFile); // �������� ���� ��� ������
    if (inUsersData.is_open())
    {
        while (getline(inUsersData, line))
        {
            arrData.push_back(line);
        }
    }
    else {
        cout << "���� " + pathFile + " �� ���������� \n";
    }
    inUsersData.close();     // ��������� ����

    return arrData;
}

// ��������� ������ �� ��������� � ������������ �� �������
vector<string> MessangerUser::parsingStringWithSpaceDelimiter(string line)
{
    vector<string> arr;
    string delim(" ");
    size_t prev = 0;
    size_t next;
    size_t delta = delim.length();

    while ((next = line.find(delim, prev)) != string::npos) {
        string tmp = line.substr(prev, next - prev); //�������-start
        arr.push_back(line.substr(prev, next - prev)); //�������-end
        prev = next + delta;
    }
    string tmp = line.substr(prev); //�������-start
    arr.push_back(line.substr(prev)); //�������-end

    return arr;
}

void MessangerUser::setName(string name)
{
    _name = name;
}

void MessangerUser::setLogin(string login)
{
    _login = login;
}

void MessangerUser::setPassword(string password)
{
    _password = password;
}

// ��������� ����� ��� ����������� � ������
void MessangerUser::setUserChat(string chatID, string loginCompanion)
{
    if (chatIDVerification(chatID)) {
        PrivateChat chat{ chatID, loginCompanion };
        _listOfChats.insert_or_assign(chatID, chat);
    }
    else
        cout << "\n Warning! ������ chatID: " + chatID + " ��� ������������! ��������� ������������ ������!\n";
}

// �������� �� ������������� ���� � ������ ID
bool MessangerUser::chatIDVerification(string chatID)
{
    if (_listOfChats.count(chatID)) {
        return false; // ��� � � ���� ID ��� ����������
    }
    return true; // ������ ID �������� ��� �������������
}

// �������� ������������� ���� � ������ �������������
bool MessangerUser::checkForChat(string loginCompanion)
{
    map <string, PrivateChat> ::iterator it = _listOfChats.begin();
    for (; it != _listOfChats.end(); ++it) {
        if (loginCompanion == it->second.getloginCompanion()) {
            return false; // ��� ��� ����������
        }
    }
    return true; // ��� � ������ ������������� �����������
}

string MessangerUser::getName() const
{
    return _name;
}

string MessangerUser::getLogin() const
{
    return _login;
}

string MessangerUser::getPassword() const
{
    return _password;
}

string MessangerUser::getFullName() const
{
    return _name + " " + _login + " " + _password;
}

// ���������� ��������� ID ����
int MessangerUser::generateChatID()
{
    random_device random_device;
    mt19937 generator(random_device());
    uniform_int_distribution<> distribution(10000, 99999);

    return distribution(generator);
}

// ������� ����� ��� � ���������� ID � ���������� ���������� � ��� ���� ������������� � ����
void MessangerUser::createNewChat(string loginCompanion)
{
    int id{ 0 };

    for (;;) {
        id = generateChatID();

        if (chatIDVerification(to_string(id))) {
            setUserChat(to_string(id), loginCompanion);
            addingUsersToChat(to_string(id), loginCompanion);
            break;
        }
    }
}

//����� ������ ����� ��������� ������� ������������!
void MessangerUser::outUserChatList()
{
    cout << "N: \t ID ���� \t ����� \n";
    map <string, PrivateChat> ::iterator it = _listOfChats.begin();
    for (int i = 0; it != _listOfChats.end(); ++it, ++i) {
        cout << i + 1 << ":\t " + it->second.getChatID() + "\t " + it->second.getloginCompanion() << endl;
    }
}

// ������ ����� � ������������, � ����������� ������� ������ ��� �� ����� ID
void MessangerUser::userChatList()
{
    string input = "";

    for (;;) {
        system("cls");
        outUserChatList();

        cout << "\n ����� ��������: \n ������� ID ���� �� ������ ���� ��� �������� � ��� � �������������. \n ������� exit ��� ������. \n : ";
        cin >> input;
        cout << '\n';

        if (input == "exit") break;

        if (!chatIDVerification(input)) {
            privateChat(input);
        }
        else {
            cout << "\n ��� � id: " + input + "�� ���������� ��� ��� ������� � ����! \n";
        }
    }

}

// ��������� ���������� ���� � ��������� ���������
void MessangerUser::privateChat(string chatID)
{
    string message = "", tmp_message = "";

    for (;;)
    {
        system("cls");
        _listOfChats[chatID].outChatHistory();
        tmp_message = "";

        cout << "\n Enther - ����� ��������� ���������, ��� ������ ������� �����: exit \n";
        char temp = '0';
        SetConsoleCP(1251);
        while (temp != '\n') {
            cin.get(temp);
            tmp_message += temp;
        }
        SetConsoleCP(866);

        if (tmp_message == "exit\n") {
            break;
        }

        if (tmp_message.size() > 1) {
            message = getLogin() + ":" + getCurrentTime() + ": " + tmp_message;
            if (message.rfind("\n") != string::npos) {
                message.erase(message.rfind("\n"));
            }

            _listOfChats[chatID].setMesageChat(message);

            string pathFile = pathPrivateChats + chatID + ".txt";
            writingDataToFile(pathFile, message);
        }
    }
}