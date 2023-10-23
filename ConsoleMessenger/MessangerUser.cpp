#include "MessangerUser.h"

// Логирование сообщений временем отправки
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

// Загрузка информации из файлов об списке приватных чатов пользователя и их переписок!
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
            cout << "\n Warning! Ошибка в структуре файла: " + pathToUsersChats << endl;
        }
    }

    map <string, PrivateChat> ::iterator it = _listOfChats.begin();
    for (; it != _listOfChats.end(); ++it) {
        pathFile = pathPrivateChats + it->second.getChatID() + ".txt";
        arrData.clear(); //  Можно и не делать, и так инициализируется другим списком

        arrData = readingDataFromFile(pathFile);

        for (string line : arrData) {
            it->second.setMesageChat(line);
        }
    }
}

// Запись информации о чате в файл к каждому из участников чата
void MessangerUser::addingUsersToChat(string chatID, string loginCompanion)
{
    string pathFile = pathToUsersChats + getLogin() + ".txt";
    string data = chatID + " " + loginCompanion;
    writingDataToFile(pathFile, data);

    pathFile = pathToUsersChats + loginCompanion + ".txt";
    data = chatID + " " + getLogin();
    writingDataToFile(pathFile, data);
}

// Можно было бы использовать try cath, но из-за простоты структуры файлов и их ихерархии хранения  ограничился стандартной проверкой
// Добавляем ID чата и логин собеседника в файл пользователя
void MessangerUser::writingDataToFile(string file, string data)
{
    ofstream out(file, ios::app);
    if (out.is_open())  
    {
        out << data << endl;
    }
    else {
        cout << "Не удалось создать файл: " + file;
    }
    out.close();
}

// Чтение данных из файла по указанному пути
list<string> MessangerUser::readingDataFromFile(string pathFile)
{
    list<string> arrData;
    string line;

    ifstream inUsersData(pathFile); // окрываем файл для чтения
    if (inUsersData.is_open())
    {
        while (getline(inUsersData, line))
        {
            arrData.push_back(line);
        }
    }
    else {
        cout << "Файл " + pathFile + " не существует \n";
    }
    inUsersData.close();     // закрываем файл

    return arrData;
}

// Разбиваем строку на подстроки с разделителем по пробелу
vector<string> MessangerUser::parsingStringWithSpaceDelimiter(string line)
{
    vector<string> arr;
    string delim(" ");
    size_t prev = 0;
    size_t next;
    size_t delta = delim.length();

    while ((next = line.find(delim, prev)) != string::npos) {
        string tmp = line.substr(prev, next - prev); //Отладка-start
        arr.push_back(line.substr(prev, next - prev)); //Отладка-end
        prev = next + delta;
    }
    string tmp = line.substr(prev); //Отладка-start
    arr.push_back(line.substr(prev)); //Отладка-end

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

// Добавляем новый чат пользвателю в данные
void MessangerUser::setUserChat(string chatID, string loginCompanion)
{
    if (chatIDVerification(chatID)) {
        PrivateChat chat{ chatID, loginCompanion };
        _listOfChats.insert_or_assign(chatID, chat);
    }
    else
        cout << "\n Warning! Данный chatID: " + chatID + " уже используется! проверьте корректность данных!\n";
}

// Проверка на сущетсвование чата с данным ID
bool MessangerUser::chatIDVerification(string chatID)
{
    if (_listOfChats.count(chatID)) {
        return false; // Чат с с этим ID уже существует
    }
    return true; // Данный ID доступен для использования
}

// Проверка существования чата с данным пользователем
bool MessangerUser::checkForChat(string loginCompanion)
{
    map <string, PrivateChat> ::iterator it = _listOfChats.begin();
    for (; it != _listOfChats.end(); ++it) {
        if (loginCompanion == it->second.getloginCompanion()) {
            return false; // Чат уже существует
        }
    }
    return true; // Чат с данным пользователем отсутствует
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

// Генерируем случайный ID чата
int MessangerUser::generateChatID()
{
    random_device random_device;
    mt19937 generator(random_device());
    uniform_int_distribution<> distribution(10000, 99999);

    return distribution(generator);
}

// Создаем новый чат с уникальным ID и записываем информациб о нем обим пользователям в файл
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

//Вывод списка чатов доступных данному пользователю!
void MessangerUser::outUserChatList()
{
    cout << "N: \t ID чата \t Логин \n";
    map <string, PrivateChat> ::iterator it = _listOfChats.begin();
    for (int i = 0; it != _listOfChats.end(); ++it, ++i) {
        cout << i + 1 << ":\t " + it->second.getChatID() + "\t " + it->second.getloginCompanion() << endl;
    }
}

// Список чатов у пользователя, и возможность открыть нужный чат по вводу ID
void MessangerUser::userChatList()
{
    string input = "";

    for (;;) {
        system("cls");
        outUserChatList();

        cout << "\n Набор действий: \n Введите ID чата из списка выше для перехода в чат с пользователем. \n Введите exit для выхода. \n : ";
        cin >> input;
        cout << '\n';

        if (input == "exit") break;

        if (!chatIDVerification(input)) {
            privateChat(input);
        }
        else {
            cout << "\n Чат с id: " + input + "не существует или нет доступа к нему! \n";
        }
    }

}

// Интерфейс приватного чата с отправкой сообщений
void MessangerUser::privateChat(string chatID)
{
    string message = "", tmp_message = "";

    for (;;)
    {
        system("cls");
        _listOfChats[chatID].outChatHistory();
        tmp_message = "";

        cout << "\n Enther - чтобы отправить сообщение, для выхода введите слово: exit \n";
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