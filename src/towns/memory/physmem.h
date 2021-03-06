/* LICENSE>>
Copyright 2020 Soji Yamakawa (CaptainYS, http://www.ysflight.com)

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

<< LICENSE */
#ifndef PHYSMEM_IS_INCLUDED
#define PHYSMEM_IS_INCLUDED
/* { */

#include <vector>
#include <string>

#ifdef REG_NONE  // F**king Windows headers!
#undef REG_NONE
#endif

#include "i486.h"
#include "i486debug.h"

#include "device.h"
#include "ramrom.h"
#include "townsdef.h"



class TownsMemAccess : public MemoryAccess
{
public:
	class TownsPhysicalMemory *physMemPtr;
	class i486DX *cpuPtr;
	TownsMemAccess();
	void SetPhysicalMemoryPointer(TownsPhysicalMemory *ptr);
	void SetCPUPointer(class i486DX *cpuPtr);
};


class TownsMainRAMAccess : public TownsMemAccess
{
public:
	virtual unsigned int FetchByte(unsigned int physAddr) const;
	virtual unsigned int FetchWord(unsigned int physAddr) const;
	virtual unsigned int FetchDword(unsigned int physAddr) const;
	virtual void StoreByte(unsigned int physAddr,unsigned char data);
	virtual void StoreWord(unsigned int physAddr,unsigned int data);
	virtual void StoreDword(unsigned int physAddr,unsigned int data);

	virtual ConstPointer GetReadAccessPointer(unsigned int physAddr) const;
};

class TownsMappedSysROMAccess : public TownsMemAccess
{
public:
	virtual unsigned int FetchByte(unsigned int physAddr) const;
	virtual unsigned int FetchWord(unsigned int physAddr) const;
	virtual unsigned int FetchDword(unsigned int physAddr) const;
	virtual void StoreByte(unsigned int physAddr,unsigned char data);
	virtual void StoreWord(unsigned int physAddr,unsigned int data);
	virtual void StoreDword(unsigned int physAddr,unsigned int data);

	virtual ConstPointer GetReadAccessPointer(unsigned int physAddr) const;
};

class TownsFMRVRAMAccess : public TownsMemAccess
{
public:
	bool breakOnFMRVRAMWrite,breakOnFMRVRAMRead;
	bool breakOnCVRAMWrite,breakOnCVRAMRead;
	TownsFMRVRAMAccess();
	virtual unsigned int FetchByte(unsigned int physAddr) const;
	virtual unsigned int FetchWord(unsigned int physAddr) const;
	virtual unsigned int FetchDword(unsigned int physAddr) const;
	virtual void StoreByte(unsigned int physAddr,unsigned char data);
	virtual void StoreWord(unsigned int physAddr,unsigned int data);
	virtual void StoreDword(unsigned int physAddr,unsigned int data);
};

class TownsMappedDICROMandCMOSRAMAccess : public TownsMemAccess
{
public:
	virtual unsigned int FetchByte(unsigned int physAddr) const;
	virtual void StoreByte(unsigned int physAddr,unsigned char data);
};

class TownsNativeDICROMAccess : public TownsMemAccess
{
public:
	virtual unsigned int FetchByte(unsigned int physAddr) const;
	virtual void StoreByte(unsigned int physAddr,unsigned char data);
};

class TownsNativeCMOSRAMAccess : public TownsMemAccess
{
public:
	virtual unsigned int FetchByte(unsigned int physAddr) const;
	virtual void StoreByte(unsigned int physAddr,unsigned char data);
};

