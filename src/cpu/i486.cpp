/* LICENSE>>
Copyright 2020 Soji Yamakawa (CaptainYS, http://www.ysflight.com)

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

<< LICENSE */
#include <iostream>

#include "cpputil.h"
#include "i486.h"


const char *const i486DX::Reg8Str[8]=
{
	"AL","CL","DL","BL","AH","CH","DH","BH"
};

const char *const i486DX::Reg16Str[8]=
{
	"AX","CX","DX","BX","SP","BP","SI","DI"
};

const char *const i486DX::Reg32Str[8]=
{
	"EAX","ECX","EDX","EBX","ESP","EBP","ESI","EDI"
};

const char *const i486DX::Sreg[8]=
{
	"ES","CS","SS","DS","FS","GS"
};

const char *const i486DX::RegToStr[REG_TOTAL_NUMBER_OF_REGISTERS]=
{
	"(none)",

	"EAX",
	"ECX",
	"EDX",
	"EBX",
	"ESP",
	"EBP",
	"ESI",
	"EDI",

	"AL",
	"CL",
	"DL",
	"BL",
	"AH",
	"CH",
	"DH",
	"BH",

	"AX",
	"CX",
	"DX",
	"BX",
	"SP",
	"BP",
	"SI",
	"DI",

	"EIP",
	"EFLAGS",

	"ES",
	"CS",
	"SS",
	"DS",
	"FS",
	"GS",
	"GDT",
	"LDT",
	"TR",
	"IDTR",
	"CR0",
	"CR1",
	"CR2",
	"CR3",
	"DR0",
	"DR1",
	"DR2",
	"DR3",
	"DR4",
	"DR5",
	"DR6",
	"DR7",

	"TEST0",
	"TEST1",
	"TEST2",
	"TEST3",
	"TEST4",
	"TEST5",
	"TEST6",
	"TEST7",
};

const bool i486DX::ParityTable[256]=
{
true, false,false,true, false,true, true, false,false,true, true, false,true, false,false,true,
false,true, true, false,true, false,false,true, true, false,false,true, false,true, true, false,
false,true, true, false,true, false,false,true, true, false,false,true, false,true, true, false,
true, false,false,true, false,true, true, false,false,true, true, false,true, false,false,true,
false,true, true, false,true, false,false,true, true, false,false,true, false,true, true, false,
true, false,false,true, false,true, true, false,false,true, true, false,true, false,false,true,
true, false,false,true, false,true, true, false,false,true, true, false,true, false,false,true,
false,true, true, false,true, false,false,true, true, false,false,true, false,true, true, false,
false,true, true, false,true, false,false,true, true, false,false,true, false,true, true, false,
true, false,false,true, false,true, true, false,false,true, true, false,true, false,false,true,
true, false,false,true, false,true, true, false,false,true, true, false,true, false,false,true,
false,true, true, false,true, false,false,true, true, false,false,true, false,true, true, false,
true, false,false,true, false,true, true, false,false,true, true, false,true, false,false,true,
false,true, true, false,true, false,false,true, true, false,false,true, false,true, true, false,
false,true, true, false,true, false,false,true, true, false,false,true, false,true, true, false,
true, false,false,true, false,true, true, false,false,true, true, false,true, false,false,true,
};

std::string i486DX::FarPointer::Format(void) const
{
	return cpputil::Uitox(SEG)+":"+cpputil::Uitox(OFFSET);
}

void i486DX::FarPointer::MakeFromString(const std::string &str)
{
	Nullify();
	for(unsigned int i=0; i<str.size(); ++i)
	{
		if(':'==str[i])
		{
			this->OFFSET=cpputil::Xtoi(str.data()+i+1);

			auto segPart=str;
			segPart.resize(i);
			cpputil::Capitalize(segPart);
			if("CS"==segPart)
			{
				this->SEG=i486DX::FarPointer::SEG_REGISTER|i486DX::REG_CS;
			}
			else if("SS"==segPart)
			{
				this->SEG=i486DX::FarPointer::SEG_REGISTER|i486DX::REG_SS;
			}
			else if("DS"==segPart)
			{
				this->SEG=i486DX::FarPointer::SEG_REGISTER|i486DX::REG_DS;
			}
			else if("ES"==segPart)
			{
				this->SEG=i486DX::FarPointer::SEG_REGISTER|i486DX::REG_ES;
			}
			else if("FS"==segPart)
			{
				this->SEG=i486DX::FarPointer::SEG_REGISTER|i486DX::REG_FS;
			}
			else if("GS"==segPart)
			{
				this->SEG=i486DX::FarPointer::SEG_REGISTER|i486DX::REG_GS;
			}
			else if("PHYS"==segPart || "P"==segPart)
			{
				this->SEG=i486DX::FarPointer::PHYS_ADDR;
			}
			else if("LINE"==segPart || "L"==segPart)
			{
				this->SEG=i486DX::FarPointer::LINEAR_ADDR;
			}
			else if('R'==segPart[0] || 'r'==segPart[0])
			{
				this->SEG=i486DX::FarPointer::REAL_ADDR|cpputil::Xtoi(str.data()+1);
			}
			else
			{
				this->SEG=cpputil::Xtoi(str.data());
			}
			return;
		}
	}

	this->SEG=i486DX::FarPointer::NO_SEG;
	this->OFFSET=cpputil::Xtoi(str.data());
}

i486DX::FarPointer i486DX::TranslateFarPointer(FarPointer ptr) const
{
	if(ptr.SEG==FarPointer::NO_SEG)
	{
		ptr.SEG=state.CS().value;
	}
	else if((ptr.SEG&0xFFFF0000)==FarPointer::SEG_REGISTER)
	{
		ptr.SEG=GetRegisterValue(ptr.SEG&0xFFFF);
	}
	return ptr;
}

void i486DX::FarPointer::LoadSegmentRegister(SegmentRegister &seg,i486DX &cpu,const Memory &mem) const
{
	if(SEG==NO_SEG)
	{
		seg=cpu.state.CS();
	}
	else if(0==(SEG&0xFFFF0000))
	{
		cpu.LoadSegmentRegister(seg,SEG&0xFFFF,mem);
	}
	else if((SEG&0xFFFF0000)==SEG_REGISTER)
	{
		seg=cpu.state.GetSegmentRegister(SEG&0xFFFF);
	}
	else if((SEG&0xFFFF0000)==LINEAR_ADDR)
	{
		seg.value=0;
		seg.baseLinearAddr=0;
		seg.operandSize=32;
		seg.addressSize=32;
		seg.limit=0xFFFFFFFF;
	}
	else if((SEG&0xFFFF0000)==REAL_ADDR)
	{
		seg.value=SEG&0xFFFF;
		seg.baseLinearAddr=seg.value*0x10;
		seg.operandSize=16;
		seg.addressSize=16;
		seg.limit=0xFFFF;
	}
}



////////////////////////////////////////////////////////////


i486DX::i486DX(VMBase *vmPtr) : CPU(vmPtr)
{
	state.NULL_and_reg32[ 0]=0;
	state.NULL_and_reg32[16]=0;

	Reset();
	enableCallStack=false;
	debuggerPtr=nullptr;

	for(auto &sregPtr : baseRegisterToDefaultSegment)
	{
		sregPtr=&state.sreg[REG_DS-REG_SEGMENT_REG_BASE];
	}
	baseRegisterToDefaultSegment[REG_SP]=&state.sreg[REG_SS-REG_SEGMENT_REG_BASE];
	baseRegisterToDefaultSegment[REG_BP]=&state.sreg[REG_SS-REG_SEGMENT_REG_BASE];
	baseRegisterToDefaultSegment[REG_ESP]=&state.sreg[REG_SS-REG_SEGMENT_REG_BASE];
	baseRegisterToDefaultSegment[REG_EBP]=&state.sreg[REG_SS-REG_SEGMENT_REG_BASE];

	for(int i=0; i<sizeof(state.sreg)/sizeof(state.sreg[0]); ++i)
	{
		sregIndexToSregPtrTable[i]=&state.sreg[i];
	}
	sregIndexToSregPtrTable[NUM_SEGMENT_REGISTERS]=nullptr;

	for(auto &i : segPrefixToSregIndex)
	{
		i=NUM_SEGMENT_REGISTERS;
	}
	segPrefixToSregIndex[SEG_OVERRIDE_CS]=REG_CS-REG_SEGMENT_REG_BASE;
	segPrefixToSregIndex[SEG_OVERRIDE_SS]=REG_SS-REG_SEGMENT_REG_BASE;
	segPrefixToSregIndex[SEG_OVERRIDE_DS]=REG_DS-REG_SEGMENT_REG_BASE;
	segPrefixToSregIndex[SEG_OVERRIDE_ES]=REG_ES-REG_SEGMENT_REG_BASE;
	segPrefixToSregIndex[SEG_OVERRIDE_FS]=REG_FS-REG_SEGMENT_REG_BASE;
	segPrefixToSregIndex[SEG_OVERRIDE_GS]=REG_GS-REG_SEGMENT_REG_BASE;
}

void i486DX::Reset(void)
{
	// page 10-1 [1]
	state.EFLAGS=RESET_EFLAGS;

	state.EIP=RESET_EIP;
	state.CS().value=RESET_CS;
	state.CS().baseLinearAddr=0xFFFF0000;

	LoadSegmentRegisterRealMode(state.DS(),RESET_DS);
	LoadSegmentRegisterRealMode(state.SS(),RESET_SS);
	LoadSegmentRegisterRealMode(state.ES(),RESET_ES);
	LoadSegmentRegisterRealMode(state.FS(),RESET_FS);
	LoadSegmentRegisterRealMode(state.GS(),RESET_GS);

	state.GDTR.linearBaseAddr=RESET_GDTRBASE;
	state.GDTR.limit=RESET_GDTRLIMIT;

	state.IDTR.linearBaseAddr=RESET_IDTRBASE;
	state.IDTR.limit=RESET_IDTRLIMIT;

	state.LDTR.linearBaseAddr=RESET_LDTRBASE;
	state.LDTR.limit=RESET_LDTRLIMIT;
	state.LDTR.selector=RESET_LDTRSELECTOR;

	state.TR.linearBaseAddr=RESET_TRBASE;
	state.TR.limit=RESET_TRLIMIT;
	state.TR.selector=RESET_TRSELECTOR;
	state.TR.attrib=RESET_TRATTRIB;

	state.DR[7]=RESET_DR7;

	state.EAX()=RESET_EAX;
	SetDX(RESET_DX);
	SetCR(0,RESET_CR0);
	if(true!=state.fpuState.FPUEnabled)
	{
		auto cr0=state.GetCR(0);
		cr0&=(~CR0_MATH_PRESENT);
		SetCR(0,cr0);
	}
	// Better than leaving it uninitialized
	SetCR(1,0);
	SetCR(2,0);
	state.pageDirectoryCache.Clear();

	for(auto &t : state.TEST)
	{
		t=0;
	}

	state.halt=false;
	state.holdIRQ=false;
	state.exception=false;
}

