/* LICENSE>>
Copyright 2020 Soji Yamakawa (CaptainYS, http://www.ysflight.com)

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

<< LICENSE */
#include "cpputil.h"
#include "keyboard.h"
#include "townsdef.h"
#include "pic.h"
#include "towns.h"


void TownsKeyboard::State::Reset(void)
{
	IRQEnabled=false;
	KBINT=false;
	bootKeyCombSequenceCounter=0;
}
TownsKeyboard::TownsKeyboard(FMTowns *townsPtr,TownsPIC *picPtr) : Device(townsPtr)
{
	this->townsPtr=townsPtr;
	this->picPtr=picPtr;

	state.Reset();
	state.bootKeyComb=BOOT_KEYCOMB_NONE;
	nFifoFilled=0;
}

void TownsKeyboard::PushFifo(unsigned char code1,unsigned char code2)
{
	if(true==debugBreakOnReturnKey && code2==TOWNS_JISKEY_RETURN)
	{
		this->townsPtr->debugger.ExternalBreak("Break on Enter Key");
	}
	if(nFifoFilled+1<FIFO_BUF_LEN)
	{
		fifoBuf[nFifoFilled  ]=code1;
		fifoBuf[nFifoFilled+1]=code2;
		nFifoFilled+=2;
	}
	state.KBINT=true;
	if(true==state.IRQEnabled)
	{
		picPtr->SetInterruptRequestBit(TOWNSIRQ_KEYBOARD,true);
	}
}

void TownsKeyboard::BootSequenceStarted(void)
{
	state.bootKeyComb=BOOT_KEYCOMB_NONE;
	state.bootKeyCombSequenceCounter=0;
}
void TownsKeyboard::SetBootKeyCombination(unsigned int keyComb)
{
	if(BOOT_KEYCOMB_PAD_A!=keyComb &&
	   BOOT_KEYCOMB_PAD_B!=keyComb)
	{
		state.bootKeyComb=keyComb;
		state.bootKeyCombSequenceCounter=0;
	}
}

