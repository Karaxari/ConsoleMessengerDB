#pragma once
//#include "MessangerUser.h"
#include "ConnectionDB.h"

const string pathUsersData = "DataController\\allUsers.txt"; // данные пользователей: Имя, логин и пароль
const string pathGeneralChatData = "DataController\\generalChat.txt"; // переписка общего чата

class DataController
{
public:
	DataController();
	~DataController() = default;

	void dataLoader();
	void setUser(MessangerUser user);
	void setMessageGeneralChat(string message);
	void loadUser(MessangerUser user);

	void outUsers();
	void outMessagesGeneralChat();

	bool authentication(string login, string password);
	bool loginVerification(string login);
	void authorization(string login, string password);

private:
	map<string, MessangerUser> _listOfUsers; // Закрытое наслеование, релизация отнощения "содержит"
	list <string> _generalСhat;
	ConnectionDB connectionDB; // Объект для взаимодействия с БД

	void personalAccount(string login);
	void generalChat(string login);
	void privateChat(string login);
	void createNewPrivateChat(string login);
	void sendingMessagesGeneralChat(string login);
};