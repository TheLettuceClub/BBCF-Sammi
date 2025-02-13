#pragma once
#include "struct_util.h"

struct BATTLE_CObjectManager {
	FIELD(0x30, int, side)
	FIELD(0x1A0, int, oldCharDrive1) //Bang's first seal and robot swords??
	FIELD(0x1A4, int, oldCharDrive2) //Bang's second seal
	FIELD(0x1A8, int, oldCharDrive3) //bang's third seal
	FIELD(0x1AC, int, oldCharDrive4) //bang's fourth seal and maybe some other stuff
	FIELD(0x1C0, int, overdriveTimeleft)
	FIELD(0x1F0, BATTLE_CObjectManager*, enemy) // actually a ptr to same struct but other player
	FIELD(0x254, short, stateFlag)
	FIELD(0x256, short, stateFlag2)
	FIELD(0x25C, short, stateFlag3)
	FIELD(0x25E, short, stateFlag4)
	FIELD(0x26C, int, posy)
	FIELD(0x488, int, moveType)
	FIELD(0x48C, int, attackLevel)
	FIELD(0x490, int, damageUnscaledNot5A)
	FIELD(0x4A0, int, attackFlag)
	FIELD(0x4B0, int, hitstopOverride) // usually -1
	FIELD(0x668, int, airPushbackY)
	FIELD(0x684, int, untechTime)
	FIELD(0x694, int, airPushbackX)
	FIELD(0x9D0, int, prevHealth)
	FIELD(0x9D4, int, health)
	FIELD(0xB28, int, posx)
	FIELD(0x1E9C0, int, installDriveUnlocked) // susan's unlocked seals
	FIELD(0x1E9C4, int, installDriveSelected) // which seal susan is selecting on the HUD
	ARRAY_FIELD(0x1338, char[16], sprite)
	ARRAY_FIELD(0x1FF4, char[32], prevAction)
	ARRAY_FIELD(0x2014, char[32], currAction)
	FIELD(0x5770, int, incomingScalingTicks)
	FIELD(0x5774, int, incomingComboCount) // seemingly always same value as above. 0 on block
	FIELD(0x5780, int, incomingDamage)
	FIELD(0x5AD4, int, heat)
	FIELD(0x5AE4, int, OverdriveAmnt)
	FIELD(0x5B04, int, barrierGauge)
	FIELD(0x2005C, int, activeFlow)
	FIELD(0x20108, int, maxDrive) //if a char has a timer on any move, this is the max value. ex: bullet drive, robo grav seed
	FIELD(0x2010C, int, drive) //if a char has a timer, this is the current value of it. ex: bullet drive, robo grav seed
};

//TODO: expand with offsets from here: https://github.com/libreofficecalc/BBCF-Improvement-Mod/blob/master/src/Game/CharData.h (make sure to test!)