template <const unsigned int VRAMADDR_BASE,const unsigned int VRAMADDR_END>
class TownsVRAMAccessTemplate : public TownsMemAccess
{
public:
	virtual unsigned int FetchByte(unsigned int physAddr) const
	{
		auto &state=physMemPtr->state;
		auto offset=physAddr-VRAMADDR_BASE;
		if(offset<VRAMADDR_END-VRAMADDR_BASE)
		{
			return state.VRAM[offset];
		}
		return 0xff;
	}
	virtual unsigned int FetchWord(unsigned int physAddr) const
	{
		auto &state=physMemPtr->state;
		auto offset=physAddr-VRAMADDR_BASE;
		if(offset<VRAMADDR_END-VRAMADDR_BASE-1)
		{
			return state.VRAM[offset]|(state.VRAM[offset+1]<<8);
		}
		else if(offset<VRAMADDR_END-VRAMADDR_BASE)
		{
			return state.VRAM[offset];
		}
		return 0xffff;
	}
	virtual unsigned int FetchDword(unsigned int physAddr) const
	{
		auto &state=physMemPtr->state;
		auto offset=physAddr-VRAMADDR_BASE;
		if(offset<VRAMADDR_END-VRAMADDR_BASE-3)
		{
			return state.VRAM[offset]|
			      (state.VRAM[offset+1]<<8)|
			      (state.VRAM[offset+2]<<16)|
			      (state.VRAM[offset+3]<<24);
		}
		else if(offset<VRAMADDR_END-VRAMADDR_BASE-2)
		{
			return state.VRAM[offset]|
			      (state.VRAM[offset+1]<<8)|
			      (state.VRAM[offset+2]<<16);
		}
		else if(offset<VRAMADDR_END-VRAMADDR_BASE-1)
		{
			return state.VRAM[offset]|
			      (state.VRAM[offset+1]<<8);
		}
		else if(offset<VRAMADDR_END-VRAMADDR_BASE)
		{
			return state.VRAM[offset];
		}
		return 0xffffffff;
	}
	virtual void StoreByte(unsigned int physAddr,unsigned char data)
	{
		auto &state=physMemPtr->state;
		auto offset=physAddr-VRAMADDR_BASE;
		if(offset<VRAMADDR_END-VRAMADDR_BASE)
		{
			state.VRAM[offset]=data;
		}
	}
	virtual void StoreWord(unsigned int physAddr,unsigned int data)
	{
		auto &state=physMemPtr->state;
		auto offset=physAddr-VRAMADDR_BASE;
		if(offset<VRAMADDR_END-VRAMADDR_BASE-1)
		{
			state.VRAM[offset  ]= data    &255;
			state.VRAM[offset+1]=(data>>8)&255;
		}
		else if(offset<VRAMADDR_END-VRAMADDR_BASE)
		{
			state.VRAM[offset]=data;
		}
	}
	virtual void StoreDword(unsigned int physAddr,unsigned int data)
	{
		auto &state=physMemPtr->state;
		auto offset=physAddr-VRAMADDR_BASE;
		if(offset<VRAMADDR_END-VRAMADDR_BASE-3)
		{
			state.VRAM[offset  ]= data     &255;
			state.VRAM[offset+1]=(data>>8) &255;
			state.VRAM[offset+2]=(data>>16)&255;
			state.VRAM[offset+3]=(data>>24)&255;
		}
		else if(offset<VRAMADDR_END-VRAMADDR_BASE-2)
		{
			state.VRAM[offset  ]= data     &255;
			state.VRAM[offset+1]=(data>>8) &255;
			state.VRAM[offset+2]=(data>>16)&255;
		}
		else if(offset<VRAMADDR_END-VRAMADDR_BASE-1)
		{
			state.VRAM[offset  ]= data    &255;
			state.VRAM[offset+1]=(data>>8)&255;
		}
		else if(offset<VRAMADDR_END-VRAMADDR_BASE)
		{
			state.VRAM[offset]=data;
		}
	}
};

class TownsSpriteRAMAccess : public TownsMemAccess
{
public:
	virtual unsigned int FetchByte(unsigned int physAddr) const;
	virtual void StoreByte(unsigned int physAddr,unsigned char data);
};

class TownsOsROMAccess : public TownsMemAccess
{
public:
	virtual unsigned int FetchByte(unsigned int physAddr) const;
	virtual unsigned int FetchWord(unsigned int physAddr) const;
	virtual unsigned int FetchDword(unsigned int physAddr) const;
	virtual void StoreByte(unsigned int physAddr,unsigned char data);
};

class TownsFontROMAccess : public TownsMemAccess
{
public:
	virtual unsigned int FetchByte(unsigned int physAddr) const;
	virtual void StoreByte(unsigned int physAddr,unsigned char data);
};

class TownsWaveRAMAccess : public TownsMemAccess
{
public:
	virtual unsigned int FetchByte(unsigned int physAddr) const;
	virtual void StoreByte(unsigned int physAddr,unsigned char data);

	class RF5C68 *pcmPtr;
	TownsWaveRAMAccess(class RF5C68 *pcmPtr);
};

class TownsSysROMAccess : public TownsMemAccess
{
public:
	virtual unsigned int FetchByte(unsigned int physAddr) const;
	virtual void StoreByte(unsigned int physAddr,unsigned char data);

	virtual ConstPointer GetReadAccessPointer(unsigned int physAddr) const;
};


template <class MemAccessClass>
class TownsMemAccessDebug : public MemAccessClass
{
public:
	bool breakOnWrite=false;
	bool breakOnRead=false;

