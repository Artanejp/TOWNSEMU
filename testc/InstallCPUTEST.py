import os
import subprocess
import shutil
import sys

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


def Run(argv):
	os.chdir(BUILDDIR)
	subprocess.Popen([
		"./main_cui/main_cui"+ExeExtension(),
		ROMDIR,
		"-FD0",
		os.path.join(DISKDIR,"FILETFR.bin"),
		"-FD1",
		os.path.join(DISKDIR,"CPUTEST.bin"),
		"-UNITTEST",
		"-HOST2VM",
		os.path.join(THISDIR,"build","RUNTEST.EXP"),
		"B:\RUNTEST.EXP",
	]+argv).wait()
	subprocess.Popen([
		"./main_cui/main_cui"+ExeExtension(),
		ROMDIR,
		"-FD0",
		os.path.join(DISKDIR,"FILETFR.bin"),
		"-FD1",
		os.path.join(DISKDIR,"CPUTESTU.bin"),
		"-UNITTEST",
		"-HOST2VM",
		os.path.join(THISDIR,"build","RUNTEST.EXP"),
		"B:\RUNTEST.EXP",
	]+argv).wait()




if __name__=="__main__":
	Run(sys.argv)
