#include <iostream>
#include <thread>
#include <string>
#include "network.h"

// Function to handle receiving messages in a separate thread
void receiveMessages(Receiver& receiver) {
  // Start listening for incoming messages
  receiver.listen();
  
  // Process messages as they arrive
  while (true) {
    Message* msg = receiver.getQueueMessage();
    if (msg != nullptr) {
      std::cout << *msg << std::endl;
      delete msg;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

int main() {
  std::string username;
  std::cout << "Enter your username: ";
  std::getline(std::cin, username);
  
  Receiver receiver(BROADCAST_PORT);
  Broadcaster broadcaster(BROADCAST_PORT);
  
  // Start message receiving thread
  std::thread receiveThread(receiveMessages, std::ref(receiver));
  receiveThread.detach();  // Detach thread so it runs independently
  
  std::cout << "Chat started. Type your messages and press Enter to send.\n";
  std::cout << "Type 'exit' to quit the program.\n";
  
  std::string userInput;
  while (true) {
    std::getline(std::cin, userInput);
    
    if (userInput == "exit") {
      break;
    }
    
    if (!userInput.empty()) {
      // Create a message object with user input as content
      std::string localIP = asio::ip::host_name();  // Get local hostname as IP
      Message message(userInput, username, localIP);
      broadcaster.broadcastMessage(message);
    }
  }
  
  std::cout << "Exiting chat...\n";
  return 0;
}