	virtual unsigned int FetchByte(unsigned int physAddr) const
	{
		if(true==breakOnRead && nullptr!=this->cpuPtr->debuggerPtr)
		{
			this->cpuPtr->debuggerPtr->ExternalBreak("Memory Read");
		}
		return MemAccessClass::FetchByte(physAddr);
	}
	virtual unsigned int FetchWord(unsigned int physAddr) const
	{
		if(true==breakOnRead && nullptr!=this->cpuPtr->debuggerPtr)
		{
			this->cpuPtr->debuggerPtr->ExternalBreak("Memory Read");
		}
		return MemAccessClass::FetchWord(physAddr);
	}
	virtual unsigned int FetchDword(unsigned int physAddr) const
	{
		if(true==breakOnRead && nullptr!=this->cpuPtr->debuggerPtr)
		{
			this->cpuPtr->debuggerPtr->ExternalBreak("Memory Read");
		}
		return MemAccessClass::FetchDword(physAddr);
	}
	virtual void StoreByte(unsigned int physAddr,unsigned char data)
	{
		if(true==breakOnWrite && nullptr!=this->cpuPtr->debuggerPtr)
		{
			this->cpuPtr->debuggerPtr->ExternalBreak("Memory Write");
		}
		MemAccessClass::StoreByte(physAddr,data);
	}
	virtual void StoreWord(unsigned int physAddr,unsigned int data)
	{
		if(true==breakOnWrite && nullptr!=this->cpuPtr->debuggerPtr)
		{
			this->cpuPtr->debuggerPtr->ExternalBreak("Memory Write");
		}
		MemAccessClass::StoreWord(physAddr,data);
	}
	virtual void StoreDword(unsigned int physAddr,unsigned int data)
	{
		if(true==breakOnWrite && nullptr!=this->cpuPtr->debuggerPtr)
		{
			this->cpuPtr->debuggerPtr->ExternalBreak("Memory Write");
		}
		MemAccessClass::StoreDword(physAddr,data);
	}
};



class TownsPhysicalMemory : public Device
{
public:
	class KanjiROMAccess
	{
	public:
		unsigned char JISCodeHigh; // 000CFF94 Big Endian?
		unsigned char JISCodeLow;  // 000CFF95
		int row;

		void Reset();
		inline unsigned int JISCode(){return JISCodeLow|(JISCodeHigh<<8);}
		inline unsigned int FontROMCode(void)
		{
			if(JISCodeHigh<0x28)
			{
				// 32x8 Blocks
				unsigned int BLK=(JISCodeLow-0x20)>>5;
				unsigned int x=JISCodeLow&0x1F;
				unsigned int y=JISCodeHigh&7;
				if(BLK==1)
				{
					BLK=2;
				}
				else if(BLK==2)
				{
					BLK=1;
				}
				return BLK*32*8+y*32+x;
			}
			else
			{
				// 32x16 Blocks;
				unsigned int BlkX=(JISCodeLow-0x20)>>5;
				unsigned int BlkY=(JISCodeHigh-0x30)>>4;
				unsigned int BLK=BlkY*3+BlkX;
				unsigned int x=JISCodeLow&0x1F;
				unsigned int y=JISCodeHigh&0x0F;
				return 0x400+BLK*32*16+y*32+x;
			}
		}
	};

	class State
	{
	public:
		// Memo for myself:  When implementing state load, make sure to refresh mapping 
		//                   according to sysRomMapping, dicRom, and FMRVRAM flags.
		bool sysRomMapping;  // Whenever changing this flag, synchronously change memory access mapping.
		bool dicRom;
		unsigned int DICROMBank;
		bool FMRVRAM;
		bool TVRAMWrite;
		bool ANKFont;
		unsigned int FMRVRAMMask;
		unsigned int FMRDisplayMode;
		unsigned int FMRVRAMWriteOffset;

		std::vector <unsigned char> RAM;
		std::vector <unsigned char> VRAM;
		std::vector <unsigned char> CVRAM;
		std::vector <unsigned char> spriteRAM;
		std::vector <unsigned char> waveRAM;
		unsigned char CMOSRAM[TOWNS_CMOS_SIZE];

		KanjiROMAccess kanjiROMAccess;

		void Reset(void);
	};

	State state;
	std::vector <unsigned char> sysRom,dosRom,fontRom,font20Rom,dicRom;
	enum
	{
		SERIAL_ROM_LENGTH=32
	};
	unsigned char serialROM[SERIAL_ROM_LENGTH];

