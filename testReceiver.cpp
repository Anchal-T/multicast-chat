#include <iostream>
#define ASIO_STANDALONE
#include "asio.hpp"

int main() {
    try {
        asio::io_context io_context;
        asio::ip::udp::endpoint listen_endpoint(asio::ip::address_v4::any(), 2223);
        asio::ip::udp::socket socket(io_context);

        socket.open(listen_endpoint.protocol());
        socket.set_option(asio::ip::udp::socket::reuse_address(true));
        socket.bind(listen_endpoint);
        
        asio::ip::address multicast_address = asio::ip::make_address("239.255.255.250");
        
        // *** ADD THIS LINE: Replace "YOUR_ETH0_IP_ADDRESS" with the actual IP of eth0 ***
        asio::ip::address_v4 local_interface = asio::ip::make_address_v4("172.22.50.49"); 
        
        // Modify this line to specify the interface
        socket.set_option(asio::ip::multicast::join_group(multicast_address));
        
        socket.set_option(asio::ip::multicast::enable_loopback(true)); // for local testing

        std::cout << "Listening for multicast messages on 239.255.255.250:2223 via interface " << local_interface.to_string() << "\n";
        char data[1024];
        while (true) {
            asio::ip::udp::endpoint sender_endpoint;
            size_t length = socket.receive_from(asio::buffer(data), sender_endpoint);
            std::cout << "Received: " << std::string(data, length) << std::endl;
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}