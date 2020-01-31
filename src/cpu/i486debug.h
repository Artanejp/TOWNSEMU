#ifndef I486DEBUG_IS_INCLUDED
#define I486DEBUG_IS_INCLUDED
/* { */


#include <set>
#include "i486.h"

class i486Debugger
{
public:
	typedef i486DX::FarPointer CS_EIP;

	std::set <CS_EIP> breakPoint;
	CS_EIP oneTimeBreakPoint;
	std::string externalBreakReason;

	bool stop;

	bool disassembleEveryStep;

	CS_EIP lastDisassembleAddr;


	i486Debugger();
	void CleanUp(void);

	void AddBreakPoint(unsigned int CS,unsigned int EIP);
	void RemoveBreakPoint(unsigned int CS,unsigned int EIP);

	void SetOneTimeBreakPoint(unsigned int CS,unsigned int EIP);

	/*! Callback from i486DX::RunOneInstruction.
	*/
	void BeforeRunOneInstruction(i486DX &cpu,Memory &mem,InOut &io,const i486DX::Instruction &inst);

	/*! Callback from i486DX::RunOneInstruction.
	*/
	void AfterRunOneInstruction(unsigned int clocksPassed,i486DX &cpu,Memory &mem,InOut &io,const i486DX::Instruction &inst);

	/*! Return formatted call-stack text.
	*/
	std::vector <std::string> GetCallStackText(const i486DX &cpu) const;

	/*! Break from an external reason.
	    It sets stop==1 and externalBreakReason=reason.
	*/
	void ExternalBreak(const std::string &reason);

	/*! Clear stop flag and externalBreakReason.
	*/
	void ClearStopFlag(void);
};


/* } */
#endif