std::vector <std::string> i486DX::GetStateText(void) const
{
	std::vector <std::string> text;

	text.push_back(
	     "CS:EIP="
	    +cpputil::Ustox(state.CS().value)+":"+cpputil::Uitox(state.EIP)
	    +"  LINEAR:"+cpputil::Uitox(state.CS().baseLinearAddr+state.EIP)
	    +"  EFLAGS="+cpputil::Uitox(state.EFLAGS));

	text.push_back(
	     "EAX="+cpputil::Uitox(state.EAX())
	    +"  EBX="+cpputil::Uitox(state.EBX())
	    +"  ECX="+cpputil::Uitox(state.ECX())
	    +"  EDX="+cpputil::Uitox(state.EDX())
	    );

	text.push_back(
	     "ESI="+cpputil::Uitox(state.ESI())
	    +"  EDI="+cpputil::Uitox(state.EDI())
	    +"  EBP="+cpputil::Uitox(state.EBP())
	    +"  ESP="+cpputil::Uitox(state.ESP())
	    );

	text.push_back(
	     "CS="+cpputil::Ustox(state.CS().value)
	    +"(LIN:"+cpputil::Uitox(state.CS().baseLinearAddr)+")"
	    +"  "
	     "DS="+cpputil::Ustox(state.DS().value)
	    +"(LIN:"+cpputil::Uitox(state.DS().baseLinearAddr)+")"
	    +"  "
	     "ES="+cpputil::Ustox(state.ES().value)
	    +"(LIN:"+cpputil::Uitox(state.ES().baseLinearAddr)+")"
	    +"  ");

	text.push_back(
	     "FS="+cpputil::Ustox(state.FS().value)
	    +"(LIN:"+cpputil::Uitox(state.FS().baseLinearAddr)+")"
	    +"  "
	     "GS="+cpputil::Ustox(state.GS().value)
	    +"(LIN:"+cpputil::Uitox(state.GS().baseLinearAddr)+")"
	    +"  "
	    +"SS="+cpputil::Ustox(state.SS().value)
	    +"(LIN:"+cpputil::Uitox(state.SS().baseLinearAddr)+")"
	    +"  ");

	text.push_back(
	     "CR0="+cpputil::Uitox(state.GetCR(0))
	    +"  CR1="+cpputil::Uitox(state.GetCR(1))
	    +"  CR2="+cpputil::Uitox(state.GetCR(2))
	    +"  CR3="+cpputil::Uitox(state.GetCR(3))
	    );

	text.push_back(
	     std::string("CF")+cpputil::BoolToNumberStr(GetCF())
	    +"  PF"+cpputil::BoolToNumberStr(GetPF())
	    +"  AF"+cpputil::BoolToNumberStr(GetAF())
	    +"  ZF"+cpputil::BoolToNumberStr(GetZF())
	    +"  SF"+cpputil::BoolToNumberStr(GetSF())
	    +"  TF"+cpputil::BoolToNumberStr(GetTF())
	    +"  IF"+cpputil::BoolToNumberStr(GetIF())
	    +"  DF"+cpputil::BoolToNumberStr(GetDF())
	    +"  OF"+cpputil::BoolToNumberStr(GetOF())
	    +"  IOPL"+cpputil::Ubtox(GetIOPL())
	    +"  NT"+cpputil::BoolToNumberStr(GetNT())
	    +"  RF"+cpputil::BoolToNumberStr(GetRF())
	    +"  VM"+cpputil::BoolToNumberStr(GetVM())
	    +"  AC"+cpputil::BoolToNumberStr(GetAC())
	    );

	text.push_back(
	     std::string("Default Operand Size=")+cpputil::Itoa(state.CS().operandSize)
	    +std::string("  Default Address Size=")+cpputil::Itoa(state.CS().addressSize)
	    +std::string("  Stack Address Size=")+cpputil::Itoa(state.SS().addressSize));

	if(true==state.exception)
	{
		text.push_back("!EXCEPTION!");
	}
	if(true==state.holdIRQ)
	{
		text.push_back("HOLD IRQ for 1 Instruction");
	}

	return text;
}

std::vector <std::string> i486DX::GetGDTText(const Memory &mem) const
{
	std::vector <std::string> text;
	text.push_back("GDT  Limit=");
	text.back()+=cpputil::Ustox(state.GDTR.limit);
	text.back()+="  LinearBase=";
	text.back()+=cpputil::Uitox(state.GDTR.linearBaseAddr);


	for(unsigned int selector=0; selector<state.GDTR.limit; selector+=8)
	{
		unsigned int DTLinearBaseAddr=state.GDTR.linearBaseAddr+selector;
		const unsigned char rawDesc[8]=
		{
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr),
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr+1),
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr+2),
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr+3),
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr+4),
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr+5),
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr+6),
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr+7)
		};

		// Sample GDT from WRHIGH.ASM
		//	DB		0FFH,0FFH	; Segment Limit (0-15)
		//	DB		0,0,010H		; Base Address 0-23
		//	DB		10010010B	; P=1, DPL=00, S=1, TYPE=0010
		//	DB		11000111B	; G=1, DB=1, (Unused)=0, A=0, LIMIT 16-19=0011
		//	DB		0			; Base Address 24-31

		unsigned int segLimit=rawDesc[0]|(rawDesc[1]<<8)|((rawDesc[6]&0x0F)<<16);
		unsigned int segBase=rawDesc[2]|(rawDesc[3]<<8)|(rawDesc[4]<<16)|(rawDesc[7]<<24);
		if((0x80&rawDesc[6])==0) // G==0
		{
			segLimit=segLimit;
		}
		else
		{
			segLimit=(segLimit+1)*4096-1;
		}

		unsigned int addressSize,operandSize;
		if((0x40&rawDesc[6])==0) // D==0
		{
			addressSize=16;
			operandSize=16;
		}
		else
		{
			addressSize=32;
			operandSize=32;
		}

		std::string empty;
		text.push_back(empty);
		text.back()+=cpputil::Ustox(selector);
		text.back()+=":";
		text.back()+="Lin-Base=";
		text.back()+=cpputil::Uitox(segBase);
		text.back()+="  Limit=";
		text.back()+=cpputil::Uitox(segLimit);
		text.back()+="  OperSize=";
		text.back()+=cpputil::Ubtox(operandSize);
		text.back()+="H  AddrSize=";
		text.back()+=cpputil::Ubtox(addressSize);
		text.back()+="H";
	}
	return text;
}

std::vector <std::string> i486DX::GetLDTText(const Memory &mem) const
{
	std::vector <std::string> text;
	text.push_back("LDT  Limit=");
	text.back()+=cpputil::Ustox(state.LDTR.limit);
	text.back()+="  LinearBase=";
	text.back()+=cpputil::Uitox(state.LDTR.linearBaseAddr);
	text.back()+="  Selector=";
	text.back()+=cpputil::Uitox(state.LDTR.selector);


	for(unsigned int selector=0; selector<state.LDTR.limit; selector+=8)
	{
		unsigned int DTLinearBaseAddr=state.LDTR.linearBaseAddr+selector;
		const unsigned char rawDesc[8]=
		{
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr),
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr+1),
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr+2),
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr+3),
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr+4),
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr+5),
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr+6),
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr+7)
		};

		// Sample LDT from WRHIGH.ASM
		//	DB		0FFH,0FFH	; Segment Limit (0-15)
		//	DB		0,0,010H		; Base Address 0-23
		//	DB		10010010B	; P=1, DPL=00, S=1, TYPE=0010
		//	DB		11000111B	; G=1, DB=1, (Unused)=0, A=0, LIMIT 16-19=0011
		//	DB		0			; Base Address 24-31

		unsigned int segLimit=rawDesc[0]|(rawDesc[1]<<8)|((rawDesc[6]&0x0F)<<16);
		unsigned int segBase=rawDesc[2]|(rawDesc[3]<<8)|(rawDesc[4]<<16)|(rawDesc[7]<<24);
		if((0x80&rawDesc[6])==0) // G==0
		{
			segLimit=segLimit;
		}
		else
		{
			segLimit=(segLimit+1)*4096-1;
		}

		unsigned int addressSize,operandSize;
		if((0x40&rawDesc[6])==0) // D==0
		{
			addressSize=16;
			operandSize=16;
		}
		else
		{
			addressSize=32;
			operandSize=32;
		}

		std::string empty;
		text.push_back(empty);
		text.back()+=cpputil::Ustox(selector|4);
		text.back()+=":";
		text.back()+="Lin-Base=";
		text.back()+=cpputil::Uitox(segBase);
		text.back()+="  Limit=";
		text.back()+=cpputil::Uitox(segLimit);
		text.back()+="  OperSize=";
		text.back()+=cpputil::Ubtox(operandSize);
		text.back()+="H  AddrSize=";
		text.back()+=cpputil::Ubtox(addressSize);
		text.back()+="H";
	}
	return text;
}

std::vector <std::string> i486DX::GetIDTText(const Memory &mem) const
{
	std::vector <std::string> text;
	std::string empty;

	text.push_back(empty);
	text.back()="Limit="+cpputil::Uitox(state.IDTR.limit);
	for(unsigned int offset=0; offset<state.IDTR.limit && offset<0x800; offset+=8)
	{
		auto desc=GetInterruptDescriptor(offset/8,mem);
		text.push_back(empty);
		text.back()=cpputil::Ubtox(offset/8);
		text.back()+=":";
		text.back()+="SEG=";
		text.back()+=cpputil::Ustox(desc.SEG);
		text.back()+="  OFFSET=";
		text.back()+=cpputil::Uitox(desc.OFFSET);

		auto type=(desc.flags>>8)&0x1F;
		text.back()+="  TYPE=";
		text.back()+=cpputil::Ubtox(type);
		text.back()+="(";
		// https://wiki.osdev.org/Interrupt_Descriptor_Table
		switch(type)
		{
		default:
			text.back()+="UNKNOWN        ";
			break;
		case 0b0101:
			text.back()+="386 32-bit Task";
			break;
		case 0b0110:
			text.back()+="286 16-bit INT";
			break;
		case 0b0111:
			text.back()+="286 16-bit Trap";
			break;
		case 0b1110:
			text.back()+="386 32-bit INT";
			break;
		case 0b1111:
			text.back()+="386 32-bit Trap";
			break;
		}
		text.back()+=")";
	}

	return text;
}

