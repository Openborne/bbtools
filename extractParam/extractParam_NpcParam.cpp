// extact NpcParam.param file based on info from NpcParam.paramdef
// albeartron


//#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <inttypes.h>
#include <string>

#define VERSION "Rev0-1"

#pragma pack(1)	//bytewise
struct param_header {
	/* 0x0000 */	std::uint32_t	infoStartAddr;	//where the text information list starts
	/* 0x0004 */	std::uint16_t	unknown01;	//pad? always zeros
	/* 0x0006 */	std::uint16_t	unknown02;	//low value
	/* 0x0008 */	std::uint16_t	unknown03;	//low value
	/* 0x000A */	std::uint16_t	numEntries;	//number of param entries
	/* 0x000C */	char	title[32];	//null terminated string
	/* 0x002C */	std::uint16_t	unknown04;	//always 0x0400
	/* 0x002E */	std::uint16_t	unknown05;	//always 0x0007 or 0x0006
	/* 0x0030 */	std::uint64_t	paramStartAddr;	//where the param data starts
	/* 0x0038 */	std::uint64_t	pad01;

	/* 0x0040 */	//std::uint64_t firstId;	//probably the first entry's ID number
	/* 0x0048 */	//std::uint64_t firstParamAddr;	//probably the first entry's param addr
	/* 0x0050 */	//std::uint64_t firstInfoAddr;	//probably the first entry's info addr
};

struct param_id_chunk {
	std::uint64_t	entryId;	//entry's ID number
	std::uint64_t	paramAddr;	//entry's param addr
	std::uint64_t	infoAddr;	//entry's info addr
};

