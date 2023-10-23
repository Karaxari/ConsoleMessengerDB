#pragma once
#include <iostream>
#include <list>

using namespace std;

class ChatMessages
{
public:
	ChatMessages() = default;
	ChatMessages(list<string> chatHistory) : _chatHistory(chatHistory) { }

	void setChatHistory(list<string> chatHistory);
	void setMesageChat(string mesage);

	void outChatHistory();

	list<string> getChatHistory();

private:
	list<string> _chatHistory;
};