#pragma once
#include "struct_util.h"

struct BATTLE_CObjectManager {
	FIELD(0x30, int, side)
	FIELD(0x1C0, int, overdriveTimeleft)
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
	ARRAY_FIELD(0x1338, char[16], sprite)
	ARRAY_FIELD(0x1FF4, char[32], prevAction)
	ARRAY_FIELD(0x2014, char[32], currAction)
	FIELD(0x5770, int, incomingScalingTicks)
	FIELD(0x5774, int, incomingComboCount) // seemingly always same value as above. 0 on block
	FIELD(0x5780, int, incomingDamage)
	FIELD(0x5AD4, int, heat)
	FIELD(0x5AE4, int, OverdriveAmnt)
	FIELD(0x5B04, int, barrierGauge)
	FIELD(0x20108, int, maxDrive)
	FIELD(0x2010C, int, drive)
};

//class BATTLE_CObjectManager //from cfim, causes crashes!
//{
//public:
//	class OBJ_CCharBase* objCharbase; //0x0000
//	char pad_0004[12]; //0x0004
//	int32_t unknownStatus1; //0x0010
//	char pad_0014[4]; //0x0014
//	int32_t stateChangedCount; //0x0018
//	char pad_001C[20]; //0x001C
//	int32_t side; //0x30
//	int32_t charIndex; //0x0034
//	char pad_0038[20]; //0x0038
//	class JonbEntry* pJonbEntryBegin; //0x004C
//	char pad_0050[68]; //0x0050
//	uint32_t hurtboxCount; //0x0094
//	uint32_t hitboxCount; //0x0098
//	char pad_009C[60]; //0x009C
//	char* current_sprite_img; //0x00D8
//	char pad_00DC[208]; //0x00DC
//	int32_t lambda_nu_drive_hitcount; //0x01AC
//	char pad_01B0[16]; //0x01B0
//	int32_t overdriveTimeleft; //0x01C0
//	int32_t overdriveTimerStartedAt; //0x01C4
//	char pad_01C8[20]; //0x01C8
//	int32_t some_distortion_timer; //0x01DC
//	int32_t isDoingDistortion; //0x01E0
//	char pad_01E4[4]; //0x01E4
//	class BATTLE_CObjectManager* ownerEntity; //0x01E8
//	char pad_01EC[104]; //0x01EC
//	int16_t stateFlag; //0x254
//	int16_t stateFlag2; //0x256
//	int32_t pad_0258;
//	int16_t stateFlag3; //0x25C
//	int16_t stateFlag4; //0x25E
//	char pad_0260[4];
//	int32_t facingLeft; //0x0264
//	int32_t position_x; //0x0268
//	int32_t position_y; //0x026C
//	char pad_0270[4]; //0x0270
//	int32_t offsetX_1; //0x0274
//	char pad_0278[4]; //0x0278
//	int32_t rotationDegrees; //0x027C
//	char pad_0280[4]; //0x0280
//	int32_t scaleX; //0x0284
//	int32_t scaleY; //0x0288
//	char pad_028C[80]; //0x028C
//	int32_t position_x_dupe; //0x02DC
//	int32_t position_y_dupe; //0x02E0
//	char pad_02E4[16]; //0x02E4
//	int32_t offsetX_2; //0x02F4
//	char pad_02F8[4]; //0x02F8
//	int32_t offsetY_2; //0x02FC
//	char pad_0300[52]; //0x0300
//	int32_t stageEdgeTouchTimer; //0x0334
//	char pad_0338[336]; //0x0338
//	int32_t heatUsageType; //0x0488. more like "move type", 1= normal, 2=special, 3=super
//	int32_t attackLevel; //0x048C
//	int32_t damageUnscaledNot5A; //0x490
//	char pad_0494[12]; //0x0494
//	int32_t attackFlag; //0x4a0
//	char pad_04a4[12];
//	int32_t hitstopOverride; //0x4b0
//	char pad_04B4[436]; //0x04b4
//	int32_t airPushbackY; //0x668
//	char pad_066C[24];
//	int32_t untechTime; //0x684
//	char pad_0688[12];
//	int32_t airPushbackX; //0x694
//	char pad_0698[824];
//	int32_t previousHP; //0x09D0
//	int32_t currentHP; //0x09D4
//	int32_t maxHP; //0x09D8
//	char pad_09DC[2396]; //0x09DC
//	char sprite[16]; //0x1338
//	char pad_1348[3244];
//	char lastAction[32]; //0x1FF4
//	char currAction[32]; //0x2014
//	char pad_2034[532];
//	char char_abbr[4]; //0x2248 //BROKEN!!!!!!
//	char pad_224C[13604]; //0x224C
//	int32_t incomingScalingTicks; //0x5770
//	int32_t incomingComboCount; //0x5774
//	char pad_5778[8];
//	int32_t incomingDamage; //0x5780
//	char pad_5784[848];
//	int32_t heatMeter; //0x5AD4
//	char pad_5AD8[4]; //0x5AD8
//	int32_t heatGainCooldown; //0x5ADC
//	char pad_5AE0[4]; //0x5AE0
//	int32_t overdriveMeter; //0x5AE4
//	char pad_5AE8[16]; //0x5AE8
//	int32_t overdriveMaxtime; //0x5AF8
//	char pad_5AFC[8]; //0x5AFC
//	int32_t barrier; //0x5B04
//	char pad_5B08[108032]; //0x5B08
//	int32_t maxDrive; //0x20108
//	int32_t Drive1; //0x2010C
//	char pad_20110[12]; //0x20110
//	int32_t Drive1_type; //0x2011C
//	char pad_20120[16]; //0x20120
//	int32_t Drive2; //0x20130
//	char pad_20134[32]; //0x20134
//	int32_t Drive3; //0x20154
//	char pad_20158[4972]; //0x20158
//}; //Size: 0x214C4
//
//static_assert(sizeof(BATTLE_CObjectManager) == 0x214C4);