// Extract_Paramdef.cpp : Defines the entry point for the console application.
// compiled with g++, --std=c++11


//#include "stdafx.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <inttypes.h>
#include <string>

#define VERSION "WIP"

#pragma pack(1)
struct paramdef_header {
	/* 0x0000 */	std::uint32_t fileLength;
	/* 0x0004 */	std::uint16_t unknown01;
	/* 0x0006 */	std::uint16_t unknown02;
	/* 0x0008 */	std::uint16_t numParamdefs;
	/* 0x000A */	std::uint16_t paramdefLength;
	/* 0x000C */	char title[32];
	/* 0x002C */	std::uint32_t unknown03;
	/* 0x0030 */	std::uint32_t firstParamdefAddr;
	/* 0x0034 */	std::uint8_t padding1[4];	//addr might be 64 bit but these are always zero
};

struct param_entry {
	std::uint16_t description[32];	//description, null terminated string (unicode)
	char type[8];	//e.g. s32, u8, f32, etc.
	char printvar[14];	//e.g. %d, %0.2f, etc.
	std::uint16_t unknown01[9];
	std::uint32_t infoEntryAddr;	//address of info string
	std::uint8_t padding1[4];
	char type2[32];	//repeat of type but sometimes a custom type name
	char name[32];	//variable name (english)
	std::uint16_t unknown02[16];
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
	std::ifstream inputFile(filenametemp, std::ios::binary);

	paramdef_header paramHeaderData;
	param_entry paramEntryData;

	std::size_t returnAddress;
	std::size_t noteLength;

	const int MAX_BUFFSIZE = 512;
	char stringBuffer[MAX_BUFFSIZE];
	char buffer[MAX_BUFFSIZE] = {};
	std::uint16_t unicodeBuffer[MAX_BUFFSIZE];

	inputFile.read((char *)&paramHeaderData, sizeof(paramHeaderData));	//read header data
	
	//print header info
	//std::printf("Title:\t%s\n", paramHeaderData.title);
	//std::printf("File Length:\t0x%08X\n", paramHeaderData.fileLength);
	//std::printf("Unknown:\t0x%04X\n", paramHeaderData.unknown01);
	//std::printf("Unknown:\t0x%04X\n", paramHeaderData.unknown02);
	//std::printf("NumParamdefs:\t0x%04X\n", paramHeaderData.numParamdefs);
	//std::printf("ParamdefLength:\t0x%04X\n", paramHeaderData.paramdefLength);
	//std::printf("Title:\t%s\n", paramHeaderData.title);
	//std::printf("Unknown:\t0x%08X\n", paramHeaderData.unknown03);
	//std::printf("FirstAddr:\t0x%08X\n", paramHeaderData.firstParamdefAddr);

	//print column headers
	std::printf(
		"no.\t"
		"filename\t"
		"paramDef\t"
		"type\t"
		"printf\t"
		"infoaddr\t"
		"type2\t"
		"varname\t"
		"name\t"
		"namej\t"
		"namee\t"
		"info\t"
		"infoj\t"
		"infoe\n"
	);
	
	for (size_t i = 0; i < paramHeaderData.numParamdefs; i++)
	{
		//print entry number
		std::printf("%d\t", i+1);

		//print filename (paramdef file)
		std::printf("%s\t", filenametemp);

		//print paramdef name
		std::printf("%s\t", paramHeaderData.title);

		//read a paramdef chunk
		inputFile.read((char *)&paramEntryData, sizeof(paramEntryData));
		
		//then the type
		std::printf("%s\t", paramEntryData.type);
		//then the printf var
		std::printf("%s\t", paramEntryData.printvar);
		
		//then the addr of the info entry
		std::printf("0x%08X\t", paramEntryData.infoEntryAddr);
		//then the type2
		std::printf("%s\t", paramEntryData.type2);
		//then the name
		std::printf("%s\t", paramEntryData.name);


		//print the description
		for (size_t a = 0; a < sizeof(paramEntryData.description); a++)
		{
			if (paramEntryData.description[a] != 0x0000)
			{
				std::printf(" %04x", paramEntryData.description[a]);
			}
			else {
				a = sizeof(paramEntryData);
			}
		}
		std::printf("\t\t\t");	//extra tabs for translated text, filled in later manually for now

		//print the info entry
		returnAddress = inputFile.tellg();
		noteLength = 0;	//default info length is zero unless updated
		if (paramEntryData.infoEntryAddr)	//if info entry addr is not zero, get the info
		{
			inputFile.seekg(paramEntryData.infoEntryAddr);
			for (size_t j = 0; j < MAX_BUFFSIZE; j++) {
				inputFile.read((char *)&unicodeBuffer[j], sizeof(unicodeBuffer[j]));	//read one unicode character
				if ((unicodeBuffer[j] == 0x0000))	//if the current character = unicode null...
				{
					noteLength = j;	//remember the length of this string
					j = MAX_BUFFSIZE;	//breaks loop if last character was unicode null
				}
			}
			//return to the previous spot for the next read
			inputFile.seekg(returnAddress);
		}

		for (size_t k = 0; k < noteLength; k++)
		{
			std::printf(" %04x", unicodeBuffer[k]);
		}
		std::printf("\t\t\n");	//extra tabs for translated text, filled in later manually for now
	}
	
	return 0;
}

static void show_usage(std::string execName) {
	std::cerr
		<< "Revision: " << VERSION << '\n'
		<< "Usage: " << execName << " <FILE1>\n"
		<< "\t<FILE1>\tinput file (required)\n";
}