void i486DX::PrintState(void) const
{
	for(auto &str : GetStateText())
	{
		std::cout << str << std::endl;
	}
}

void i486DX::PrintGDT(const Memory &mem) const
{
	for(auto &str : GetGDTText(mem))
	{
		std::cout << str << std::endl;
	}
}

void i486DX::PrintLDT(const Memory &mem) const
{
	for(auto &str : GetLDTText(mem))
	{
		std::cout << str << std::endl;
	}
}

void i486DX::PrintIDT(const Memory &mem) const
{
	for(auto &str : GetIDTText(mem))
	{
		std::cout << str << std::endl;
	}
}

inline void i486DX::LoadSegmentRegisterQuiet(SegmentRegister &reg,unsigned int value,const Memory &mem,bool isInRealMode) const
{
	if(true==isInRealMode)
	{
		reg.value=value;
		reg.baseLinearAddr=(value<<4);
		reg.addressSize=16;
		reg.operandSize=16;
		reg.limit=0xffff;
	}
	else
	{
		auto RPL=(value&3);
		auto TI=(0!=(value&4));
		auto INDEX=(value>>3)&0b0001111111111111;

		unsigned int DTLinearBaseAddr=0;
		if(0==TI)
		{
			DTLinearBaseAddr=state.GDTR.linearBaseAddr;
		}
		else
		{
			DTLinearBaseAddr=state.LDTR.linearBaseAddr;
		}
		DTLinearBaseAddr+=(8*INDEX);

		const unsigned char rawDesc[8]=
		{
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr),
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr+1),
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr+2),
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr+3),
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr+4),
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr+5),
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr+6),
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr+7)
		};

		// Sample GDT from WRHIGH.ASM
		//	DB		0FFH,0FFH	; Segment Limit (0-15)
		//	DB		0,0,010H		; Base Address 0-23
		//	DB		10010010B	; P=1, DPL=00, S=1, TYPE=0010
		//	DB		11000111B	; G=1, DB=1, (Unused)=0, A=0, LIMIT 16-19=0011
		//	DB		0			; Base Address 24-31

		unsigned int segLimit=rawDesc[0]|(rawDesc[1]<<8)|((rawDesc[6]&0x0F)<<16);
		unsigned int segBase=rawDesc[2]|(rawDesc[3]<<8)|(rawDesc[4]<<16)|(rawDesc[7]<<24);
		if((0x80&rawDesc[6])==0) // G==0
		{
			reg.limit=segLimit;
		}
		else
		{
			reg.limit=(segLimit+1)*4096-1;
		}
		reg.baseLinearAddr=segBase;
		reg.value=value;

		if((0x40&rawDesc[6])==0) // D==0
		{
			reg.addressSize=16;
			reg.operandSize=16;
		}
		else
		{
			reg.addressSize=32;
			reg.operandSize=32;
		}
	}
}

void i486DX::LoadSegmentRegister(SegmentRegister &reg,unsigned int value,const Memory &mem)
{
	if(&reg==&state.SS())
	{
		state.holdIRQ=true;
	}
	LoadSegmentRegisterQuiet(reg,value,mem,IsInRealMode());
}

void i486DX::LoadSegmentRegisterRealMode(SegmentRegister &reg,unsigned int value)
{
	if(&reg==&state.SS())
	{
		state.holdIRQ=true;
	}
	reg.value=value;
	reg.baseLinearAddr=(value<<4);
	reg.addressSize=16;
	reg.operandSize=16;
	reg.limit=0xffff;
}

void i486DX::LoadDescriptorTableRegister(SystemAddressRegister &reg,int operandSize,const unsigned char byteData[])
{
	reg.limit=byteData[0]|(byteData[1]<<8);
	if(16==operandSize)
	{
		reg.linearBaseAddr=byteData[2]|(byteData[3]<<8)|(byteData[4]<<16);
	}
	else
	{
		reg.linearBaseAddr=byteData[2]|(byteData[3]<<8)|(byteData[4]<<16)|(byteData[5]<<24);
	}
}

i486DX::InterruptDescriptor i486DX::GetInterruptDescriptor(unsigned int INTNum,const Memory &mem) const
{
	InterruptDescriptor desc;
	if(8*INTNum<state.IDTR.limit)
	{
		auto DTLinearBaseAddr=state.IDTR.linearBaseAddr;
		DTLinearBaseAddr+=(8*INTNum);
		const unsigned char rawDesc[8]=
		{
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr),
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr+1),
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr+2),
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr+3),
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr+4),
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr+5),
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr+6),
			(unsigned char)FetchByteByLinearAddress(mem,DTLinearBaseAddr+7)
		};
		desc.SEG=rawDesc[2]|(rawDesc[3]<<8);
		desc.OFFSET= (unsigned int)rawDesc[0]
		           |((unsigned int)rawDesc[1]<<8)
		           |((unsigned int)rawDesc[6]<<16)
		           |((unsigned int)rawDesc[7]<<24);
		desc.flags=  (unsigned short)rawDesc[4]
		           |((unsigned short)rawDesc[5]<<8);
	}
	else
	{
		desc.SEG=FarPointer::NO_SEG;
		desc.OFFSET=0;
	}
	return desc;
}

i486DX::OperandValue i486DX::DescriptorTableToOperandValue(const SystemAddressRegister &reg,int operandSize) const
{
	OperandValue operaValue;
	operaValue.numBytes=6;
	operaValue.byteData[0]=reg.limit&0xFF;
	operaValue.byteData[1]=(reg.limit>>8)&0xFF;
	operaValue.byteData[2]=reg.linearBaseAddr&0xFF;
	operaValue.byteData[3]=(reg.linearBaseAddr>>8)&0xFF;
	operaValue.byteData[4]=(reg.linearBaseAddr>>16)&0xFF;
	operaValue.byteData[5]=(reg.linearBaseAddr>>24)&0xFF;
	if(16==operandSize)
	{
		operaValue.byteData[5]=0;
	}
	return operaValue;
}

/* static */ unsigned int i486DX::GetRegisterSize(int reg)
{
	switch(reg)
	{
	case REG_AL:
	case REG_CL:
	case REG_DL:
	case REG_BL:
	case REG_AH:
	case REG_CH:
	case REG_DH:
	case REG_BH:
		return 1;

	case REG_AX:
	case REG_CX:
	case REG_DX:
	case REG_BX:
	case REG_SP:
	case REG_BP:
	case REG_SI:
	case REG_DI:
		return 2;

	case REG_EAX:
	case REG_ECX:
	case REG_EDX:
	case REG_EBX:
	case REG_ESP:
	case REG_EBP:
	case REG_ESI:
	case REG_EDI:
	case REG_EIP:
	case REG_EFLAGS:
		return 4;

	case REG_ES:
	case REG_CS:
	case REG_SS:
	case REG_DS:
	case REG_FS:
	case REG_GS:
		return 2;

	//case REG_GDT:
	//case REG_LDT:
	//case REG_TR:
	//case REG_IDTR:

	case REG_CR0:
	case REG_CR1:
	case REG_CR2:
	case REG_CR3:
	case REG_DR0:
	case REG_DR1:
	case REG_DR2:
	case REG_DR3:
	case REG_DR4:
	case REG_DR5:
	case REG_DR6:
	case REG_DR7:
		return 4;

	case REG_TEST0:
	case REG_TEST1:
	case REG_TEST2:
	case REG_TEST3:
	case REG_TEST4:
	case REG_TEST5:
	case REG_TEST6:
	case REG_TEST7:
		return 4;
	}
	return 0;
}

unsigned int i486DX::GetStackAddressingSize(void) const
{
	if(true==IsInRealMode())
	{
		return 16;
	}
	else
	{
		return state.SS().addressSize;
	}
	return 0;
}

void i486DX::Push(Memory &mem,unsigned int operandSize,unsigned int value)
{
	auto addressSize=GetStackAddressingSize();
	auto &ESP=state.ESP();
	// When addressSize==16, ESP will be &ed with 0xFFFF in StoreWord/StoreDword.
	// Also ESP crossing 16-bit boundary would be an exception if addressSize==16.
	// I cannot check it here, but to run a valid application, it shouldn't happen.
	if(16==operandSize)
	{
		ESP-=2;
		StoreWord(mem,addressSize,state.SS(),ESP,value);
	}
	else if(32==operandSize)
	{
		ESP-=4;
		StoreDword(mem,addressSize,state.SS(),ESP,value);
	}
}

unsigned int i486DX::Pop(Memory &mem,unsigned int operandSize)
{
	unsigned int value;
	auto addressSize=GetStackAddressingSize();
	auto &ESP=state.ESP();
	// When addressSize==16, ESP will be &ed with 0xFFFF in StoreWord/StoreDword.
	// Also ESP crossing 16-bit boundary would be an exception if addressSize==16.
	// I cannot check it here, but to run a valid application, it shouldn't happen.
	if(16==operandSize)
	{
		value=FetchWord(addressSize,state.SS(),ESP,mem);
		ESP+=2;
	}
	else if(32==operandSize)
	{
		value=FetchDword(addressSize,state.SS(),ESP,mem);
		ESP+=4;
	}
	return value;
}

std::string i486DX::Disassemble(const Instruction &inst,const Operand &op1,const Operand &op2,SegmentRegister seg,unsigned int offset,const Memory &mem,const class i486SymbolTable &symTable) const
{
	std::string disasm;
	disasm+=cpputil::Ustox(seg.value);
	disasm+=":";
	disasm+=cpputil::Uitox(offset);
	disasm+=" ";

	for(unsigned int i=0; i<inst.numBytes; ++i)
	{
		disasm+=cpputil::Ubtox(DebugFetchByte(seg.addressSize,seg,offset+i,mem));
	}
	disasm+=" ";

	cpputil::ExtendString(disasm,40);
	disasm+=inst.Disassemble(op1,op2,seg,offset,symTable);

	return disasm;
}

