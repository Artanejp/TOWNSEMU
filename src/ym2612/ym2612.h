/* LICENSE>>
Copyright 2020 Soji Yamakawa (CaptainYS, http://www.ysflight.com)

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

<< LICENSE */
#ifndef YM2612_IS_INCLUDED
#define YM2612_IS_INCLUDED
/* { */

#include <vector>
#include <string>


/*! G** D*** I*!  I didn't realize data sheet of YM2612 is not available today!
*/
class YM2612
{
public:
	// [2] pp. 200  Calculation of timer.  Intenral clock is 600KHz 1tick=(1/600K)sec=1667ns.
	// [2] pp. 201:
	// Timer A takes (12*(1024-NATick))/(600)ms to count up.  NATick is 10-bit counter.
	// Timer B takes (192*(256-NBTick))/(600)ms to count up.  NBTick is 8-bit counter.
	// NATick counts up every 12 internal-clock ticks.
	// NBTick counts up every 192 internal-clock ticks.

	enum
	{
		NUM_SLOTS=4,
		NUM_CHANNELS=6,

		// Sine table
		PHASE_STEPS=4096,      // 4096=360degrees
		PHASE_MASK=4095,
		UNSCALED_MAX=2048,

		TONE_CHOPOFF_MILLISEC=4000,

		WAVE_SAMPLING_RATE=44100,
		WAVE_OUTPUT_AMPLITUDE_MAX=32767,
	};

	enum
	{
		REG_TIMER_A_COUNT_HIGH=0x24,
		REG_TIMER_A_COUNT_LOW=0x25,
		REG_TIMER_B_COUNT=0x26,
		REG_TIMER_CONTROL=0x27,
		REG_KEY_ON_OFF=0x28,
	};

	enum
	{
		TICK_DURATION_IN_NS=1667,
		TIMER_A_PER_TICK=12,
		TIMER_B_PER_TICK=192,
		NTICK_TIMER_A=1024*TIMER_A_PER_TICK,
		NTICK_TIMER_B= 256*TIMER_B_PER_TICK,
	};

	enum {
		CH_IDLE=0,
		CH_PLAYING=1,
		CH_RELEASE=2,
	};

	class Slot
	{
	public:
		unsigned int DT,MULTI;
		unsigned int TL;
		unsigned int KS,AR;
		unsigned int AM;
		unsigned int DR;
		unsigned int SR;
		unsigned int SL,RR;
		unsigned int SSG_EG;

		// Cache for wave-generation >>
		unsigned int phase12;      // 5-bit phase=((phase>>12)&0x1F)
		unsigned int phase12Step;  // Increment of phase12 per time step.
		mutable unsigned int nextPhase12; // Cached in MakeWave
		unsigned int env[6];       // Envelope: Amplitude is 0 to 4095 scale.
		unsigned int envDurationCache; // in milliseconds
		unsigned int RRCache;      // Calibrated Release Rate
		bool InReleasePhase;
		unsigned int ReleaseStartTime,ReleaseEndTime;
		unsigned int ReleaseStartAmplitude;
		mutable unsigned int lastAmplitudeCache;  // 0 to 4095 scale.
		// Cache for wave-generation <<

		void Clear(void);

		inline int UnscaledOutput(int phase) const;
		inline int UnscaledOutput(int phase,unsigned int FB) const;
		inline int InterpolateEnvelope(unsigned int timeInMS) const;
		inline int EnvelopedOutput(int phase,unsigned int timeInMS,unsigned int FB) const;
		inline int EnvelopedOutput(int phase,unsigned int timeInMS) const;
	};
	class Channel
	{
	public:
		unsigned int F_NUM,BLOCK;
		unsigned int FB,CONNECT;
		unsigned int L,R,AMS,PMS;
		unsigned int usingSlot;
		Slot slots[NUM_SLOTS];

		// Cache for wave-generation >>
		unsigned int playState;
		unsigned long long int toneDuration12;  // In (microsec<<12).
		unsigned long long int microsec12;      // Microsec from start of a tone by (microsec12>>12)
		mutable unsigned long long int nextMicrosec12; // Cached in MakeWave.
		// Cache for wave-generation <<

		void Clear();
	};

	class State
	{
	public:
		unsigned long long int deviceTimeInNS;
		unsigned long long int lastTickTimeInNS;
		Channel channels[NUM_CHANNELS];
		unsigned int F_NUM_3CH[3],BLOCK_3CH[3];
		unsigned int F_NUM_6CH[3],BLOCK_6CH[3];
		unsigned char reg[256];  // I guess only 0x21 to 0xB6 are used.
		unsigned long long int timerCounter[2];
		bool timerUp[2];
		unsigned int playingCh; // Bit 0 to 5.

		void PowerOn(void);
		void Reset(void);
	};

	State state;

	static int sineTable[PHASE_STEPS];
	static unsigned int TLtoDB100[128];   // 100 times dB
	static unsigned int SLtoDB100[16];    // 100 times dB
	static unsigned int DB100to4095Scale[9601]; // dB to 0 to 4095 scale
	static unsigned int DB100from4095Scale[4096]; // 4095 scale to dB
	static const unsigned int connToOutChannel[8][4];

