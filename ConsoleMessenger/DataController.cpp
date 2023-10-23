#include "DataController.h"

DataController::DataController() {
    cout << "Конструктор DataController \n";
    dataLoader();
}

// Загрузка списка зарегистрированных пользователей и переписки в общем чате
void DataController::dataLoader()
{
    // Реализация с чтением из DB
    list<MessangerUser> arrUsers = connectionDB.selectAllUsers();
    for (MessangerUser user : arrUsers) {
        setUser(user);
    }

    list<string> arrMessage = connectionDB.selectMessageGeneralChat();
    for (string message : arrMessage) {
        setMessageGeneralChat(message);
    }

    // Реализация с чтением из файла
    /*MessangerUser* tmpUser = new MessangerUser();
    list<string> arrData = tmpUser->readingDataFromFile(pathUsersData);
    for (string line : arrData) {
        vector<string> arr = tmpUser->parsingStringWithSpaceDelimiter(line);

        if (arr.size() == 3) {
            MessangerUser user{ arr[0], arr[1], arr[2] };
            setUser(user);
        }
        else {
            cout << "\n Warning! Ошибка в структуре файла: " + pathUsersData << endl;
        }
    }

    arrData.clear(); //  Можно и не делать, и так инициализируется другим списком
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
    _generalСhat.push_back(message);
}

void DataController::loadUser(MessangerUser user)
{
    setUser(user);
    //Сохранение данных пользователя в DB
    connectionDB.insertUser(user);
    

    //Сохранение данных пользователя в файле
    //string data = user.getFullName();
    //user.writingDataToFile(pathUsersData, data);
}

// Выводит список пользователей с данными, тестировачная функция!
void DataController::outUsers()
{
    cout << "N: \t Имя \t Логин \t Пароль \n";
    map <string, MessangerUser> ::iterator it = _listOfUsers.begin();
    for (int i = 0; it != _listOfUsers.end(); ++it, ++i) {
        cout << i + 1 << ":\t " + it->second.getName() + "\t " + it->second.getLogin() + "\t " + it->second.getPassword() << endl;
    }
}

// Вывод сообщений из общего чата
void DataController::outMessagesGeneralChat()
{
    for (string message : _generalСhat)
    {
        cout << message << endl;
    }
}

// Проверка корректности логина и пароля пользователя
bool DataController::authentication(string login, string password)
{
    if (_listOfUsers.count(login)) {
        if (password == _listOfUsers[login].getPassword()) {
            return true;
        }
    }
    return false;
}

//Проверка на сущетсвование пользователя с данным логином
bool DataController::loginVerification(string login)
{
    if (_listOfUsers.count(login)) {
        return false; // Пользователь с таким логином уже существует
    }
    return true; // Данный логин не зарегистрирован в системе
}

//Авторизация пользователя после ввода логина и пароля
void DataController::authorization(string login, string password)
{
    if (authentication(login, password)) // Дополнительная проверка корректности данных
    {
        personalAccount(login);
    }
    else {
        cout << "\n Отказано в доступе! \n";
    }
}

// Интерфейс меню с выбором общего и приватных чатов
void DataController::personalAccount(string login)
{
    char choice{ '0' };

    for (;;) {
        system("cls");
        cout << " Набор действий: \n 1 - Общий чат \n 2 - Личные чаты \n 3 - Выход \n\n Введите действие: ";
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

// Интерфейс общего чата, с выводом переписки и отправкой новых сообщений
void DataController::generalChat(string login)
{
    char choice{ '0' };

    for (;;) {
        system("cls");
        outMessagesGeneralChat();

        cout << "\n Набор действий: \n 1 - Написать сообщение \n 2 - Выход \n\n Введите действие: ";
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

// Отправка сообщения в общий чат
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

        cout << "\n Enther - чтобы отправить сообщение, для выхода введите слово: exit \n";
        SetConsoleCP(1251);
        while (temp != '\n') {
            cin.get(temp);
            tmp_message += temp;
        }
        SetConsoleCP(866);

        if (tmp_message == "exit\n") {
            break;
        }
        
        //Запись сообщения общего чата в DB
        if (tmp_message.size() > 1) {
            if (tmp_message.rfind("\n") != string::npos) {
                tmp_message.erase(tmp_message.rfind("\n"));
            }
            connectionDB.insertGeneralChatMessage(login, getCurrentTimeDB(), tmp_message);
            message = login + ":" + getCurrentTimeDB() + ":" + tmp_message;
            setMessageGeneralChat(message);
        }


        //Запись сообщения общего чата в файл
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

// Интерфейс приватных чатов пользователя
void DataController::privateChat(string login)
{
    char choice{ '0' };

    for (;;) {
        system("cls");

        cout << " Набор действий: \n 1 - Открыть список чатов \n 2 - Создать новый чат \n 3 - Выход \n\n Введите действие: ";
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

// Создание нового приватного чата
void DataController::createNewPrivateChat(string login)
{
    system("cls");
    string loginCompanion = "";

    for (;;) {

        cout << "\n Введите логин пользователся с которым хотите создать чат, либо exit для выхода: ";
        cin >> loginCompanion;
        cout << '\n';

        if (loginCompanion == "exit") {
            return;
        }

        if (loginCompanion == login) {
            cout << " Нельзя создать чат с самим с собой! \n";
            continue;
        }

        if (!loginVerification(loginCompanion)) {
            if (_listOfUsers[login].checkForChat(loginCompanion)) {
                _listOfUsers[login].createNewChat(loginCompanion);
                return;
            }
            else {
                cout << " Чат с данным пользователем: " + loginCompanion + " уже существует!\n";
            }
        }
        else {
            cout << " Пользователь с логином: " + loginCompanion + " не найден в системе!\n";
        }
    }
}