#include "network.h"

Broadcaster::Broadcaster(const int broadcastPort){
	this->_broadcastPort = broadcastPort;
}

Broadcaster::~Broadcaster(){}

void Broadcaster::BroadcastMessage(const Message message){
  try{
    asio::io_context io_context;
    asio::ip::udp::socket socket(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0));
    socket.set_option(udp::socket::reuse_address(true));
    asio::ip::udp::endpoint broadcast_endpoint(asio::ip::make_address(BROADCAST_IP), _broadcastPort);
    socket.send_to(asio::buffer(message.encodeForBoadcast()), broadcast_endpoint);
  }
  catch (std::exception& e)
  {
    std::cerr << "UDP BROADCAST FAILED >>> " << e.what() << std::endl;
  }
}

Reciever::Reciever(const int recieverPort)
  : _recievePort(receivePort), _socket(_io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), receivePort)) 
  {
  asio::ip::address multicast_address = asio::ip::make_address(BROADCAST_IP);
  _socket.set_option(asio::ip::multicast::join_group(multicast_address));
  
  Receiver::~Reciver(){
    while(!_messageQueue.empty()){
      delete _messageQueue.front();
      _messageQueue.pop();
    }
  }
{

void Reciver::parseMessage(const std::string& udpData, const std::string& name, const std::string& content){
  
  if(udp.empty()){
    std::cerr << "Recieved bad data, ignoring" << "\n";
    name = "Unknown";
    content = "";
    return;
  }

  size_t firstSplit = udpData.find(FIELD_SPLITTER);
    if (firstSplit == std::string::npos) {
        std::cerr << "Message parsing failed: No field splitter found." << std::endl;
        name = "Unknown";
        content = udpData;
        return;
    }

    size_t secondSplit = udpData.find(FIELD_SPLITTER, firstSplit + strlen(FIELD_SPLITTER));
    if (secondSplit == std::string::npos) {
        std::cerr << "Message parsing failed: Only one field splitter found." << std::endl;
        name = "Unknown";
        content = udpData;
        return;
    }

    name = udpData.substr(0, firstSplit);
    
    if (secondSplit + strlen(FIELD_SPLITTER) < udpData.size()) {
        content = udpData.substr(secondSplit + strlen(FIELD_SPLITTER));
    } else {
        content = "";
    }
}

Reciver::catchMessage(Message& message){
  char buffer[RECV_BUFFER_SIZE];
  asio::ip::udp::endpoint sender_endpoint;

  while(true){
    std::size_t len = _socket.receive_from(asio::buffer(buffer), sender_endpoint);
    std::string received_message(buffer, len);
    std::string sender_name, message_content;
    std::string sender_IP = sender_endpoint.address().to_string();
    parseMessage(received_message, sender_name, message_content);
    message = Message(message_name, sender_name, sender_IP);
    return;
  }
} 


