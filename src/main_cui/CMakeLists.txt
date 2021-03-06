set(TARGET_NAME Tsugaru_CUI)

add_executable(Tsugaru_CUI MACOSX_BUNDLE main.cpp)
target_link_libraries(${TARGET_NAME} 
	towns 
	townscommand 
	townsargv
	townsrender 
	outside_world
	fssimplewindow_connection
)

add_subdirectory(command)
add_subdirectory(argv)

add_test(
 	NAME CPUTEST
	COMMAND 
		${TARGET_NAME} 
		"${CMAKE_SOURCE_DIR}/../../TOWNSEMU_TEST/ROM_MX"
		-FD0
		"${CMAKE_SOURCE_DIR}/../../TOWNSEMU_TEST/DISKIMG/CPUTESTU.bin"
		-SYM
		"${CMAKE_SOURCE_DIR}/../symtables/UNITTEST.txt"
		-UNITTEST
)

add_test(
 	NAME 2MX_V21L30
	COMMAND 
		${TARGET_NAME} 
		"${CMAKE_SOURCE_DIR}/../../TOWNSEMU_TEST/ROM_MX"
		-FD0
		"${CMAKE_SOURCE_DIR}/../../TOWNSEMU_TEST/DISKIMG/testset01.bin"
		-SYM
		"${CMAKE_SOURCE_DIR}/../symtables/UNITTEST.txt"
		-UNITTEST
)

add_test(
 	NAME 2F_V21L30
	COMMAND 
		${TARGET_NAME} 
		"${CMAKE_SOURCE_DIR}/../../TOWNSEMU_TEST/ROM_2F"
		-FD0
		"${CMAKE_SOURCE_DIR}/../../TOWNSEMU_TEST/DISKIMG/testset01.bin"
		-SYM
		"${CMAKE_SOURCE_DIR}/../symtables/UNITTEST.txt"
		-UNITTEST
)

add_test(
 	NAME MX_V21L20
	COMMAND 
		${TARGET_NAME} 
		"${CMAKE_SOURCE_DIR}/../../TOWNSEMU_TEST/ROM_MX"
		-FD0
		"${CMAKE_SOURCE_DIR}/../../TOWNSEMU_TEST/DISKIMG/testset02.bin"
		-SYM
		"${CMAKE_SOURCE_DIR}/../symtables/UNITTEST.txt"
		-UNITTEST
)

add_test(
 	NAME 2F_V21L20
	COMMAND 
		${TARGET_NAME} 
		"${CMAKE_SOURCE_DIR}/../../TOWNSEMU_TEST/ROM_2F"
		-FD0
		"${CMAKE_SOURCE_DIR}/../../TOWNSEMU_TEST/DISKIMG/testset02.bin"
		-SYM
		"${CMAKE_SOURCE_DIR}/../symtables/UNITTEST.txt"
		-UNITTEST
)

add_test(
 	NAME MX_DOSCOMMANDS
	COMMAND 
		${TARGET_NAME} 
		"${CMAKE_SOURCE_DIR}/../../TOWNSEMU_TEST/ROM_MX"
		-FD0
		"${CMAKE_SOURCE_DIR}/../../TOWNSEMU_TEST/DISKIMG/testset03.bin"
		-SYM
		"${CMAKE_SOURCE_DIR}/../symtables/UNITTEST.txt"
		-UNITTEST
)

add_test(
 	NAME 2F_DOSCOMMANDS
	COMMAND 
		${TARGET_NAME} 
		"${CMAKE_SOURCE_DIR}/../../TOWNSEMU_TEST/ROM_2F"
		-FD0
		"${CMAKE_SOURCE_DIR}/../../TOWNSEMU_TEST/DISKIMG/testset03.bin"
		-SYM
		"${CMAKE_SOURCE_DIR}/../symtables/UNITTEST.txt"
		-UNITTEST
)

add_test(
 	NAME 2F_CDROM_ISO_V21L20
	COMMAND 
		${TARGET_NAME} 
		"${CMAKE_SOURCE_DIR}/../../TOWNSEMU_TEST/ROM_2F"
		-FD0
		"${CMAKE_SOURCE_DIR}/../../TOWNSEMU_TEST/DISKIMG/testcdV2.1L20.bin"
		-SYM
		"${CMAKE_SOURCE_DIR}/../symtables/UNITTEST.txt"
		-CD
		"${CMAKE_SOURCE_DIR}/../testdata/ISOIMG/TESTDISC.iso"
		-UNITTEST
)

add_test(
 	NAME MX_CDROM_ISO_V21L30
	COMMAND 
		${TARGET_NAME} 
		"${CMAKE_SOURCE_DIR}/../../TOWNSEMU_TEST/ROM_MX"
		-FD0
		"${CMAKE_SOURCE_DIR}/../../TOWNSEMU_TEST/DISKIMG/testcdV2.1L30.bin"
		-SYM
		"${CMAKE_SOURCE_DIR}/../symtables/UNITTEST.txt"
		-CD
		"${CMAKE_SOURCE_DIR}/../testdata/ISOIMG/TESTDISC.iso"
		-UNITTEST
)

add_test(
 	NAME MX_V11L10_GUI
	COMMAND 
		${TARGET_NAME} 
		"${CMAKE_SOURCE_DIR}/../../TOWNSEMU_TEST/ROM_MX"
		-SYM
		"${CMAKE_SOURCE_DIR}/../symtables/UNITTEST.txt"
		-CD
		"${DISC_IMAGE_DIR}/TOSV1.1L10.iso"
		-EVTLOG
		"${CMAKE_SOURCE_DIR}/../testdata/eventLog/V11L10TEST.txt"
		-UNITTEST
		-FREQ
		20
)

add_test(
	NAME 2F_V11L30_GUI
	COMMAND 
		${TARGET_NAME} 
		"${CMAKE_SOURCE_DIR}/../../TOWNSEMU_TEST/ROM_2F"
		-SYM
		"${CMAKE_SOURCE_DIR}/../symtables/UNITTEST.txt"
		-CD
		"${DISC_IMAGE_DIR}/TOSV1.1L30.iso"
		-EVTLOG
		"${CMAKE_SOURCE_DIR}/../testdata/eventLog/V11L30TEST.txt"
		-UNITTEST
)

add_test(
 	NAME 2F_V21L20_GUI
	COMMAND 
		${TARGET_NAME} 
		"${CMAKE_SOURCE_DIR}/../../TOWNSEMU_TEST/ROM_2F"
		-CD
		"${DISC_IMAGE_DIR}/TownsOSV2.1L20.cue"
		-EVTLOG
		"${CMAKE_SOURCE_DIR}/../testdata/eventLog/V21L20TEST.txt"
		-UNITTEST
)

add_test(
 	NAME MX_V21L31_GUI
	COMMAND 
		${TARGET_NAME} 
		"${CMAKE_SOURCE_DIR}/../../TOWNSEMU_TEST/ROM_MX"
		-SYM
		"${CMAKE_SOURCE_DIR}/../symtables/UNITTEST.txt"
		-CD
		"${DISC_IMAGE_DIR}/TOSV2.1L31.iso"
		-EVTLOG
		"${CMAKE_SOURCE_DIR}/../testdata/eventLog/V21L31TEST.txt"
		-UNITTEST
)
