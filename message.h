#pragma once

#include <iostream>

class Message{
private:
	std::string _content, _senderName, _senderIP;

public:
	Message(const std::string content, const std::string senderName, const std::string senderIP);
	virtual ~Message();

	std::string getContent() const, getSenderName() const, getSenderIP() const;

	friend std::ostream& operator<<(std::ostream& os, const Message& message);
	std::string encodeForBroadcast() const;
}