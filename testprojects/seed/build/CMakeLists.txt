project(seedTest)

cmake_minimum_required(VERSION 2.8)

include(../../../build/build.config.txt)
set(BUILD_DEBUG 0)

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

set(CPP_SOURCE_PATH e:/SourceCode/cpp)
#set(CPP_SOURCE_PATH d:/source)

### build

if(MSVC)
	add_definitions(-D_SECURE_SCL=0 -D_HAS_ITERATOR_DEBUGGING=0)
endif(MSVC)

file(GLOB GLOB_SEED ${SEED_SRC_PATH}/*.cpp)
#file(GLOB GLOB_WX ${ROOT_PATH}/testprojects/metagen_sfml/cpp/src/*.cpp)
#file(GLOB GLOB_WX ${ROOT_PATH}/testprojects/metagen_wxwidgets/cpp/src/*.cpp)

set(HAS_UNITTEST 1)
if(${HAS_UNITTEST})
	set(UNITTEST_FRAMEWORK_SUBDIR Posix)

	if(WIN32)
		set(UNITTEST_FRAMEWORK_SUBDIR Win32)
	endif(WIN32)

	file(GLOB GLOB_UNITTEST_FRAMEWORK
		${UNIT_TEST_DIR}/src/*.cpp
		${UNIT_TEST_DIR}/src/${UNITTEST_FRAMEWORK_SUBDIR}/*.cpp
	)
	
	include_directories(${UNIT_TEST_DIR}/src)
endif(${HAS_UNITTEST})

include_directories(E:/SourceCode/CPP/boost)

set(SRC_UNITTEST_FRAMEWORK
	${GLOB_UNITTEST_FRAMEWORK}
)

set(SRC_META
	${GLOB_SEED}
#	${GLOB_WX}
	
	${SRC_UNITTEST_FRAMEWORK}
)
set(XXX	

	${SRC_PATH}/gapiutil.cpp
	${SRC_PATH}/gbytearray.cpp
	${SRC_PATH}/gexception.cpp
	${SRC_PATH}/gmetaannotation.cpp
	${SRC_PATH}/gmetaapi.cpp
	${SRC_PATH}/gmetaclass.cpp
	${SRC_PATH}/gmetaclasstraveller.cpp
	${SRC_PATH}/gmetacommon.cpp
	${SRC_PATH}/gmetaenum.cpp
	${SRC_PATH}/gmetaextendtype.cpp
	${SRC_PATH}/gmetafield.cpp
	${SRC_PATH}/gmetafundamental.cpp
	${SRC_PATH}/gmetamethod.cpp
	${SRC_PATH}/gmetaoperator.cpp
	${SRC_PATH}/gmetaproperty.cpp
	${SRC_PATH}/gmetamodule.cpp
	${SRC_PATH}/gmetatype.cpp
	${SRC_PATH}/gvariant.cpp

	${SRC_PATH}/scriptbind/gbindcommon.cpp
	${SRC_PATH}/scriptbind/gscriptbind.cpp
	${SRC_PATH}/scriptbind/gscriptbindapi.cpp
	${SRC_PATH}/scriptbind/gscriptbindutil.cpp
	${SRC_PATH}/scriptbind/gscriptrunner.cpp
	
	${SRC_PATH}/metatraits/gmetaconverter_string.cpp
	${SRC_PATH}/metatraits/gmetaserializer_array.cpp
	${SRC_PATH}/metatraits/gmetaserializer_string.cpp
	${SRC_PATH}/metatraits/gmetaserializer_trapall.cpp
	
	${SRC_PATH}/serialization/gmetaarchivecommon.cpp
	${SRC_PATH}/serialization/gmetaarchivereader.cpp
	${SRC_PATH}/serialization/gmetaarchivetypemap.cpp
	${SRC_PATH}/serialization/gmetaarchivewriter.cpp
	${SRC_PATH}/serialization/gmetastorage_json.cpp
	${SRC_PATH}/serialization/gmetastorage_textstream.cpp
	${SRC_PATH}/serialization/gmetastorage_xml.cpp
	
	${SRC_PATH}/thirdparty/jsoncpp/json_reader.cpp
	${SRC_PATH}/thirdparty/jsoncpp/json_value.cpp
	${SRC_PATH}/thirdparty/jsoncpp/json_writer.cpp
	
#	${SRC_PATH}/scriptbind/gluabind.cpp
	${SRC_PATH}/scriptbind/gv8bind.cpp
	${SRC_PATH}/scriptbind/gv8runner.cpp
)

include_directories(
	${ROOT_PATH}/include
	${SEED_PATH}/include
	${SRC_PATH}
	${ROOT_PATH}/test
	${SEED_PATH}/src

	${ROOT_PATH}/testprojects/metagen_wxwidgets/cpp/include
	${CPP_SOURCE_PATH}/wxWidgets/include
	${CPP_SOURCE_PATH}/SFML-1.6/include
	../../metagen_sfml/cpp/include
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

target_link_libraries(
	${TARGET}
#	${CPP_SOURCE_PATH}/wxWidgets/lib/gcc_dll/libwxmsw29u.a
#	${CPP_SOURCE_PATH}/V8/libv8.a
#	${CPP_SOURCE_PATH}/SFML-1.6/lib/mingw/libsfml-audio-d.a
#	${CPP_SOURCE_PATH}/SFML-1.6/lib/mingw/libsfml-graphics-d.a
#	${CPP_SOURCE_PATH}/SFML-1.6/lib/mingw/libsfml-network-d.a
#	${CPP_SOURCE_PATH}/SFML-1.6/lib/mingw/libsfml-system-d.a
#	${CPP_SOURCE_PATH}/SFML-1.6/lib/mingw/libsfml-window-d.a
)
