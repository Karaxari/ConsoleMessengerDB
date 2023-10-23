#include "PrivateChat.h"

PrivateChat::PrivateChat(const PrivateChat& ob)
{
	_chatID = ob._chatID;
	_loginCompanion = ob._loginCompanion;
}

void PrivateChat::setChatID(string chatID) {
	_chatID = chatID;
}

void PrivateChat::setUsers(string loginCompanion) {
	_loginCompanion = loginCompanion;
}

string PrivateChat::getChatID() {
	return _chatID;
}

string PrivateChat::getloginCompanion() {
	return _loginCompanion;
}