#include "network.h"

Broadcaster::Broadcaster(const int broadcastPort)
{
  this->_broadcastPort = broadcastPort;
}

Broadcaster::~Broadcaster() {}

void Broadcaster::broadcastMessage(const Message message) const
{
  try
  {
    asio::io_context io_context;
    asio::ip::udp::socket socket(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0));
    socket.set_option(asio::socket_base::reuse_address(true));
    asio::ip::udp::endpoint broadcast_endpoint(asio::ip::make_address(BROADCAST_IP), _broadcastPort);
    socket.send_to(asio::buffer(message.encodeForBroadcast()), broadcast_endpoint);
  }
  catch (std::exception &e)
  {
    std::cerr << "UDP BROADCAST FAILED >>> " << e.what() << std::endl;
  }
}

Receiver::Receiver(const int receiverPort)
    : _receivePort(receiverPort), _socket(_io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), receiverPort))
{
  asio::ip::address multicast_address = asio::ip::make_address(BROADCAST_IP);
  _socket.set_option(asio::ip::multicast::join_group(multicast_address));
}

Receiver::~Receiver()
{
  // delete the queue
  while (!_messageQueue.empty())
  {
    delete _messageQueue.front();
    _messageQueue.pop();
  }
}

void Receiver::ParseMessage(const std::string &udpData, std::string &name, std::string &context)
{
  if (udpData.empty())
  {
    std::cerr << "Recieved bad data, ignoring" << "\n";
    name = "Unknown";
    context = "";
    return;
  }

  size_t firstSplit = udpData.find(FIELD_SPLITTER);
  if (firstSplit == std::string::npos)
  {
    std::cerr << "Message parsing failed: No field splitter found." << std::endl;
    name = "Unknown";
    context = udpData;
    return;
  }

  size_t secondSplit = udpData.find(FIELD_SPLITTER, firstSplit + strlen(FIELD_SPLITTER));
  if (secondSplit == std::string::npos)
  {
    std::cerr << "Message parsing failed: Only one field splitter found." << std::endl;
    name = "Unknown";
    context = udpData;
    return;
  }

  name = udpData.substr(0, firstSplit);

  if (secondSplit + strlen(FIELD_SPLITTER) < udpData.size())
  {
    context = udpData.substr(secondSplit + strlen(FIELD_SPLITTER));
  }
  else
  {
    context = "";
  }
}

void Receiver::CatchMessage(Message &message)
{
  char buffer[RECV_BUFFER_SIZE];
  asio::ip::udp::endpoint sender_endpoint;

  while (true)
  {
    std::size_t len = _socket.receive_from(asio::buffer(buffer), sender_endpoint);
    std::string received_message(buffer, len);
    std::string sender_name, message_content;
    std::string sender_IP = sender_endpoint.address().to_string();
    ParseMessage(received_message, sender_name, message_content);
    message = Message(message_content, sender_name, sender_IP);
    return;
  }
}

void Receiver::listen()
{
  while (true)
  {
    Message *message = new Message("", "", "");
    CatchMessage(*message);
    std::lock_guard<std::mutex> lock(this->_messageQueueMutex);
    this->_messageQueue.push(message);
  }
}

Message* Receiver::getQueueMessage()
{
  std::lock_guard<std::mutex> lock(_messageQueueMutex);
  if (_messageQueue.empty())
  {
    return nullptr;
  }
  Message *message = _messageQueue.front();
  _messageQueue.pop();
  return message;
}
