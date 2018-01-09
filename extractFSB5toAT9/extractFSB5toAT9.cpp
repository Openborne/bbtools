// fsb5_at9.cpp : Defines the entry point for the console application.
//
// extracts ATRAC9 audio files from FMOD sound bank (FSB5) files for Bloodborne
//by albeartron
/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <inttypes.h>
//#include <string>
//#include <string.h>
//#include "FSB5struct.h"

#define VERSION "Rev0-2"
//0-1	initial draft
//0-2	removed most debug printf stuff and added blockAlign and avgBytesPerSec corrections (anything extracted with this tool before this change probably has audio issues or diminished quality)

#pragma pack(1)	//bytewise
struct ATRAC9_HEADER {
	//used to assemble the header that goes on the output data
	//ATRAC9 header info from PS4 SDK documentation
	unsigned char	RIFF_Chunk_ID[4] = { 0x52, 0x49, 0x46, 0x46 };	//alwyas "RIFF"
	std::uint32_t	RIFF_Chunk_Data_Size;		//should be 92 + datasize (0x5C + datasize)
	unsigned char   RIFF_Type_ID[4] = { 0x57, 0x41, 0x56, 0x45 };	//always 'WAVE"
																	//fmt chunk
	unsigned char   fmt_Chunk_ID[4] = { 0x66, 0x6d, 0x74, 0x20 };	//always "fmt "
	std::uint32_t   fmt_Chunk_Data_Size = 52;	//size of the fmt chunk is always 52 (0x34)
	std::uint16_t   wFormatTag = 0xFFFE;		//WAVE_FORMAT_EXTENSIBLE = 0xFFFE (a.k.a. AT9)
	std::uint16_t   nChannels;					//determined by FSB header
	std::uint32_t   nSamplesPerSec;				//determined by FSB header
	std::uint32_t   nAvgBytesPerSec;			//Average byte rate [Byte/s], Calculated by rounding off nBlockAlign * nSamplesPerSec / wSamplesPerBlock
	std::uint16_t	nBlockAlign;				//Audio block size [Byte]
	std::uint16_t   wBitsPerSample = 0;			//Quantifying bit number (0)
	std::uint16_t   cbSize = 34;				//Extension size (34)
	std::uint16_t   wSamplesPerBlock = 1024;	//Number of output samples of the audio block
	std::uint32_t   dwChannelMask = 4;			//Channel map of speaker position
	std::uint8_t    SubFormat[16] = { 0xD2, 0x42, 0xE1, 0x47, 0xBA, 0x36, 0x8D, 0x4D, 0x88, 0xFC, 0x61, 0x65, 0x4F, 0x8C, 0x83, 0x6C };	//GUID,  Codec identifier (KSDATAFORMAT_SUBTYPE_ATRAC9 = {47E142D2-36BA-4d8d-88FC-61654F8C836C})
	std::uint32_t   dwVersionInfo = 1;			//Version information (1 or 2)
												//ATRAC9 config data bitfield
	unsigned char   atrac9_id : 8;
	std::uint8_t    reserved1 : 1;
	std::uint8_t    sampling_rate_index : 3;
	std::uint8_t    channel_config_index : 3;
	std::uint8_t    reserved2 : 1;
	std::uint16_t   frame_length_index : 11;
	std::uint8_t    super_frame_index : 2;
	std::uint8_t    reserved3 : 3;
	std::uint32_t	reserved4 = 0;	//padding
									//fact chunk
	unsigned char   fact_Chunk_ID[4] = { 0x66, 0x61 ,0x63, 0x74 };	//always "fact"
	std::uint32_t   fact_Chunk_Data_Size = 12;	//size of the fact chunk is always 12
	std::uint32_t   Total_Samples;				//found in FSB sample header
	std::uint32_t   Input_And_Overlap_Delay_Samples = 256;
	std::uint32_t   Encoder_Delay_Samples = 256;
	//data chunk
	unsigned char   data_Chunk_ID[4] = { 0x64, 0x61, 0x74, 0x61 };	//always "data"
	std::uint32_t   data_Chunk_Data_Size;
};

