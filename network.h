#pragma once

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ip/host_name.hpp>
#include <asio/ip/tcp.hpp>
#include <iostream>
#include <queue>
#include <mutex>
#include "message.h"

#define BROADCAST_PORT 2223
#define BROADCAST_IP "239.255.255.250"
#define RECV_BUFFER_SIZE 8192

class Broadcaster{
	private:
		int _broadcastPort;
	

	public:
		Broadcaster(const int broadcastPort);
		virtual ~Broadcaster();
		void broadcastMessage(const Message message) const;
};

class Receiver{
private:
	int _receivePort;
	std::queue<Message*> _messageQueue;
	std::mutex _messageQueueMutex;

	asio::io_context _io_context;
	asio::ip::udp::socket _socket;

public:
	Receiver(const int receiverPort);
	virtual ~Receiver();

	void ParseMessage(const std::string& udpData, std::string& name, std::string& context);
	void CatchMessage(Message& message);
	void listen();

	Message* getQueueMessage();
};
