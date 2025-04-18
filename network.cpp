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
}