std::string i486DX::DisassembleData(unsigned int addressSize,SegmentRegister seg,unsigned int offset,const Memory &mem,unsigned int unitBytes,unsigned int segBytes,unsigned int repeat,unsigned int chopOff) const
{
	if(4<unitBytes)
	{
		return "4+ unit byte count not supported";
	}
	if(4<segBytes)
	{
		return "4+ segment byte count not supported";
	}

	std::string disasm;

	unsigned int perLine;
	if(1==unitBytes && 0==segBytes)
	{
		perLine=8;
	}
	else
	{
		perLine=1;
	}
	for(unsigned int i=0; i<repeat && i<chopOff; ++i)
	{
		if(0==i)
		{
			disasm+=cpputil::Ustox(seg.value);
			disasm+=":";
			disasm+=cpputil::Uitox(offset);
			disasm+=" ";
		}
		else if(0==(i%perLine))
		{
			disasm+="              ";
		}

		if(1==unitBytes && 0==segBytes)
		{
			disasm+=cpputil::Ubtox(DebugFetchByte(addressSize,seg,offset,mem));
			++offset;
		}
		else
		{
			unsigned char dword[4],segpart[4]={0,0,0,0};
			for(int i=0; i<(int)unitBytes; ++i)
			{
				dword[i]=DebugFetchByte(addressSize,seg,offset,mem);
				++offset;
			}
			for(int i=0; i<(int)segBytes; ++i)
			{
				segpart[i]=DebugFetchByte(addressSize,seg,offset,mem);
				++offset;
			}

			if(0<segBytes)
			{
				disasm+=cpputil::Ubtox(segpart[1]);
				disasm+=cpputil::Ubtox(segpart[0]);
				disasm.push_back(':');
			}
			for(int i=unitBytes-1; 0<=i; --i)
			{
				disasm+=cpputil::Ubtox(dword[i]);
			}
		}

		if(0==(i+1)%perLine)
		{
			disasm.push_back('\n');
		}
	}
	return disasm;
}

void i486DX::Move(Memory &mem,int addressSize,int segmentOverride,const Operand &dst,const Operand &src)
{
	auto value=EvaluateOperand(mem,addressSize,segmentOverride,src,dst.GetSize());
	StoreOperandValue(dst,mem,addressSize,segmentOverride,value);
}

// OF SF ZF AF PF
void i486DX::DecrementWordOrDword(unsigned int operandSize,unsigned int &value)
{
	if(16==operandSize)
	{
		DecrementWord(value);
	}
	else
	{
		DecrementDword(value);
	}
}
void i486DX::DecrementDword(unsigned int &value)
{
	--value;
	SetOF(value==0x7FFFFFFF);
	SetSF(0!=(value&0x80000000));
	SetZF(0==value);
	SetAF(0x0F==(value&0x0F));
	SetPF(CheckParity(value&0xFF));
}
void i486DX::DecrementWord(unsigned int &value)
{
	value=((value-1)&0xFFFF);
	SetOF(value==0x7FFF);
	SetSF(0!=(value&0x8000));
	SetZF(0==value);
	SetAF(0x0F==(value&0x0F));
	SetPF(CheckParity(value&0xFF));
}
void i486DX::DecrementByte(unsigned int &value)
{
	value=((value-1)&0xFF);
	SetOF(value==0x7F);
	SetSF(0!=(value&0x80));
	SetZF(0==value);
	SetAF(0x0F==(value&0x0F));
	SetPF(CheckParity(value&0xFF));
}
void i486DX::DecrementWithMask(unsigned int &value,unsigned int mask,unsigned int signBit)
{
	value=((value-1)&mask);
	SetOF(signBit-1==value);
	SetAF(0x0F==(value&0x0F));
	SetSF(value&signBit);
	SetZF(0==value);
	SetPF(CheckParity(value&0xFF));
}

void i486DX::IncrementWordOrDword(unsigned int operandSize,unsigned int &value)
{
	if(16==operandSize)
	{
		IncrementWord(value);
	}
	else
	{
		IncrementDword(value);
	}
}
void i486DX::IncrementDword(unsigned int &value)
{
	SetAF(0x0F==(value&0x0F));
	++value;
	SetOF(value==0x80000000);
	SetSF(0!=(value&0x80000000));
	SetZF(0==value);
	SetPF(CheckParity(value&0xFF));
}
void i486DX::IncrementWord(unsigned int &value)
{
	SetAF(0x0F==(value&0x0F));
	value=(value+1)&0xffff;
	SetOF(value==0x8000);
	SetSF(0!=(value&0x8000));
	SetZF(0==value);
	SetPF(CheckParity(value&0xFF));
}
void i486DX::IncrementByte(unsigned int &value)
{
	SetAF(0x0F==(value&0x0F));
	value=(value+1)&0xff;
	SetOF(value==0x80);
	SetSF(0!=(value&0x80));
	SetZF(0==value);
	SetPF(CheckParity(value&0xFF));
}
void i486DX::IncrementWithMask(unsigned int &value,unsigned int mask,unsigned int signBit)
{
	SetAF(0x0F==(value&0x0F));
	value=(value+1)&mask;
	SetOF(value==signBit);
	SetSF(value&signBit);
	SetZF(0==value);
	SetPF(CheckParity(value&0xFF));
}



