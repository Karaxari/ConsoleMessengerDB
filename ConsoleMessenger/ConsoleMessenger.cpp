#include "DataController.h"
#include <algorithm>

DataController* dataController; // Объект главного класса контроллера

// Ограничение на ввод кирилицы
constexpr bool IsRussianChar1251(const char c)
{
    return (c >= 'А' && c <= 'я') || c == 'ё' || c == 'Ё';
}

// Прототипы функций
void authentication();
void registration();

int main()
{
    setlocale(LC_ALL, "rus");
    dataController = new DataController();

    //connectionDB.setSQL_SELECT(L"SELECT * from test");
    //connectionDB.setSQL_INSERT(L"INSERT INTO test(id, name) values(default, 'Vika')");
    //connectionDB.setSQL_INSERT(L"CREATE TABLE test_2(id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(255))");
    //connectionDB.setSQL_INSERT(L"INSERT INTO test_2(id, name) values(default, 'Vika')");
    //connectionDB.setSQL_SELECT(L"SELECT * from test_2");

    char choice{ '0' };

    // Мейн интерфейс
    for (;;) {
        system("cls");
        cout << " Набор действий: \n 1 - Войти \n 2 - Регистрация \n 3 - Выход \n\n Введите действие: ";
        cin >> choice;
        cout << '\n';

        switch (choice)
        {
        case '1': cout << "Выбрана авторизация \n"; authentication(); break;
        case '2': cout << "Выбрана регистрация \n"; registration(); break;
        case '3': return 0;
        default:
            system("cls");
            cin.clear();
            break;
        }
    }

    delete dataController; // Очистка памяти, удаление главного контроллера
}

// Авторизация пользователя 
void authentication()
{
    string login, password;

    for (;;) {
        system("cls");

        cout << " Введите логин: ";
        cin >> login;
        cout << "\n Введите пароль: ";
        cin >> password;

        if (dataController->authentication(login, password)) {
            cout << "\n Вы вошли как: " + login << endl;
            dataController->authorization(login, password);
            break;
        }
        else {
            cout << "\n Неверный пароль или логин пользователя! Если хотите попробовать еще раз нажмителюбую кнопку, либо q - для выхода \n";
            char ch;
            cin >> ch;
            if (ch == 'q') {
                break;
            }
            cin.clear();
        }
    }
}

// Регистрация пользователя в системе с проверками и записью данных после успешной регистрации
void registration()
{
    system("cls");

    string name, login, password;
    cout << "Для выхода введите: exit \nВведите имя: ";
    SetConsoleCP(1251);
    cin >> name;
    SetConsoleCP(866);

    if (name == "exit") return;

    for (;;) {
        cout << "\n Придумайте логин: ";
        cin >> login;

        if (login == "exit") return;

        if (dataController->loginVerification(login)) {
            if (any_of(login.cbegin(), login.cend(), IsRussianChar1251)) {
                cout << "\n Использование кирилицы в логине не допустимо!";
            }
            else {
                break;
            }
        }
        else {
            cout << "\n Пользователь с таким логином уже есть в системе. Придумайте другой!";
        }
    }
    for (;;) {
        cout << "\n Придумайте пароль: ";
        cin >> password;

        if (password == "exit") return;

        if (any_of(password.cbegin(), password.cend(), IsRussianChar1251)) {
            cout << "\n Использование кирилицы в пароле не допустимо!";
            continue;
        }

        string tmp_password;
        cout << "\n Повторите пароль: ";
        cin >> tmp_password;
        if (password == tmp_password) {
            cout << "\n Учетная запись успешно создана!";
            MessangerUser user  { name, login, password };
            dataController->loadUser(user);
            break;
        }
        else {
            cout << "\n Пароли не совпадают, повторите еще раз!";
        }
    }
}