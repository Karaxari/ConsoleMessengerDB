#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <random>
#include <windows.h>;
#include "PrivateChat.h"

using namespace std;

string getCurrentTime();
string getCurrentTimeDB();

const string pathToUsersChats = "DataController\\Users\\"; // ѕуть к директории с файлами пользователей, содержащих информацию об приватных чатах в кторых они участвуют
const string pathPrivateChats = "DataController\\PrivateChats\\"; // ѕуть к директории где хранитс€ переписки приватных чатов

class MessangerUser
{
public:
	MessangerUser() = default;
	MessangerUser(string name, string login, string password) : _name(name), _login(login), _password(password) {}
	~MessangerUser();

	void writingDataToFile(string file, string data);
	list<string> readingDataFromFile(string pathFile);
	vector<string> parsingStringWithSpaceDelimiter(string line);

	void loadingUserChats();
	void addingUsersToChat(string charID, string loginCompanion);
	void setName(string name);
	void setLogin(string login);
	void setPassword(string password);
	void setUserChat(string chatID, string loginCompanion);

	void userChatList();
	void outUserChatList();
	void createNewChat(string loginCompanion);
	void privateChat(string charID);

	int generateChatID();
	bool chatIDVerification(string chatID);
	bool checkForChat(string loginCompanion);

	string getName() const;
	string getLogin() const;
	string getPassword() const;
	string getFullName() const;

private:
	string _name;
	string _login;
	string _password;

	map<string, PrivateChat> _listOfChats; // “оже закрытое наслеование, релизаци€ отнощени€ "содержит"
};