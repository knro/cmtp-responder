CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(cmtp-responder C)

OPTION(BUILD_CMTP_RESPONDER "Build cmtp-responder" ON)
OPTION(BUILD_DESCRIPTORS "Build descriptors" OFF)

include(GNUInstallDirs)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/entity)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/transport)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/util)

AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/src SRCS)
AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/src/entity SRCS)
AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/src/transport SRCS)
AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/src/util SRCS)

string(LENGTH "${CMAKE_SOURCE_DIR}/" SRC_PATH_LEN)
add_definitions("-DSRC_PATH_LEN=${SRC_PATH_LEN}")

IF (BUILD_CMTP_RESPONDER)
	INCLUDE(FindPkgConfig)
	pkg_check_modules(pkgs REQUIRED glib-2.0 libsystemd)

	FOREACH(flag ${pkgs_CFLAGS})
		SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
	ENDFOREACH(flag)

	SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} -Wall -Werror=implicit-function-declaration")
	SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} -fexceptions -fvisibility=hidden")

	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS} -fPIE")
	SET(CMAKE_EXE_LINKER_FLAGS " -Wl,--as-needed -pie -Wl,--hash-style=both")

	ADD_EXECUTABLE(${PROJECT_NAME} ${SRCS})
	TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${pkgs_LDFLAGS} pthread rt)

	INSTALL(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
	INSTALL(FILES cmtp-responder.conf DESTINATION ${CMAKE_INSTALL_SYSCONFDIR})
ENDIF ()

IF (BUILD_DESCRIPTORS)
	ADD_EXECUTABLE(extract_descs_strs ${CMAKE_SOURCE_DIR}/src/extract_descs/mtp_extract_descs_strs.c
		${CMAKE_SOURCE_DIR}/src/transport/mtp_descs_strings.c)
	ADD_CUSTOM_COMMAND(OUTPUT descs strs
		COMMAND ./extract_descs_strs
		DEPENDS extract_descs_strs)
	ADD_CUSTOM_TARGET(descs_strs ALL DEPENDS descs strs)

	INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/descs DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/cmtp-responder)
	INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/strs DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/cmtp-responder)
ENDIF ()
