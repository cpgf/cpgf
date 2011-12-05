project(cpgf)

cmake_minimum_required(VERSION 2.8)

include(build.config.txt)

set(CMAKE_BUILD_TYPE Release)

if(${BUILD_DEBUG})
	set(CMAKE_BUILD_TYPE Debug)
	message(STATUS "Build as debug.")
endif(${BUILD_DEBUG})

set(TARGET_LIB lib)
set(TARGET_SAMPLELUA samplelua)
set(TARGET_SAMPLEREFLECTION samplereflection)
set(TARGET_UNITTEST test)

set(OUTNAME_LIB cpgf)
set(OUTNAME_SAMPLELUA samplelua)
set(OUTNAME_SAMPLEREFLECTION samplereflection)
set(OUTNAME_UNITTEST test)

set(ROOT_PATH ${PROJECT_SOURCE_DIR}/..)
set(BIN_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIB_PATH ${ROOT_PATH}/lib)

set(SRC_PATH ${ROOT_PATH}/src)
set(SAMPLE_ROOT ${ROOT_PATH}/samples)
set(SAMPLE_REFLECT_PATH ${SAMPLE_ROOT}/reflection)
set(SAMPLE_BIND_PATH ${SAMPLE_ROOT}/scriptbind)

set(TEST_PATH ${ROOT_PATH}/test)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_PATH})


### build

if(MSVC)
	add_definitions(-D_SECURE_SCL=0 -D_HAS_ITERATOR_DEBUGGING=0)
endif(MSVC)

set(SRC_META
	${SRC_PATH}/gapi.cpp
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
	${SRC_PATH}/gmetareflect.cpp
	${SRC_PATH}/gmetatype.cpp
	${SRC_PATH}/gmetatypereg.cpp

	${SRC_PATH}/scriptbind/gbindcommon.cpp
	${SRC_PATH}/scriptbind/gscriptbind.cpp
	${SRC_PATH}/scriptbind/gscriptbindapi.cpp
)