void i486DX::AddWordOrDword(int operandSize,unsigned int &value1,unsigned int value2)
{
	if(16==operandSize)
	{
		AddWord(value1,value2);
	}
	else
	{
		AddDword(value1,value2);
	}
}
void i486DX::AddDword(unsigned int &value1,unsigned int value2)
{
	auto prevValue=value1&0xffffffff;
	value1=(value1+value2)&0xffffffff;
	state.EFLAGS&=~(
		EFLAGS_OVERFLOW|
		EFLAGS_SIGN|
		EFLAGS_ZERO|
		EFLAGS_AUX_CARRY|
		EFLAGS_CARRY|
		EFLAGS_PARITY
	);
	RaiseOF((prevValue&0x80000000)==(value2&0x80000000) && (prevValue&0x80000000)!=(value1&0x80000000)); // Two sources have same sign, but the result sign is different.
	RaiseSF(0!=(value1&0x80000000));
	RaiseZF(0==value1);
	RaiseAF((prevValue&0x0F)<(value1&0x0F));
	RaiseCF(value1<prevValue);
	RaisePF(CheckParity(value1&0xFF));
}
void i486DX::AddWord(unsigned int &value1,unsigned int value2)
{
	auto prevValue=value1&0xffff;
	value1=(value1+value2)&0xffff;
	state.EFLAGS&=~(
		EFLAGS_OVERFLOW|
		EFLAGS_SIGN|
		EFLAGS_ZERO|
		EFLAGS_AUX_CARRY|
		EFLAGS_CARRY|
		EFLAGS_PARITY
	);
	RaiseOF((prevValue&0x8000)==(value2&0x8000) && (prevValue&0x8000)!=(value1&0x8000)); // Two sources have same sign, but the result sign is different.
	RaiseSF(0!=(value1&0x8000));
	RaiseZF(0==value1);
	RaiseAF((prevValue&0x0F)<(value1&0x0F));
	RaiseCF(value1<prevValue);
	RaisePF(CheckParity(value1&0xFF));
}
void i486DX::AddByte(unsigned int &value1,unsigned int value2)
{
	auto prevValue=value1&0xff;
	value1=(value1+value2)&0xff;
	state.EFLAGS&=~(
		EFLAGS_OVERFLOW|
		EFLAGS_SIGN|
		EFLAGS_ZERO|
		EFLAGS_AUX_CARRY|
		EFLAGS_CARRY|
		EFLAGS_PARITY
	);
	RaiseOF((prevValue&0x80)==(value2&0x80) && (prevValue&0x80)!=(value1&0x80)); // Two sources have same sign, but the result sign is different.
	RaiseSF(0!=(value1&0x80));
	RaiseZF(0==value1);
	RaiseAF((prevValue&0x0F)<(value1&0x0F));
	RaiseCF(value1<prevValue);
	RaisePF(CheckParity(value1&0xFF));
}
void i486DX::AndWordOrDword(int operandSize,unsigned int &value1,unsigned int value2)
{
	if(16==operandSize)
	{
		AndWord(value1,value2);
	}
	else
	{
		AndDword(value1,value2);
	}
}
void i486DX::AndDword(unsigned int &value1,unsigned int value2)
{
	ClearCFOF();
	value1&=value2;
	SetSF(0!=(0x80000000&value1));
	SetZF(0==value1);
	SetPF(CheckParity(value1&0xFF));
}
void i486DX::AndWord(unsigned int &value1,unsigned int value2)
{
	ClearCFOF();
	value1&=value2;
	value1&=0xFFFF;
	SetSF(0!=(0x8000&value1));
	SetZF(0==value1);
	SetPF(CheckParity(value1&0xFF));
}
void i486DX::AndByte(unsigned int &value1,unsigned int value2)
{
	ClearCFOF();
	value1&=value2;
	value1&=0xFF;
	SetSF(0!=(0x80&value1));
	SetZF(0==value1);
	SetPF(CheckParity(value1&0xFF));
}
void i486DX::SubByteWordOrDword(int operandSize,unsigned int &value1,unsigned int value2)
{
	if(8==operandSize)
	{
		SubByte(value1,value2);
	}
	else if(16==operandSize)
	{
		SubWord(value1,value2);
	}
	else
	{
		SubDword(value1,value2);
	}
}
void i486DX::SubWordOrDword(int operandSize,unsigned int &value1,unsigned int value2)
{
	SubByteWordOrDword(operandSize,value1,value2);
}
void i486DX::SubDword(unsigned int &value1,unsigned int value2)
{
	auto prevValue=value1&0xffffffff;
	value1=(value1-value2)&0xffffffff;
	state.EFLAGS&=~(
		EFLAGS_OVERFLOW|
		EFLAGS_SIGN|
		EFLAGS_ZERO|
		EFLAGS_AUX_CARRY|
		EFLAGS_CARRY|
		EFLAGS_PARITY
	);
	RaiseOF((prevValue&0x80000000)!=(value2&0x80000000) && (prevValue&0x80000000)!=(value1&0x80000000)); // Source values have different signs, but the sign flipped.
	RaiseSF(0!=(value1&0x80000000));
	RaiseZF(0==value1);
	RaiseAF((prevValue&0xFF)>=0x10 && (value1&0xFF)<=0x10);
	RaiseCF(value1>prevValue);
	RaisePF(CheckParity(value1&0xFF));
}
void i486DX::SubWord(unsigned int &value1,unsigned int value2)
{
	auto prevValue=value1&0xffff;
	value1=(value1-value2)&0xffff;
	state.EFLAGS&=~(
		EFLAGS_OVERFLOW|
		EFLAGS_SIGN|
		EFLAGS_ZERO|
		EFLAGS_AUX_CARRY|
		EFLAGS_CARRY|
		EFLAGS_PARITY
	);
	RaiseOF((prevValue&0x8000)!=(value2&0x8000) && (prevValue&0x8000)!=(value1&0x8000)); // Source values have different signs, but the sign flipped.
	RaiseSF(0!=(value1&0x8000));
	RaiseZF(0==value1);
	RaiseAF((prevValue&0xFF)>=0x10 && (value1&0xFF)<=0x10);
	RaiseCF(value1>prevValue);
	RaisePF(CheckParity(value1&0xFF));
}
void i486DX::SubByte(unsigned int &value1,unsigned int value2)
{
	auto prevValue=value1&0xff;
	value1=(value1-value2)&0xff;
	state.EFLAGS&=~(
		EFLAGS_OVERFLOW|
		EFLAGS_SIGN|
		EFLAGS_ZERO|
		EFLAGS_AUX_CARRY|
		EFLAGS_CARRY|
		EFLAGS_PARITY
	);
	RaiseOF((prevValue&0x80)!=(value2&0x80) && (prevValue&0x80)!=(value1&0x80)); // Source values have different signs, but the sign flipped.
	RaiseSF(0!=(value1&0x80));
	RaiseZF(0==value1);
	RaiseAF((prevValue&0xFF)>=0x10 && (value1&0xFF)<=0x10);
	RaiseCF(value1>prevValue);
	RaisePF(CheckParity(value1&0xFF));
}
void i486DX::AdcWordOrDword(int operandSize,unsigned int &value1,unsigned int value2)
{
	if(16==operandSize)
	{
		AdcWord(value1,value2);
	}
	else
	{
		AdcDword(value1,value2);
	}
}
void i486DX::AdcDword(unsigned int &value1,unsigned int value2)
{
	auto carry=(0!=(state.EFLAGS&EFLAGS_CARRY) ? 1 : 0);
	auto prevValue=value1&0xffffffff;
	state.EFLAGS&=~(
		EFLAGS_OVERFLOW|
		EFLAGS_SIGN|
		EFLAGS_ZERO|
		EFLAGS_AUX_CARRY|
		EFLAGS_CARRY|
		EFLAGS_PARITY
	);
	value1=(value1+value2+carry)&0xffffffff;
	RaiseOF((prevValue&0x80000000)==(value2&0x80000000) && (prevValue&0x80000000)!=(value1&0x80000000)); // Two sources have same sign, but the result sign is different.
	RaiseSF(0!=(value1&0x80000000));
	RaiseZF(0==value1);
	RaiseAF((prevValue&0x0F)<(value1&0x0F));
	RaiseCF(value1<prevValue);
	RaisePF(CheckParity(value1&0xFF));
}
void i486DX::AdcWord(unsigned int &value1,unsigned int value2)
{
	auto carry=(0!=(state.EFLAGS&EFLAGS_CARRY) ? 1 : 0);
	auto prevValue=value1&0xffff;
	value1=(value1+value2+carry)&0xffff;
	state.EFLAGS&=~(
		EFLAGS_OVERFLOW|
		EFLAGS_SIGN|
		EFLAGS_ZERO|
		EFLAGS_AUX_CARRY|
		EFLAGS_CARRY|
		EFLAGS_PARITY
	);
	RaiseOF((prevValue&0x8000)==(value2&0x8000) && (prevValue&0x8000)!=(value1&0x8000)); // Two sources have same sign, but the result sign is different.
	RaiseSF(0!=(value1&0x8000));
	RaiseZF(0==value1);
	RaiseAF((prevValue&0x0F)<(value1&0x0F));
	RaiseCF(value1<prevValue);
	RaisePF(CheckParity(value1&0xFF));
}
void i486DX::AdcByte(unsigned int &value1,unsigned int value2)
{
	auto carry=(0!=(state.EFLAGS&EFLAGS_CARRY) ? 1 : 0);
	auto prevValue=value1&0xff;
	value1=(value1+value2+carry)&0xff;
	state.EFLAGS&=~(
		EFLAGS_OVERFLOW|
		EFLAGS_SIGN|
		EFLAGS_ZERO|
		EFLAGS_AUX_CARRY|
		EFLAGS_CARRY|
		EFLAGS_PARITY
	);
	RaiseOF((prevValue&0x80)==(value2&0x80) && (prevValue&0x80)!=(value1&0x80)); // Two sources have same sign, but the result sign is different.
	RaiseSF(0!=(value1&0x80));
	RaiseZF(0==value1);
	RaiseAF((prevValue&0x0F)<(value1&0x0F));
	RaiseCF(value1<prevValue);
	RaisePF(CheckParity(value1&0xFF));
}
void i486DX::SbbWordOrDword(int operandSize,unsigned int &value1,unsigned int value2)
{
	if(16==operandSize)
	{
		SbbWord(value1,value2);
	}
	else
	{
		SbbDword(value1,value2);
	}
}
void i486DX::SbbDword(unsigned int &value1,unsigned int value2)
{
	auto carry=(0!=(state.EFLAGS&EFLAGS_CARRY) ? 1 : 0);
	auto prevValue=value1&0xffffffff;
	value1=(value1-value2-carry)&0xffffffff;
	SetOF((prevValue&0x80000000)!=(value2&0x80000000) && (prevValue&0x80000000)!=(value1&0x80000000)); // Source values have different signs, but the sign flipped.
	SetSF(0!=(value1&0x80000000));
	SetZF(0==value1);
	SetAF((prevValue&0xFF)>=0x10 && (value1&0xFF)<=0x10);
	SetCF(value1>prevValue);
	SetPF(CheckParity(value1&0xFF));
}
void i486DX::SbbWord(unsigned int &value1,unsigned int value2)
{
	auto carry=(0!=(state.EFLAGS&EFLAGS_CARRY) ? 1 : 0);
	auto prevValue=value1&0xffff;
	value1=(value1-value2-carry)&0xffff;
	SetOF((prevValue&0x8000)!=(value2&0x8000) && (prevValue&0x8000)!=(value1&0x8000)); // Source values have different signs, but the sign flipped.
	SetSF(0!=(value1&0x8000));
	SetZF(0==value1);
	SetAF((prevValue&0xFF)>=0x10 && (value1&0xFF)<=0x10);
	SetCF(value1>prevValue);
	SetPF(CheckParity(value1&0xFF));
}
void i486DX::SbbByte(unsigned int &value1,unsigned int value2)
{
	auto carry=(0!=(state.EFLAGS&EFLAGS_CARRY) ? 1 : 0);
	auto prevValue=value1&0xff;
	value1=(value1-value2-carry)&0xff;
	SetOF((prevValue&0x80)!=(value2&0x80) && (prevValue&0x80)!=(value1&0x80)); // Source values have different signs, but the sign flipped.
	SetSF(0!=(value1&0x80));
	SetZF(0==value1);
	SetAF((prevValue&0xFF)>=0x10 && (value1&0xFF)<=0x10);
	SetCF(value1>prevValue);
	SetPF(CheckParity(value1&0xFF));
}
void i486DX::OrWordOrDword(int operandSize,unsigned int &value1,unsigned int value2)
{
	if(16==operandSize)
	{
		OrWord(value1,value2);
	}
	else
	{
		OrDword(value1,value2);
	}
}
void i486DX::OrDword(unsigned int &value1,unsigned int value2)
{
	ClearCFOF();
	//SetCF(false);
	//SetOF(false);
	value1|=value2;
	SetSF(0!=(0x80000000&value1));
	SetZF(0==value1);
	SetPF(CheckParity(value1&0xFF));
}
void i486DX::OrWord(unsigned int &value1,unsigned int value2)
{
	ClearCFOF();
	//SetCF(false);
	//SetOF(false);
	value1|=value2;
	value1&=0xFFFF;
	SetSF(0!=(0x8000&value1));
	SetZF(0==value1);
	SetPF(CheckParity(value1&0xFF));
}
void i486DX::OrByte(unsigned int &value1,unsigned int value2)
{
	ClearCFOF();
	//SetCF(false);
	//SetOF(false);
	value1|=value2;
	value1&=0xFF;
	SetSF(0!=(0x80&value1));
	SetZF(0==value1);
	SetPF(CheckParity(value1&0xFF));
}
void i486DX::XorWordOrDword(int operandSize,unsigned int &value1,unsigned int value2)
{
	if(16==operandSize)
	{
		XorWord(value1,value2);
	}
	else
	{
		XorDword(value1,value2);
	}
}
void i486DX::XorDword(unsigned int &value1,unsigned int value2)
{
	ClearCFOF();
	//SetCF(false);
	//SetOF(false);
	value1^=value2;
	SetSF(0!=(0x80000000&value1));
	SetZF(0==value1);
	SetPF(CheckParity(value1&0xFF));
}
void i486DX::XorWord(unsigned int &value1,unsigned int value2)
{
	ClearCFOF();
	//SetCF(false);
	//SetOF(false);
	value1^=value2;
	value1&=0xFFFF;
	SetSF(0!=(0x8000&value1));
	SetZF(0==value1);
	SetPF(CheckParity(value1&0xFF));
}
void i486DX::XorByte(unsigned int &value1,unsigned int value2)
{
	ClearCFOF();
	//SetCF(false);
	//SetOF(false);
	value1^=value2;
	value1&=0xFF;
	SetSF(0!=(0x80&value1));
	SetZF(0==value1);
	SetPF(CheckParity(value1&0xFF));
}

template <unsigned int valueMask,unsigned int countMask,unsigned int bitLength,unsigned int signBit>
inline void i486DX::RolTemplate(unsigned int &value,unsigned int ctr)
{
	unsigned long long int mask=valueMask;
	ctr&=countMask;
	mask>>=(bitLength-ctr);
	value=(value<<ctr)|((value>>(bitLength-ctr))&mask);
	value&=valueMask;
	SetCF(0!=ctr && 0!=(value&1));
	if(1==ctr)
	{
		bool sgn=(0!=(value&signBit));
		SetOF(sgn!=GetCF());
	}
}

void i486DX::RolByteWordOrDword(int operandSize,unsigned int &value,unsigned int ctr)
{
	switch(operandSize)
	{
	case 8:
		RolByte(value,ctr);
		break;
	case 16:
		RolWord(value,ctr);
		break;
	default:
	case 32:
		RolDword(value,ctr);
		break;
	}
}

void i486DX::RolDword(unsigned int &value,unsigned int ctr)
{
	RolTemplate<0xFFFFFFFF,0x1F,32,0x80000000>(value,ctr);

	/* unsigned long long int mask=0xFFFFFFFF;
	ctr&=0x1F;
	mask>>=(32-ctr);
	SetCF(0!=(value&0x80000000));
	value=(value<<ctr)|((value>>(32-ctr))&mask);
	value&=0xFFFFFFFF; */
}

void i486DX::RolWord(unsigned int &value,unsigned int ctr)
{
	RolTemplate<0xFFFF,0xF,16,0x8000>(value,ctr);

	/* unsigned long long int mask=0xFFFF;
	ctr&=0xF;
	mask>>=(16-ctr);
	SetCF(0!=(value&0x8000));
	value=(value<<ctr)|((value>>(16-ctr))&mask);
	value&=0xFFFF; */
}