	struct ConnectionToOutputSlot
	{
		unsigned int nOutputSlots;
		unsigned int slots[4];
	};
	static const struct ConnectionToOutputSlot connectionToOutputSlots[8];


	YM2612();
	~YM2612();
private:
	void MakeSineTable(void);
	void MakeTLtoDB100(void);
	void MakeSLtoDB100(void);
	void MakeDB100to4095Scale(void);
public:
	void PowerOn(void);
	void Reset(void);

	/*! Writes to a register, and if a channel starts playing a tone, it calls KeyOn and returns between 0 to 5.
	    65535 otherwise.
	*/
	unsigned int WriteRegister(unsigned int channelBase,unsigned int reg,unsigned int value);
	unsigned int ReadRegister(unsigned int channelBase,unsigned int reg) const;

	void Run(unsigned long long int systemTimeInNS);

	bool TimerAUp(void) const;
	bool TimerBUp(void) const;

	/*! Returns timer-up state of 
	*/
	bool TimerUp(unsigned int timerId) const;

	/*! Cache parameters for calculating wave.
	*/
	void KeyOn(unsigned int ch);

	/*!
	*/
	std::vector <unsigned char> MakeWave(unsigned int ch,unsigned long long int millisec) const;
private:
	/*! Returns the longest duration of the tone in milliseconds if no key off.
	*/
	unsigned int CalculateToneDurationMilliseconds(unsigned int chNum) const;

	/*!
	*/
	int CalculateAmplitude(int chNum,unsigned int timeInMS,const unsigned int slotPhase[4]) const;


public:
	/*! 
	*/
	void NextWave(unsigned int chNum);

	/*! Change channel state to RELEASE.
	*/
	void KeyOff(unsigned int ch);


	/*! Check if the tone is done, and update playingCh and playing state.
	*/
	void CheckToneDone(unsigned int chNum);


	/*! BLOCK_NOTE is as calculated by [2] pp.204.  Isn't it just high-5 bits of BLOCK|F_NUM2?
	    Return value:
	       true    Envelope calculated
	       false   Envelope not calculated. (AR==0)
	    Envelope:
	       env[0]  Duration for attack (in microseconds)
	       env[1]  TL amplitude (0-127)
	       env[2]  Duration for decay (in microseconds)
	       env[3]  SL amplitude (0-127)
	       env[4]  Duration after reaching SL.
	       env[5]  Zero
	    Release Rate:
	       RR
	*/
	bool CalculateEnvelope(unsigned int env[6],unsigned int &RR,unsigned int BLOCK_NOTE,const Slot &slot) const;



	/*! Based on [2] Table I-5-37
		BLOCK=4		    	Freq Ratio	Freq/Fnum
		C5	523.3	1371	        	0.381692195
		B4	493.9	1294	1.05952622	0.381684699
		A4#	466.2	1222	1.059416559	0.381505728
		A4	440  	1153	1.059545455	0.381613183
		G4#	415.3	1088	1.059475078	0.381709559
		G4	392  	1027	1.059438776	0.381694255
		F4#	370 	969 	1.059459459	0.381836945
		F4	349.2	915 	1.059564719	0.381639344
		E4	329.6	864 	1.059466019	0.381481481
		D4#	311.1	815 	1.05946641	0.381717791
		D4	293.7	769 	1.059244127	0.381924577
		C4#	277.2	726 	1.05952381	0.381818182

		Average Freq/Fnum
		0.381693162

		BLOCK=7		Fnum*0.381693162* 8=Freq
		BLOCK=6		Fnum*0.381693162* 4=Freq
		BLOCK=5		Fnum*0.381693162* 2=Freq
		BLOCK=4		Fnum*0.381693162   =Freq
		BLOCK=3		Fnum*0.381693162/ 2=Freq
		BLOCK=2		Fnum*0.381693162/ 4=Freq
		BLOCK=1		Fnum*0.381693162/ 8=Freq
		BLOCK=0		Fnum*0.381693162/16=Freq

		F-Number Sampled from F-BASIC 386.
		PLAY "O4A"  -> 1038 must correspond to 440Hz -> Ratio should be 0.423892100192678.
	*/
	inline unsigned int BLOCK_FNUM_to_FreqX16(unsigned int BLOCK,unsigned int FNUM)
	{
		/* Value based on [2]
		static const unsigned int scale[8]=
		{
			(3817*16/10)/16,
			(3817*16/10)/8,
			(3817*16/10)/4,
			(3817*16/10)/2,
			(3817*16/10),
			(3817*16/10)*2,
			(3817*16/10)*4,
			(3817*16/10)*8,
		}; */
		// Value based on the observation.
		static const unsigned int scale[8]=
		{
			(423892    /1000),   // (4238*16/10)/16,
			(423892  *2/1000),   // (4239*16/10)/8,
			(423892  *4/1000),   // (4239*16/10)/4,
			(423892  *8/1000),   // (4239*16/10)/2,
			(423892 *16/1000),   // (4239*16/10),
			(423892 *32/1000),   // (4239*16/10)*2,
			(423892 *64/1000),   // (4239*16/10)*4,
			(423892*128/1000),   // (4239*16/10)*8,
		};
		FNUM*=scale[BLOCK&7];
		FNUM/=1000;
		return FNUM;
	}



	std::vector <std::string> GetStatusText(void) const;
};


/* } */
#endif