struct FSB5_HEADER {
	//this info found in "FSB5.bt" template by Simon Pinfold for "010 Editor"
	unsigned char	FSB5_ID[4];			//FSB5 identifier
	std::uint32_t	FSB5_version;		//sub version number, e.g. 5.1
	std::uint32_t	numSamples;			//number of file/sample entries in this FSB
	std::uint32_t	sampleHeaderSize;	//size of the headers info section (following this main header)
	std::uint32_t	nameTableSize;		//size of the names table section (following the headers section)
	std::uint32_t	dataSize;			//size of the data section (following the names table section)
										//settings
	std::uint32_t	mode;				//at9 files are 0x0000000D = 13; not included in the original FSB5.bt template
	std::uint8_t    unknown01[8];		//unknown
	std::uint8_t    unknown02[16];		//unknown hash?
	std::uint8_t    unknown03[8];		//unknown
										//headers table follows this section
};

struct FSB5_SAMPLE_HEADER {
	//just the sample header:
	std::uint32_t	extraParams : 1;	//whether or not there are extra chunks after this info
	std::uint32_t	frequency : 4;
	std::uint32_t	twoChannels : 1;
	std::uint32_t	dataOffset : 28;
	std::uint32_t	samples : 30;
	//64 bits... 8 bytes
};

struct FSB5_SAMPLE_CHUNK_DEF {
	//fsb5 has different chunks in each sample header, each begins with this structure
	std::uint32_t	next : 1;	//more chunks to follow?
	std::uint32_t	size : 24;	//size of this chunk (just the data)
	std::uint32_t	chunkType : 7;	//chunk type 3=loop, 9=ATRAC9, etc. only caring about ATRAC9 for this
};

struct FSB5_SAMPLE_CHANNELS_CHUNK {
	//chunk type 1
	std::uint8_t	channels;
};

struct FSB5_SAMPLE_LOOP_CHUNK {
	//chunk type 3
	std::uint32_t	loopStart;
	std::uint32_t	loopEnd;
};

struct FSB5_SAMPLE_ATRAC9_CHUNK {
	//chunk type 9
	std::uint32_t	unknownByte;	//there's a byte at the beginning of this chunk that might be important but is currently unknown, probably a bitfield that has the info for blockAlign/other things
	std::uint8_t	atrac9_id : 8;				//ATRAC9 format data(0xFE)
	std::uint8_t    reserved1 : 1;				//Reserved area
	std::uint8_t    sampling_rate_index : 3;	//Sampling rate and number of samples per frame
	std::uint8_t    channel_config_index : 3;	//Number of channels and audio blocks
	std::uint8_t    reserved2 : 1;				//Reserved area
	std::uint16_t   frame_length_index : 11;	//Frame length for bitstreaming, The actual frame length will be (frame_length_index + 1) bytes.
	std::uint8_t    super_frame_index : 2;		//Number of frames in a superframe
	std::uint8_t    reserved3 : 3;				//Reserved area
};
#pragma pack()

static void show_usage(std::string);