struct param_entry {
	//REPLACE THIS ENTIRE STRUCTURE WITH INFO FROM ANOTHER PARAMDEF
	std::int32_t behaviorVariationId;
	std::int32_t aiThinkId;
	std::int32_t nameId;
	float turnVellocity;
	float hitHeight;
	float hitRadius;
	std::uint32_t weight;
	float hitYOffset;
	std::uint32_t hp;
	std::uint32_t mp;
	std::uint32_t getSoul;
	std::int32_t itemLotId_1;
	std::int32_t itemLotId_2;
	std::int32_t itemLotId_3;
	std::int32_t itemLotId_4;
	std::int32_t itemLotId_5;
	std::int32_t itemLotId_6;
	std::int32_t humanityLotId;
	std::int32_t spEffectID0;
	std::int32_t spEffectID1;
	std::int32_t spEffectID2;
	std::int32_t spEffectID3;
	std::int32_t spEffectID4;
	std::int32_t spEffectID5;
	std::int32_t spEffectID6;
	std::int32_t spEffectID7;
	std::int32_t GameClearSpEffectID;
	float physGuardCutRate;
	float magGuardCutRate;
	float fireGuardCutRate;
	float thunGuardCutRate;
	std::int32_t animIdOffset;
	std::int32_t moveAnimId;
	std::int32_t spMoveAnimId1;
	std::int32_t spMoveAnimId2;
	float networkWarpDist;
	std::int32_t dbgBehaviorR1;
	std::int32_t dbgBehaviorL1;
	std::int32_t dbgBehaviorR2;
	std::int32_t dbgBehaviorL2;
	std::int32_t dbgBehaviorRL;
	std::int32_t dbgBehaviorRR;
	std::int32_t dbgBehaviorRD;
	std::int32_t dbgBehaviorRU;
	std::int32_t dbgBehaviorLL;
	std::int32_t dbgBehaviorLR;
	std::int32_t dbgBehaviorLD;
	std::int32_t dbgBehaviorLU;
	std::int32_t animIdOffset2;
	float partsDamageRate1;
	float partsDamageRate2;
	float partsDamageRate3;
	float partsDamageRate4;
	float partsDamageRate5;
	float partsDamageRate6;
	float partsDamageRate7;
	float partsDamageRate8;
	float weakPartsDamageRate;
	float superArmorRecoverCorrection;
	float superArmorBrakeKnockbackDist;
	std::uint16_t stamina;
	std::uint16_t staminaRecoverBaseVel;
	std::uint16_t def_phys;
	std::int16_t def_slash;
	std::int16_t def_blow;
	std::int16_t def_thrust;
	std::uint16_t def_mag;
	std::uint16_t def_fire;
	std::uint16_t def_thunder;
	std::uint16_t defFlickPower;
	std::uint16_t resist_poison;
	std::uint16_t resist_desease;
	std::uint16_t resist_blood;
	std::uint16_t resist_curse;
	std::int16_t ghostModelId;
	std::int16_t normalChangeResouceId;
	std::int16_t guardAngle;
	std::int16_t slashGuardCutRate;
	std::int16_t blowGuardCutRate;
	std::int16_t thrustGuardCutRate;
	std::int16_t superArmorDurability;
	std::int16_t normalChangeTexChrId;
	std::uint16_t dropType;
	std::uint8_t knockbackRate;
	std::uint8_t knockbackParamId;
	std::uint8_t fallDamageDump;
	std::uint8_t staminaGuardDef;
	std::uint8_t pcAttrB;
	std::uint8_t pcAttrW;
	std::uint8_t pcAttrL;
	std::uint8_t pcAttrR;
	std::uint8_t areaAttrB;
	std::uint8_t areaAttrW;
	std::uint8_t areaAttrL;
	std::uint8_t areaAttrR;
	std::uint8_t mpRecoverBaseVel;
	std::uint8_t flickDamageCutRate;
	std::int8_t defaultLodParamId;
	std::uint8_t drawType;
	std::uint8_t npcType;
	std::uint8_t teamType;
	std::uint8_t moveType;
	std::uint8_t lockDist;
	std::uint8_t material;
	std::uint8_t materialSfx;
	std::uint8_t material_Weak;
	std::uint8_t materialSfx_Weak;
	std::uint8_t partsDamageType;
	std::uint8_t maxUndurationAng;
	std::int8_t guardLevel;
	std::uint8_t burnSfxType;
	std::int8_t poisonGuardResist;
	std::int8_t diseaseGuardResist;
	std::int8_t bloodGuardResist;
	std::int8_t curseGuardResist;
	std::uint8_t parryAttack;
	std::uint8_t parryDefence;
	std::uint8_t sfxSize;
	std::uint8_t pushOutCamRegionRadius;
	std::uint8_t hitStopType;
	std::uint8_t ladderEndChkOffsetTop;
	std::uint8_t ladderEndChkOffsetLow;
	std::uint8_t useRagdollCamHit : 1;
	std::uint8_t disableClothRigidHit : 1;
	std::uint8_t useRagdoll : 1;
	std::uint8_t isDemon : 1;
	std::uint8_t isGhost : 1;
	std::uint8_t isNoDamageMotion : 1;
	std::uint8_t isUnduration : 1;
	std::uint8_t isChangeWanderGhost : 1;
	std::uint8_t modelDispMask0 : 1;
	std::uint8_t modelDispMask1 : 1;
	std::uint8_t modelDispMask2 : 1;
	std::uint8_t modelDispMask3 : 1;
	std::uint8_t modelDispMask4 : 1;
	std::uint8_t modelDispMask5 : 1;
	std::uint8_t modelDispMask6 : 1;
	std::uint8_t modelDispMask7 : 1;
	std::uint8_t modelDispMask8 : 1;
	std::uint8_t modelDispMask9 : 1;
	std::uint8_t modelDispMask10 : 1;
	std::uint8_t modelDispMask11 : 1;
	std::uint8_t modelDispMask12 : 1;
	std::uint8_t modelDispMask13 : 1;
	std::uint8_t modelDispMask14 : 1;
	std::uint8_t modelDispMask15 : 1;
	std::uint8_t isEnableNeckTurn : 1;
	std::uint8_t disableRespawn : 1;
	std::uint8_t isMoveAnimWait : 1;
	std::uint8_t isCrowd : 1;
	std::uint8_t isWeakSaint : 1;
	std::uint8_t isWeakA : 1;
	std::uint8_t isWeakB : 1;
	std::uint8_t isEnableDropSoulCapture : 1;
	std::uint8_t vowType : 3;
	std::uint8_t disableInitializeDead : 1;
	std::uint8_t isSoulGetByBoss : 1;
	std::uint8_t isMultilingual : 1;
	std::uint8_t pad3 : 2;
	std::uint8_t modelDispMask16 : 1;
	std::uint8_t modelDispMask17 : 1;
	std::uint8_t modelDispMask18 : 1;
	std::uint8_t modelDispMask19 : 1;
	std::uint8_t modelDispMask20 : 1;
	std::uint8_t modelDispMask21 : 1;
	std::uint8_t modelDispMask22 : 1;
	std::uint8_t modelDispMask23 : 1;
	std::uint8_t modelDispMask24 : 1;
	std::uint8_t modelDispMask25 : 1;
	std::uint8_t modelDispMask26 : 1;
	std::uint8_t modelDispMask27 : 1;
	std::uint8_t modelDispMask28 : 1;
	std::uint8_t modelDispMask29 : 1;
	std::uint8_t modelDispMask30 : 1;
	std::uint8_t modelDispMask31 : 1;
	float itemSearchRadius;
	std::int16_t normalChangeModelId;
	std::int8_t clothUpdateOffset;
	std::uint8_t pad4;
	float subHitOffset;
	float subHitHeight;
	float subHitRadius;
	std::uint16_t paintRenderTargetSize;
	std::int16_t normalChangeAnimChrId;
	float regainRate_PhysNeutral;
	float regainRate_PhysSlash;
	float regainRate_PhysBlow;
	float regainRate_PhysThrust;
	float regainRate_Magic;
	float regainRate_Fire;
	float regainRate_Thunder;
	float maxAnkleAngle;
	//END OF PARAMDEF SPECIFIC INFO
};
#pragma pack()