file(GLOB GLOB_SAMPLEREFLECTION ${SAMPLE_REFLECT_PATH}/*.cpp)
set(SRC_SAMPLEREFLECTION
	${SAMPLE_ROOT}/samplemain.cpp
	
	${GLOB_SAMPLEREFLECTION}
)

set(SRC_LUA_BIND )
set(SRC_V8_BIND )

if(${HAS_LUA})
	set(SRC_LUA_BIND
		${SRC_PATH}/scriptbind/gluabind.cpp
	)
endif(${HAS_LUA})

if(${HAS_V8})
	set(SRC_V8_BIND
		${SRC_PATH}/scriptbind/gv8bind.cpp
	)
endif(${HAS_V8})

set(SRC_LIB
	${SRC_META}
	
	${SRC_LUA_BIND}
	${SRC_V8_BIND}
)

file(GLOB GLOB_SAMPLELUA ${SAMPLE_BIND_PATH}/luabind/*.cpp)
set(SRC_SAMPLELUA
	${SAMPLE_ROOT}/samplemain.cpp
	
	${SAMPLE_BIND_PATH}/testscriptbindmetadata.cpp
	
	${GLOB_SAMPLELUA}
)

include_directories(../include)
if(${HAS_LUA})
	include_directories(${LUA_INC})
endif(${HAS_LUA})

if(${HAS_V8})
	include_directories(${V8_INC})
endif(${HAS_V8})

set(LIB_COMPILE_FLAGS " ")
set(TEST_COMPILE_FLAGS " ")
if(CMAKE_COMPILER_IS_GNUCC)
	set(LIB_COMPILE_FLAGS "-Wall -Wextra -Wundef -O3")
	set(TEST_COMPILE_FLAGS "-Wall -Wextra -Wundef -O3")
	set(UNITTEST_COMPILE_FLAGS "-Wall -Wextra -Wundef -O3")
if(TEST_GCOV)
	set(UNITTEST_COMPILE_FLAGS "-Wall -Wextra -Wundef -O0 -fprofile-arcs -ftest-coverage")
endif(TEST_GCOV)
endif(CMAKE_COMPILER_IS_GNUCC)

if(MSVC)
	set(LIB_COMPILE_FLAGS "-W4 -Wp64")
	set(TEST_COMPILE_FLAGS "-W4 -Wp64")
	set(UNITTEST_COMPILE_FLAGS "-W4 -Wp64")
endif(MSVC)

add_library(
	${TARGET_LIB}
		STATIC
		${SRC_LIB}
)

set_target_properties(
	${TARGET_LIB}
	PROPERTIES
	OUTPUT_NAME ${OUTNAME_LIB}
	ARCHIVE_OUTPUT_DIRECTORY ${LIB_PATH}
	COMPILE_FLAGS ${LIB_COMPILE_FLAGS}
)

add_executable(
	${TARGET_SAMPLEREFLECTION}
	EXCLUDE_FROM_ALL
		${SRC_SAMPLEREFLECTION}
)

set_target_properties(
	${TARGET_SAMPLEREFLECTION}
	PROPERTIES
	OUTPUT_NAME ${OUTNAME_SAMPLEREFLECTION}
	COMPILE_FLAGS ${TEST_COMPILE_FLAGS}
)

target_link_libraries(${TARGET_SAMPLEREFLECTION} ${TARGET_LIB})

if(${HAS_LUA})
	add_executable(
		${TARGET_SAMPLELUA}
		EXCLUDE_FROM_ALL
			${SRC_SAMPLELUA}
	)

	set_target_properties(
		${TARGET_SAMPLELUA}
		PROPERTIES
		OUTPUT_NAME ${OUTNAME_SAMPLELUA}
		COMPILE_FLAGS ${TEST_COMPILE_FLAGS}
	)

	target_link_libraries(${TARGET_SAMPLELUA} ${TARGET_LIB} ${LUA_LIB})
endif(${HAS_LUA})




### unit test

set(HAS_UNITTEST 1)
if((not DEFINED UNIT_TEST_DIR) OR (UNIT_TEST_DIR STREQUAL ""))
	set(HAS_UNITTEST 0)
endif((not DEFINED UNIT_TEST_DIR) OR (UNIT_TEST_DIR STREQUAL ""))

if(${HAS_UNITTEST})
	find_file(UTEST_EXIST
		NAMES "UnitTest++.h"
		PATHS ${UNIT_TEST_DIR}/src NO_DEFAULT_PATH
	)

	if(${UTEST_EXIST} STREQUAL "UTEST_EXIST-NOTFOUND")
		set(HAS_UNITTEST 0)

		message(WARNING "Can't find UnitTest++ in ${UNIT_TEST_DIR}")
	endif(${UTEST_EXIST} STREQUAL "UTEST_EXIST-NOTFOUND")
endif(${HAS_UNITTEST})

if(${HAS_UNITTEST})
	if(HAS_LUA LESS 1)
		message(WARNING "Can't find Lua for unit test. Unit test will not include Lua binding test.")
	endif(HAS_LUA LESS 1)

	if(HAS_V8 LESS 1)
		message(WARNING "Can't find V8 for unit test. Unit test will not include V8 binding test.")
	endif(HAS_V8 LESS 1)
endif(${HAS_UNITTEST})

if(${HAS_UNITTEST})

	set(UNITTEST_FRAMEWORK_SUBDIR Posix)

	if(WIN32)
		set(UNITTEST_FRAMEWORK_SUBDIR Win32)
	endif(WIN32)

	file(GLOB GLOB_UNITTEST_FRAMEWORK
		${UNIT_TEST_DIR}/src/*.cpp
		${UNIT_TEST_DIR}/src/${UNITTEST_FRAMEWORK_SUBDIR}/*.cpp
	)

	set(SRC_UNITTEST_FRAMEWORK
		${GLOB_UNITTEST_FRAMEWORK}
	)

	file(GLOB GLOB_TEST_SRC
		${TEST_PATH}/misc/*.cpp
		${TEST_PATH}/reflection/*.cpp
		${TEST_PATH}/callback/*.cpp
		${TEST_PATH}/scriptbind/*.cpp
		${TEST_PATH}/scriptbind/general/*.cpp
	)

	set(GLOB_TEST_LUA )
	if(HAS_LUA)
		file(GLOB GLOB_TEST_LUA
			${TEST_PATH}/scriptbind/luabind/*.cpp
		)
	endif(HAS_LUA)

	set(GLOB_TEST_V8 )
	if(HAS_V8)
		file(GLOB GLOB_TEST_V8
			${TEST_PATH}/scriptbind/v8bind/*.cpp
		)
	endif(HAS_V8)

	set(SRC_TEST_SRC
		${TEST_PATH}/testmain.cpp
		
		${GLOB_TEST_SRC}

		${GLOB_TEST_LUA}
		${GLOB_TEST_V8}
		
		${SRC_LIB}

		${SRC_UNITTEST_FRAMEWORK}
	)

	include_directories(${UNIT_TEST_DIR}/src)

	add_definitions(-DENABLE_LUA=${HAS_LUA} -DENABLE_V8=${HAS_V8})

	add_executable(
		${TARGET_UNITTEST}
		EXCLUDE_FROM_ALL
			${SRC_TEST_SRC}
	)

	if(CMAKE_COMPILER_IS_GNUCC)
		if(TEST_GCOV)
			target_link_libraries(${TARGET_UNITTEST} gcov)
		endif(TEST_GCOV)
	endif(CMAKE_COMPILER_IS_GNUCC)

	if(HAS_LUA)
		target_link_libraries(${TARGET_UNITTEST} ${LUA_LIB})
	endif(HAS_LUA)
	if(HAS_V8)
		target_link_libraries(${TARGET_UNITTEST} ${V8_LIB})
	endif(HAS_V8)
	
	set_target_properties(
		${TARGET_UNITTEST}
		PROPERTIES
		OUTPUT_NAME ${OUTNAME_UNITTEST}
		COMPILE_FLAGS ${UNITTEST_COMPILE_FLAGS}
	)

endif(${HAS_UNITTEST})