	// bool preventCMOSInitToSingleDriveMode=true;

	bool takeJISCodeLog;
	std::vector <unsigned char> JISCodeLog; // Log KanjiROM Read Access

	class i486DX *cpuPtr;
	class Memory *memPtr;

	TownsMainRAMAccess mainRAMAccess;
	TownsMappedSysROMAccess mappedSysROMAccess;
	TownsFMRVRAMAccess FMRVRAMAccess;
	TownsMappedDICROMandCMOSRAMAccess mappedDicROMandDicRAMAccess;
	TownsNativeDICROMAccess nativeDicROMAccess;
	TownsNativeCMOSRAMAccess nativeCMOSRAMAccess;
	TownsFontROMAccess fontROMAccess;

	TownsVRAMAccessTemplate <TOWNSADDR_VRAM0_BASE        ,TOWNSADDR_VRAM0_END        > VRAMAccess0;
	TownsVRAMAccessTemplate <TOWNSADDR_VRAM1_BASE        ,TOWNSADDR_VRAM1_END        > VRAMAccess1;
	TownsVRAMAccessTemplate <TOWNSADDR_VRAM_HIGHRES0_BASE,TOWNSADDR_VRAM_HIGHRES0_END> VRAMAccessHighRes0;
	TownsVRAMAccessTemplate <TOWNSADDR_VRAM_HIGHRES1_BASE,TOWNSADDR_VRAM_HIGHRES1_END> VRAMAccessHighRes1;
	TownsVRAMAccessTemplate <TOWNSADDR_VRAM_HIGHRES2_BASE,TOWNSADDR_VRAM_HIGHRES2_END> VRAMAccessHighRes2;

	TownsMemAccessDebug <TownsVRAMAccessTemplate <TOWNSADDR_VRAM0_BASE        ,TOWNSADDR_VRAM0_END        > > VRAMAccess0Debug;
	TownsMemAccessDebug <TownsVRAMAccessTemplate <TOWNSADDR_VRAM1_BASE        ,TOWNSADDR_VRAM1_END        > > VRAMAccess1Debug;
	TownsMemAccessDebug <TownsVRAMAccessTemplate <TOWNSADDR_VRAM_HIGHRES0_BASE,TOWNSADDR_VRAM_HIGHRES0_END> > VRAMAccessHighRes0Debug;
	TownsMemAccessDebug <TownsVRAMAccessTemplate <TOWNSADDR_VRAM_HIGHRES1_BASE,TOWNSADDR_VRAM_HIGHRES1_END> > VRAMAccessHighRes1Debug;
	TownsMemAccessDebug <TownsVRAMAccessTemplate <TOWNSADDR_VRAM_HIGHRES2_BASE,TOWNSADDR_VRAM_HIGHRES2_END> > VRAMAccessHighRes2Debug;


	TownsSpriteRAMAccess spriteRAMAccess;
	TownsOsROMAccess osROMAccess;
	TownsWaveRAMAccess waveRAMAccess;
	TownsSysROMAccess sysROMAccess;


	virtual const char *DeviceName(void) const{return "MEMORY";}



	TownsPhysicalMemory(class FMTowns *townsPtr,class i486DX *cpuPtr,class Memory *memPtr,class RF5C68 *pcmPtr);

	bool LoadROMImages(const char dirName[]);

	/*! Set CMOS image.
	*/
	void SetCMOS(const std::vector <unsigned char> &cmos);

	/*! Sets the main RAM size.
	*/
	void SetMainRAMSize(long long int size);

	/*! Sets the VRAM size.
	*/
	void SetVRAMSize(long long int size);

	/*! Sets the Character VRAM size.
	*/
	void SetCVRAMSize(long long int size);

	/*! Sets the SpriteRAM size.
	*/
	void SetSpriteRAMSize(long long int size);

	/*! Sets the WaveRAM size.
	*/
	void SetWaveRAMSize(long long int size);

	/*!
	*/
	void SetUpMemoryAccess(void);

	/*!
	*/
	void SetUpVRAMAccess(bool breakOnRead,bool breakOnWrite);

	/*! 
	*/
	void SetSysROMDicROMMappingFlag(bool sysRomMapping,bool dicRomMapping);

	/*!
	*/
	void SetFMRVRAMMappingFlag(bool FMRVRAMMapping);

	virtual void IOWriteByte(unsigned int ioport,unsigned int data);
	virtual unsigned int IOReadByte(unsigned int ioport);
	void Reset(void);



	std::vector <std::string> GetStatusText(void) const;
};


/* } */
#endif