int main(int argc, char *argv[]) {
	if (argc < 2) {
		show_usage(argv[0]);
		return 1;
	}

	char* filenametemp = argv[1];
	std::ifstream inputFile(argv[1], std::ios::binary);		//open file as binary

	ATRAC9_HEADER outputHeader;	//the at9 output file's header, to be populated by this program

								//header/chunk structs:
	FSB5_HEADER	fsbHeader;
	FSB5_SAMPLE_HEADER fsbSampleHeader, nextSampleHeader;	//each file's header info
	FSB5_SAMPLE_CHUNK_DEF fsbSampleChunk;	//if extraParams != 0
	FSB5_SAMPLE_CHANNELS_CHUNK fsbChannelsChunk;
	FSB5_SAMPLE_LOOP_CHUNK fsbLoopChunk;
	FSB5_SAMPLE_ATRAC9_CHUNK fsbAt9Chunk;	//important chunk that will dictate ATRAC9 header info

											//variables
	std::uint32_t	sampleTableStart;	//where the samples data starts
	std::uint32_t	nameTableStart;		//where the name table starts (the nametable offset list)
	std::uint32_t	dataTableStart;		//where the actual data table starts
	std::uint32_t	dataTableEnd;		//where the end of the data table is (EOF)

	std::uint32_t	returnAddress;	//where to come back to after getting name + data
	std::uint32_t	sampleTableCurrent;	//the current working location in the sample table
	std::uint32_t	nameTableCurrent;	//the current working location in the name table (offsets list)
	std::uint32_t	nameTableTextOffset;	//start + offset = where the text is
	std::uint32_t	dataTableCurrent;	//the current working location in the data table
	std::uint32_t	nextDataOffset;	//used to know where to stop reading
	std::uint32_t	dataEntrySize = 0;

	const int MAX_BUFFSIZE = 512;
	char stringBuffer[MAX_BUFFSIZE];

	unsigned char tempChar = 0xFF;
	std::uint8_t	tempByte;
	std::uint8_t	tempChannels;	//number of channels in the output
	std::string		tempString = "";	//use for filename output


										//read FSB5 main header (done once)
	inputFile.read((char *)&fsbHeader, sizeof(fsbHeader));

	/*	don't print the header info for now
	std::printf(
		"ID" "\t"
		"subVersion" "\t"
		"numSamples" "\t"
		"headerSize" "\t"
		"namesSize " "\t"
		"dataSize  " "\t"
		"FSBmode" "\t"
		"unknown01         " "\t"
		"unknown02 (Hash?)                 " "\t"
		"unknown03         " "\n"
	);
	*/

	//std::printf("%.*s\t", sizeof(fsbHeader.FSB5_ID), fsbHeader.FSB5_ID);	//string of char [4] (print size/ptr because non null terminated)
	//std::printf("0x%08X\t", fsbHeader.FSB5_version);
	//std::printf("0x%08X\t", fsbHeader.numSamples);
	sampleTableStart = 0x3C;	//not actually important
	//std::printf("0x%08X\t", fsbHeader.sampleHeaderSize);
	nameTableStart = sampleTableStart + fsbHeader.sampleHeaderSize;	//name table starts after FSBheader + sampleHeader
	nameTableCurrent = nameTableStart;	//initial position in name table
	//std::printf("0x%08X\t", fsbHeader.nameTableSize);
	dataTableStart = nameTableStart + fsbHeader.nameTableSize;
	dataTableCurrent = dataTableStart;	//initial position in the data table
	//std::printf("0x%08X\t", fsbHeader.dataSize);
	dataTableEnd = dataTableStart + fsbHeader.dataSize;	//should probably verify that this = EOF

	//std::printf("0x%08X\t", fsbHeader.mode);	//it's AT9 mode for the files we care about
	/*
	switch (fsbHeader.mode)
	{
	case 0x02:
		std::printf("PCM16  \t");
		break;
	case 0x0B:
		std::printf("MPEG   \t");
		break;
	case 0x0D:
		std::printf("AT9    \t");
		break;
	case 0x0F:
		std::printf("VORBIS \t");
		break;
	default:
		std::printf("UNKNOWN\t");
		break;
	}
	*/
	//catches most errors if wrong mode or not FSB5
	if (fsbHeader.mode != 0x0D)
	{
		std::printf("ERROR: FSB mode not supported - 0x%08X\n", fsbHeader.mode);
		return 1;
	}

	//std::printf("0x");
	//for (std::uint8_t i = 0; i < sizeof(fsbHeader.unknown01); i++) { std::printf("%02X", fsbHeader.unknown01[i]); } std::printf("\t");
	//std::printf("0x");
	//for (std::uint8_t i = 0; i < sizeof(fsbHeader.unknown02); i++) { std::printf("%02X", fsbHeader.unknown02[i]); } std::printf("\t");
	//std::printf("0x");
	//for (std::uint8_t i = 0; i < sizeof(fsbHeader.unknown03); i++) { std::printf("%02X", fsbHeader.unknown03[i]); } std::printf("\n");
	//std::printf("\n");

	/*
	std::printf(
		"no." "\t"
		"xtra" "\t"
		"freq" "\t"
		"2ch?" "\t"
		"datoffset" "\t"
		"#samp" "\t"
		"chnks" "\t"
		"filename" "\t"
		//"dataBegin" "\t"
		//add loop info
		"dataSize" "\n"
	);
	*/


	for (size_t sampleNumber = 0; sampleNumber < fsbHeader.numSamples; sampleNumber++)
		//for (size_t sampleNumber = 0; sampleNumber < 1; sampleNumber++)	//one once for debugging
	{
	  
		std::printf("%zu\t", sampleNumber);
		//read SAMPLE header (done for each sample, numsamples times)
		inputFile.read((char *)&fsbSampleHeader, sizeof(fsbSampleHeader));	//read sample header
																			//show it
		std::printf("%d\t", fsbSampleHeader.extraParams);

		if(fsbSampleHeader.frequency != 9){
		  std::printf("*");
		}
		std::printf("%d\t", fsbSampleHeader.frequency);
		
		//change frequency output to this:
		/*
			1: "8000Hz"
			2: "11000Hz"
			3: "11025Hz"
			4: "16000Hz"
			5: "22050Hz"
			6: "24000Hz"
			7: "32000Hz"
			8: "44100Hz"
			9: "48000Hz"
		*/
		std::printf("%d\t", fsbSampleHeader.twoChannels);
		tempChannels = fsbSampleHeader.twoChannels + 1;	//0+1=1, 1+1=2; may be overwritten by CHANNELS chunk info later
		std::printf("0x%08X\t", fsbSampleHeader.dataOffset);	//this is dataStart + this offset
		dataTableCurrent = dataTableStart + (fsbSampleHeader.dataOffset * 16);
		std::printf("%d\t", fsbSampleHeader.samples);

		//read SAMPLE header's extra params (if there are any)
		if (fsbSampleHeader.extraParams) {
			do
			{
				//read the SAMPLE header's extra param chunk info
				inputFile.read((char *)&fsbSampleChunk, sizeof(fsbSampleChunk));
				std::printf("%d", fsbSampleChunk.chunkType);
				if (fsbSampleChunk.next) { std::printf(","); }
				switch (fsbSampleChunk.chunkType)	//do something based on the chunk type
				{
				case 1:	//channels chunk
					inputFile.read((char *)&fsbChannelsChunk, sizeof(fsbChannelsChunk));
					tempChannels = fsbChannelsChunk.channels;
					//inputFile.read((char *)&tempChannels, sizeof(tempChannels));	//if there's a channels chunk, read it and store the #
																					//only used for files with >2 channels
					break;
				case 3:	//loop chunk
					inputFile.read((char *)&fsbLoopChunk, sizeof(fsbLoopChunk));
					//inputFile.seekg(fsbSampleChunk.size, std::ios::cur);	//skip for now
																			//could add loop info to the ATRAC9 header
					break;
				case 5:	//unknown chunk
					inputFile.seekg(fsbSampleChunk.size, std::ios::cur);	//skip for now
					//need to reverse engineer FMOD output to determine the purpose and structure of this chunk
				case 9: //ATRAC9 chunk
					inputFile.read((char *)&fsbAt9Chunk, sizeof(fsbAt9Chunk));	//read the ATRAC9 config data
					break;
				default:
					inputFile.seekg(fsbSampleChunk.size, std::ios::cur);	//skip this unknown chunk
																			//several other chunk types were encountered in the Bloodborne files but no time was spent to learn what they do as they don't seem important for decoding
					break;
				}
			} while (fsbSampleChunk.next);	//if next = 1, repeat the above since there are more chunks for this sample
		}
		std::printf("\t");	//tab after list of chunk types
		sampleTableCurrent = inputFile.tellg();	//remember where we were in the sampleHeaders, last step for this file will be returning here
												//go to the name table (first time this is at nameTableStart)
		inputFile.seekg(nameTableCurrent);
		//determine the name location, save in fileNameOffset
		inputFile.read((char *)&nameTableTextOffset, sizeof(nameTableTextOffset));	//read the offset from the nametable offset list
		nameTableCurrent = inputFile.tellg();	//remember where the next name offset is for next time
		inputFile.seekg(nameTableTextOffset + nameTableStart);	//go to the actual filename location, which is at nameTableStart + whatever offset

		tempString.clear();
		inputFile.getline(stringBuffer, sizeof(stringBuffer), '\0');
		tempString = stringBuffer;
		tempString += ".at9\0";

		std::printf("%s\t", tempString.c_str());	//this should be the filename

		std::ofstream outputFile(tempString, std::ios::binary);		//create the output file and open file as binary

		//doesn't have data chunk size info so:
		//check if this is the last entry
		if (sampleNumber + 1 == fsbHeader.numSamples)
		{
			//this is the last sample, read until EOF
			dataEntrySize = dataTableEnd - dataTableCurrent;	//size of entry is EOF - currentoffset
		}
		else
		{
			//not the last sample, so get next sampleheader to find where the next sample begins
			inputFile.seekg(sampleTableCurrent);
			inputFile.read((char *)&nextSampleHeader, sizeof(nextSampleHeader));
			dataEntrySize = (dataTableStart + (nextSampleHeader.dataOffset * 16)) - dataTableCurrent;	//size of this entry is nextoffset - currentoffset
		}

		//write the ATRAC9 config data to the at9 output's header here
		//riff
		outputHeader.RIFF_Chunk_Data_Size = 0x58 + dataEntrySize;	//ATRAC9 could have extra chunks but this program doesn't add them in
		outputHeader.nChannels = tempChannels;
		outputHeader.nSamplesPerSec = 48000;	//need to add code to interpret fsbSampleHeader.frequency, but '9' = 48000Hz and almost all the files appear the same for Bloodborne

		//channels defaults:
		//#ch	kbps	bytes/s	blockalign
		//1ch	72  	9000	192
		//2ch	144 	18000	384
		//4ch	240 	30000	640
		//outputHeader.nAvgBytesPerSec = 18000;	//should calculate this instead, but at9tool will still extract audio with incorrect value
		//outputHeader.nBlockAlign = 384;	//should calculate this instead, but at9tool will still extract audio with incorrect value (may be in FSB ATRAC9 chunk's first half)
		switch (outputHeader.nChannels)
		{
		case 1:
			outputHeader.nAvgBytesPerSec = 9000;
			outputHeader.nBlockAlign = 192;
			break;
		case 2:
			outputHeader.nAvgBytesPerSec = 18000;
			outputHeader.nBlockAlign = 384;
			break;
		case 4:
			outputHeader.nAvgBytesPerSec = 30000;
			outputHeader.nBlockAlign = 640;
			break;
		default:
			outputHeader.nAvgBytesPerSec = 9000;
			outputHeader.nBlockAlign = 192;
			break;
		}

		//config
		outputHeader.atrac9_id = fsbAt9Chunk.atrac9_id;
		outputHeader.reserved1 = fsbAt9Chunk.reserved1;
		outputHeader.sampling_rate_index = fsbAt9Chunk.sampling_rate_index;
		outputHeader.channel_config_index = fsbAt9Chunk.channel_config_index;
		outputHeader.reserved2 = fsbAt9Chunk.reserved2;
		outputHeader.frame_length_index = fsbAt9Chunk.frame_length_index;
		outputHeader.super_frame_index = fsbAt9Chunk.super_frame_index;
		outputHeader.reserved3 = fsbAt9Chunk.reserved3;
		//fact
		outputHeader.Total_Samples = fsbSampleHeader.samples;
		outputHeader.data_Chunk_Data_Size = dataEntrySize;
		//could also write loop data here, based on WAVE RIFF "smpl" chunk structure
		//http://www-mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/Docs/RIFFNEW.pdf

		outputFile.write((char *)&outputHeader, sizeof(outputHeader));	//write the ATRAC9 header

		inputFile.seekg(dataTableCurrent);	//go to where the data starts
											//read data starting here, total size of dataEntrySize

		//std::printf("0x%08X\t", dataTableCurrent);
		//should print loop info (start/end)
		std::printf("0x%08X\t", dataEntrySize);
		std::printf("\n");

		//send the data from the FSB into the new file
		//there's probably a better way to do this, but this seems to work OK
		
		for (size_t i = 0; i < dataEntrySize; i++)
		{
		inputFile.read((char *)&tempByte, sizeof(tempByte));
		outputFile.write((char *)&tempByte, sizeof(tempByte));
		}
		
		

		outputFile.close();
		//then move onto the next sample and repeat the process numSamples times total 
		inputFile.seekg(sampleTableCurrent);
		
	}


	return 0;
}

static void show_usage(std::string execName) {
	std::cerr
		<< "Revision: " << VERSION << '\n'
		<< "Usage: " << execName << " <FILE1>\n"
		<< "\t<FILE1>\tFSB5 input file (required)\n";
}
