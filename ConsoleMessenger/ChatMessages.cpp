#include "ChatMessages.h"

void ChatMessages::setChatHistory(list<string> chatHistory) {
	_chatHistory = chatHistory;
}

void ChatMessages::setMesageChat(string message) {
	_chatHistory.push_back(message);
}

list<string> ChatMessages::getChatHistory() {
	return _chatHistory;
}

void ChatMessages::outChatHistory() {
	for (string message : _chatHistory)
	{
		cout << message << endl;
	}
}