#include <iostream>

#include "townsargv.h"
#include "cpputil.h"



TownsARGV::TownsARGV()
{
	autoStart=true;
	interactive=true;
}
void TownsARGV::PrintHelp(void) const
{
	std::cout << "Usage:" << std::endl;
	std::cout << "-HELP,-H,-?" << std::endl;
	std::cout << "  Print Help." << std::endl;
	std::cout << "-PAUSE" << std::endl;
	std::cout << "  Machine state is PAUSE on start up." << std::endl;
	std::cout << "-UNITTEST" << std::endl;
	std::cout << "  Let it run automatically to the end without taking control commands." << std::endl;
	std::cout << "-FD0 filename" << std::endl;
	std::cout << "  Floppy disk image file name for Drive A." << std::endl;
	std::cout << "-FD1 filename" << std::endl;
	std::cout << "  Floppy disk image file name for Drive B." << std::endl;
	std::cout << "-SYM filename" << std::endl;
	std::cout << "  Specify symbol file name." << std::endl;
}

bool TownsARGV::AnalyzeCommandParameter(int argc,char *argv[])
{
	for(int i=1; i<argc; ++i)
	{
		std::string ARG=cpputil::Capitalize(argv[i]);
		if("-HELP"==ARG || "-H"==ARG || "-?"==ARG)
		{
			PrintHelp();
		}
		else if("-PAUSE"==ARG && i+1<argc)
		{
			autoStart=false;
			++i;
		}
		else if("-UNITTEST"==ARG && i+1<argc)
		{
			interactive=false;
			++i;
		}
		else if("-FD0"==ARG && i+1<argc)
		{
			fdImgFName[0]=argv[i+1];
			++i;
		}
		else if("-FD1"==ARG && i+1<argc)
		{
			fdImgFName[1]=argv[i+1];
			++i;
		}
		else if("-SYM"==ARG && i+1<argc)
		{
			symbolFName=argv[i+1];
			++i;
		}
		else
		{
			if(1==i)
			{
				ROMPath=argv[i];
			}
			else
			{
				std::cout << "Undefined Option or Insufficient Parameters:" << argv[i] << std::endl;
			}
		}
	}
	return true;
}