void i486DX::RolByte(unsigned int &value,unsigned int ctr)
{
	RolTemplate<0xFF,0x7,8,0x80>(value,ctr);

	/* unsigned long long int mask=0xFF;
	ctr&=0x7;
	mask>>=(8-ctr);
	SetCF(0!=(value&0x80));
	value=(value<<ctr)|((value>>(8-ctr))&mask);
	value&=0xFF; */
}

void i486DX::RorByteWordOrDword(int operandSize,unsigned int &value,unsigned int ctr)
{
	switch(operandSize)
	{
	case 8:
		RorByte(value,ctr);
		break;
	case 16:
		RorWord(value,ctr);
		break;
	default:
	case 32:
		RorDword(value,ctr);
		break;
	}
}

template <unsigned int bitCount,unsigned int allBits,unsigned int signBit>
inline void i486DX::RorTemplate(unsigned int &value,unsigned int ctr)
{
	auto prevValue=value;
	unsigned int rightBitsMask=(allBits>>(bitCount-ctr));
	unsigned int rightBits=(value&rightBitsMask);
	value&=allBits;
	value>>=ctr;
	value|=(rightBits<<(bitCount-ctr));
	SetCF(0!=ctr && 0!=(value&signBit));
	if(1==ctr)
	{
		SetOF((prevValue&signBit)!=(value&signBit));
	}
}

void i486DX::RorDword(unsigned int &value,unsigned int ctr)
{
	RorTemplate<32,0xffffffff,0x80000000>(value,ctr);
}

void i486DX::RorWord(unsigned int &value,unsigned int ctr)
{
	RorTemplate<16,0xffff,0x8000>(value,ctr);
}

void i486DX::RorByte(unsigned int &value,unsigned int ctr)
{
	RorTemplate<8,0xff,0x80>(value,ctr);
}

void i486DX::RclWordOrDword(int operandSize,unsigned int &value,unsigned int ctr)
{
	if(16==operandSize)
	{
		RclWord(value,ctr);
	}
	else
	{
		RclDword(value,ctr);
	}
}
void i486DX::RclDword(unsigned int &value,unsigned int ctr)
{
	auto prevValue=value;
	for(unsigned int i=0; i<ctr; ++i)
	{
		auto orValue=(GetCF() ? 1 : 0);
		SetCF(0!=(value&0x80000000));
		prevValue=value;
		value=(value<<1)|orValue;
	}
	if(1==ctr)
	{
		SetOF((prevValue&0x80000000)!=(value&0x80000000));
	}
}
void i486DX::RclWord(unsigned int &value,unsigned int ctr)
{
	auto prevValue=value;
	for(unsigned int i=0; i<ctr; ++i)
	{
		auto orValue=(GetCF() ? 1 : 0);
		SetCF(0!=(value&0x8000));
		prevValue=value;
		value=(value<<1)|orValue;
	}
	value&=0xffff;
	if(1==ctr)
	{
		SetOF((prevValue&0x8000)!=(value&0x8000));
	}
}
void i486DX::RclByte(unsigned int &value,unsigned int ctr)
{
	auto prevValue=value;
	for(unsigned int i=0; i<ctr; ++i)
	{
		auto orValue=(GetCF() ? 1 : 0);
		SetCF(0!=(value&0x80));
		prevValue=value;
		value=(value<<1)|orValue;
	}
	value&=0xff;
	if(1==ctr)
	{
		SetOF((prevValue&0x80)!=(value&0x80));
	}
}

void i486DX::RcrWordOrDword(int operandSize,unsigned int &value,unsigned int ctr)
{
	if(16==operandSize)
	{
		RcrWord(value,ctr);
	}
	else
	{
		RcrDword(value,ctr);
	}
}
void i486DX::RcrDword(unsigned int &value,unsigned int ctr)
{
	if(1==ctr)
	{
		bool sgn=(0!=(value&0x80000000));
		SetOF(sgn!=GetCF());
	}
	for(unsigned int i=0; i<ctr; ++i)
	{
		unsigned int highBit=(GetCF() ? 0x80000000 : 0);
		SetCF(0!=(value&1));
		value=(value>>1)|highBit;
	}
}
void i486DX::RcrWord(unsigned int &value,unsigned int ctr)
{
	value&=0xffff;
	if(1==ctr)
	{
		bool sgn=(0!=(value&0x8000));
		SetOF(sgn!=GetCF());
	}
	for(unsigned int i=0; i<ctr; ++i)
	{
		unsigned int highBit=(GetCF() ? 0x8000 : 0);
		SetCF(0!=(value&1));
		value=(value>>1)|highBit;
	}
}
void i486DX::RcrByte(unsigned int &value,unsigned int ctr)
{
	value&=0xff;
	if(1==ctr)
	{
		bool sgn=(0!=(value&0x80));
		SetOF(sgn!=GetCF());
	}
	for(unsigned int i=0; i<ctr; ++i)
	{
		unsigned int highBit=(GetCF() ? 0x80 : 0);
		SetCF(0!=(value&1));
		value=(value>>1)|highBit;
	}
}

void i486DX::SarByteWordOrDword(int operandSize,unsigned int &value,unsigned int ctr)
{
	switch(operandSize)
	{
	case 8:
		SarByte(value,ctr);
		break;
	case 16:
		SarWord(value,ctr);
		break;
	default:
	case 32:
		SarDword(value,ctr);
		break;
	}
}
void i486DX::SarDword(unsigned int &value,unsigned int ctr)
{
	unsigned long long int value64=value;
	if(0!=(value64&0x80000000))
	{
		value64|=0xFFFFFFFF00000000LL;
	}
	SetCF(0<ctr && 0!=(value&(1<<(ctr-1))));
	value64>>=ctr;
	value64&=0xFFFFFFFF;
	SetZF(0==value64);
	SetSF(0!=(value64&0x80000000));
	SetPF(CheckParity(value64&0xFF));
	if(1==ctr)
	{
		SetOF(false);
	}
	value=(unsigned int)(value64);
}
void i486DX::SarWord(unsigned int &value,unsigned int ctr)
{
	unsigned int value32=value;
	if(0!=(value32&0x8000))
	{
		value32|=0xFFFF0000;
	}
	SetCF(0<ctr && 0!=(value&(1<<(ctr-1))));
	value32>>=ctr;
	value32&=0xFFFF;
	SetZF(0==value32);
	SetSF(0!=(value32&0x8000));
	SetPF(CheckParity(value32&0xFF));
	if(1==ctr)
	{
		SetOF(false);
	}
	value=value32;
}
void i486DX::SarByte(unsigned int &value,unsigned int ctr)
{
	unsigned int value16=value;
	if(0!=(value16&0x80))
	{
		value16|=0xFF00;
	}
	SetCF(0<ctr && 0!=(value&(1<<(ctr-1))));
	value16>>=ctr;
	value16&=0xFF;
	SetZF(0==value16);
	SetSF(0!=(value16&0x80));
	SetPF(CheckParity(value16&0xFF));
	if(1==ctr)
	{
		SetOF(false);
	}
	value=value16;
}

template <unsigned int bitCount,unsigned int maskBits,unsigned int signBit>
void i486DX::ShlTemplate(unsigned int &value,unsigned int ctr)
{
	// OF CF ZF PF SF
	ctr&=31;
	SetCF(0!=ctr && 0!=(value&(signBit>>(ctr-1))));
	if(1<ctr)
	{
		value=(value<<(ctr-1));
		value=(value<<1)&maskBits;
	}
	else if(1==ctr)
	{
		auto prevValue=value;
		value=(value<<1)&maskBits;
		SetOF((prevValue&signBit)!=(value&signBit));
	}
	SetZF(0==value);
	SetPF(CheckParity(value&0xFF));
	SetSF(0!=(value&signBit));
}
void i486DX::ShlWordOrDword(int operandSize,unsigned int &value,unsigned int ctr)
{
	if(16==operandSize)
	{
		ShlWord(value,ctr);
	}
	else
	{
		ShlDword(value,ctr);
	}
}
void i486DX::ShlDword(unsigned int &value,unsigned int ctr)
{
	ShlTemplate<32,0xFFFFFFFF,0x80000000>(value,ctr);
}
void i486DX::ShlWord(unsigned int &value,unsigned int ctr)
{
	ShlTemplate<16,0xFFFF,0x8000>(value,ctr);
}
void i486DX::ShlByte(unsigned int &value,unsigned int ctr)
{
	ShlTemplate<8,0xFF,0x80>(value,ctr);
}

template <unsigned int bitCount,unsigned int maskBits,unsigned int signBit>
inline void i486DX::ShrTemplate(unsigned int &value,unsigned int ctr)
{
	// OF CF ZF PF SF
	ctr&=31;
	SetCF(0!=ctr && 0!=(value&(1<<(ctr-1))));
	if(1<ctr)
	{
		value&=maskBits;
		value>>=ctr;
	}
	else if(1==ctr)
	{
		SetOF(0!=(value&signBit));
		value&=maskBits;
		value>>=1;
	}
	SetZF(0==value);
	SetPF(CheckParity(value&0xFF));
	SetSF(0!=(value&signBit));
}
void i486DX::ShrWordOrDword(int operandSize,unsigned int &value,unsigned int ctr)
{
	if(16==operandSize)
	{
		ShrWord(value,ctr);
	}
	else
	{
		ShrDword(value,ctr);
	}
}
void i486DX::ShrDword(unsigned int &value,unsigned int ctr)
{
	ShrTemplate<32,0xffffffff,0x80000000>(value,ctr);
}
void i486DX::ShrWord(unsigned int &value,unsigned int ctr)
{
	ShrTemplate<16,0xFFFF,0x8000>(value,ctr);
}
void i486DX::ShrByte(unsigned int &value,unsigned int ctr)
{
	ShrTemplate<8,0xFF,0x80>(value,ctr);
}



