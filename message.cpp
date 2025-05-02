#include "message.h"

Message::Message(const std::string content, const std::string senderName, const std::string senderIP)
    : _content(content), _senderName(senderName), _senderIP(senderIP)
{
}

Message::~Message()
{
}

std::string Message::getContent() const
{
    return _content;
}

std::string Message::getSenderName() const
{
    return _senderName;
}

std::string Message::getSenderIP() const
{
    return _senderIP;
}

std::ostream& operator<<(std::ostream& os, const Message& message)
{
    os << "[" << message.getSenderName() << "] " << message.getContent();
    return os;
}

std::string Message::encodeForBroadcast() const
{
    return _senderName + FIELD_SPLITTER + _senderIP + FIELD_SPLITTER + _content;
}