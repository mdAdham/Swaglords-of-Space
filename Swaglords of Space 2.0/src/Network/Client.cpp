#include "Network/NetTypes.hpp"
#include <SFML/Network.hpp>
#include <thread>
#include <atomic>
#include <iostream>

using namespace _Swag::Net;

int main() {
    sf::UdpSocket socket;
    sf::IpAddress server = "127.0.0.1";
    unsigned short port = 7777;
    socket.setBlocking(false);

    sf::Uint32 playerId = 0;
    sf::Uint16 inputSeq = 0;

    std::atomic<bool> running{true};

    std::thread recvThread([&]{
        while (running) {
            sf::Packet packet;
            sf::IpAddress sender;
            unsigned short senderPort;
            auto st = socket.receive(packet, sender, senderPort);
            if (st == sf::Socket::Done) {
                sf::Uint8 mt; packet >> mt;
                if (mt == Msg_Snapshot) {
                    sf::Uint32 tick; sf::Uint16 cnt; packet >> tick >> cnt;
                    for (int i = 0; i < cnt; ++i) {
                        EntityState e; packet >> e;
                        if (playerId == 0) playerId = e.id; // first snapshot becomes our id in this simple demo
                        if (e.id == playerId) {
                            std::cout << "Authoritative pos: " << e.x << "," << e.y << " rot=" << e.rot << " hp=" << (int)e.health << "\n";
                        }
                    }
                }
            } else if (st == sf::Socket::NotReady) {
                sf::sleep(sf::milliseconds(1));
            } else {
                sf::sleep(sf::milliseconds(10));
            }
        }
    });

    // send inputs periodically
    while (true) {
        // read keyboard state using sf::Keyboard only if linked to window; here create simple toggles
        sf::Uint8 controls = 1; // thrust on by default in this demo
        float thrust = 1.0f;
        float aim = 0.0f;

        sf::Packet p;
        p << static_cast<sf::Uint8>(Msg_Input);
        p << playerId << inputSeq++ << static_cast<sf::Uint32>(0) << controls << thrust << aim;
        socket.send(p, server, port);

        sf::sleep(sf::milliseconds(33));
    }

    running = false;
    recvThread.join();
}
