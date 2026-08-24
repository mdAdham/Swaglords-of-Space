#pragma once

#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <vector>

namespace _Swag::Net {

    enum MsgType : sf::Uint8 {
        Msg_Input = 1,
        Msg_Snapshot = 2,
        Msg_Ping = 3
    };

    struct InputPacket {
        sf::Uint32 playerId = 0; // 0 = register request
        sf::Uint16 inputSeq = 0;
        sf::Uint32 clientTick = 0;
        sf::Uint8 controls = 0; // bitmask
        float thrust = 0.f;
        float aim = 0.f;
    };

    struct EntityState {
        sf::Uint32 id = 0;
        float x = 0.f;
        float y = 0.f;
        float rot = 0.f;
        float vx = 0.f;
        float vy = 0.f;
        sf::Uint8 health = 100;
    };

    using Snapshot = std::vector<EntityState>;

    // SFML packet serializers
    inline sf::Packet& operator<<(sf::Packet& p, const InputPacket& ip) {
        p << static_cast<sf::Uint8>(Msg_Input);
        p << ip.playerId;
        p << ip.inputSeq;
        p << ip.clientTick;
        p << ip.controls;
        p << ip.thrust;
        p << ip.aim;
        return p;
    }

    inline sf::Packet& operator>>(sf::Packet& p, InputPacket& ip) {
        sf::Uint8 mt;
        if (!(p >> mt)) return p; // read msg type first
        // mt is consumed by caller usually; but keep it here
        p >> ip.playerId;
        p >> ip.inputSeq;
        p >> ip.clientTick;
        p >> ip.controls;
        p >> ip.thrust;
        p >> ip.aim;
        return p;
    }

    inline sf::Packet& operator<<(sf::Packet& p, const EntityState& e) {
        p << e.id << e.x << e.y << e.rot << e.vx << e.vy << e.health;
        return p;
    }

    inline sf::Packet& operator>>(sf::Packet& p, EntityState& e) {
        p >> e.id >> e.x >> e.y >> e.rot >> e.vx >> e.vy >> e.health;
        return p;
    }

}
