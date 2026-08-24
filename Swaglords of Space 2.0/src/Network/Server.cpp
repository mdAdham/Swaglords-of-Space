#include "Network/NetTypes.hpp"
#include <SFML/Network.hpp>
#include <thread>
#include <atomic>
#include <unordered_map>
#include <mutex>
#include <iostream>

using namespace _Swag::Net;

int main() {
    sf::UdpSocket socket;
    if (socket.bind(7777) != sf::Socket::Done) {
        std::cerr << "Failed to bind UDP socket on 7777\n";
        return -1;
    }
    socket.setBlocking(false);
    std::cout << "Server listening on 7777\n";

    std::mutex mtx;
    std::unordered_map<sf::Uint32, sf::IpAddress> clientAddrs;
    std::unordered_map<sf::Uint32, unsigned short> clientPorts;
    std::unordered_map<sf::Uint32, EntityState> players;
    sf::Uint32 nextId = 1;

    std::atomic<bool> running{true};

    std::thread recvThread([&]{
        while (running) {
            sf::Packet packet;
            sf::IpAddress sender;
            unsigned short port;
            sf::Socket::Status st = socket.receive(packet, sender, port);
            if (st == sf::Socket::Done) {
                sf::Uint8 mt;
                packet >> mt;
                if (mt == Msg_Input) {
                    InputPacket ip;
                    packet >> ip.playerId >> ip.inputSeq >> ip.clientTick >> ip.controls >> ip.thrust >> ip.aim;
                    std::lock_guard<std::mutex> lk(mtx);
                    if (ip.playerId == 0 || players.find(ip.playerId) == players.end()) {
                        // register new client
                        sf::Uint32 id = nextId++;
                        EntityState e; e.id = id; e.x = 0; e.y = 0; e.rot = 0; e.vx = 0; e.vy = 0; e.health = 100;
                        players[id] = e;
                        clientAddrs[id] = sender;
                        clientPorts[id] = port;
                        std::cout << "Registered client " << id << " from " << sender.toString() << ":" << port << "\n";
                        continue;
                    }
                    // apply simple authoritative update
                    auto &p = players[ip.playerId];
                    float accel = (ip.controls & 1) ? ip.thrust : 0.f;
                    float dir = p.rot + ip.aim;
                    p.vx += std::cos(dir) * accel * 0.02f;
                    p.vy += std::sin(dir) * accel * 0.02f;
                    p.x += p.vx;
                    p.y += p.vy;
                    if (ip.controls & 2) p.rot -= 0.05f;
                    if (ip.controls & 4) p.rot += 0.05f;
                }
            } else if (st == sf::Socket::NotReady) {
                sf::sleep(sf::milliseconds(1));
            } else {
                sf::sleep(sf::milliseconds(10));
            }
        }
    });

    // broadcast loop
    const int tickRate = 30;
    const sf::Time tickTime = sf::milliseconds(1000 / tickRate);
    while (true) {
        sf::Clock c;
        Snapshot snap;
        {
            std::lock_guard<std::mutex> lk(mtx);
            for (auto &kv : players) snap.push_back(kv.second);
        }
        // build packet
        sf::Packet out;
        out << static_cast<sf::Uint8>(Msg_Snapshot);
        out << static_cast<sf::Uint32>(c.getElapsedTime().asMilliseconds());
        out << static_cast<sf::Uint16>(snap.size());
        for (auto &e : snap) out << e;
        // send to all
        {
            std::lock_guard<std::mutex> lk(mtx);
            for (auto &kv : clientAddrs) {
                socket.send(out, kv.second, clientPorts[kv.first]);
            }
        }

        sf::sleep(tickTime - c.getElapsedTime());
    }

    running = false;
    recvThread.join();
    return 0;
}