i486DX::OperandValue i486DX::EvaluateOperand(
    const Memory &mem,int addressSize,int segmentOverride,const Operand &op,int destinationBytes) const
{
	static const unsigned int addressMask[2]=
	{
		0x0000FFFF,
		0xFFFFFFFF,
	};

	i486DX::OperandValue value;
	value.numBytes=0;
	switch(op.operandType)
	{
	case OPER_UNDEFINED:
		Abort("Tried to evaluate an undefined operand.");
		break;
	case OPER_ADDR:
		{
			value.numBytes=destinationBytes;

			unsigned int offset;
			const SegmentRegister &seg=*ExtractSegmentAndOffset(offset,op,segmentOverride);

			offset&=addressMask[addressSize>>5];
			switch(value.numBytes)
			{
			case 1:
				value.byteData[0]=FetchByte(addressSize,seg,offset,mem);
				break;
			case 2:
				value.SetWord(FetchWord(addressSize,seg,offset,mem));
				break;
			case 4:
				value.SetDword(FetchDword(addressSize,seg,offset,mem));
				break;
			default:
				for(unsigned int i=0; i<value.numBytes; ++i)
				{
					value.byteData[i]=FetchByte(addressSize,seg,offset+i,mem);
				}
				break;
			}
		}
		break;
	case OPER_FARADDR:
		Abort("Tried to evaluate FAR ADDRESS.");
		break;
	case OPER_REG32:
		{
			unsigned int reg=state.NULL_and_reg32[op.reg];
			value.numBytes=4;
			value.byteData[0]=( reg     &255);
			value.byteData[1]=((reg>> 8)&255);
			value.byteData[2]=((reg>>16)&255);
			value.byteData[3]=((reg>>24)&255);
		}
		break;
	case OPER_REG16:
		{
			unsigned int reg=state.NULL_and_reg32[op.reg&15];
			value.numBytes=2;
			value.byteData[0]=( reg     &255);
			value.byteData[1]=((reg>> 8)&255);
		}
		break;
	case OPER_REG8:
		{
			unsigned int regIdx=op.reg-REG_AL;
			unsigned int shift=(regIdx<<1)&8;
			value.numBytes=1;
			value.byteData[0]=((state.reg32()[regIdx&3]>>shift)&255);
		}
		break;
	case OPER_SREG:
		value.numBytes=2;
		value.byteData[0]=( state.sreg[op.reg-REG_SEGMENT_REG_BASE].value&255);
		value.byteData[1]=((state.sreg[op.reg-REG_SEGMENT_REG_BASE].value>>8)&255);
		break;
	case OPER_REG:
		switch(op.reg)
		{
		case REG_AL:
		case REG_CL:
		case REG_BL:
		case REG_DL:
		case REG_AH:
		case REG_CH:
		case REG_BH:
		case REG_DH:
			Abort("OPER_REG8 should be used for AL,CL,DL,BL,AH,CH,DH,BH");
			break;

		case REG_AX:
		case REG_CX:
		case REG_DX:
		case REG_BX:
		case REG_SP:
		case REG_BP:
		case REG_SI:
		case REG_DI:
			Abort("OPER_REG16 should be used for AX,CX,DX,BX,SP,BP,SI,DI");
			break;

		case REG_EAX:
		case REG_ECX:
		case REG_EDX:
		case REG_EBX:
		case REG_ESP:
		case REG_EBP:
		case REG_ESI:
		case REG_EDI:
			Abort("OPER_REG32 should be used for AX,CX,DX,BX,SP,BP,SI,DI");
			break;

		case REG_EIP:
			value.numBytes=4;
			value.byteData[0]=(state.EIP&255);
			value.byteData[1]=((state.EIP>>8)&255);
			value.byteData[2]=((state.EIP>>16)&255);
			value.byteData[3]=((state.EIP>>24)&255);
			break;
		case REG_EFLAGS:
			value.numBytes=4;
			value.byteData[0]=(state.EFLAGS&255);
			value.byteData[1]=((state.EFLAGS>>8)&255);
			value.byteData[2]=((state.EFLAGS>>16)&255);
			value.byteData[3]=((state.EFLAGS>>24)&255);
			break;

		case REG_ES:
		case REG_CS:
		case REG_SS:
		case REG_DS:
		case REG_FS:
		case REG_GS:
			Abort("OPER_SREG should be used for AX,CX,DX,BX,SP,BP,SI,DI");
			break;

		case REG_GDT:
			Abort("i486DX::EvaluateOperand, Check GDT Byte Order");
			value.numBytes=6;
			value.byteData[0]=(state.GDTR.linearBaseAddr&255);
			value.byteData[1]=((state.GDTR.linearBaseAddr>>8)&255);
			value.byteData[2]=((state.GDTR.linearBaseAddr>>16)&255);
			value.byteData[3]=((state.GDTR.linearBaseAddr>>24)&255);
			value.byteData[4]=(state.GDTR.limit&255);
			value.byteData[5]=((state.GDTR.limit>>8)&255);
			break;
		case REG_LDT:
			Abort("i486DX::EvaluateOperand, Check LDT Byte Order");
			value.numBytes=6;
			value.byteData[0]=(state.LDTR.linearBaseAddr&255);
			value.byteData[1]=((state.LDTR.linearBaseAddr>>8)&255);
			value.byteData[2]=((state.LDTR.linearBaseAddr>>16)&255);
			value.byteData[3]=((state.LDTR.linearBaseAddr>>24)&255);
			value.byteData[4]=(state.LDTR.limit&255);
			value.byteData[5]=((state.LDTR.limit>>8)&255);
			break;
		case REG_TR:
			Abort("i486DX::EvaluateOperand, Check TR Byte Order");
			value.numBytes=10;
			value.byteData[0]=(state.TR.linearBaseAddr&255);
			value.byteData[1]=((state.TR.linearBaseAddr>>8)&255);
			value.byteData[2]=((state.TR.linearBaseAddr>>16)&255);
			value.byteData[3]=((state.TR.linearBaseAddr>>24)&255);
			value.byteData[4]=(state.TR.limit&255);
			value.byteData[5]=((state.TR.limit>>8)&255);
			value.byteData[6]=(state.TR.selector&255);
			value.byteData[7]=((state.TR.selector>>8)&255);
			value.byteData[8]=(state.TR.attrib&255);
			value.byteData[9]=((state.TR.attrib>>8)&255);
			break;
		case REG_IDTR:
			Abort("i486DX::EvaluateOperand, Check IDTR Byte Order");
			value.numBytes=6;
			value.byteData[0]=(state.IDTR.linearBaseAddr&255);
			value.byteData[1]=((state.IDTR.linearBaseAddr>>8)&255);
			value.byteData[2]=((state.IDTR.linearBaseAddr>>16)&255);
			value.byteData[3]=((state.IDTR.linearBaseAddr>>24)&255);
			value.byteData[4]=(state.IDTR.limit&255);
			value.byteData[5]=((state.IDTR.limit>>8)&255);
			break;

		case REG_CR0:
		case REG_CR1:
		case REG_CR2:
		case REG_CR3:
			value.numBytes=4;
			value.byteData[0]=( state.GetCR(op.reg-REG_CR0)&255);
			value.byteData[1]=((state.GetCR(op.reg-REG_CR0)>>8)&255);
			value.byteData[2]=((state.GetCR(op.reg-REG_CR0)>>16)&255);
			value.byteData[3]=((state.GetCR(op.reg-REG_CR0)>>24)&255);
			break;

		case REG_DR0:
		case REG_DR1:
		case REG_DR2:
		case REG_DR3:
		case REG_DR4:
		case REG_DR5:
		case REG_DR6:
		case REG_DR7:
			value.numBytes=4;
			value.byteData[0]=( state.DR[op.reg-REG_DR0]&255);
			value.byteData[1]=((state.DR[op.reg-REG_DR0]>>8)&255);
			value.byteData[2]=((state.DR[op.reg-REG_DR0]>>16)&255);
			value.byteData[3]=((state.DR[op.reg-REG_DR0]>>24)&255);
			break;

		case REG_TEST0:
		case REG_TEST1:
		case REG_TEST2:
		case REG_TEST3:
		case REG_TEST4:
		case REG_TEST5:
		case REG_TEST6:
		case REG_TEST7:
			value.numBytes=4;
			value.byteData[0]=(state.TEST[op.reg-REG_TEST0])&255;
			value.byteData[1]=(state.TEST[op.reg-REG_TEST0]>>8)&255;
			value.byteData[2]=(state.TEST[op.reg-REG_TEST0]>>16)&255;
			value.byteData[3]=(state.TEST[op.reg-REG_TEST0]>>24)&255;
			break;
		}
		break;
	}
	return value;
}

