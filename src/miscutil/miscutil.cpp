#include "miscutil.h"
#include "cpputil.h"



std::vector <std::string> miscutil::MakeMemDump(const i486DX &cpu,const Memory &mem,i486DX::FarPointer ptr,unsigned int length,bool shiftJIS)
{
	std::vector <std::string> text;

	if((ptr.SEG&0xffff0000)==i486DX::FarPointer::SEG_REGISTER)
	{
		ptr.SEG=cpu.GetRegisterValue(ptr.SEG&0xffff);
	}

	auto lineStart=(ptr.OFFSET&~0x0F);
	auto lineEnd=((ptr.OFFSET+length-1)&~0x0F);
	if((ptr.SEG&0xffff0000)==i486DX::FarPointer::LINEAR_ADDR)
	{
		text.push_back("Linear Address Dump not supported yet.");
	}
	else if((ptr.SEG&0xffff0000)==i486DX::FarPointer::PHYS_ADDR)
	{
		for(auto addr0=lineStart; addr0<lineEnd; addr0+=16)
		{
			std::string str;

			str+=cpputil::Uitox(addr0);
			for(int i=0; i<16; ++i)
			{
				auto addr=addr0+i;
				if(addr<ptr.OFFSET || ptr.OFFSET+length<=addr)
				{
					str+="   ";
				}
				else
				{
					str+=" "+cpputil::Ubtox(mem.FetchByte(addr));
				}
			}
			str.push_back('|');
			for(int i=0; i<16; ++i)
			{
				auto addr=addr0+i;
				if(addr<ptr.OFFSET || ptr.OFFSET+length<=addr)
				{
					str.push_back(' ');
				}
				else
				{
					auto byte=mem.FetchByte(addr);
					if(byte<' ' || (true!=shiftJIS && 0x80<=byte))
					{
						str.push_back(' ');
					}
					else
					{
						str.push_back(byte);
					}
				}
			}
			if(true==shiftJIS)
			{
				// Make sure to break first char of shift-JIS
				str.push_back(' ');
				str.push_back(' ');
			}

			text.push_back((std::string &&)str);
		}
	}
	else
	{
		i486DX::SegmentRegister seg;
		cpu.LoadSegmentRegisterQuiet(seg,ptr.SEG,mem,cpu.IsInRealMode());
		for(auto addr0=lineStart; addr0<lineEnd; addr0+=16)
		{
			std::string str;

			str=cpputil::Ustox(ptr.SEG);
			str.push_back(':');
			str+=cpputil::Uitox(addr0);
			for(int i=0; i<16; ++i)
			{
				auto addr=addr0+i;
				if(addr<ptr.OFFSET || ptr.OFFSET+length<=addr)
				{
					str+="   ";
				}
				else
				{
					str+=" "+cpputil::Ubtox(cpu.FetchByte(seg,addr,mem));
				}
			}
			str.push_back('|');
			for(int i=0; i<16; ++i)
			{
				auto addr=addr0+i;
				if(addr<ptr.OFFSET || ptr.OFFSET+length<=addr)
				{
					str.push_back(' ');
				}
				else
				{
					auto byte=cpu.FetchByte(seg,addr,mem);
					if(byte<' ' || (true!=shiftJIS && 0x80<=byte))
					{
						str.push_back(' ');
					}
					else
					{
						str.push_back(byte);
					}
				}
			}
			if(true==shiftJIS)
			{
				// Make sure to break first char of shift-JIS
				str.push_back(' ');
				str.push_back(' ');
			}

			text.push_back((std::string &&)str);
		}
	}

	return text;
}
