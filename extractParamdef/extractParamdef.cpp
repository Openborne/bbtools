// extractParamdef.cpp
// albeartron
// compiled with g++ --std=c++11 on cygwin
// usage:
//  run ./extractParamdef <paramdef file>
//  outputs a text file named <paramdef file>.txt with the paramdef information

#include <iostream>
#include <fstream>
#include <inttypes.h>
#include <string>

#define VERSION "Rev0-3"
//0-1	initial draft
//0-2	add functionality to also read param file (cancelled)
//0-3	writes results directly to a file using an extremely kludgey method of writing unicode characters

#pragma pack(1)
struct PARAMDEF_HEADER {
	//paramdef file header structure
	/* 0x0000 */	std::uint32_t	fileLength;
	/* 0x0004 */	std::uint16_t	unknown01;
	/* 0x0006 */	std::uint16_t	unknown02;
	/* 0x0008 */	std::uint16_t	numParamdefs;
	/* 0x000A */	std::uint16_t	paramdefLength;
	/* 0x000C */	char	title[32];
	/* 0x002C */	std::uint32_t	unknown03;
	/* 0x0030 */	std::uint32_t	firstParamdefAddr;
	/* 0x0034 */	std::uint32_t	padding1;	//addr might be 64 bit but these are always zero
};

struct PARAMDEF_ENTRY {
	//paramdef file chunk structure
	/* +0x0000 */	std::uint16_t	description[32];	//description, null terminated string (unicode)
	/* +0x0040 */	char	type[8];	//e.g. s32, u8, f32, etc.
	/* +0x0048 */	char	printvar[16];	//e.g. %d, %0.2f, etc.
	/* +0x0056 */	std::uint8_t	unknown01[8];	//this unknown may include min/max/increment info if it's anything like previous fromsoft paramdefs
	/*         */	std::uint32_t	displayMode;	//display mode
	/*         */	std::uint32_t	sizeofThis;	//size, in bytes, of this variable (in the param file)
	/* +0x0068 */	std::uint32_t	infoEntryAddr;	//address of info string
	/* +0x006C */	std::uint8_t	padding1[4];
	/* +0x0070 */	char	type2[32];	//repeat of type but sometimes a custom type name
	/* +0x0090 */	char	name[32];	//variable name (english)
	/* +0x00B0 */	std::uint32_t	unknown02;	//some sort of ID or category number
	/* +0x00B0 */	std::uint16_t	unknown03[14];
};
//paramdef file consists of header followed by entry chunks
//each entry chunk contains information for a corresponding param file
//such as variable names, types, and additional information
#pragma pack()

static void show_usage(std::string);

