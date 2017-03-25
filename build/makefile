CACHE_DIR = temp_cache
CMAKE = cmake .. $(CMAKE_FLAGS)

MK_DIR = @cmake -E make_directory
CH_DIR = @cmake -E chdir
ECHO = @cmake -E echo

MK_CACHE = $(MK_DIR) $(CACHE_DIR)
EXEC_BUILD = $(CH_DIR) $(CACHE_DIR)

PROJECT=cpgf

PROJECT_PREFIX = project

SUPPORT_MAKES = auto nmake mingw mingw_debug linux vc15 cb_mingw cb_mingw_debug cb_nmake cb_linux xcode

none: needcmake
	$(ECHO) "Usage:"
	$(ECHO) "  make MakeType [TARGET=TargetToMake] [CMAKE_FLAGS='flags passed to cmake']"
	$(ECHO) "or"
	$(ECHO) "  nmake MakeType"
	$(ECHO) "if MS VC is used"
	$(ECHO) 
	$(ECHO) "Available MakeType"
	$(ECHO) "  $(SUPPORT_MAKES)"
	$(ECHO) 
	$(ECHO) "  nmake        Generate Microsoft VC makefile and then use nmake to build."
	$(ECHO) "  mingw        Generate MinGW makefile and then use mingw32-make to build (release version)."
	$(ECHO) "  mingw_debug  Generate MinGW makefile and then use mingw32-make to build (debug version)."
	$(ECHO) "  linux        Generate Linux/Unix makefile and then use GCC make to build."
	$(ECHO) "  vc15         Generate project files for Microsoft VC 2015. No auto build. You need to open the project in VC IDE then build."
	$(ECHO) "  cb_mingw     Generate project files for Code::Blocks using MinGW g++ compiler, release version. No auto build. You need to open the project in Code::Blocks IDE then build."
	$(ECHO) "  cb_mingw_debug     Generate project files for Code::Blocks using MinGW g++ compiler, debug version. No auto build. You need to open the project in Code::Blocks IDE then build."
	$(ECHO) "  cb_nmake     Generate project files for Code::Blocks using VC compiler. No auto build. You need to open the project in Code::Blocks IDE then build."
	$(ECHO) "  cb_linux     Generate project files for Code::Blocks using Linux/Unix GCC compiler. No auto build. You need to open the project in Code::Blocks IDE then build."
	$(ECHO) "  xcode        Generate project files for Apple Xcode. No auto build. You need to open the project in Xcode IDE then build."
	$(ECHO) "  auto         Auto detect the compiler and make environment and then use make to build. NOT recommend."
	$(ECHO) 
	$(ECHO) "  clean        No build. Remove and clean all generated files, include build and projects files."
	$(ECHO) "  cleanbuild   No build. Remove and clean all build files, but leave projects files."
	$(ECHO) 
	$(ECHO) "Optional TargetToMake"
	$(ECHO) 
	$(ECHO) "  TARGET=lib                   Build the library. This is the default if TARGET is omitted."
	$(ECHO) "  TARGET=tutorials             Build the tutorials."
	$(ECHO) "  TARGET=tests                 Build the unit tests."
	$(ECHO) "  TARGET=samplereflection      Build the reflection sample application."
	$(ECHO) "  TARGET=sampleserialization   Build the serialization sample application."
	$(ECHO) "  TARGET=samplelua             Build the Lua binding sample application."
	$(ECHO) "  TARGET=sampleopengl          Build the Opengl binding sample application."
	$(ECHO) "  TARGET=samplebox2d           Build the Box2D binding sample application."
	$(ECHO) "  TARGET=samplesfml            Build the SFML binding sample application."
	$(ECHO) "  TARGET=sampleirrlicht        Build the Irrlicht binding sample application."

needcmake:
	$(ECHO)

auto: needcmake
	$(MK_CACHE)_auto
	$(CH_DIR) $(CACHE_DIR)_auto $(CMAKE)
	$(EXEC_BUILD)_auto make $(TARGET)

mingw: needcmake
	$(MK_CACHE)_mingw
	$(CH_DIR) $(CACHE_DIR)_mingw $(CMAKE) -DCMAKE_BUILD_TYPE=Release -G"MinGW Makefiles"
	$(EXEC_BUILD)_mingw mingw32-make $(TARGET)