/* virtual */ void TownsKeyboard::IOWriteByte(unsigned int ioport,unsigned int data)
{
	switch(ioport)
	{
	case TOWNSIO_KEYBOARD_DATA://       0x600, // [2] pp.234
		break;
	case TOWNSIO_KEYBOARD_STATUS_CMD:// 0x602, // [2] pp.231
		if(0xA1==data) // RESET pp.232
		{
			state.Reset();
			PushFifo(0xA0,0x7F);
			// pp.235 and 
			// Also based on Reverse Engineering of FM Towns IIMX System ROM
			/* switch(afterReset)
			{
			case SEND_CD_AFTER_RESET:
				PushFifo(0xA0,0x2C);
				PushFifo(0xA0,0x20);
				PushFifo(0xA0,0x2C);
				PushFifo(0xA0,0x20);
				PushFifo(0xA0,0x2C);
				PushFifo(0xA0,0x20);
				break;
			case SEND_F0_AFTER_RESET:
				PushFifo(0xA0,0x21);
				PushFifo(0xA0,0x0B);
				PushFifo(0xA0,0x21);
				PushFifo(0xA0,0x0B);
				PushFifo(0xA0,0x21);
				PushFifo(0xA0,0x0B);
				break;
			case SEND_F1_AFTER_RESET:
				PushFifo(0xA0,0x21);
				PushFifo(0xA0,0x02);
				PushFifo(0xA0,0x21);
				PushFifo(0xA0,0x02);
				PushFifo(0xA0,0x21);
				PushFifo(0xA0,0x02);
				break;
			} */
		}
		break;
	case TOWNSIO_KEYBOARD_IRQ://        0x604, // [2] pp.236
		state.IRQEnabled=(0!=(data&1));
		if(true==state.IRQEnabled && 0<nFifoFilled)
		{
			picPtr->SetInterruptRequestBit(TOWNSIRQ_KEYBOARD,true);
		}
		break;
	}
}
/* virtual */ unsigned int TownsKeyboard::IOReadByte(unsigned int ioport)
{
	switch(ioport)
	{
	case TOWNSIO_KEYBOARD_DATA://       0x600, // [2] pp.234
		picPtr->SetInterruptRequestBit(TOWNSIRQ_KEYBOARD,false);
		state.KBINT=false;
		if(BOOT_KEYCOMB_NONE==state.bootKeyComb)
		{
			if(0<nFifoFilled)
			{
				auto ret=fifoBuf[0];
				for(int i=0; i<nFifoFilled-1; ++i)
				{
					fifoBuf[i]=fifoBuf[i+1];
				}
				--nFifoFilled;
				if(0<nFifoFilled && true==state.IRQEnabled)
				{
					townsPtr->ScheduleDeviceCallBack(*this,townsPtr->state.townsTime+KEY_REPEAT_INTERVAL);
				}
				return ret;
			}
		}
		else
		{
			auto highLow=state.bootKeyCombSequenceCounter&1;
			auto num=(state.bootKeyCombSequenceCounter>>1);
			++state.bootKeyCombSequenceCounter;
			if(0==highLow)
			{
				return (num<6 ? 0xA0 : 0xF0); // The longest one: 7F 'D' 'E' 'B' 'U' 'G'
			}
			if(0==num)
			{
				return 0x7F;
			}
			else
			{
				switch(state.bootKeyComb)
				{
				case BOOT_KEYCOMB_CD:
					{
						const unsigned char code[2]={0x2C,0x20};
						return code[num&1];
					}
					break;
				case BOOT_KEYCOMB_F0:
					{
						const unsigned char code[2]={0x21,0x0B};
						return code[num&1];
					}
					break;
				case BOOT_KEYCOMB_F1:
					{
						const unsigned char code[2]={0x21,0x02};
						return code[num&1];
					}
					break;
				case BOOT_KEYCOMB_F2:
					{
						const unsigned char code[2]={0x21,0x03};
						return code[num&1];
					}
					break;
				case BOOT_KEYCOMB_F3:
					{
						const unsigned char code[2]={0x21,0x04};
						return code[num&1];
					}
					break;
				case BOOT_KEYCOMB_H0:
					{
						const unsigned char code[2]={0x23,0x0B};
						return code[num&1];
					}
					break;
				case BOOT_KEYCOMB_H1:
					{
						const unsigned char code[2]={0x23,0x02};
						return code[num&1];
					}
					break;
				case BOOT_KEYCOMB_H2:
					{
						const unsigned char code[2]={0x23,0x03};
						return code[num&1];
					}
					break;
				case BOOT_KEYCOMB_H3:
					{
						const unsigned char code[2]={0x23,0x04};
						return code[num&1];
					}
					break;
				case BOOT_KEYCOMB_H4:
					{
						const unsigned char code[2]={0x23,0x05};
						return code[num&1];
					}
					break;
				case BOOT_KEYCOMB_ICM:
					{
						const unsigned char code[3]={0x18,0x2C,0x30};
						return code[num%3];
					}
					break;
				case BOOT_KEYCOMB_DEBUG:
					{
						const unsigned char code[5]={0x20,0x13,0x2E,0x17,0x22};
						return code[num%5];
					}
					break;
				}
			}
		}
		return 0;

	case TOWNSIO_KEYBOARD_STATUS_CMD:// 0x602, // [2] pp.231
		if(0<nFifoFilled || BOOT_KEYCOMB_NONE!=state.bootKeyComb)
		{
			return 1; // Data (Keyboard -> CPU) Ready.
		}
		return 0;  // IBF=0 Data Empty. Always Ready.
	case TOWNSIO_KEYBOARD_IRQ://        0x604, // [2] pp.236
		return (true==state.KBINT ? 1 : 0);
	}
	return 0xff;
}
/* virtual */ void TownsKeyboard::Reset(void)
{
	state.Reset();
}

/* virtual */ void TownsKeyboard::RunScheduledTask(unsigned long long int townsTime)
{
	// 9600bps=600words per seconds.
	if(0<nFifoFilled && true==state.IRQEnabled)
	{
		state.KBINT=true;
		picPtr->SetInterruptRequestBit(TOWNSIRQ_KEYBOARD,true);
	}
}