int main(int argc, char *argv[]) {
	//check if enough command line parameters
	if (argc < 2) {
		show_usage(argv[0]);
		return 1;
	}

	//open the input file
	std::ifstream paramdefFile(argv[1], std::ios::binary);

	//open the output file
	std::string outputFileName = argv[1];
	outputFileName += ".txt";
	std::ofstream txtFile(outputFileName, std::ios::binary);

	PARAMDEF_HEADER paramdefHeaderData;
	PARAMDEF_ENTRY paramdefEntryData;

	std::size_t returnAddress;
	std::size_t noteLength;
	const char nullChar = 0x00;
	const char tabChar = 0x09;
	const char carriageReturnChar = 0x0D;
	const char linefeedChar = 0x0A;
	std::string tempString;

	const std::size_t MAX_BUFFSIZE = 512;
	std::uint16_t unicodeBuffer[MAX_BUFFSIZE];

	paramdefFile.read((char *)&paramdefHeaderData, sizeof(paramdefHeaderData));	//read header data

	/*	skip printing header info for now
	//column header for header info
	std::printf(
		"length" "\t"
		"unknown01" "\t"
		"unknown02" "\t"
		"numParamDefs" "\t"
		"paramdefLength" "\t"
		"title" "\t"
		"unknown03" "\t"
		"firstParamdefAddr" "\t"
		"padding1" "\n"
	);
	
	//paramdef header info
	std::printf("0x%08X" "\t", paramdefHeaderData.fileLength);
	std::printf("0x%04X" "\t", paramdefHeaderData.unknown01);
	std::printf("0x%04X" "\t", paramdefHeaderData.unknown02);
	std::printf("0x%04X" "\t", paramdefHeaderData.numParamdefs);
	std::printf("0x%04X" "\t", paramdefHeaderData.paramdefLength);
	std::printf("%s" "\t", paramdefHeaderData.title);
	std::printf("0x%08X" "\t", paramdefHeaderData.unknown03);
	std::printf("0x%08X" "\t", paramdefHeaderData.firstParamdefAddr);
	std::printf("0x%08X" "\n", paramdefHeaderData.padding1);
	*/
	
	/*
	std::printf(
		"no." "\t"
		"description" "\t"
		"type" "\t"
		"printf" "\t"
		"unknown01" "\t"
		"displayMode" "\t"
		"sizeofThis" "\t"
		"infoaddr" "\t"
		"padding1" "\t"
		"type2" "\t"
		"varname" "\t"
		"unknown02" "\t"
		"unknown03" "\t"
		"infolen" "\n"
	);
	*/
	

	//go to first paramdef entry location (should always be immediately after header chunk)
	paramdefFile.seekg(paramdefHeaderData.firstParamdefAddr);

	//read each paramdef entry
	for (size_t i = 0; i < paramdefHeaderData.numParamdefs; i++)
	{
		//read a paramdef chunk
		paramdefFile.read((char *)&paramdefEntryData, sizeof(paramdefEntryData));
		
		//PARAM ENTRY NUMBER
		/*
		std::printf("%d\t", i+1);	//no.
		tempString = std::to_string(i + 1);
		for (size_t j = 0; j < tempString.length(); j++)
		{
			txtFile.write((char *)&tempString.at(j), sizeof(tempString.at(j)));
			txtFile.write((char *)&nullChar, sizeof(nullChar));
		}
		txtFile.write((char *)&tabChar, sizeof(tabChar));
		txtFile.write((char *)&nullChar, sizeof(nullChar));
		*/
		
		//PARAMDEF TITLE
		for (size_t b = 0; b < sizeof(paramdefHeaderData.title); b++)
		{
			if (paramdefHeaderData.title[b] == 0x00)
			{
				break;
			}
			txtFile.write((char *)&paramdefHeaderData.title[b], sizeof(paramdefHeaderData.title[b]));
			txtFile.write((char *)&nullChar, sizeof(nullChar));
		}
		txtFile.write((char *)&tabChar, sizeof(tabChar));
		txtFile.write((char *)&nullChar, sizeof(nullChar));


		//DESCRIPTION
		for (size_t a = 0; a < sizeof(paramdefEntryData.description)/sizeof(*paramdefEntryData.description); a++)
		{
			if (paramdefEntryData.description[a] == 0x0000)
			{
				break;
			}
			std::printf(" %04x", paramdefEntryData.description[a]);
			txtFile.write((char *)&paramdefEntryData.description[a], sizeof(paramdefEntryData.description[a]));
		}
		txtFile.write((char *)&tabChar, sizeof(tabChar));
		txtFile.write((char *)&nullChar, sizeof(nullChar));
		std::printf("\t");
			
		//DATA TYPE
		std::printf("%s\t", paramdefEntryData.type);	//type
		for (size_t j = 0; j < sizeof(paramdefEntryData.type); j++)
		{
			if (paramdefEntryData.type[j] == 0x00)
			{
				break;
			}
			txtFile.write((char *)&paramdefEntryData.type[j], sizeof(paramdefEntryData.type[j]));
			txtFile.write((char *)&nullChar, sizeof(nullChar));
		}
		txtFile.write((char *)&tabChar, sizeof(tabChar));
		txtFile.write((char *)&nullChar, sizeof(nullChar));		

		//PRINTF VAR
		std::printf("%s\t", paramdefEntryData.printvar);	//printf type
		for (size_t j = 0; j < sizeof(paramdefEntryData.printvar); j++)
		{
			if (paramdefEntryData.printvar[j] == 0x00)
			{
				break;
			}
			txtFile.write((char *)&paramdefEntryData.printvar[j], sizeof(paramdefEntryData.printvar[j]));
			txtFile.write((char *)&nullChar, sizeof(nullChar));
		}
		txtFile.write((char *)&tabChar, sizeof(tabChar));
		txtFile.write((char *)&nullChar, sizeof(nullChar));

		//unknown01, uint8_t[8]
		for (size_t b = 0; b < 8; b++)
		{
			std::printf("%02X ", paramdefEntryData.unknown01[b]);
		}
		std::printf("\t");
		
		//DISPLAY MODE
		std::printf("%d\t", paramdefEntryData.displayMode);
		tempString = std::to_string(paramdefEntryData.displayMode);
		for (size_t j = 0; j < tempString.length(); j++)
		{
			txtFile.write((char *)&tempString.at(j), sizeof(tempString.at(j)));
			txtFile.write((char *)&nullChar, sizeof(nullChar));
		}
		txtFile.write((char *)&tabChar, sizeof(tabChar));
		txtFile.write((char *)&nullChar, sizeof(nullChar));

		//SIZE OF VAR IN BYTES
		std::printf("%d\t", paramdefEntryData.sizeofThis);	//size of this variable (in bytes)
		tempString = std::to_string(paramdefEntryData.sizeofThis);
		for (size_t j = 0; j < tempString.length(); j++)
		{
			txtFile.write((char *)&tempString.at(j), sizeof(tempString.at(j)));
			txtFile.write((char *)&nullChar, sizeof(nullChar));
		}
		txtFile.write((char *)&tabChar, sizeof(tabChar));
		txtFile.write((char *)&nullChar, sizeof(nullChar));

		//INFO ADDRESS
		std::printf("0x%08X\t", paramdefEntryData.infoEntryAddr);	//addr of the info entry

		//print padding1, uint8_t[4]
		std::printf("0x");
		for (size_t c = 0; c < sizeof(paramdefEntryData.padding1)/sizeof(*paramdefEntryData.padding1); c++)
		{
			std::printf("%02X", paramdefEntryData.padding1[c]);
		}
		std::printf("\t");

		//TYPE2
		std::printf("%s\t", paramdefEntryData.type2);	//type2
		for (size_t j = 0; j < sizeof(paramdefEntryData.type2); j++)
		{
			if (paramdefEntryData.type2[j] == 0x00)
			{
				break;
			}
			txtFile.write((char *)&paramdefEntryData.type2[j], sizeof(paramdefEntryData.type2[j]));
			txtFile.write((char *)&nullChar, sizeof(nullChar));
		}
		txtFile.write((char *)&tabChar, sizeof(tabChar));
		txtFile.write((char *)&nullChar, sizeof(nullChar));

		//VARNAME
		std::printf("%s\t", paramdefEntryData.name);	//varname
		for (size_t j = 0; j < sizeof(paramdefEntryData.name); j++)
		{
			if (paramdefEntryData.name[j] == 0x00)
			{
				break;
			}
			txtFile.write((char *)&paramdefEntryData.name[j], sizeof(paramdefEntryData.name[j]));
			txtFile.write((char *)&nullChar, sizeof(nullChar));
		}
		txtFile.write((char *)&tabChar, sizeof(tabChar));
		txtFile.write((char *)&nullChar, sizeof(nullChar));


		std::printf("0x%08X\t", paramdefEntryData.unknown02);	//unknown02

		//unknown03, uint16_t[14]
		for (size_t d = 0; d < sizeof(paramdefEntryData.unknown03)/sizeof(*paramdefEntryData.unknown03); d++)
		{
			std::printf("%04X ", paramdefEntryData.unknown03[d]);
		}
		std::printf("\t");

		
		//print the info entry
		returnAddress = paramdefFile.tellg();
		noteLength = 0;	//default info length is zero unless updated
		if (paramdefEntryData.infoEntryAddr)	//if info entry addr is not zero, get the info
		{
			paramdefFile.seekg(paramdefEntryData.infoEntryAddr);
			for (size_t j = 0; j < MAX_BUFFSIZE; j++) {
				paramdefFile.read((char *)&unicodeBuffer[j], sizeof(unicodeBuffer[j]));	//read one unicode character
				if ((unicodeBuffer[j] == 0x0000))	//if the current character = unicode null...
				{
					noteLength = j;	//remember the length of this string
					j = MAX_BUFFSIZE;	//breaks loop if last character was unicode null
				}
			}
			//return to the previous spot for the next read
			paramdefFile.seekg(returnAddress);
		}

		
		for (size_t k = 0; k < noteLength; k++)
		{
			std::printf(" %04x", unicodeBuffer[k]);
			txtFile.write((char *)&unicodeBuffer[k], sizeof(unicodeBuffer[k]));
		}
		
		std::printf("\n");

		//newline in output file
		txtFile.write((char *)&carriageReturnChar, sizeof(carriageReturnChar));
		txtFile.write((char *)&nullChar, sizeof(nullChar));
		txtFile.write((char *)&linefeedChar, sizeof(linefeedChar));
		txtFile.write((char *)&nullChar, sizeof(nullChar));

	}
	
	txtFile.close();

	return 0;
}

static void show_usage(std::string execName) {
	std::cerr
		<< "Revision: " << VERSION << '\n'
		<< "Usage: " << execName << " <FILE1> <FILE2>\n"
		<< "\t<FILE1>\tparamdef file (required)\n"
		<< "\t<FILE2>\tparam file (required)\n";
}
