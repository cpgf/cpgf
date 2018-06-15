LOCAL_PATH := $(call my-dir)
 
include $(CLEAR_VARS)
LOCAL_MODULE := lua
LOCAL_SRC_FILES := liblua.so
include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)

ROOT_PATH := ${LOCAL_PATH}//../../../..
 
SRC_PATH := $(ROOT_PATH)/src

LOCAL_CFLAGS += -O3 -frtti -fexceptions -std=c++11 -DENABLE_LUA=1
LOCAL_MODULE    := cpgf

#LIB_PATH := /source/lua-5.3.4/android/libs/armeabi-v7a/
#LOCAL_LDLIBS += -l${LIB_PATH}/liblua.so
LOCAL_SHARED_LIBRARIES := lua

LOCAL_C_INCLUDES += \
	/projects/cpgf/include \
	/source/lua-5.3.4/src

LOCAL_SRC_FILES := \
	${SRC_PATH}/gapiutil.cpp \
	${SRC_PATH}/glifecycle.cpp \
	${SRC_PATH}/gexception.cpp \
	${SRC_PATH}/gmemorypool.cpp \
	${SRC_PATH}/gmetaannotation.cpp \
	${SRC_PATH}/gmetaapi.cpp \
	${SRC_PATH}/gmetaapiutil.cpp \
	${SRC_PATH}/gmetaclass.cpp \
	${SRC_PATH}/gmetaclasstraveller.cpp \
	${SRC_PATH}/gmetacommon.cpp \
	${SRC_PATH}/gmetaenum.cpp \
	${SRC_PATH}/gmetaextendtype.cpp \
	${SRC_PATH}/gmetafield.cpp \
	${SRC_PATH}/gmetafundamental.cpp \
	${SRC_PATH}/gmetamethod.cpp \
	${SRC_PATH}/gmetaoperator.cpp \
	${SRC_PATH}/gmetaproperty.cpp \
	${SRC_PATH}/gmetamodule.cpp \
	${SRC_PATH}/gmetatype.cpp \
	${SRC_PATH}/gstringutil.cpp \
	${SRC_PATH}/gvariant.cpp \
	${SRC_PATH}/scriptbind/gbindcommon.cpp \
	${SRC_PATH}/scriptbind/gbindcontext.cpp \
	${SRC_PATH}/scriptbind/gbindconverttoscript.cpp \
	${SRC_PATH}/scriptbind/gbindgluedata.cpp \
	${SRC_PATH}/scriptbind/gbindmetamap.cpp \
	${SRC_PATH}/scriptbind/gbindmethods.cpp \
	${SRC_PATH}/scriptbind/gbindobject.cpp \
	${SRC_PATH}/scriptbind/gscriptbind.cpp \
	${SRC_PATH}/scriptbind/gscriptbindapi.cpp \
	${SRC_PATH}/scriptbind/gscriptbindutil.cpp \
	${SRC_PATH}/scriptbind/gscriptbuiltinlibrary.cpp \
	${SRC_PATH}/scriptbind/gscriptlibraryapi.cpp \
	${SRC_PATH}/scriptbind/gscriptrunner.cpp \
	${SRC_PATH}/scriptbind/gscriptservice.cpp \
	${SRC_PATH}/scriptbind/gscriptvalue.cpp \
	${SRC_PATH}/scriptbind/gscriptwrapper.cpp \
	${SRC_PATH}/metautility/gmetabytearray.cpp \
	${SRC_PATH}/metautility/gmetaobjectarray.cpp \
	${SRC_PATH}/metautility/gmetadebug.cpp \
	${SRC_PATH}/metautility/gmetacore.cpp \
	${SRC_PATH}/metatraits/gmetaconverter_string.cpp \
	${SRC_PATH}/metatraits/gmetaconverter_widestring.cpp \
	${SRC_PATH}/metatraits/gmetaserializer_array.cpp \
	${SRC_PATH}/metatraits/gmetaserializer_string.cpp \
	${SRC_PATH}/metatraits/gmetaserializer_trapall.cpp \
	${SRC_PATH}/metatraits/gmetaobjectlifemanager.cpp \
	${SRC_PATH}/metatraits/gmetaobjectlifemanager_iobject.cpp \
	${SRC_PATH}/serialization/gmetaarchivecommon.cpp \
	${SRC_PATH}/serialization/gmetaarchivereader.cpp \
	${SRC_PATH}/serialization/gmetaarchivetypemap.cpp \
	${SRC_PATH}/serialization/gmetaarchivewriter.cpp \
	${SRC_PATH}/serialization/gmetastorage_json.cpp \
	${SRC_PATH}/serialization/gmetastorage_textstream.cpp \
	${SRC_PATH}/serialization/gmetastorage_xml.cpp \
	${SRC_PATH}/tween/gtween.cpp \
	${SRC_PATH}/tween/gtweenlist.cpp \
	${SRC_PATH}/tween/gtweencommon.cpp \
	${SRC_PATH}/tween/gtimeline.cpp \
	${SRC_PATH}/thirdparty/jsoncpp/json_reader.cpp \
	${SRC_PATH}/thirdparty/jsoncpp/json_value.cpp \
	${SRC_PATH}/thirdparty/jsoncpp/json_writer.cpp \
	${SRC_PATH}/scriptbind/gluabind.cpp \
	${SRC_PATH}/scriptbind/gluarunner.cpp

include $(BUILD_SHARED_LIBRARY)
