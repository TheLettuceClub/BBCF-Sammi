#pragma once
#include <string>
#include "json.hpp"

enum MatchState
{
	MatchState_NotStarted = 0,
	MatchState_RebelActionRoundSign = 2,
	MatchState_Fight = 3,
	MatchState_FinishSign = 4,
	MatchState_WinLoseSign = 5,
	MatchState_VictoryScreen = 7
};

enum GameState
{
	GameState_ArcsysLogo = 2,
	GameState_IntroVideoPlaying = 3,
	GameState_TitleScreen = 4,
	GameState_CharacterSelectionScreen = 6,
	GameState_ArcadeActSelectScreen = 11,
	GameState_ScoreAttackModeSelectScreen = 11,
	GameState_SpeedStarModeSelectScreen = 11,
	GameState_ArcadeCharInfoScreen = 12,
	GameState_ArcadeStageSelectScreen = 13,
	GameState_VersusScreen = 14,
	GameState_InMatch = 15,
	GameState_VictoryScreen = 16,
	GameState_StoryMenu = 24,
	GameState_GalleryMenu = 25,
	GameState_ItemMenu = 25,
	GameState_ReplayMenu = 26,
	GameState_MainMenu = 27,
	GameState_TutorialMenu = 28,
	GameState_LibraryMenu = 28,
	GameState_Lobby = 31,
	GameState_StoryPlaying = 33,
	GameState_AbyssMenu = 34,
	GameState_DCodeEdit = 39,
};

enum GameMode
{
	GameMode_Arcade = 1,
	GameMode_Story = 4,
	GameMode_Versus = 5,
	GameMode_Training = 6,
	GameMode_Tutorial = 7,
	GameMode_Challenge = 8,
	GameMode_Gallery = 9,
	GameMode_ItemShop = 10,
	GameMode_ReplayTheater = 11,
	GameMode_TitleScreen = 12,
	GameMode_MainMenuScreen = 13,
	GameMode_Online = 15,
	GameMode_Abyss = 16,
	GameMode_DCodeEdit = 18,
};

struct PlayerState {
	unsigned int health{};
	int posx{};
	int posy{};
	std::string character{};
	std::string prevAction{};
	std::string currAction{};
	unsigned int heat{};
	int Overdrive{};
	int barrierGauge{};
	int drive{};
	int maxDrive{};
	int side{};
	int ODTimeRemaining{};
	int moveType{};
	int activeFlow{};
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PlayerState, activeFlow, moveType, posy, ODTimeRemaining, health, posx, character, prevAction, currAction, heat, Overdrive, barrierGauge, drive, maxDrive, side)

struct StateUpdate {
	PlayerState p1{};
	PlayerState p2{};
	int inGameTimer{};
	int matchState{}; // TODO: enum these later
	int gameMode{};
	int gameState{};
	unsigned long int frameCount{};
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(StateUpdate, matchState, inGameTimer, gameMode, gameState, p1, p2, frameCount)

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
	int moveType{};
	unsigned int damage{};
	unsigned int scalingTicks{};
	unsigned int comboCount{};
	int attackerActiveFlow{};
	int defenderActiveFlow{};
	unsigned long int frameCount{};
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(HitEvent, attackerActiveFlow, defenderActiveFlow, moveType, comboCount, scalingTicks, attacker, defender, attackerAction, defenderAction, defenderPrevAction, attackLevel, attackFlag, untechTime, hitstopOverride, airPushbackX, airPushbackY, damage, frameCount)

struct GuardEvent {
	std::string attacker{};
	std::string defender{};
	std::string attackerAction{};
	std::string defenderAction{};
	unsigned int attackLevel{};
	std::string blockDir{};
	std::string blockMethod{};
	unsigned int chipDamage{};
	int moveType{};
	int attackerActiveFlow{};
	int defenderActiveFlow{};
	unsigned long int frameCount{};
};

std::string getBlockDir(short, short);
std::string getBlockMeth(short);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GuardEvent, attackerActiveFlow, defenderActiveFlow, moveType, chipDamage, attacker, defender, attackerAction, defenderAction, attackLevel, blockDir, blockMethod, frameCount)

struct CreateObject {
	std::string sprite{};
	std::string currAction{};
	unsigned long int frameCount{}; //TODO: owner if possible
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CreateObject, sprite, currAction, frameCount)

struct RoundTransition {
	std::string likelyNext{};
	std::string p1Act{};
	std::string p2Act{};
	int p1Health{};
	int p2Health{};
	unsigned long int frameCount{};
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(RoundTransition, likelyNext, p1Act, p2Act, p1Health, p2Health, frameCount)