mingw_debug: needcmake
	$(MK_CACHE)_mingw_debug
	$(CH_DIR) $(CACHE_DIR)_mingw_debug $(CMAKE) -DCMAKE_BUILD_TYPE=Debug -G"MinGW Makefiles"
	$(EXEC_BUILD)_mingw_debug mingw32-make $(TARGET)

nmake: needcmake
	$(ECHO) NOTE: *****************************
	$(ECHO) NOTE: If cmake raises errors, try run this in Visual Studio Command Prompt from the VS package.
	$(ECHO) NOTE: *****************************
	$(MK_CACHE)_nmake
	$(CH_DIR) $(CACHE_DIR)_nmake $(CMAKE) -G"NMake Makefiles"
	$(EXEC_BUILD)_nmake nmake $(TARGET)

linux: needcmake
	$(MK_CACHE)_linux
	$(CH_DIR) $(CACHE_DIR)_linux $(CMAKE) -G"Unix Makefiles"
	$(EXEC_BUILD)_linux make $(TARGET)

vc15: needcmake
	$(MK_DIR) $(PROJECT_PREFIX)_vc15
	$(CH_DIR) $(PROJECT_PREFIX)_vc15 $(CMAKE) -G"Visual Studio 14"
	$(ECHO) Please open the solution $(PROJECT).sln in $(PROJECT_PREFIX)_vc15 in VC IDE.

cb_mingw: needcmake
	$(MK_DIR) $(PROJECT_PREFIX)_cb_mingw
	$(CH_DIR) $(PROJECT_PREFIX)_cb_mingw $(CMAKE) -G"CodeBlocks - MinGW Makefiles"
	$(ECHO) Please open the project file $(PROJECT).cbp in $(PROJECT_PREFIX)_cb_mingw in Code::Blocks.

cb_mingw_debug: needcmake
	$(MK_DIR) $(PROJECT_PREFIX)_cb_mingw_debug
	$(CH_DIR) $(PROJECT_PREFIX)_cb_mingw_debug $(CMAKE) -DCMAKE_BUILD_TYPE=Debug -G"CodeBlocks - MinGW Makefiles"
	$(ECHO) Please open the project file $(PROJECT).cbp in $(PROJECT_PREFIX)_cb_mingw_debug in Code::Blocks.

cb_nmake: needcmake
	$(MK_DIR) $(PROJECT_PREFIX)_cb_nmake
	$(CH_DIR) $(PROJECT_PREFIX)_cb_nmake $(CMAKE) -G"CodeBlocks - NMake Makefiles"
	$(ECHO) Please open the project file $(PROJECT).cbp in $(PROJECT_PREFIX)_cb_nmake in Code::Blocks.

cb_linux: needcmake
	$(MK_DIR) $(PROJECT_PREFIX)_cb_linux
	$(CH_DIR) $(PROJECT_PREFIX)_cb_linux $(CMAKE) -G"CodeBlocks - Unix Makefiles"
	$(ECHO) Please open the project file $(PROJECT).cbp in $(PROJECT_PREFIX)_cb_linux in Code::Blocks.

xcode: needcmake
	cmake -G"Xcode"
	$(ECHO) Please open the solution $(PROJECT).xcodeproj in Xcode IDE.

cleanbuild: needcmake
	@cmake -E remove_directory bin
	@cmake -E remove_directory $(CACHE_DIR)

clean: needcmake cleanbuild
	@cmake -E remove_directory $(PROJECT_PREFIX)_vc05
	@cmake -E remove_directory $(PROJECT_PREFIX)_vc08
	@cmake -E remove_directory $(PROJECT_PREFIX)_vc10
	@cmake -E remove_directory $(PROJECT_PREFIX)_vc12
	@cmake -E remove_directory $(PROJECT_PREFIX)_vc13
	@cmake -E remove_directory $(PROJECT_PREFIX)_cb_mingw
	@cmake -E remove_directory $(PROJECT_PREFIX)_cb_nmake
	@cmake -E remove_directory $(PROJECT_PREFIX)_cb_linux

.PHONY: clean