void i486DX::StoreOperandValue(
    const Operand &dst,Memory &mem,int addressSize,int segmentOverride,OperandValue value)
{
	static const unsigned int addressMask[2]=
	{
		0x0000FFFF,
		0xFFFFFFFF,
	};

	switch(dst.operandType)
	{
	case OPER_UNDEFINED:
		Abort("Tried to evaluate an undefined operand.");
		break;
	case OPER_ADDR:
		{
			unsigned int offset;
			const SegmentRegister &seg=*ExtractSegmentAndOffset(offset,dst,segmentOverride);

			offset&=addressMask[addressSize>>5];
			switch(value.numBytes)
			{
			case 1:
				StoreByte(mem,addressSize,seg,offset,value.byteData[0]);
				break;
			case 2:
				StoreWord(mem,addressSize,seg,offset,cpputil::GetWord(value.byteData));// cpputil::GetWord is faster than using value.GetAsWord.
				break;
			case 4:
				StoreDword(mem,addressSize,seg,offset,cpputil::GetDword(value.byteData));// cpputil::GetWord is faster than using value.GetAsDword.
				break;
			default:
				for(unsigned int i=0; i<value.numBytes; ++i)
				{
					StoreByte(mem,addressSize,seg,offset+i,value.byteData[i]);
				}
				break;
			}
		}
		break;
	case OPER_FARADDR:
		Abort("Tried to evaluate FAR ADDRESS.");
		break;
	case OPER_REG32:
		{
			state.NULL_and_reg32[dst.reg]=
				 value.byteData[0]   |
				(value.byteData[1]<<8)|
				(value.byteData[2]<<16)|
				(value.byteData[3]<<24);
		}
		break;
	case OPER_REG16:
		{
			state.NULL_and_reg32[dst.reg&15]&=0xFFFF0000;
			state.NULL_and_reg32[dst.reg&15]|=
				 value.byteData[0]    |
				(value.byteData[1]<<8);
		}
		break;
	case OPER_REG8:
		{
			static const unsigned int highLowMask[2]=
			{
				0xFFFFFF00,
				0xFFFF00FF,
			};
			unsigned int regIdx=dst.reg-REG_AL;
			unsigned int highLow=regIdx>>2;
			state.reg32()[regIdx&3]&=highLowMask[highLow];
			state.reg32()[regIdx&3]|=(value.byteData[0]<<(highLow<<3));
		}
		break;
	case OPER_SREG:
		LoadSegmentRegister(state.sreg[dst.reg-REG_SEGMENT_REG_BASE],cpputil::GetWord(value.byteData),mem);
		break;
	case OPER_REG:
		switch(dst.reg)
		{
		case REG_AL:
		case REG_CL:
		case REG_BL:
		case REG_DL:
		case REG_AH:
		case REG_CH:
		case REG_BH:
		case REG_DH:
			Abort("OPER_REG8 should be used for AL,CL,DL,BL,AH,CH,DH,BH");
			break;

		case REG_AX:
		case REG_CX:
		case REG_DX:
		case REG_BX:
		case REG_SP:
		case REG_BP:
		case REG_SI:
		case REG_DI:
			Abort("OPER_REG16 should be used for AX,CX,DX,BX,SP,BP,SI,DI");
			break;

		case REG_EAX:
		case REG_ECX:
		case REG_EDX:
		case REG_EBX:
		case REG_ESP:
		case REG_EBP:
		case REG_ESI:
		case REG_EDI:
			Abort("OPER_REG32 should be used for AX,CX,DX,BX,SP,BP,SI,DI");
			break;

		case REG_EIP:
			state.EIP=cpputil::GetDword(value.byteData);
			break;
		case REG_EFLAGS:
			state.EFLAGS=cpputil::GetDword(value.byteData);
			break;

		case REG_ES:
		case REG_CS:
		case REG_SS:
		case REG_DS:
		case REG_FS:
		case REG_GS:
			Abort("OPER_SREG should be used for AX,CX,DX,BX,SP,BP,SI,DI");
			break;

		case REG_GDT:
			Abort("i486DX::StoreOperandValue, I don't think GDTR can be an operand.");
			break;
		case REG_LDT:
			Abort("i486DX::StoreOperandValue, I don't think LDTR can be an operand.");
			break;
		case REG_TR:
			Abort("i486DX::StoreOperandValue, I don't think TR can be an operand.");
			break;
		case REG_IDTR:
			Abort("i486DX::StoreOperandValue, I don't think IDTR can be an operand.");
			break;
		case REG_CR0:
		case REG_CR1:
		case REG_CR2:
		case REG_CR3:
			Abort("i486DX::StoreOperandValue must not be used for CRx.");
			break;
		case REG_DR0:
			state.DR[0]=cpputil::GetDword(value.byteData);
			break;
		case REG_DR1:
			state.DR[1]=cpputil::GetDword(value.byteData);
			break;
		case REG_DR2:
			state.DR[2]=cpputil::GetDword(value.byteData);
			break;
		case REG_DR3:
			state.DR[3]=cpputil::GetDword(value.byteData);
			break;
		case REG_DR4:
			state.DR[4]=cpputil::GetDword(value.byteData);
			break;
		case REG_DR5:
			state.DR[5]=cpputil::GetDword(value.byteData);
			break;
		case REG_DR6:
			state.DR[6]=cpputil::GetDword(value.byteData);
			break;
		case REG_DR7:
			state.DR[7]=cpputil::GetDword(value.byteData);
			break;
		case REG_TEST0:
		case REG_TEST1:
		case REG_TEST2:
		case REG_TEST3:
		case REG_TEST4:
		case REG_TEST5:
		case REG_TEST6:
		case REG_TEST7:
			state.TEST[dst.reg-REG_TEST0]=cpputil::GetDword(value.byteData);
			break;
		}
		break;
	}
}

bool i486DX::REPCheck(unsigned int &clocksPassed,unsigned int instPrefix,unsigned int addressSize)
{
	if(INST_PREFIX_REP==instPrefix || INST_PREFIX_REPNE==instPrefix)
	{
		auto counter=GetCXorECX(addressSize);
		if(0==counter)
		{
			clocksPassed+=5;
			return false;
		}
		--counter;
		SetCXorECX(addressSize,counter);
		clocksPassed+=7;
	}
	return true;
}

bool i486DX::REPEorNECheck(unsigned int &clocksForRep,unsigned int instPrefix,unsigned int addressSize)
{
	if(INST_PREFIX_REPE==instPrefix)
	{
		return GetZF();
	}
	else if(INST_PREFIX_REPNE==instPrefix)
	{
		return (true!=GetZF());
	}
	return false;
}

inline i486DX::CallStack i486DX::MakeCallStack(
	    bool isInterrupt,unsigned short INTNum,unsigned short AX,
	    unsigned int CR0,
	    unsigned int fromCS,unsigned int fromEIP,unsigned int callOpCodeLength,
	    unsigned int procCS,unsigned int procEIP)
{
	CallStack stk;
	if(true==isInterrupt)
	{
		stk.INTNum=INTNum;
		stk.AX=AX;
	}
	else
	{
		stk.INTNum=0xffff;
		stk.AX=0xffff;
	}
	stk.fromCS=fromCS;
	stk.fromEIP=fromEIP;
	stk.callOpCodeLength=callOpCodeLength;
	stk.procCS=procCS;
	stk.procEIP=procEIP;
	return stk;
}
void i486DX::PushCallStack(
	    bool isInterrupt,unsigned short INTNum,unsigned short AX,
	    unsigned int CR0,
	    unsigned int fromCS,unsigned int fromEIP,unsigned int callOpCodeLength,
	    unsigned int procCS,unsigned int procEIP,
	    const Memory &mem)
{
	callStack.push_back(MakeCallStack(isInterrupt,INTNum,AX,CR0,fromCS,fromEIP,callOpCodeLength,procCS,procEIP));
	if(true==isInterrupt)
	{
		if(0x21==INTNum)
		{
			if((0x3D00==(AX&0xFF00) || 0x4B00==(AX&0xFF00)))
			{
				if(0==(CR0&1))  // Real Mode
				{
					callStack.back().str=DebugFetchString(16,state.DS(),GetDX(),mem);
				}
				else
				{
					callStack.back().str=DebugFetchString(32,state.DS(),GetEDX(),mem);
				}
			}
			if(nullptr!=int21HInterceptorPtr)
			{
				int21HInterceptorPtr->InterceptINT21H(GetAX(),callStack.back().str);
			}
		}
	}
}
void i486DX::PopCallStack(unsigned int CS,unsigned int EIP)
{
	if(true!=callStack.empty())
	{
		int nPop=1;
		bool match=false;
		for(auto iter=callStack.rbegin(); iter!=callStack.rend(); ++iter)
		{
			if(CS==iter->fromCS && EIP==iter->fromEIP+iter->callOpCodeLength)
			{
				match=true;
				break;
			}
			++nPop;
		}

		if(true!=match) // Prob: Jump by RET?
		{
			nPop=0;
		}
		while(0<nPop)
		{
			callStack.pop_back();
			--nPop;
		}
	}
}
void i486DX::AttachDebugger(i486Debugger *debugger)
{
	this->debuggerPtr=debugger;
}
void i486DX::DetachDebugger(void)
{
	debuggerPtr=nullptr;
}

/*! Fetch a byte for debugger.  It won't change exception status.
*/
unsigned int i486DX::DebugFetchByte(unsigned int addressSize,const SegmentRegister &seg,unsigned int offset,const Memory &mem) const
{
	offset&=AddressMask((unsigned char)addressSize);
	auto addr=seg.baseLinearAddr+offset;
	if(true==PagingEnabled())
	{
		addr=LinearAddressToPhysicalAddress(addr,mem);
	}
	auto returnValue=mem.FetchByte(addr);
	return returnValue;
}

/*! Fetch a dword.  It won't change exception status.
*/
unsigned int i486DX::DebugFetchWord(unsigned int addressSize,const SegmentRegister &seg,unsigned int offset,const Memory &mem) const
{
	offset&=AddressMask((unsigned char)addressSize);
	auto addr=seg.baseLinearAddr+offset;
	if(true==PagingEnabled())
	{
		addr=LinearAddressToPhysicalAddress(addr,mem);
		if(0xFFC<(addr&0xfff)) // May hit the page boundary
		{
			return DebugFetchByte(addressSize,seg,offset,mem)|(DebugFetchByte(addressSize,seg,offset+1,mem)<<8);
		}
	}
	auto returnValue=mem.FetchWord(addr);
	return returnValue;
}

/*! Fetch a dword for debugging.  It won't change exception status.
*/
unsigned int i486DX::DebugFetchDword(unsigned int addressSize,const SegmentRegister &seg,unsigned int offset,const Memory &mem) const
{
	offset&=AddressMask((unsigned char)addressSize);
	auto addr=seg.baseLinearAddr+offset;
	if(true==PagingEnabled())
	{
		addr=LinearAddressToPhysicalAddress(addr,mem);
		if(0xFF8<(addr&0xfff)) // May hit the page boundary
		{
			auto returnValue=
			     FetchByte(addressSize,seg,offset,mem)
			   |(FetchByte(addressSize,seg,offset+1,mem)<<8)
			   |(FetchByte(addressSize,seg,offset+2,mem)<<16)
			   |(FetchByte(addressSize,seg,offset+3,mem)<<24);
			return returnValue;
		}
	}
	auto returnValue=mem.FetchDword(addr);
	return returnValue;
}
/*! Fetch a byte, word, or dword for debugging.
    It won't change the exception status.
    Function name is left as FetchWordOrDword temporarily for the time being.
    Will be unified to FetchByteWordOrDword in the future.
*/
unsigned int i486DX::DebugFetchWordOrDword(unsigned int operandSize,unsigned int addressSize,const SegmentRegister &seg,unsigned int offset,const Memory &mem) const
{
	switch(operandSize)
	{
	case 8:
		return DebugFetchByte(addressSize,seg,offset,mem);
	case 16:
		return DebugFetchWord(addressSize,seg,offset,mem);
	default:
	case 32:
		return DebugFetchDword(addressSize,seg,offset,mem);
	}
}
inline unsigned int i486DX::DebugFetchByteWordOrDword(unsigned int operandSize,unsigned int addressSize,const SegmentRegister &seg,unsigned int offset,const Memory &mem) const
{
	return DebugFetchWordOrDword(operandSize,addressSize,seg,offset,mem);
}

/*! Fetch a byte by linear address for debugging.  It won't change exception status.
*/
inline unsigned int i486DX::DebugFetchByteByLinearAddress(const Memory &mem,unsigned int linearAddr) const
{
	if(true==PagingEnabled())
	{
		linearAddr=LinearAddressToPhysicalAddress(linearAddr,mem);
	}
	auto returnValue=mem.FetchByte(linearAddr);
	return returnValue;
}

std::string i486DX::DebugFetchString(int addressSize,const SegmentRegister &seg,unsigned int offset,const Memory &mem) const
{
	std::string str;
	for(int i=0; i<255; ++i)
	{
		auto c=DebugFetchByte(addressSize,seg,offset++,mem);
		if(0==c)
		{
			break;
		}
		str.push_back(c);
	}
	return str;
}

/* static */ int i486DX::StrToReg(const std::string &regName)
{
	for(int i=0; i<REG_TOTAL_NUMBER_OF_REGISTERS; ++i)
	{
		if(regName==RegToStr[i])
		{
			return i;
		}
	}
	return REG_NULL;
}
