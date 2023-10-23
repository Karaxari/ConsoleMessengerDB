#pragma once
#include "ChatMessages.h"

class PrivateChat : public ChatMessages // Публичное наследование, без него никак!
{
public:
	PrivateChat() = default;
	PrivateChat(string chatID, string loginCompanion) : _chatID(chatID), _loginCompanion(loginCompanion) { }
	PrivateChat(string chatID, list<string> chatHistory, string loginCompanion) : ChatMessages(chatHistory) { _chatID = chatID; _loginCompanion = loginCompanion; }

	PrivateChat(const PrivateChat& ob); // Конструктор копирования. Добавил думаю что ошибка была в некорректной передаче объекта!
	//Но оказалось все проще: Рекурсивный вызов функции )). Спасибо дебагингу, а то обшика выводила отсутвует файла, вместо оверфлов

	void setChatID(string chatID);
	void setUsers(string users);

	string getChatID();
	string getloginCompanion();
private:
	string _chatID;
	string _loginCompanion;
};