project(cpgf)

cmake_minimum_required(VERSION 2.8)

include(../../../build/build.config.txt)

set(CMAKE_BUILD_TYPE Release)

if(${BUILD_DEBUG})
	set(CMAKE_BUILD_TYPE Debug)
	message(STATUS "Build as debug.")
endif(${BUILD_DEBUG})

set(SEED_PATH ${PROJECT_SOURCE_DIR}/..)
set(ROOT_PATH ${PROJECT_SOURCE_DIR}/../../../)
set(BIN_PATH ${PROJECT_SOURCE_DIR}/bin)

set(TARGET seed)

set(SRC_PATH ${ROOT_PATH}/src)
set(SEED_SRC_PATH ${SEED_PATH}/src)

set(TEST_PATH ${ROOT_PATH}/test)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_PATH})

### build

if(MSVC)
	add_definitions(-D_SECURE_SCL=0 -D_HAS_ITERATOR_DEBUGGING=0)
endif(MSVC)

set(SRC_META
	${SEED_SRC_PATH}/main.cpp

	${SRC_PATH}/gapi.cpp
	${SRC_PATH}/gbytearray.cpp
	${SRC_PATH}/gexception.cpp
	${SRC_PATH}/gmetaannotation.cpp
	${SRC_PATH}/gmetaapi.cpp
	${SRC_PATH}/gmetaclass.cpp
	${SRC_PATH}/gmetaclasstraveller.cpp
	${SRC_PATH}/gmetacommon.cpp
	${SRC_PATH}/gmetaconverter.cpp
	${SRC_PATH}/gmetaenum.cpp
	${SRC_PATH}/gmetafield.cpp
	${SRC_PATH}/gmetafundamental.cpp
	${SRC_PATH}/gmetamethod.cpp
	${SRC_PATH}/gmetaoperator.cpp
	${SRC_PATH}/gmetaproperty.cpp
	${SRC_PATH}/gmetatype.cpp
	${SRC_PATH}/gmetatypereg.cpp
	${SRC_PATH}/gvariant.cpp

	${SRC_PATH}/scriptbind/gbindcommon.cpp
	${SRC_PATH}/scriptbind/gscriptbind.cpp
	${SRC_PATH}/scriptbind/gscriptbindapi.cpp
	${SRC_PATH}/scriptbind/gscriptbindutil.cpp

#	${SRC_PATH}/scriptbind/gluabind.cpp
#	${SRC_PATH}/scriptbind/gv8bind.cpp
)

include_directories(
	${ROOT_PATH}/include
	${SEED_PATH}/include
)
if(${HAS_LUA})
	include_directories(${LUA_INC})
endif(${HAS_LUA})

if(${HAS_V8})
	include_directories(${V8_INC})
endif(${HAS_V8})

set(SEED_COMPILE_FLAGS " ")
set(TEST_COMPILE_FLAGS " ")
if(CMAKE_COMPILER_IS_GNUCC)
	set(SEED_COMPILE_FLAGS "-Wall -Wextra -Wundef -O3")

	if(SUPPORT_GCC_0X)
		if(MINGW)
			set(SEED_COMPILE_FLAGS "${SEED_COMPILE_FLAGS} -std=gnu++0x")
		endif(MINGW)

		if(NOT MINGW)
			set(SEED_COMPILE_FLAGS "${SEED_COMPILE_FLAGS} -std=c++0x")
		endif(NOT MINGW)
	endif(SUPPORT_GCC_0X)

endif(CMAKE_COMPILER_IS_GNUCC)

if(MSVC)
	set(SEED_COMPILE_FLAGS "-W4 -Wp64")
endif(MSVC)

add_executable(
	${TARGET}
	EXCLUDE_FROM_ALL
	${SRC_META}
)

set_target_properties(
	${TARGET}
	PROPERTIES
	COMPILE_FLAGS ${SEED_COMPILE_FLAGS}
)

