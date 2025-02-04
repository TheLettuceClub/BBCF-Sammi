#pragma once
#include <string>
#include "json.hpp"

struct PlayerState {
	unsigned int health{};
	int posx{};
	std::string character{};
	std::string prevAction{};
	std::string currAction{};
	unsigned int heat{};
	int Overdrive{};
	int barrierGauge{};
	int drive{};
	int maxDrive{};
	int side{};
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PlayerState, health, posx, character, prevAction, currAction, heat, Overdrive, barrierGauge, drive, maxDrive, side)

struct StateUpdate {
	PlayerState p1{};
	PlayerState p2{};
	unsigned long int frameCount{};
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(StateUpdate, p1, p2, frameCount)

struct HitEvent {
	std::string attacker{};
	std::string defender{};
	std::string attackerAction{};
	std::string defenderAction{};
	std::string defenderPrevAction{};
	unsigned int attackLevel{};
	int attackFlag{};
	int untechTime{};
	int hitstopOverride{};
	int airPushbackX{};
	int airPushbackY{};
	unsigned int damage{};
	unsigned int scalingTicks{};
	unsigned long int frameCount{};
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(HitEvent, scalingTicks, attacker, defender, attackerAction, defenderAction, defenderPrevAction, attackLevel, attackFlag, untechTime, hitstopOverride, airPushbackX, airPushbackY, damage, frameCount)

struct CreateObject {
	std::string sprite{};
	std::string currAction{};
	unsigned long int frameCount{}; //TODO: owner if possible
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CreateObject, sprite, currAction, frameCount)

struct RoundStart {
	unsigned long int frameCount{}; //TODO: fill in later
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(RoundStart, frameCount)

struct RoundEnd {
	unsigned long int frameCount{}; //TODO: fill in later
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(RoundEnd, frameCount)