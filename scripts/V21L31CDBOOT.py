import os
import subprocess
import shutil
import sys

import build

TOWNSTYPE="MX"

THISFILE=os.path.realpath(__file__)
THISDIR=os.path.dirname(THISFILE)
BUILDDIR=os.path.join(THISDIR,"..","build")
SRCDIR=os.path.join(THISDIR,"..","src")
ROMDIR=os.path.join(THISDIR,"..","..","TOWNSEMU_TEST","ROM_"+TOWNSTYPE)
DISKDIR=os.path.join(THISDIR,"..","..","TOWNSEMU_TEST","DISKIMG")



def ExeExtension():
	if sys.platform.startswith('win'):
		return ".exe"
	else:
		return ""


def Run():
	os.chdir(BUILDDIR)
	subprocess.Popen([
		"./main_cui/Tsugaru_CUI"+ExeExtension(),
		ROMDIR,
		"-FD0",
		os.path.join(DISKDIR,"V2.1L20.bin"),
		"-CD",
		"D:/ISOImageSubset/TOSV2.1L31.iso",
		"-BOOTKEY",
		"CD",
		"-SYM",
		"../symtables/V2.1L31CDBOOT_"+TOWNSTYPE+".txt",
		"-HD0",
		os.path.join(DISKDIR,"hddimage.bin"),
		"-CMOS",
		"../testdata/CMOS.bin",
		"-DEBUG",
		"-PAUSE",
	]+argv).wait()



if __name__=="__main__":
	build.Run(sys.argv[1:])
	Run()
