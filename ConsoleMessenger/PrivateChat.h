#pragma once
#include "ChatMessages.h"

class PrivateChat : public ChatMessages // ��������� ������������, ��� ���� �����!
{
public:
	PrivateChat() = default;
	PrivateChat(string chatID, string loginCompanion) : _chatID(chatID), _loginCompanion(loginCompanion) { }
	PrivateChat(string chatID, list<string> chatHistory, string loginCompanion) : ChatMessages(chatHistory) { _chatID = chatID; _loginCompanion = loginCompanion; }

	PrivateChat(const PrivateChat& ob); // ����������� �����������. ������� ����� ��� ������ ���� � ������������ �������� �������!
	//�� ��������� ��� �����: ����������� ����� ������� )). ������� ���������, � �� ������ �������� ��������� �����, ������ ��������

	void setChatID(string chatID);
	void setUsers(string users);

	string getChatID();
	string getloginCompanion();
private:
	string _chatID;
	string _loginCompanion;
};