static void show_usage(std::string);

int main(int argc, char *argv[]) {
	//check if enough command line parameters
	if (argc < 2) {
		show_usage(argv[0]);
		return 1;
	}

	char* filenametemp = argv[1];
	std::ifstream inputFile(argv[1], std::ios::binary);	//open the param file as binary

	param_header	paramHeaderData;
	param_id_chunk	paramIdChunk;
	param_entry		paramEntryData;

	std::uint64_t	bytes8;
	std::uint32_t	bytes4;
	std::uint16_t	bytes2;
	std::uint8_t	bytes1;

	std::uint64_t	entryLocation;
	std::uint64_t	infoLocation;
	std::uint64_t	entryID;

	std::size_t returnAddress;

	const int MAX_BUFFSIZE = 512;
	char stringBuffer[MAX_BUFFSIZE];
	char buffer[MAX_BUFFSIZE] = {};
	std::uint16_t unicodeBuffer[MAX_BUFFSIZE];

	inputFile.read((char *)&paramHeaderData, sizeof(paramHeaderData));	//read header data

	//print header info
	/*
	std::printf("infoStartAddr:\t0x%04X\n", paramHeaderData.infoStartAddr);
	std::printf("numEntries:\t0x%02X\t%d\n", paramHeaderData.numEntries, paramHeaderData.numEntries);
	std::printf("title:\t%s\n", paramHeaderData.title);
	std::printf("paramStartAddr:\t0x%08X\n", paramHeaderData.paramStartAddr);
	*/

	std::printf(
		"entryID   \t"
		"paramAddr \t"
		"infoAddr  \t"
	);
	std::printf(
		//REPLACE WITH PARAMDEF SPECIFIC INFO
		"behaviorVariationId\t"
		"aiThinkId\t"
		"nameId\t"
		"turnVellocity\t"
		"hitHeight\t"
		"hitRadius\t"
		"weight\t"
		"hitYOffset\t"
		"hp\t"
		"mp\t"
		"getSoul\t"
		"itemLotId_1\t"
		"itemLotId_2\t"
		"itemLotId_3\t"
		"itemLotId_4\t"
		"itemLotId_5\t"
		"itemLotId_6\t"
		"humanityLotId\t"
		"spEffectID0\t"
		"spEffectID1\t"
		"spEffectID2\t"
		"spEffectID3\t"
		"spEffectID4\t"
		"spEffectID5\t"
		"spEffectID6\t"
		"spEffectID7\t"
		"GameClearSpEffectID\t"
		"physGuardCutRate\t"
		"magGuardCutRate\t"
		"fireGuardCutRate\t"
		"thunGuardCutRate\t"
		"animIdOffset\t"
		"moveAnimId\t"
		"spMoveAnimId1\t"
		"spMoveAnimId2\t"
		"networkWarpDist\t"
		"dbgBehaviorR1\t"
		"dbgBehaviorL1\t"
		"dbgBehaviorR2\t"
		"dbgBehaviorL2\t"
		"dbgBehaviorRL\t"
		"dbgBehaviorRR\t"
		"dbgBehaviorRD\t"
		"dbgBehaviorRU\t"
		"dbgBehaviorLL\t"
		"dbgBehaviorLR\t"
		"dbgBehaviorLD\t"
		"dbgBehaviorLU\t"
		"animIdOffset2\t"
		"partsDamageRate1\t"
		"partsDamageRate2\t"
		"partsDamageRate3\t"
		"partsDamageRate4\t"
		"partsDamageRate5\t"
		"partsDamageRate6\t"
		"partsDamageRate7\t"
		"partsDamageRate8\t"
		"weakPartsDamageRate\t"
		"superArmorRecoverCorrection\t"
		"superArmorBrakeKnockbackDist\t"
		"stamina\t"
		"staminaRecoverBaseVel\t"
		"def_phys\t"
		"def_slash\t"
		"def_blow\t"
		"def_thrust\t"
		"def_mag\t"
		"def_fire\t"
		"def_thunder\t"
		"defFlickPower\t"
		"resist_poison\t"
		"resist_desease\t"
		"resist_blood\t"
		"resist_curse\t"
		"ghostModelId\t"
		"normalChangeResouceId\t"
		"guardAngle\t"
		"slashGuardCutRate\t"
		"blowGuardCutRate\t"
		"thrustGuardCutRate\t"
		"superArmorDurability\t"
		"normalChangeTexChrId\t"
		"dropType\t"
		"knockbackRate\t"
		"knockbackParamId\t"
		"fallDamageDump\t"
		"staminaGuardDef\t"
		"pcAttrB\t"
		"pcAttrW\t"
		"pcAttrL\t"
		"pcAttrR\t"
		"areaAttrB\t"
		"areaAttrW\t"
		"areaAttrL\t"
		"areaAttrR\t"
		"mpRecoverBaseVel\t"
		"flickDamageCutRate\t"
		"defaultLodParamId\t"
		"drawType\t"
		"npcType\t"
		"teamType\t"
		"moveType\t"
		"lockDist\t"
		"material\t"
		"materialSfx\t"
		"material_Weak\t"
		"materialSfx_Weak\t"
		"partsDamageType\t"
		"maxUndurationAng\t"
		"guardLevel\t"
		"burnSfxType\t"
		"poisonGuardResist\t"
		"diseaseGuardResist\t"
		"bloodGuardResist\t"
		"curseGuardResist\t"
		"parryAttack\t"
		"parryDefence\t"
		"sfxSize\t"
		"pushOutCamRegionRadius\t"
		"hitStopType\t"
		"ladderEndChkOffsetTop\t"
		"ladderEndChkOffsetLow\t"
		"useRagdollCamHit:1\t"
		"disableClothRigidHit:1\t"
		"useRagdoll:1\t"
		"isDemon:1\t"
		"isGhost:1\t"
		"isNoDamageMotion:1\t"
		"isUnduration:1\t"
		"isChangeWanderGhost:1\t"
		"modelDispMask0:1\t"
		"modelDispMask1:1\t"
		"modelDispMask2:1\t"
		"modelDispMask3:1\t"
		"modelDispMask4:1\t"
		"modelDispMask5:1\t"
		"modelDispMask6:1\t"
		"modelDispMask7:1\t"
		"modelDispMask8:1\t"
		"modelDispMask9:1\t"
		"modelDispMask10:1\t"
		"modelDispMask11:1\t"
		"modelDispMask12:1\t"
		"modelDispMask13:1\t"
		"modelDispMask14:1\t"
		"modelDispMask15:1\t"
		"isEnableNeckTurn:1\t"
		"disableRespawn:1\t"
		"isMoveAnimWait:1\t"
		"isCrowd:1\t"
		"isWeakSaint:1\t"
		"isWeakA:1\t"
		"isWeakB:1\t"
		"isEnableDropSoulCapture:1\t"
		"vowType:3\t"
		"disableInitializeDead:1\t"
		"isSoulGetByBoss:1\t"
		"isMultilingual:1\t"
		"pad3:2\t"
		"modelDispMask16:1\t"
		"modelDispMask17:1\t"
		"modelDispMask18:1\t"
		"modelDispMask19:1\t"
		"modelDispMask20:1\t"
		"modelDispMask21:1\t"
		"modelDispMask22:1\t"
		"modelDispMask23:1\t"
		"modelDispMask24:1\t"
		"modelDispMask25:1\t"
		"modelDispMask26:1\t"
		"modelDispMask27:1\t"
		"modelDispMask28:1\t"
		"modelDispMask29:1\t"
		"modelDispMask30:1\t"
		"modelDispMask31:1\t"
		"itemSearchRadius\t"
		"normalChangeModelId\t"
		"clothUpdateOffset\t"
		"pad4\t"
		"subHitOffset\t"
		"subHitHeight\t"
		"subHitRadius\t"
		"paintRenderTargetSize\t"
		"normalChangeAnimChrId\t"
		"regainRate_PhysNeutral\t"
		"regainRate_PhysSlash\t"
		"regainRate_PhysBlow\t"
		"regainRate_PhysThrust\t"
		"regainRate_Magic\t"
		"regainRate_Fire\t"
		"regainRate_Thunder\t"
		"maxAnkleAngle\n"
		//END REPLACE WITH PARAMDEF SPECIFIC INFO
	);

	//do the next steps paramHeaderData.num_entries -1 times:
	for (size_t i = 0; i < paramHeaderData.numEntries - 1; i++) {
		//read the param entry chunk
		inputFile.read((char *)&paramIdChunk, sizeof(paramIdChunk));
		entryLocation = paramIdChunk.paramAddr;
		infoLocation = paramIdChunk.infoAddr;
		entryID = paramIdChunk.entryId;

		std::printf("0x%08X\t", paramIdChunk.entryId);		//id
		std::printf("0x%08X\t", paramIdChunk.paramAddr);	//param addr
		std::printf("0x%08X\t", paramIdChunk.infoAddr);		//info addr

		//get entry address
		//inputFile.read((char *)&entryLocation, sizeof(entryLocation));
		//get info address
		//inputFile.read((char *)&infoLocation, sizeof(infoLocation));
		//get entry ID
		//inputFile.read((char *)&entryID, sizeof(entryID));
		//get the current position in the file
		returnAddress = inputFile.tellg();
		//go to the param entry and read it
		inputFile.seekg(paramIdChunk.paramAddr);
		inputFile.read((char *)&paramEntryData, sizeof(paramEntryData));	//this is blank for now, data dictated by paramdef file
		//go to the info address and read until null character
		inputFile.seekg(paramIdChunk.infoAddr);
		//inputFile.getline(stringBuffer, MAX_BUFFSIZE, );	//doesn't work because unicode D:

		size_t noteLength;
		for (size_t j = 0; j < MAX_BUFFSIZE; j++) {
			inputFile.read((char *)&unicodeBuffer[j], sizeof(unicodeBuffer[j]));	//read one unicode character
			if ((unicodeBuffer[j] == 0x0000))	//if the current character = unicode null...
			{
				noteLength = j;		//remember the length of this string
				j = MAX_BUFFSIZE;	//breaks loop if last character was unicode null
			}
		}
		//return to the previous spot for the next read
		inputFile.seekg(returnAddress);
		
		//print the info text
		/* hide for now
		for (size_t k = 0; k < noteLength; k++)
		{
			std::printf(" %04x", unicodeBuffer[k]);
		}
		*/

		//now print the param info (last)
		//REPLACE WITH PARAMDEF SPECIFIC INFO TO PRINT PARAM VALUES
		std::printf("%d\t", paramEntryData.behaviorVariationId);
		std::printf("%d\t", paramEntryData.aiThinkId);
		std::printf("%d\t", paramEntryData.nameId);
		std::printf("%f\t", paramEntryData.turnVellocity);
		std::printf("%f\t", paramEntryData.hitHeight);
		std::printf("%f\t", paramEntryData.hitRadius);
		std::printf("%d\t", paramEntryData.weight);
		std::printf("%f\t", paramEntryData.hitYOffset);
		std::printf("%d\t", paramEntryData.hp);
		std::printf("%d\t", paramEntryData.mp);
		std::printf("%d\t", paramEntryData.getSoul);
		std::printf("%d\t", paramEntryData.itemLotId_1);
		std::printf("%d\t", paramEntryData.itemLotId_2);
		std::printf("%d\t", paramEntryData.itemLotId_3);
		std::printf("%d\t", paramEntryData.itemLotId_4);
		std::printf("%d\t", paramEntryData.itemLotId_5);
		std::printf("%d\t", paramEntryData.itemLotId_6);
		std::printf("%d\t", paramEntryData.humanityLotId);
		std::printf("%d\t", paramEntryData.spEffectID0);
		std::printf("%d\t", paramEntryData.spEffectID1);
		std::printf("%d\t", paramEntryData.spEffectID2);
		std::printf("%d\t", paramEntryData.spEffectID3);
		std::printf("%d\t", paramEntryData.spEffectID4);
		std::printf("%d\t", paramEntryData.spEffectID5);
		std::printf("%d\t", paramEntryData.spEffectID6);
		std::printf("%d\t", paramEntryData.spEffectID7);
		std::printf("%d\t", paramEntryData.GameClearSpEffectID);
		std::printf("%0.1f\t", paramEntryData.physGuardCutRate);
		std::printf("%0.1f\t", paramEntryData.magGuardCutRate);
		std::printf("%0.1f\t", paramEntryData.fireGuardCutRate);
		std::printf("%0.1f\t", paramEntryData.thunGuardCutRate);
		std::printf("%d\t", paramEntryData.animIdOffset);
		std::printf("%d\t", paramEntryData.moveAnimId);
		std::printf("%d\t", paramEntryData.spMoveAnimId1);
		std::printf("%d\t", paramEntryData.spMoveAnimId2);
		std::printf("%f\t", paramEntryData.networkWarpDist);
		std::printf("%d\t", paramEntryData.dbgBehaviorR1);
		std::printf("%d\t", paramEntryData.dbgBehaviorL1);
		std::printf("%d\t", paramEntryData.dbgBehaviorR2);
		std::printf("%d\t", paramEntryData.dbgBehaviorL2);
		std::printf("%d\t", paramEntryData.dbgBehaviorRL);
		std::printf("%d\t", paramEntryData.dbgBehaviorRR);
		std::printf("%d\t", paramEntryData.dbgBehaviorRD);
		std::printf("%d\t", paramEntryData.dbgBehaviorRU);
		std::printf("%d\t", paramEntryData.dbgBehaviorLL);
		std::printf("%d\t", paramEntryData.dbgBehaviorLR);
		std::printf("%d\t", paramEntryData.dbgBehaviorLD);
		std::printf("%d\t", paramEntryData.dbgBehaviorLU);
		std::printf("%d\t", paramEntryData.animIdOffset2);
		std::printf("%.2f\t", paramEntryData.partsDamageRate1);
		std::printf("%.2f\t", paramEntryData.partsDamageRate2);
		std::printf("%.2f\t", paramEntryData.partsDamageRate3);
		std::printf("%.2f\t", paramEntryData.partsDamageRate4);
		std::printf("%.2f\t", paramEntryData.partsDamageRate5);
		std::printf("%.2f\t", paramEntryData.partsDamageRate6);
		std::printf("%.2f\t", paramEntryData.partsDamageRate7);
		std::printf("%.2f\t", paramEntryData.partsDamageRate8);
		std::printf("%.2f\t", paramEntryData.weakPartsDamageRate);
		std::printf("%.2f\t", paramEntryData.superArmorRecoverCorrection);
		std::printf("%.2f\t", paramEntryData.superArmorBrakeKnockbackDist);
		std::printf("%d\t", paramEntryData.stamina);
		std::printf("%d\t", paramEntryData.staminaRecoverBaseVel);
		std::printf("%d\t", paramEntryData.def_phys);
		std::printf("%d\t", paramEntryData.def_slash);
		std::printf("%d\t", paramEntryData.def_blow);
		std::printf("%d\t", paramEntryData.def_thrust);
		std::printf("%d\t", paramEntryData.def_mag);
		std::printf("%d\t", paramEntryData.def_fire);
		std::printf("%d\t", paramEntryData.def_thunder);
		std::printf("%d\t", paramEntryData.defFlickPower);
		std::printf("%d\t", paramEntryData.resist_poison);
		std::printf("%d\t", paramEntryData.resist_desease);
		std::printf("%d\t", paramEntryData.resist_blood);
		std::printf("%d\t", paramEntryData.resist_curse);
		std::printf("%d\t", paramEntryData.ghostModelId);
		std::printf("%d\t", paramEntryData.normalChangeResouceId);
		std::printf("%d\t", paramEntryData.guardAngle);
		std::printf("%d\t", paramEntryData.slashGuardCutRate);
		std::printf("%d\t", paramEntryData.blowGuardCutRate);
		std::printf("%d\t", paramEntryData.thrustGuardCutRate);
		std::printf("%d\t", paramEntryData.superArmorDurability);
		std::printf("%d\t", paramEntryData.normalChangeTexChrId);
		std::printf("%d\t", paramEntryData.dropType);
		std::printf("%d\t", paramEntryData.knockbackRate);
		std::printf("%d\t", paramEntryData.knockbackParamId);
		std::printf("%d\t", paramEntryData.fallDamageDump);
		std::printf("%d\t", paramEntryData.staminaGuardDef);
		std::printf("%d\t", paramEntryData.pcAttrB);
		std::printf("%d\t", paramEntryData.pcAttrW);
		std::printf("%d\t", paramEntryData.pcAttrL);
		std::printf("%d\t", paramEntryData.pcAttrR);
		std::printf("%d\t", paramEntryData.areaAttrB);
		std::printf("%d\t", paramEntryData.areaAttrW);
		std::printf("%d\t", paramEntryData.areaAttrL);
		std::printf("%d\t", paramEntryData.areaAttrR);
		std::printf("%d\t", paramEntryData.mpRecoverBaseVel);
		std::printf("%d\t", paramEntryData.flickDamageCutRate);
		std::printf("%d\t", paramEntryData.defaultLodParamId);
		std::printf("%d\t", paramEntryData.drawType);
		std::printf("%d\t", paramEntryData.npcType);
		std::printf("%d\t", paramEntryData.teamType);
		std::printf("%d\t", paramEntryData.moveType);
		std::printf("%d\t", paramEntryData.lockDist);
		std::printf("%d\t", paramEntryData.material);
		std::printf("%d\t", paramEntryData.materialSfx);
		std::printf("%d\t", paramEntryData.material_Weak);
		std::printf("%d\t", paramEntryData.materialSfx_Weak);
		std::printf("%d\t", paramEntryData.partsDamageType);
		std::printf("%d\t", paramEntryData.maxUndurationAng);
		std::printf("%d\t", paramEntryData.guardLevel);
		std::printf("%d\t", paramEntryData.burnSfxType);
		std::printf("%d\t", paramEntryData.poisonGuardResist);
		std::printf("%d\t", paramEntryData.diseaseGuardResist);
		std::printf("%d\t", paramEntryData.bloodGuardResist);
		std::printf("%d\t", paramEntryData.curseGuardResist);
		std::printf("%d\t", paramEntryData.parryAttack);
		std::printf("%d\t", paramEntryData.parryDefence);
		std::printf("%d\t", paramEntryData.sfxSize);
		std::printf("%d\t", paramEntryData.pushOutCamRegionRadius);
		std::printf("%d\t", paramEntryData.hitStopType);
		std::printf("%d\t", paramEntryData.ladderEndChkOffsetTop);
		std::printf("%d\t", paramEntryData.ladderEndChkOffsetLow);
		std::printf("%d\t", paramEntryData.useRagdollCamHit);
		std::printf("%d\t", paramEntryData.disableClothRigidHit);
		std::printf("%d\t", paramEntryData.useRagdoll);
		std::printf("%d\t", paramEntryData.isDemon);
		std::printf("%d\t", paramEntryData.isGhost);
		std::printf("%d\t", paramEntryData.isNoDamageMotion);
		std::printf("%d\t", paramEntryData.isUnduration);
		std::printf("%d\t", paramEntryData.isChangeWanderGhost);
		std::printf("%d\t", paramEntryData.modelDispMask0);
		std::printf("%d\t", paramEntryData.modelDispMask1);
		std::printf("%d\t", paramEntryData.modelDispMask2);
		std::printf("%d\t", paramEntryData.modelDispMask3);
		std::printf("%d\t", paramEntryData.modelDispMask4);
		std::printf("%d\t", paramEntryData.modelDispMask5);
		std::printf("%d\t", paramEntryData.modelDispMask6);
		std::printf("%d\t", paramEntryData.modelDispMask7);
		std::printf("%d\t", paramEntryData.modelDispMask8);
		std::printf("%d\t", paramEntryData.modelDispMask9);
		std::printf("%d\t", paramEntryData.modelDispMask10);
		std::printf("%d\t", paramEntryData.modelDispMask11);
		std::printf("%d\t", paramEntryData.modelDispMask12);
		std::printf("%d\t", paramEntryData.modelDispMask13);
		std::printf("%d\t", paramEntryData.modelDispMask14);
		std::printf("%d\t", paramEntryData.modelDispMask15);
		std::printf("%d\t", paramEntryData.isEnableNeckTurn);
		std::printf("%d\t", paramEntryData.disableRespawn);
		std::printf("%d\t", paramEntryData.isMoveAnimWait);
		std::printf("%d\t", paramEntryData.isCrowd);
		std::printf("%d\t", paramEntryData.isWeakSaint);
		std::printf("%d\t", paramEntryData.isWeakA);
		std::printf("%d\t", paramEntryData.isWeakB);
		std::printf("%d\t", paramEntryData.isEnableDropSoulCapture);
		std::printf("%d\t", paramEntryData.vowType);
		std::printf("%d\t", paramEntryData.disableInitializeDead);
		std::printf("%d\t", paramEntryData.isSoulGetByBoss);
		std::printf("%d\t", paramEntryData.isMultilingual);
		std::printf("\t", paramEntryData.pad3);
		std::printf("%d\t", paramEntryData.modelDispMask16);
		std::printf("%d\t", paramEntryData.modelDispMask17);
		std::printf("%d\t", paramEntryData.modelDispMask18);
		std::printf("%d\t", paramEntryData.modelDispMask19);
		std::printf("%d\t", paramEntryData.modelDispMask20);
		std::printf("%d\t", paramEntryData.modelDispMask21);
		std::printf("%d\t", paramEntryData.modelDispMask22);
		std::printf("%d\t", paramEntryData.modelDispMask23);
		std::printf("%d\t", paramEntryData.modelDispMask24);
		std::printf("%d\t", paramEntryData.modelDispMask25);
		std::printf("%d\t", paramEntryData.modelDispMask26);
		std::printf("%d\t", paramEntryData.modelDispMask27);
		std::printf("%d\t", paramEntryData.modelDispMask28);
		std::printf("%d\t", paramEntryData.modelDispMask29);
		std::printf("%d\t", paramEntryData.modelDispMask30);
		std::printf("%d\t", paramEntryData.modelDispMask31);
		std::printf("%f\t", paramEntryData.itemSearchRadius);
		std::printf("%d\t", paramEntryData.normalChangeModelId);
		std::printf("%d\t", paramEntryData.clothUpdateOffset);
		std::printf("\t", paramEntryData.pad4);
		std::printf("%f\t", paramEntryData.subHitOffset);
		std::printf("%f\t", paramEntryData.subHitHeight);
		std::printf("%f\t", paramEntryData.subHitRadius);
		std::printf("%d\t", paramEntryData.paintRenderTargetSize);
		std::printf("%d\t", paramEntryData.normalChangeAnimChrId);
		std::printf("%f\t", paramEntryData.regainRate_PhysNeutral);
		std::printf("%f\t", paramEntryData.regainRate_PhysSlash);
		std::printf("%f\t", paramEntryData.regainRate_PhysBlow);
		std::printf("%f\t", paramEntryData.regainRate_PhysThrust);
		std::printf("%f\t", paramEntryData.regainRate_Magic);
		std::printf("%f\t", paramEntryData.regainRate_Fire);
		std::printf("%f\t", paramEntryData.regainRate_Thunder);
		std::printf("%f\t", paramEntryData.maxAnkleAngle);
		//END REPLACE WITH PARAMDEF SPECIFIC INFO

		for (size_t k = 0; k < noteLength; k++)
		{
			std::printf(" %04x", unicodeBuffer[k]);
		}

		std::printf("\n");
	}

	return 0;
}

static void show_usage(std::string execName) {
	std::cerr
		<< "Revision: " << VERSION << '\n'
		<< "Usage: " << execName << " <FILE1>\n"
		<< "\t<FILE1>\tinput file (required)\n";
}
