#pragma once
#include "struct_util.h"

struct Battle_CObject {
	FIELD(0x48C, int, attackLevel);
	FIELD(0x490, int, damageUnscaledNot5A);
	FIELD(0x4A0, int, attackFlag);
	FIELD(0x684, int, untechTime);
	FIELD(0x4B0, int, hitstopOverride); // usually -1
	FIELD(0x668, int, airPushbackY);
	FIELD(0x694, int, airPushbackX);
	FIELD(0x9D4, int, health);
	ARRAY_FIELD(0x1338, char[16], sprite);
	ARRAY_FIELD(0x1FF4, char[32], prevAction);
	ARRAY_FIELD(0x2014, char[32], currAction);
	FIELD(0x5AD4, int, heat);
	FIELD(0x5AE4, int, OverdriveAmnt);
	FIELD(0x5B04, int, barrierGauge);
	FIELD(0x2010C, int, drive);
	FIELD(0x20108, int, maxDrive);
	FIELD(0x2A110, int, damageScaled);
};